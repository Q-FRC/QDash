#include "dialogs/NewWidgetDialog.h"
#include "Globals.h"

#include <QColorDialog>

// TODO: make it work first time around
NewWidgetDialog::NewWidgetDialog(std::string ntTopic)
{
    m_entry = Globals::inst.GetEntry(ntTopic);
    nt::NetworkTableType type = m_entry.GetType();
    qDebug() << m_entry.Exists();

    m_isBooleanDisplay = (type == nt::NetworkTableType::kBoolean);

    m_layout = new QFormLayout(this);

    m_nameLabel = new QLabel("Name:", this);
    m_nameInput = new QLineEdit(QString::fromStdString(ntTopic), this);

    m_layout->addRow(m_nameLabel, m_nameInput);

    if (m_isBooleanDisplay) {
        m_trueColorLabel = new QLabel("True Color:", this);

        m_trueColorInput = new QWidget(this);
        m_trueColorInputLayout = new QHBoxLayout(m_trueColorInput);

        m_trueColorEdit = new QLineEdit(QColor(Qt::green).name(), m_trueColorInput);
        m_trueColorSelect = new QPushButton("Select Color...", m_trueColorInput);

        m_trueColorInputLayout->addWidget(m_trueColorEdit);
        m_trueColorInputLayout->addWidget(m_trueColorSelect);

        m_trueColorInput->setLayout(m_trueColorInputLayout);

        connect(m_trueColorSelect, &QPushButton::clicked, this, [this](bool) {
            QColor color = QColorDialog::getColor(QColor::fromString(m_falseColorEdit->text()), this);

            m_trueColorEdit->setText(color.name());
        });

        m_layout->addRow(m_trueColorLabel, m_trueColorInput);

        m_falseColorLabel = new QLabel("False Color:", this);

        m_falseColorInput = new QWidget(this);
        m_falseColorInputLayout = new QHBoxLayout(m_falseColorInput);

        m_falseColorEdit = new QLineEdit(QColor(Qt::red).name(), m_falseColorInput);
        m_falseColorSelect = new QPushButton("Select Color...", m_falseColorInput);

        m_falseColorInputLayout->addWidget(m_falseColorEdit);
        m_falseColorInputLayout->addWidget(m_falseColorSelect);

        m_falseColorInput->setLayout(m_falseColorInputLayout);

        connect(m_falseColorSelect, &QPushButton::clicked, this, [this](bool) {
            QColor color = QColorDialog::getColor(QColor::fromString(m_falseColorEdit->text()), this);

            m_falseColorEdit->setText(color.name());
        });

        m_layout->addRow(m_falseColorLabel, m_falseColorInput);
    }

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
    m_rowSpanInput->setValue(0);

    m_layout->addRow(m_rowSpanLabel, m_rowSpanInput);

    m_columnSpanLabel = new QLabel("Column Span:", this);
    m_columnSpanInput = new QSpinBox(this);
    m_columnSpanInput->setRange(0, 1000);
    m_columnSpanInput->setValue(0);

    m_layout->addRow(m_columnSpanLabel, m_columnSpanInput);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);

    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, [this, type, ntTopic]() {
        QString name = m_nameInput->text();
        QColor trueColor = m_isBooleanDisplay ? m_trueColorEdit->text() : QColor(Qt::green).name();
        QColor falseColor = m_isBooleanDisplay ? m_falseColorEdit->text() : QColor(Qt::red).name();
        QList<int> data({m_rowInput->value(), m_columnInput->value(), m_rowSpanInput->value(), m_columnSpanInput->value()});

        qDebug() << "helo";
        emit dataReady(ntTopic, type, name, trueColor, falseColor, data);
    });
}

NewWidgetDialog::~NewWidgetDialog() {
    m_entry.Unpublish();
}
