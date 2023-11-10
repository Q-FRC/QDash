#include "dialogs/NewStringChooserDialog.h"

#include "widgets/StringChooserWidget.h"

NewStringChooserDialog::NewStringChooserDialog(std::string ntTopic, QWidget *parent = nullptr, WidgetData data = WidgetData{0, 0, 0, 0, 0}) : NewWidgetDialog(ntTopic, parent, data)
{
    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this, ntTopic] {
        QString name = m_nameInput->text();
        WidgetData data;
        data.row = m_rowInput->value();
        data.col = m_columnInput->value();
        data.rowSpan = m_rowSpanInput->value();
        data.colSpan = m_columnSpanInput->value();

        StringChooserWidget *widget = new StringChooserWidget(name, QString::fromStdString(ntTopic));

        emit widgetReady(widget, data);
        close();
    });
}

NewStringChooserDialog::~NewStringChooserDialog() {}
