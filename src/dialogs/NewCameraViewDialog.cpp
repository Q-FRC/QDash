#include "dialogs/NewCameraViewDialog.h"
#include "widgets/CameraViewWidget.h"

NewCameraViewDialog::NewCameraViewDialog(QWidget *parent, WidgetData data) : NewWidgetDialog("", parent, data)
{
    m_urlEdit = new QLineEdit(this);

    m_layout->addRow("Camera URL", m_urlEdit);
    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this] {
        QString name = m_nameInput->text();
        WidgetData data;
        data.row = m_rowInput->value();
        data.col = m_columnInput->value();
        data.rowSpan = m_rowSpanInput->value();
        data.colSpan = m_columnSpanInput->value();

        CameraViewWidget *widget = new CameraViewWidget(name, QUrl(m_urlEdit->text()));

        emit widgetReady(widget, data);
        close();
    });
}

NewCameraViewDialog::~NewCameraViewDialog() {}
