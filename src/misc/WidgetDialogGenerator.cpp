#include "misc/WidgetDialogGenerator.h"

#include "widgets/BaseWidget.h"

#include <QMetaObject>

#include <QDoubleSpinBox>
#include <QColorDialog>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidgetItem>

bool operator<(QMetaProperty a, QMetaProperty b) {
    return a.name() < a.name();
}

WidgetDialogGenerator::WidgetDialogGenerator(BaseWidget *widget, bool isResize, WidgetData data)
{
    m_widget = widget;
    m_isResize = isResize;

    QFormLayout *m_layout = new QFormLayout(this);

    m_nameInput = new QLineEdit(isResize ? widget->title() : "", this);

    m_layout->addRow("Name:", m_nameInput);

    m_rowInput = new QSpinBox(this);
    m_rowInput->setRange(0, 1000);
    m_rowInput->setValue(data.row);

    m_layout->addRow("Row:", m_rowInput);

    m_columnInput = new QSpinBox(this);
    m_columnInput->setRange(0, 1000);
    m_columnInput->setValue(data.col);

    m_layout->addRow("Column:", m_columnInput);

    m_rowSpanInput = new QSpinBox(this);
    m_rowSpanInput->setRange(0, 1000);
    m_rowSpanInput->setValue((data.rowSpan == 0 ? 1 : data.rowSpan));

    m_layout->addRow("Row Span:", m_rowSpanInput);

    m_columnSpanInput = new QSpinBox(this);
    m_columnSpanInput->setRange(0, 1000);
    m_columnSpanInput->setValue((data.colSpan == 0 ? 1 : data.colSpan));

    m_layout->addRow("Column Span:", m_columnSpanInput);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);

    // only properties at BaseWidget and above
    int offset = BaseWidget::staticMetaObject.propertyOffset();
    int propertyCount = widget->metaObject()->propertyCount();

    for (int i = offset; i < propertyCount; ++i) {
        QMetaProperty property = widget->metaObject()->property(i);
        if (!property.isRequired()) {
            continue;
        }

        QWidget *widgetToAdd;

        switch (property.metaType().id()) {
            case QMetaType::Double: {
                // QDoubleSpinBox
                QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
                spinBox->setRange(-1000000, 1000000);
                /*if (isResize)*/ spinBox->setValue(property.read(widget).toDouble());

                auto func = std::bind(&QDoubleSpinBox::value, spinBox);
                bindMetaProperty(property, func);

                widgetToAdd = spinBox;
                break;
            }
            case QMetaType::Int: {
                // QSpinBox
                QSpinBox *spinBox = new QSpinBox(this);
                spinBox->setRange(-1000000, 1000000);
                /*if (isResize)*/ spinBox->setValue(property.read(widget).toInt());

                auto func = std::bind(&QSpinBox::value, spinBox);
                bindMetaProperty(property, func);

                widgetToAdd = spinBox;
                break;
            }
            case QMetaType::QColor: {
                // QColorDialog etc.
                QWidget *input = new QWidget(this);
                QHBoxLayout *inputLayout = new QHBoxLayout(input);

                QLineEdit *colorEdit = new QLineEdit(this);
                /*if (isResize)*/ colorEdit->setText(property.read(widget).toString());

                QPushButton *colorSelect = new QPushButton("Select Color...", this);

                inputLayout->addWidget(colorEdit);
                inputLayout->addWidget(colorSelect);

                connect(colorSelect, &QPushButton::clicked, this, [colorEdit, this] {
                    QColor color = QColorDialog::getColor(QColor::fromString(colorEdit->text()), this);

                    colorEdit->setText(color.name());
                });

                auto func = std::bind(&QLineEdit::text, colorEdit);
                bindMetaProperty(property, func);

                widgetToAdd = input;
                break;
            }
            case QMetaType::QVariantMap: {
                // Table setup
                // An insert button for certain types as well.

                // TODO: serialize map into table
                QTableWidget *table = new QTableWidget(0, 2, this);

                QPushButton *addButton = new QPushButton("Add", this);
                connect(addButton, &QPushButton::clicked, this, [table] {
                    int rowCount = table->rowCount();

                    table->insertRow(rowCount);
                });

                QPushButton *removeButton = new QPushButton("Delete", this);
                removeButton->setShortcut(Qt::Key_Delete);
                connect(removeButton, &QPushButton::clicked, this, [table] {
                    QList<QTableWidgetItem *> selectedItems = table->selectedItems();

                    if (selectedItems.empty()) return;

                    table->removeRow(selectedItems.at(0)->row());
                });

                QWidget *tableLayoutWidget = new QWidget;

                QVBoxLayout *tableLayout = new QVBoxLayout(tableLayoutWidget);

                QHBoxLayout *buttons = new QHBoxLayout;

                buttons->addWidget(addButton);
                buttons->addWidget(removeButton);
//                buttons->addWidget(m_colorButton);

//                m_layout->addWidget(m_tableWidget);
//                m_layout->addRow(m_buttonLayout);

                tableLayout->addWidget(table);
                tableLayout->addLayout(buttons);

                widgetToAdd = tableLayoutWidget;

                // TODO: Insert buttons for certain types (images, etc)

                auto func = std::bind(&WidgetDialogGenerator::serializeTable, this, table);
                bindMetaProperty(property, func);
                break;
            }
            case QMetaType::QBitmap: {
                // idk what type to ues
                // but a shape select
                qCritical() << "Not implemented";
                break;
            }
            case QMetaType::QImage: {
                // image
                qCritical() << "Not Implemented";
                break;
            }
            case QMetaType::QFont: {
                // font dialog
            }
            case QMetaType::QString:
            case QMetaType::QUrl: {
                // bruh
                QLineEdit *edit = new QLineEdit(this);
                /*if (isResize)*/ edit->setText(property.read(widget).toString());

                auto func = std::bind(&QLineEdit::text, edit);
                bindMetaProperty(property, func);

                widgetToAdd = edit;
                break;
            }
            default: {
                qCritical() << "Bad metatype for property" << property.name();
                break;
            }
        } // switch

        m_layout->addRow(QString(property.name()).replace('_', ' '), widgetToAdd);
    }

    m_layout->addRow(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this] {
        // preset values
        QString name = m_nameInput->text();

        WidgetData widgetData;
        widgetData.row = m_rowInput->value();
        widgetData.col = m_columnInput->value();
        widgetData.rowSpan = m_rowSpanInput->value();
        widgetData.colSpan = m_columnSpanInput->value();

        m_widget->setTitle(name);

        QMultiMapIterator<const QMetaProperty, Getter> iter(m_propertyGetterMap);

        while (iter.hasNext()) {
            iter.next();

            const QMetaProperty property = iter.key();
            Getter getter = iter.value();

            property.write(m_widget, getter());
        }

        emit widgetReady(m_widget, widgetData);
        close();
    });
}

WidgetDialogGenerator::~WidgetDialogGenerator() {}

void WidgetDialogGenerator::bindMetaProperty(const QMetaProperty property, Getter getter) {
    m_propertyGetterMap.insert(property, getter);
}

QVariantMap WidgetDialogGenerator::serializeTable(QTableWidget *widget) {
    QVariantMap variantMap{};

    for (int i = 0; i < widget->rowCount(); ++i) {
        QString key = widget->item(i, 0)->text();
        QString value = widget->item(i, 1)->text();

        variantMap.insert(key, value);
    }

    return variantMap;
}
