import QtQuick
import QtQuick.Dialogs

MessageDialog {
    title: "About QFRCDashboard"

    text: "QFRCDashboard is a reliable, high-performance FRC dashboard with" +
          " a low resource cost, suited for low-end computers and for maximizing" +
          " Driver Station resources.\n\n" +

          buildConfig.buildInfo()

    informativeText: "Copyleft 2023-2024 Q-FRC"

    buttons: MessageDialog.Close
}
