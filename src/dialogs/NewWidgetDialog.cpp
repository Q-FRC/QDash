#include "dialogs/NewWidgetDialog.h"
#include "dialogs/NewBooleanCheckboxDialog.h"
#include "dialogs/NewBooleanDisplayDialog.h"
#include "dialogs/NewStringDisplayDialog.h"
#include "dialogs/NewDoubleDisplayDialog.h"
#include "dialogs/NewDoubleDialDialog.h"
#include "dialogs/NewStringChooserDialog.h"

#include "Globals.h"

#include <QColorDialog>

// default to a zeroed-out data
NewWidgetDialog::NewWidgetDialog(std::string ntTopic, QWidget *parent = nullptr, const WidgetData &data = WidgetData{0, 0, 0, 0, 0}) : QDialog(parent)
{
    bool isNewWidget = (data == WidgetData{0, 0, 0, 0, 0});

    m_layout = new QFormLayout(this);

    if (isNewWidget) {
        m_nameLabel = new QLabel("Name:", this);
        m_nameInput = new QLineEdit(QString::fromStdString(ntTopic), this);

        m_layout->addRow(m_nameLabel, m_nameInput);
    } else {
        // create an invisible dummy
        // due to a skill issue this is the simplest solution to
        // prevent a nullptr dereference in the subclass connections.

        m_nameInput = new QLineEdit("dummy", this);
        m_nameInput->setVisible(false);
    }

    m_rowLabel = new QLabel("Row:", this);
    m_rowInput = new QSpinBox(this);
    m_rowInput->setRange(0, 1000);
    m_rowInput->setValue(data.row);

    m_layout->addRow(m_rowLabel, m_rowInput);

    m_columnLabel = new QLabel("Column:", this);
    m_columnInput = new QSpinBox(this);
    m_columnInput->setRange(0, 1000);
    m_columnInput->setValue(data.col);

    m_layout->addRow(m_columnLabel, m_columnInput);

    m_rowSpanLabel = new QLabel("Row Span:", this);
    m_rowSpanInput = new QSpinBox(this);
    m_rowSpanInput->setRange(0, 1000);
    m_rowSpanInput->setValue((data.rowSpan == 0 ? 1 : data.rowSpan));

    m_layout->addRow(m_rowSpanLabel, m_rowSpanInput);

    m_columnSpanLabel = new QLabel("Column Span:", this);
    m_columnSpanInput = new QSpinBox(this);
    m_columnSpanInput->setRange(0, 1000);
    m_columnSpanInput->setValue((data.colSpan == 0 ? 1 : data.colSpan));

    m_layout->addRow(m_columnSpanLabel, m_columnSpanInput);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);
}

NewWidgetDialog::~NewWidgetDialog() {}

NewWidgetDialog *NewWidgetDialog::fromWidgetType(WidgetTypes type, std::string ntTopic, QWidget *parent, const WidgetData &data) {
    switch(type) {
    case WidgetTypes::BooleanCheckbox: {
        return new NewBooleanCheckboxDialog(ntTopic, parent, data);
        break;
    }
    case WidgetTypes::BooleanDisplay: {
        return new NewBooleanDisplayDialog(ntTopic, parent, data);
        break;
    }
    case WidgetTypes::DoubleDisplay: {
        return new NewDoubleDisplayDialog(ntTopic, parent, data);
        break;
    }
    case WidgetTypes::DoubleDial: {
        return new NewDoubleDialDialog(ntTopic, parent, data);
        break;
    }
    case WidgetTypes::SendableChooser: {
        return new NewStringChooserDialog(ntTopic, parent, data);
        break;
    }
    case WidgetTypes::StringDisplay:
    default: {
        return new NewStringDisplayDialog(ntTopic, parent, data);
        break;
    }
    }
}
