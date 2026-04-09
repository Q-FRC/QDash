// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
pragma Singleton
import QtQml 2.15

QtObject {
    // Maps a dataType string to a list of available widget type descriptors.
    // Each entry has:
    //   name     – human-readable label shown in the "Switch Widget…" menu
    //   type     – internal widget type string used as model.type / DelegateChoice roleValue
    //   readOnly – true when the widget only displays data (cannot write back to NT)
    //
    // This is the single source of truth for widget switching.
    // Filtering out the currently-active widgetType is done at call-site (see BaseWidget).

    readonly property var typeMap: ({
        "double": [
            { "name": "Spin Box",      "type": "double",       "readOnly": false },
            { "name": "Dial",          "type": "doubleDial",   "readOnly": false },
            { "name": "Radial Gauge",  "type": "doubleGauge",  "readOnly": true  },
            { "name": "Progress Bar",  "type": "doubleBar",    "readOnly": true  },
            { "name": "Number Display","type": "doubleDisplay","readOnly": true  },
            { "name": "Match Time",    "type": "matchTime",    "readOnly": true  },
            { "name": "Phase Display", "type": "phaseShift",   "readOnly": true  }
        ],
        "int": [
            { "name": "Spin Box",      "type": "int",          "readOnly": false },
            { "name": "Dial",          "type": "dial",         "readOnly": false },
            { "name": "Radial Gauge",  "type": "gauge",        "readOnly": true  },
            { "name": "Number Display","type": "intDisplay",   "readOnly": true  }
        ],
        "bool": [
            { "name": "Checkbox",      "type": "bool",         "readOnly": false },
            { "name": "Color Display", "type": "color",        "readOnly": true  }
        ],
        "string": [
            { "name": "Text Field",    "type": "string",       "readOnly": false },
            { "name": "Text Display",  "type": "textDisplay",  "readOnly": true  },
            { "name": "Color",         "type": "colorText",    "readOnly": true  }
        ]
    })

    // Returns the list of widget type entries for the given dataType,
    // excluding the entry whose type matches currentWidgetType.
    function widgetsForType(dataType, currentWidgetType) {
        let options = typeMap[dataType]
        if (!options)
            return []
        return options.filter(function(w) { return w.type !== currentWidgetType })
    }
}
