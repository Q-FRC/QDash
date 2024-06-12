#include "dialogs/NewCameraViewDialog.h"
#include "widgets/CameraViewWidget.h"

NewCameraViewDialog::NewCameraViewDialog() : NewWidgetDialog("")
{
    m_urlEdit = new QLineEdit(this);

    m_layout->addRow("Camera URL", m_urlEdit);
    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this] {
        QString name = m_nameInput->text();
        QList<int> data({m_rowInput->value(), m_columnInput->value(), m_rowSpanInput->value(), m_columnSpanInput->value()});

        CameraViewWidget *widget = new CameraViewWidget(name, QUrl(m_urlEdit->text()));

        emit widgetReady(widget, data);
        close();
    });
}

NewCameraViewDialog::~NewCameraViewDialog() {}
