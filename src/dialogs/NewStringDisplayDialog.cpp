#include "dialogs/NewStringDisplayDialog.h"

#include "widgets/StringDisplayWidget.h"

NewStringDisplayDialog::NewStringDisplayDialog(std::string ntTopic) : NewWidgetDialog(ntTopic)
{
    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this, ntTopic] {
        QString name = m_nameInput->text();
        QList<int> data({m_rowInput->value(), m_columnInput->value(), m_rowSpanInput->value(), m_columnSpanInput->value()});

        StringDisplayWidget *widget = new StringDisplayWidget(name, "", QString::fromStdString(ntTopic));

        emit widgetReady(widget, data);
        close();
    });
}

NewStringDisplayDialog::~NewStringDisplayDialog() {}
