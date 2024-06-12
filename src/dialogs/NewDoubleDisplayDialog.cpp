#include "dialogs/NewDoubleDisplayDialog.h"

#include "widgets/NumberDisplayWidget.h"

NewDoubleDisplayDialog::NewDoubleDisplayDialog(std::string ntTopic) : NewWidgetDialog(ntTopic)
{
    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this, ntTopic] {
        QString name = m_nameInput->text();
        QList<int> data({m_rowInput->value(), m_columnInput->value(), m_rowSpanInput->value(), m_columnSpanInput->value()});

        NumberDisplayWidget *widget = new NumberDisplayWidget(name, 0., QString::fromStdString(ntTopic));

        emit widgetReady(widget, data);
        close();
    });
}

NewDoubleDisplayDialog::~NewDoubleDisplayDialog() {}
