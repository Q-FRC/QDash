#include "dialogs/NewBooleanDisplayDialog.h"

#include "widgets/BooleanDisplayWidget.h"

#include <QColorDialog>

NewBooleanDisplayDialog::NewBooleanDisplayDialog(std::string ntTopic, QWidget *parent = nullptr, WidgetData data = WidgetData{0, 0, 0, 0, 0}) : NewWidgetDialog(ntTopic, parent, data)
{
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

    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this, ntTopic]() {
        QString name = m_nameInput->text();
        QColor trueColor = m_trueColorEdit->text();
        QColor falseColor = m_falseColorEdit->text();

        WidgetData data;
        data.row = m_rowInput->value();
        data.col = m_columnInput->value();
        data.rowSpan = m_rowSpanInput->value();
        data.colSpan = m_columnSpanInput->value();

        BooleanDisplayWidget *widget = new BooleanDisplayWidget(name, false, QString::fromStdString(ntTopic));
        widget->setTrueColor(trueColor);
        widget->setFalseColor(falseColor);

        emit widgetReady(widget, data);
        close();
    });
}

NewBooleanDisplayDialog::~NewBooleanDisplayDialog() {}
