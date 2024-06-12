#include "dialogs/NewWidgetDialog.h"
#include "dialogs/NewBooleanCheckboxDialog.h"
#include "dialogs/NewBooleanDisplayDialog.h"
#include "dialogs/NewStringDisplayDialog.h"
#include "dialogs/NewDoubleDisplayDialog.h"
#include "dialogs/NewDoubleDialDialog.h"
#include "dialogs/NewStringChooserDialog.h"

#include "Globals.h"

#include <QColorDialog>

NewWidgetDialog::NewWidgetDialog(std::string ntTopic)
{
    m_entry = Globals::inst.GetEntry(ntTopic);

    m_layout = new QFormLayout(this);

    m_nameLabel = new QLabel("Name:", this);
    m_nameInput = new QLineEdit(QString::fromStdString(ntTopic), this);

    m_layout->addRow(m_nameLabel, m_nameInput);  

    m_rowLabel = new QLabel("Row:", this);
    m_rowInput = new QSpinBox(this);
    m_rowInput->setRange(0, 1000);
    m_rowInput->setValue(0);

    m_layout->addRow(m_rowLabel, m_rowInput);

    m_columnLabel = new QLabel("Column:", this);
    m_columnInput = new QSpinBox(this);
    m_columnInput->setRange(0, 1000);
    m_columnInput->setValue(0);

    m_layout->addRow(m_columnLabel, m_columnInput);

    m_rowSpanLabel = new QLabel("Row Span:", this);
    m_rowSpanInput = new QSpinBox(this);
    m_rowSpanInput->setRange(0, 1000);
    m_rowSpanInput->setValue(1);

    m_layout->addRow(m_rowSpanLabel, m_rowSpanInput);

    m_columnSpanLabel = new QLabel("Column Span:", this);
    m_columnSpanInput = new QSpinBox(this);
    m_columnSpanInput->setRange(0, 1000);
    m_columnSpanInput->setValue(1);

    m_layout->addRow(m_columnSpanLabel, m_columnSpanInput);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect(m_buttonBox, &QDialogButtonBox::rejected, m_buttonBox, &QDialogButtonBox::close);
}

NewWidgetDialog::~NewWidgetDialog() {
    m_entry.Unpublish();
}

NewWidgetDialog *NewWidgetDialog::fromWidgetType(NewWidgetDialog::WidgetTypes type, std::string ntTopic) {
    switch(type) {
    case WidgetTypes::BooleanCheckbox: {
        return new NewBooleanCheckboxDialog(ntTopic);
        break;
    }
    case WidgetTypes::BooleanDisplay: {
        return new NewBooleanDisplayDialog(ntTopic);
        break;
    }
    case WidgetTypes::DoubleDisplay: {
        return new NewDoubleDisplayDialog(ntTopic);
        break;
    }
    case WidgetTypes::DoubleDial: {
        return new NewDoubleDialDialog(ntTopic);
        break;
    }
    case WidgetTypes::SendableChooser: {
        return new NewStringChooserDialog(ntTopic);
        break;
    }
    case WidgetTypes::StringDisplay:
    default: {
        return new NewStringDisplayDialog(ntTopic);
        break;
    }
    }
}
