#include "dialogs/NewEnumWidgetDialog.h"

#include "widgets/EnumWidget.h"

#include <QColorDialog>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>

NewEnumWidgetDialog::NewEnumWidgetDialog(std::string ntTopic, QWidget *parent = nullptr, WidgetData data = WidgetData{0, 0, 0, 0, 0}) : NewWidgetDialog(ntTopic, parent, data)
{
    m_tableWidget = new QTableWidget(0, 2, this);

    m_addItemButton = new QPushButton("Add", this);
    connect(m_addItemButton, &QPushButton::clicked, this, [this] {
        int rowCount = m_tableWidget->rowCount();

        m_tableWidget->insertRow(rowCount);
    });

    m_removeItemButton = new QPushButton("Delete", this);
    m_removeItemButton->setShortcut(Qt::Key_Delete);
    connect(m_removeItemButton, &QPushButton::clicked, this, [this] {
        QList<QTableWidgetItem *> selectedItems = m_tableWidget->selectedItems();

        if (selectedItems.empty()) return;

        m_tableWidget->removeRow(selectedItems.at(0)->row());
    });

    m_colorButton = new QPushButton("Insert Color", this);
    connect(m_colorButton, &QPushButton::clicked, this, [this] {
        QString color = QColorDialog::getColor(Qt::white, this, "Select Color").name();
        QList<QTableWidgetItem *> selectedItems = m_tableWidget->selectedItems();

        if (selectedItems.empty()) {
            qApp->clipboard()->setText(color);
            QMessageBox::information(this, "Copied to Clipboard", "Copied color selection to clipboard.");
            return;
        }

        int row = selectedItems.at(0)->row();

        m_tableWidget->item(row, 1)->setText(color);
    });

    m_buttonLayout = new QHBoxLayout;
    m_buttonLayout->addWidget(m_addItemButton);
    m_buttonLayout->addWidget(m_removeItemButton);
    m_buttonLayout->addWidget(m_colorButton);

    m_layout->addWidget(m_tableWidget);
    m_layout->addRow(m_buttonLayout);

    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this, ntTopic] {
        QString name = m_nameInput->text();
        WidgetData data;
        data.row = m_rowInput->value();
        data.col = m_columnInput->value();
        data.rowSpan = m_rowSpanInput->value();
        data.colSpan = m_columnSpanInput->value();

        EnumWidget *widget = new EnumWidget(name, "", QString::fromStdString(ntTopic));

        widget->setColors(serializeTable());

        emit widgetReady(widget, data);
        close();
    });
}

NewEnumWidgetDialog::~NewEnumWidgetDialog() {}

QMap<QString, QColor> NewEnumWidgetDialog::serializeTable() {
    QMap<QString, QColor> colorMap{};

    for (int i = 0; i < m_tableWidget->rowCount(); ++i) {
        QString key = m_tableWidget->item(i, 0)->text();
        QString color = m_tableWidget->item(i, 1)->text();

        colorMap.insert(key, QColor(color));
    }

    return colorMap;
}
