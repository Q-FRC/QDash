#include "dialogs/NewBooleanCheckboxDialog.h"

#include "widgets/BooleanCheckboxWidget.h"

NewBooleanCheckboxDialog::NewBooleanCheckboxDialog(std::string ntTopic) : NewWidgetDialog(ntTopic)
{
    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this, ntTopic] {
        QString name = m_nameInput->text();
        QList<int> data({m_rowInput->value(), m_columnInput->value(), m_rowSpanInput->value(), m_columnSpanInput->value()});

        BooleanCheckboxWidget *widget = new BooleanCheckboxWidget(name, false, QString::fromStdString(ntTopic));

        emit widgetReady(widget, data);
        close();
    });
}

NewBooleanCheckboxDialog::~NewBooleanCheckboxDialog() {}
