# QDash Repository-Wide Security and Bug Audit

**Date:** 2026-04-09  
**Scope:** Full repository — C++ source, QML, shell scripts, build system, and configuration files  
**Auditor:** Automated + Manual Review

---

## Table of Contents

1. [Executive Summary](#executive-summary)
2. [Critical Severity](#critical-severity)
3. [High Severity](#high-severity)
4. [Medium Severity](#medium-severity)
5. [Low Severity](#low-severity)
6. [Informational](#informational)
7. [Summary Table](#summary-table)

---

## Executive Summary

A full audit of the QDash codebase was performed across all source languages (C++, QML, shell scripts, JSON, Java). The project is a Qt/QML-based FRC robot dashboard that communicates with a robot via NetworkTables over a local network.

**Total issues found: 18**

| Severity  | Count |
|-----------|-------|
| Critical  | 3     |
| High      | 6     |
| Medium    | 5     |
| Low       | 4     |

No hardcoded credentials or secret material were found. The majority of issues are C++ memory-safety and logic bugs that could cause crashes or incorrect behavior under certain inputs.

---

## Critical Severity

---

### C-1 — Path Traversal / Arbitrary File Write in `RemoteLayoutModel::download`

**File:** `QDash/Native/Models/RemoteLayoutModel.cpp`  
**Lines:** 102–128  
**Type:** Path Traversal  

#### Description

The `download()` slot accepts a `filename` parameter that originates from a QML file-save dialog, and writes the response body from a remote HTTP server directly to that path without any sanitisation or containment. A malicious filename (e.g. one containing `..` path components) or a compromised robot-side HTTP server could cause files to be written to arbitrary filesystem locations on the dashboard host.

The filename is only subjected to a simple string replacement to strip the `file://` URL prefix, which does not prevent directory traversal:

```cpp
void RemoteLayoutModel::download(const QUrl &url, const QString &filename)
{
    ...
    connect(reply, &QNetworkReply::finished, this, [this, reply, name] {
        QFile file(name);   // 'name' is unvalidated
        ...
        file.write(content);
        ...
        emit fileOpened(name);
    });
}
```

#### Recommendation

Resolve the canonical path after stripping the `file://` prefix and reject it if it falls outside an allowed directory (e.g. `QStandardPaths::AppLocalDataLocation`). Use `QFileInfo::canonicalFilePath()` to resolve `..` segments before opening the file.

---

### C-2 — Uninitialized Member Variable `m_connected` in `ConnManager`

**File:** `QDash/Native/Managers/ConnManager.h`  
**Line:** 43  
**Type:** Undefined Behaviour  

#### Description

`bool m_connected` is declared without an initializer:

```cpp
bool m_connected;
```

In C++, non-static members of built-in type without an in-class initializer are default-initialized to an *indeterminate* value. The first call to `connected()` (e.g. from QML during startup, before `setConnected` is ever called) reads this indeterminate value, which is undefined behaviour and can produce incorrect UI state.

#### Recommendation

Add an in-class initializer:

```cpp
bool m_connected = false;
```

---

### C-3 — Type Mismatch Bug in `TopicStore::toVariant` for Integer Arrays

**File:** `QDash/Native/NT/TopicStore.cpp`  
**Lines:** 257–263  
**Type:** Logic Error / Data Corruption  

#### Description

When converting a NetworkTables integer array to a `QVariant`, the loop variable is declared as `size_t` instead of `int64_t`:

```cpp
} else if (value.IsIntegerArray()) {
    const std::span<const int64_t> a = value.GetIntegerArray();
    QList<int64_t> newList;
    for (const size_t i : a)   // <-- should be int64_t
        newList << i;
    v = QVariant::fromValue(newList);
}
```

On a 64-bit platform `size_t` and `int64_t` are the same width, so values are *reinterpreted* rather than truncated. However, any negative `int64_t` value is reinterpreted as a very large unsigned number, silently corrupting all negative integers received via the NetworkTables integer array type (e.g. FMS alliance/station data).

#### Recommendation

Change `size_t i` to `int64_t i` (or simply `auto`):

```cpp
for (const int64_t i : a)
    newList << i;
```

---

## High Severity

---

### H-1 — Out-of-Bounds Access in `RemoteLayoutModel::name` and `RemoteLayoutModel::url`

**File:** `QDash/Native/Models/RemoteLayoutModel.cpp`  
**Lines:** 133, 138  
**Type:** Out-of-Bounds Memory Access  

#### Description

Both public invokable methods access `m_data[index]` without range checking:

```cpp
QString RemoteLayoutModel::name(int index)
{
    return m_data[index].name;   // no bounds check
}

QUrl RemoteLayoutModel::url(int index)
{
    return m_data[index].url;    // no bounds check
}
```

These are called directly from QML (e.g. `rlm.url(list.currentIndex)`). If `list.currentIndex` is `-1` (the default when no item is selected) or any out-of-range value, the application will access memory out of bounds, likely crashing.

#### Recommendation

```cpp
QString RemoteLayoutModel::name(int index)
{
    if (index < 0 || index >= m_data.count()) return {};
    return m_data[index].name;
}
```

---

### H-2 — Crash on Empty Accent Name in `AccentsListModel::names`

**File:** `QDash/Native/Models/AccentsListModel.cpp`  
**Lines:** 134–136  
**Type:** Out-of-Bounds Access / Crash  

#### Description

The `names()` function calls `name.at(0)` and `name.last(name.size() - 1)` without checking whether `name` is empty. If any accent entry has an empty `name` field in `accents.json`, the application crashes:

```cpp
QChar first = name.at(0).toUpper();          // crash if name.isEmpty()
list.append(first + name.last(name.size() - 1));
```

`name.last(name.size() - 1)` also computes `name.last(-1)` for a single-character name, which is undefined behaviour.

#### Recommendation

```cpp
if (name.isEmpty()) {
    list.append({});
    continue;
}
QChar first = name.at(0).toUpper();
list.append(first + (name.size() > 1 ? name.last(name.size() - 1) : QString{}));
```

---

### H-3 — Logic Inversion in `TopicStore::hasEntry`

**File:** `QDash/Native/NT/TopicStore.cpp` / `QDash/Native/NT/TopicStore.h`  
**Type:** Logic Error  

#### Description

`hasEntry` is implemented as `return !entry(topic)`, meaning it returns `true` when no entry exists and `false` when one does — the opposite of what the name implies:

```cpp
bool TopicStore::hasEntry(QString topic) {
    return !entry(topic);   // returns true when entry does NOT exist
}
```

Although this method is currently private and not referenced elsewhere in the codebase, the inverted logic is a latent defect that will cause incorrect behaviour if it is ever called.

#### Recommendation

Fix the return value:

```cpp
bool TopicStore::hasEntry(QString topic) {
    return entry(topic) != nullptr;
}
```

---

### H-4 — Null Pointer Dereference After `TabWidgetsModel::loadObject` in `TabListModel::load`

**File:** `QDash/Native/Models/TabListModel.cpp`  
**Lines:** 212–228  
**Type:** Null Dereference  

#### Description

After calling `TabWidgetsModel::loadObject`, the result is immediately dereferenced without a null check:

```cpp
t.model = TabWidgetsModel::loadObject(this, obj.value("widgets").toArray());
t.model->setCols(t.cols);   // crash if loadObject returns null
t.model->setRows(t.rows);
```

While `loadObject` currently always returns a heap-allocated object and does not return `nullptr`, this assumption is fragile. Any future change that introduces an early return (e.g. for error handling) without also checking all call sites will silently crash here.

#### Recommendation

```cpp
t.model = TabWidgetsModel::loadObject(this, obj.value("widgets").toArray());
if (!t.model) {
    m_logs->critical("Layout", "Failed to create widget model for tab " + t.title);
    continue;
}
```

---

### H-5 — Default `rowSpan`/`colSpan` of 0 Produces Invalid Widget Geometry

**File:** `QDash/Native/Models/TabWidgetsModel.cpp`  
**Lines:** 274–275  
**Type:** Logic Error / Data Corruption  

#### Description

When loading a widget from JSON, `rowSpan` and `colSpan` fall back to `0` if the key is missing or malformed:

```cpp
w.rowSpan = obj.value("rowSpan").toInt(0);
w.colSpan = obj.value("colSpan").toInt(0);
```

A span of `0` is not a valid value: it results in a zero-width/zero-height widget rectangle. `QRect(col, row, 0, 0)` will never intersect anything, so collision detection is silently bypassed and widgets with a span of 0 are placed without any overlap checking. In QML, `grid.colWidth * 0 - 16 = -16` pixels is set as the widget dimension, producing invisible or negatively-sized items.

#### Recommendation

Use `1` as the default span:

```cpp
w.rowSpan = obj.value("rowSpan").toInt(1);
w.colSpan = obj.value("colSpan").toInt(1);
```

---

### H-6 — `QTimer` / `QMetaObject::Connection` Leak on Failed `invokeMethod`

**File:** `QDash/Native/NT/TopicStore.cpp`  
**Lines:** 72–90  
**Type:** Resource Leak  

#### Description

In `Listener::update`, a `QTimer` and heap-allocated `QMetaObject::Connection` are created before calling `QMetaObject::invokeMethod`. If `invokeMethod` returns `false` (e.g. the target thread has exited), the timer is never started, its `timeout` signal never fires, and `deleteLater()` on the timer is never called. The heap-allocated `Connection` object is similarly never freed:

```cpp
QTimer* timer = new QTimer();
...
QMetaObject::Connection *conn = new QMetaObject::Connection;
*conn = connect(timer, &QTimer::timeout, ...);

QMetaObject::invokeMethod(timer, "start", Qt::QueuedConnection, ...);
// if invokeMethod fails, timer and conn leak
```

#### Recommendation

Check the return value of `invokeMethod` and clean up on failure:

```cpp
bool ok = QMetaObject::invokeMethod(timer, "start", Qt::QueuedConnection, Q_ARG(int, 0));
if (!ok) {
    disconnect(*conn);
    delete conn;
    timer->deleteLater();
}
```

---

## Medium Severity

---

### M-1 — Unvalidated HTTP Request / Lack of TLS in `RemoteLayoutModel`

**File:** `QDash/Native/Models/RemoteLayoutModel.cpp`  
**Lines:** 62–90  
**Type:** Network Security (Plain-text HTTP)  

#### Description

All requests to fetch and download remote layouts use plain-text HTTP:

```cpp
QUrl url = "http://" + ip + ":5800/?format=json";
...
QUrl url = "http://" + ip + ":5800/" + name;
```

On a robot competition network, traffic is not encrypted. A malicious actor on the same network segment (or a compromised robot) can perform a man-in-the-middle attack to inject arbitrary JSON responses or file content. Because the downloaded file is then loaded directly into the layout engine (see `RemoteLayoutsDialog.qml`), injected content could cause the dashboard to behave unexpectedly.

The robot IP is taken directly from the first NetworkTables connection without format validation.

#### Recommendation

- Validate the IP string against an IPv4/IPv6 pattern before using it in a URL.
- If the robot's web server supports HTTPS (WPILib WebServer supports it), prefer it.
- At minimum, document that the remote-layout feature is only safe on a trusted network.

---

### M-2 — Unhandled Network Error in `RemoteLayoutModel::load` and `download`

**File:** `QDash/Native/Models/RemoteLayoutModel.cpp`  
**Lines:** 69–91, 114–130  
**Type:** Missing Error Handling  

#### Description

Neither the `load()` nor the `download()` lambda checks `reply->error()` before reading the response. If the HTTP request fails (connection refused, timeout, HTTP 4xx/5xx), `reply->readAll()` returns an empty byte array, `QJsonDocument::fromJson()` returns a null document, and `doc.object()` returns an empty object — silently producing an empty list with no user feedback.

```cpp
connect(reply, &QNetworkReply::finished, this, [this, reply, ip] {
    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    // no error check
    QJsonObject obj = doc.object();
    ...
```

Additionally, the `QNetworkReply` objects are never explicitly deleted. Qt's parent–child ownership mechanism (via `m_manager`) will eventually clean them up, but `deleteLater()` should be called explicitly inside the lambda to release the reply promptly after use.

#### Recommendation

```cpp
connect(reply, &QNetworkReply::finished, this, [this, reply, ip] {
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        emit failed();
        return;
    }
    ...
```

---

### M-3 — Shell Logic Bug in `ci_package` Guard Condition

**File:** `tools/cpm/package/fetch.sh`  
**Line:** ~75  
**Type:** Shell Script Logic Error  

#### Description

The guard at the start of `ci_package()` is:

```sh
[ "$REPO" != null ] || echo "-- ! No repo defined" && return
```

Due to shell operator precedence, `&&` binds more tightly than `||`. This expression is parsed as:

```sh
[ "$REPO" != null ] || (echo "-- ! No repo defined" && return)
```

When `$REPO` is not `null` (the normal case), the `||` branch is skipped, so `return` is never executed — correct. However when `$REPO` *is* `null`, the `echo` is printed but `return` may not be evaluated as a function return in all POSIX shells when used inside an `&&` chain. The safe and unambiguous form requires braces:

```sh
[ "$REPO" != null ] || { echo "-- ! No repo defined"; return; }
```

#### Recommendation

Replace the guard line with the brace-grouped form above.

---

### M-4 — `RemoteLayoutsDialog.qml` Uses Undefined Variable in Path Construction

**File:** `QDash/Dialogs/remote/RemoteLayoutsDialog.qml`  
**Lines:** 24–27  
**Type:** QML Runtime Error / Wrong Behavior  

#### Description

Inside `onAccepted`, the default save path passed to `FileSelect.getSaveFileName` uses `filename` before it has been assigned:

```qml
onAccepted: {
    selected = rlm.url(list.currentIndex)
    let filename = FileSelect.getSaveFileName(
            qsTr("Save Layout"), StandardPaths.writableLocation(
                StandardPaths.AppLocalDataLocation) + "/" + filename,  // 'filename' is undefined here
            "JSON files (*.json);;All files (*)")
    rlm.download(selected, filename)
}
```

`filename` is declared on the same line as its use in the default-path expression. In JavaScript, the variable is hoisted but not yet initialized (temporal dead zone for `let`), so the second argument to `getSaveFileName` will contain `undefined` in place of a meaningful default path.

#### Recommendation

Use a different identifier for the temporary variable, or construct the default path separately:

```qml
onAccepted: {
    selected = rlm.url(list.currentIndex)
    let defaultPath = StandardPaths.writableLocation(StandardPaths.AppLocalDataLocation) + "/layout.json"
    let filename = FileSelect.getSaveFileName(qsTr("Save Layout"), defaultPath, "JSON files (*.json);;All files (*)")
    rlm.download(selected, filename)
}
```

---

### M-5 — `reportError` Releases Lock Before Publishing, Risking Stale Publish

**File:** `lib/QFRCLib.cpp`  
**Lines:** 71–95  
**Type:** TOCTOU / Race Condition  

#### Description

`reportError` appends to `errorsDeque` inside one `lock_guard` scope, then calls `publishErrors()` after the lock is released. `publishErrors()` re-acquires the same mutex. Between the first unlock and the second lock, another thread may call `reportError` again, appending another entry. The second call's `publishErrors()` will then include the first call's entry too, so the first publish is effectively a no-op and one NT update is wasted. More critically, if `publishErrors()` is called from a thread that holds the lock for another reason, this pattern could produce a subtle double-publish ordering issue.

This is a correctness and performance concern rather than a memory-safety one, but the intent of using the mutex (preventing NT from seeing partially-written data) is partially defeated.

#### Recommendation

Options include (a) keeping a local copy of the deque snapshot before releasing the lock and publishing from the snapshot, or (b) making `publishErrors()` an internal helper that is called while the caller already holds the lock (by extracting its body).

---

## Low Severity

---

### L-1 — Log File Error Message Says "reading" Instead of "writing"

**File:** `QDash/Native/Logging/Logger.cpp`  
**Line:** 30  
**Type:** Incorrect Error Message  

#### Description

```cpp
qCritical() << "Failed to open log file for reading.";
```

The file is opened with `QIODevice::Append | QIODevice::WriteOnly`, meaning the error is a failure to open for *writing*, not reading.

#### Recommendation

```cpp
qCritical() << "Failed to open log file for writing.";
```

---

### L-2 — `goto` Used in C++ Code (`TopicStore::toValue`)

**File:** `QDash/Native/NT/TopicStore.cpp`  
**Lines:** 268–308  
**Type:** Code Quality / Maintainability  

#### Description

`toValue` uses a `goto end` pattern to jump to a `return nt::Value()` statement at the function's end:

```cpp
nt::Value TopicStore::toValue(const QVariant& value) {
    if (!value.isValid())
        goto end;
    switch (value.typeId()) {
    ...
    }
    ...
end:
    return nt::Value();
}
```

Using `goto` in C++ is strongly discouraged. It circumvents RAII destructors for objects with automatic storage duration and makes control flow harder to reason about. In this particular function there are no local objects with destructors that would be skipped, so the code is not currently unsafe — but it is fragile.

#### Recommendation

Replace with an early-return pattern:

```cpp
if (!value.isValid())
    return nt::Value();
```

---

### L-3 — `cpmfile.json` Hash Lengths Are Inconsistent / Suspicious

**File:** `cpmfile.json`  
**Type:** Build Integrity  

#### Description

The SHA-512 hashes stored for `ntcore` and `carboxyl` are 127 hex characters long, whereas a valid SHA-512 digest is exactly 128 hex characters. The `zlib` hash is also 127 characters. All three hashes appear to be missing one hex character, which would cause the integrity check in `fetch.sh` to always fail (producing a mismatch against the correctly-computed hash of the downloaded archive).

```json
"hash": "7055b38800aa00cf7d44b2ed38367c3b6f1f8c6e5022b7ba34c7088a7710d187358ec2bdfea919524cdd5daba28c875cd8d18d3feec55295881afe79f909d0ad"
```
(127 characters; should be 128)

#### Recommendation

Recompute and record correct SHA-512 digests for all packages. Consider adding a CI check that validates hash lengths before any build proceeds.

---

### L-4 — Bare `catch(...)` Silences All Exceptions in `QFRCLib::sendNotification`

**File:** `lib/QFRCLib.cpp`  
**Lines:** 144–147  
**Type:** Error Handling / Debuggability  

#### Description

```cpp
try {
    std::string s = j.dump();
    notificationsPublisher.Set(s);
} catch (...) {
    // HNNNNNG
}
```

Catching all exception types with `...` and providing no diagnostic output makes it impossible to know when or why notification publishing fails. This will silently swallow both expected errors (e.g. JSON serialization failures) and unexpected ones (e.g. memory allocation failures).

#### Recommendation

At minimum, log the failure:

```cpp
} catch (const std::exception& e) {
    wpi::print(stderr, "sendNotification: exception: {}\n", e.what());
} catch (...) {
    wpi::print(stderr, "sendNotification: unknown exception\n");
}
```

---

## Informational

---

### I-1 — Unhandled Tab-Switch When Selected Tab Does Not Exist

**File:** `QDash/Native/Models/TabListModel.cpp` — `selectTab`  
**Type:** Silent Failure  

If the robot sends a `/QDash/Tab` value for a tab name that does not exist in the current layout, `selectTab` logs a warning but leaves `m_selectedTab` unchanged. No signal is emitted and the UI does not update. This is the correct defensive behaviour but is worth noting: the robot operator will get no visual confirmation that the tab switch request was ignored.

---

### I-2 — `QDashApplication::reload` Passes Original `argv` Arguments to Restarted Process

**File:** `QDash/Native/QDashApplication.cpp`  
**Lines:** 72–76  
**Type:** Informational  

`QApplication::arguments().mid(1)` re-uses the command-line arguments of the current process when spawning the reloaded instance. This is correct and intentional, but if someone invokes QDash with user-supplied arguments (e.g. via a shell script or launcher), those arguments persist across reloads without re-validation.

---

### I-3 — Robot IP Is Not Validated Before URL Construction

**File:** `QDash/Native/Models/RemoteLayoutModel.cpp`  
**Line:** 63  
**Type:** Informational / Defensive Programming  

`info.remote_ip` is taken directly from the NetworkTables connection info structure and inserted into a URL string without format validation. While in practice this value comes from the WPILib NT library and is well-formed, an adversarial NT server could theoretically provide a crafted IP string (e.g. containing `@`, `?`, or `#` characters) that changes the semantics of the constructed URL.

---

## Summary Table

| ID  | Severity  | File                                       | Description                                                |
|-----|-----------|--------------------------------------------|-------------------------------------------------------------|
| C-1 | Critical  | `Models/RemoteLayoutModel.cpp`             | Path traversal / arbitrary file write in `download()`       |
| C-2 | Critical  | `Managers/ConnManager.h`                   | Uninitialized `bool m_connected` — undefined behaviour      |
| C-3 | Critical  | `NT/TopicStore.cpp`                        | `size_t` loop variable over `int64_t` array corrupts data   |
| H-1 | High      | `Models/RemoteLayoutModel.cpp`             | Out-of-bounds access in `name(int)` / `url(int)`            |
| H-2 | High      | `Models/AccentsListModel.cpp`              | Crash on empty accent name in `names()`                     |
| H-3 | High      | `NT/TopicStore.cpp`                        | `hasEntry()` logic is inverted                              |
| H-4 | High      | `Models/TabListModel.cpp`                  | Null dereference after `loadObject()` without null check    |
| H-5 | High      | `Models/TabWidgetsModel.cpp`               | Default span of 0 produces invalid widget geometry          |
| H-6 | High      | `NT/TopicStore.cpp`                        | `QTimer` / `Connection` leak if `invokeMethod` fails        |
| M-1 | Medium    | `Models/RemoteLayoutModel.cpp`             | Plain-text HTTP; unvalidated IP in URL                      |
| M-2 | Medium    | `Models/RemoteLayoutModel.cpp`             | No network-error handling; replies not deleted promptly     |
| M-3 | Medium    | `tools/cpm/package/fetch.sh`               | Shell `&&`/`\|\|` precedence bug in guard clause            |
| M-4 | Medium    | `Dialogs/remote/RemoteLayoutsDialog.qml`   | Self-referencing `filename` variable in `onAccepted`        |
| M-5 | Medium    | `lib/QFRCLib.cpp`                          | Lock released before publish; stale publish possible        |
| L-1 | Low       | `Logging/Logger.cpp`                       | Wrong error message ("reading" vs "writing")                |
| L-2 | Low       | `NT/TopicStore.cpp`                        | `goto` usage — fragile control flow                         |
| L-3 | Low       | `cpmfile.json`                             | All three package hashes are 127 chars (should be 128)      |
| L-4 | Low       | `lib/QFRCLib.cpp`                          | Bare `catch(...)` silences all exceptions                   |
