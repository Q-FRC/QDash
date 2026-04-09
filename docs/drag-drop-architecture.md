# Drag & Drop Architecture

This document describes how widget dragging and resizing works in QDash, and which
files own each piece of the system.

---

## Overview

There are two separate drag scenarios:

1. **In-grid drag** — the user grabs an existing widget and repositions it within the tab.
2. **From-sidebar drag** — the user drags a topic out of the `TopicView` sidebar, which
   creates a new widget and places it on the tab.

Resizing is a third, related operation that shares the same grid-validation logic.

---

## Component Responsibilities

### `Tab.qml` — grid owner

`Tab` is the single source of truth for all grid geometry and validation.

**Properties exposed to widgets:**

| Property | Description |
|---|---|
| `colWidth` | Width in pixels of one grid column |
| `rowHeight` | Height in pixels of one grid row |
| `rows` / `cols` | Number of rows / columns in the grid |
| `lastOpSuccessful` | Whether the most recent drag/resize landed on a valid spot |

**Functions:**

| Function | Description |
|---|---|
| `validSpot(x, y, row, col, rowSpan, colSpan, round)` | Tests whether the pixel position `(x, y)` is an unoccupied grid cell. Updates the green/red `validRect` overlay and `lastOpSuccessful`. |
| `validResize(w, h, x, y, row, col, rowSpan, colSpan)` | Same as `validSpot`, but for resizing. |
| `getPoint(x, y, round)` | Converts a pixel position to a `(col, row)` grid point. |
| `getRect(x, y, w, h)` | Converts a pixel rect to a `(col, row, colSpan, rowSpan)` grid rect. |
| `resetValid()` | Clears the `validRect` outline. |

**Visual:**

`validRect` is a `Rectangle` child of `Tab` that shows a green border on valid
drop targets and a red border on invalid ones. It is updated by `validSpot` and
`validResize`, and cleared by `resetValid`.

**Why here?**  
Before this refactor the grid functions lived on the `Repeater` (`id: grid`).
Using a `Repeater` as a calculation engine was confusing — a `Repeater` is a
view component, not a logic component. Moving the functions to `Tab` makes the
owner obvious and removes the need for widgets to reach into the repeater by id.

---

### `BaseWidget.qml` — per-widget drag state & input handling

`BaseWidget` manages the drag/resize lifecycle for a single widget.

**Key state:**

| Property | Description |
|---|---|
| `dragForced` | `true` while the widget is in "tap-to-move" mode (set by `MainScreen` during a sidebar drag). |
| `resizeActive` | `true` while a resize handle is being dragged. |
| `Drag.active` | QML built-in; `true` while the `MouseArea` drag is live. |
| `originalRect` | Saved `(x, y, w, h)` used to animate back if the drop is invalid. |

**Key functions:**

| Function | Description |
|---|---|
| `startDrag()` | Saves `originalRect`, attaches `dragArea.drag.target`, raises z-order. |
| `cancelDrag()` | Resets all drag state, calls `tab.resetValid()`. |
| `startResize()` | Saves `originalRect`, sets `resizeActive = true`. |
| `animateBacksize()` | Smoothly returns the widget to `originalRect`. |
| `dragTapped()` | Toggles drag on/off (used in tap-only mode). |
| `checkDrag()` | Called on `x`/`y` change; passes current position to `tab.validSpot`. |
| `checkResize()` | Called on `width`/`height` change; passes current size to `tab.validResize`. |
| `fixSize()` | Snaps widget geometry back to its current grid cell (called after model row/column update). |
| `getPoint()` | Delegates to `tab.getPoint` for the widget's current position. |

**Input handling:**

A `MouseArea` (id: `dragArea`) covers the entire widget and handles:

- **Left press** → `dragTapped()` (starts or cancels drag)
- **Left release** → validates the new position via `tab.validSpot`; commits or
  animates back; calls `tab.resetValid()`
- **Right press** → opens context menu
- **Double-click** → opens context menu

A `TapHandler` additionally handles single-tap, double-tap, and long-press gestures,
primarily for platforms where `CompileDefinitions.tapOnly` is set.

---

### `ResizeComponent.qml` — resize handle wiring

`ResizeComponent` is a `Repeater` that instantiates eight `ResizeAnchor` items — one
for each cardinal and diagonal edge of the widget. It is loaded lazily inside
`BaseWidget` via a `Loader` that activates on hover (desktop) or always (tap-only).

Each `ResizeAnchor` is given a reference to its parent widget through the explicit
`control` property and accesses `tab` and `widget` via QML's lexical scope (both are
ids in the enclosing `BaseWidget` document).

On mouse release, the anchor calls `tab.validResize` and `tab.getRect` to determine
the new span, then updates `widget.mrow`, `widget.mcolumn`, `widget.mrowSpan`, and
`widget.mcolumnSpan` before calling `widget.fixSize()`.

---

### `ResizeAnchor.qml` — single resize handle

`ResizeAnchor` is a positioned `Item` that owns a `MouseArea` with cursor-shape and
drag-axis logic. It receives the target widget through the `required property var control`.

Edge-detection (`isAtEdge`) uses `control.mcolumn`, `control.mrow`, etc. to disable
handles that would push the widget outside the grid boundary.

---

### `MainScreen.qml` — top-level drag orchestration

`MainScreen` handles the **from-sidebar drag** flow:

1. `TopicView` emits `dragging(pos)` as the user moves the pointer outside the panel.
2. `MainScreen.drag(pos, fromList)` looks up the current tab's `latestWidget`,
   positions it at the pointer, and sets `dragForced = true`.
3. `TopicView` emits `dropped(pos)` when the pointer is released.
4. `MainScreen.drop(pos, fromList)` checks `tab.lastOpSuccessful`; if valid it commits
   the widget's position via `getPoint()`; otherwise it calls `cancelDrag()` and
   removes the widget via `tab.removeLatest()`.

---

### `TopicView.qml` — drag initiation from the sidebar

Each row in the `TreeView` has a `DragHandler` (target: `null`) that:

1. Detects when the pointer exits the topic panel.
2. Calls `widgetAdd(name, topic, type)`, which triggers `addWidget` on the owning
   `TopicView`, which propagates back to `MainScreen` → `Tab.fakeAdd()`.
3. Continuously emits `dragging(pos)` so `MainScreen` can update the phantom widget's
   position.
4. Emits `dropped(pos)` on pointer release.

---

## Data Flow Diagrams

### In-grid drag (desktop)

```
MouseArea.onPressed
  └─► dragTapped() → startDrag()          # widget follows pointer
        widget.x/y change
          └─► checkDrag() → tab.validSpot()   # green/red outline updates
MouseArea.onReleased
  └─► tab.validSpot()   # final check
      ├── valid   → model.row/column = getPoint() → fixSize()
      └── invalid → animateBacksize()
  └─► tab.resetValid()  # outline cleared
```

### From-sidebar drag

```
DragHandler (TopicView) detects pointer outside panel
  └─► widgetAdd() → Tab.fakeAdd()         # new widget added to model
  └─► dragging(pos) → MainScreen.drag()
        └─► w.dragForced = true, w.x/y = pos  # widget follows pointer
              w.x/y change → checkDrag() → tab.validSpot()
Pointer released → dropped(pos) → MainScreen.drop()
  ├── lastOpSuccessful → w.mrow/mcolumn = getPoint() → fixSize()
  └── !lastOpSuccessful → cancelDrag() + tab.removeLatest()
```

### Resize

```
ResizeAnchor.MouseArea.onPressed
  └─► startResize()                        # saves originalRect

widget.width/height change
  └─► checkResize() → tab.validResize()   # green/red outline updates

ResizeAnchor.MouseArea.onReleased
  ├── valid   → widget.mrowSpan/mcolumnSpan = tab.getRect() → fixSize()
  └── invalid → animateBacksize()
  └─► tab.resetValid()
```
