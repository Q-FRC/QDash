#include "Globals.h"

#include <QStringList>

nt::NetworkTableInstance Globals::inst = nt::NetworkTableInstance::GetDefault();
QString Globals::server = "0.0.0.0";

QStringList Globals::ntTopics{};
QMap<QString, Globals::TopicTypes> Globals::availableTopics;

WidgetData makeWidgetData(int row, int col, int rowSpan, int colSpan, int tabIdx) {
    WidgetData data;
    data.row = row;
    data.col = col;
    data.rowSpan = rowSpan;
    data.colSpan = colSpan;
    data.tabIdx = tabIdx;

    return data;
}

bool operator==(const WidgetData &a, const WidgetData &b) {
    return (a.row == b.row) &&
           (a.col == b.col) &&
           (a.rowSpan == b.rowSpan) &&
           (a.colSpan == b.rowSpan) &&
           (a.tabIdx == b.tabIdx);
}
