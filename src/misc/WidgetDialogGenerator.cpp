#include "misc/WidgetDialogGenerator.h"

#include "qheaderview.h"
#include "widgets/BaseWidget.h"

#include <QMetaObject>

#include <QDoubleSpinBox>
#include <QColorDialog>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QDir>
#include <QCheckBox>
#include <QFileDialog>

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
        int id = property.typeId();

#define PROPERTY_FUNCTION(type, function) if (id == type) widgetToAdd = function(property); else

        PROPERTY_FUNCTION(QMetaType::Double, doubleProperty)
        PROPERTY_FUNCTION(QMetaType::Int, intProperty)
        PROPERTY_FUNCTION(QMetaType::QColor, colorProperty)
        PROPERTY_FUNCTION(QMetaType::QVariantMap, mapProperty)
        PROPERTY_FUNCTION(QMetaType::QBitmap, bitmapProperty)
        PROPERTY_FUNCTION(QMetaType::QImage, imageProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::File, fileProperty)
        PROPERTY_FUNCTION(QMetaType::QFont, fontProperty)
        PROPERTY_FUNCTION(QMetaType::QString, stringProperty)
        PROPERTY_FUNCTION(QMetaType::QUrl, stringProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::FrameShape, shapeProperty)
        { // else
            qCritical() << "Bad metatype for property" << property.name();
            continue;
        } // else

#undef PROPERTY_FUNCTION

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

        if (!key.isEmpty() && !value.isEmpty()) variantMap.insert(key, value);
    }

    return variantMap;
}

void WidgetDialogGenerator::serializeMap(QVariantMap map, QTableWidget *widget) {
    QMapIterator<QString, QVariant> iter(map);

    int i = 0;
    while (iter.hasNext()) {
        iter.next();

        widget->insertRow(i);
        widget->setItem(i, 0, new QTableWidgetItem(iter.key()));
        widget->setItem(i, 1, new QTableWidgetItem(iter.value().toString()));
        ++i;
    }
}

QWidget *WidgetDialogGenerator::doubleProperty(QMetaProperty property) {
    // QDoubleSpinBox
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
    spinBox->setRange(-1000000, 1000000);
    /*if (isResize)*/ spinBox->setValue(property.read(m_widget).toDouble());

    auto func = std::bind(&QDoubleSpinBox::value, spinBox);
    bindMetaProperty(property, func);

    return spinBox;
}

QWidget *WidgetDialogGenerator::intProperty(QMetaProperty property) {
    // QSpinBox
    QSpinBox *spinBox = new QSpinBox(this);
    spinBox->setRange(-1000000, 1000000);
    /*if (isResize)*/ spinBox->setValue(property.read(m_widget).toInt());

    auto func = std::bind(&QSpinBox::value, spinBox);
    bindMetaProperty(property, func);

    return spinBox;
}

QWidget *WidgetDialogGenerator::colorProperty(QMetaProperty property) {
    // QColorDialog etc.
    QWidget *input = new QWidget(this);
    QHBoxLayout *inputLayout = new QHBoxLayout(input);

    QLineEdit *colorEdit = new QLineEdit(this);
    /*if (isResize)*/ colorEdit->setText(property.read(m_widget).toString());

    QPushButton *colorSelect = new QPushButton("Select Color...", this);

    inputLayout->addWidget(colorEdit);
    inputLayout->addWidget(colorSelect);

    connect(colorSelect, &QPushButton::clicked, this, [colorEdit, this] {
        QColor color = QColorDialog::getColor(QColor::fromString(colorEdit->text()), this);

        colorEdit->setText(color.name());
    });

    auto func = std::bind(&QLineEdit::text, colorEdit);
    bindMetaProperty(property, func);

    return input;
}

QWidget *WidgetDialogGenerator::mapProperty(QMetaProperty property) {
    // Table setup
    // An insert button for certain types as well.
        QTableWidget *table = new QTableWidget(0, 2, this);
    table->horizontalHeader()->setStretchLastSection(false);
    table->setHorizontalHeaderLabels({"Key", "Value"});

    serializeMap(property.read(m_widget).toMap(), table);

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

    tableLayout->addWidget(table);
    tableLayout->addLayout(buttons);

    // TODO: Insert buttons for certain types (images, etc)

    auto func = std::bind(&WidgetDialogGenerator::serializeTable, this, table);
    bindMetaProperty(property, func);

    return tableLayoutWidget;
}

QWidget *WidgetDialogGenerator::bitmapProperty(QMetaProperty property) {
    qCritical() << "Not implemented";
    return nullptr;
}

QWidget *WidgetDialogGenerator::imageProperty(QMetaProperty property) {
    qCritical() << "Not implemented";
    return nullptr;
}

QWidget *WidgetDialogGenerator::fileProperty(QMetaProperty property) {
    QWidget *layoutWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(layoutWidget);

    QComboBox *builtinBox = new QComboBox(this);

    QString value = property.read(m_widget).value<Globals::File>().fileName;

    for(const QString &image : QDir(":").entryList())
    {
        builtinBox->addItem(image);
    }

    QLineEdit *customEdit = new QLineEdit(this);
    QPushButton *fileButton = new QPushButton("Select File...", this);

    connect(fileButton, &QPushButton::clicked, this, [customEdit, this] {
        QString file = QFileDialog::getOpenFileName(this, "Open Image File", QDir::homePath(), "Images (*.png *.xpm *.jpg *.jpeg *.bmp)");
        customEdit->setText(file);
    });

    QCheckBox *switchBox = new QCheckBox("Use Built-In?", this);
    switchBox->setChecked(value.startsWith(":"));

    if (switchBox->isChecked()) {
        builtinBox->setCurrentText(value.mid(2));
    } else {
        customEdit->setText(value);
    }

    // re-lay each time for a smooth transition
    auto updateLayout = [builtinBox, customEdit, fileButton, switchBox, layout, this](bool checked) {
        layout->removeWidget(switchBox);
        if (checked) {
            layout->removeWidget(customEdit);
            layout->removeWidget(fileButton);

            customEdit->hide();
            fileButton->hide();

            layout->addWidget(builtinBox, 1);

            builtinBox->show();
        } else {
            layout->removeWidget(builtinBox);

            builtinBox->hide();

            layout->addWidget(customEdit, 1);
            layout->addWidget(fileButton, 1);

            customEdit->show();
            fileButton->show();
        }
        layout->addWidget(switchBox, 1);
    };

    updateLayout(switchBox->isChecked());

    connect(switchBox, &QCheckBox::toggled, this, updateLayout);

    Getter func = [builtinBox, customEdit, switchBox]() -> QVariant {
        QVariant value;

        if (switchBox->isChecked()) {
            value.setValue(Globals::File{":/" + builtinBox->currentText()});
        } else {
            value.setValue(Globals::File{customEdit->text()});
        }

        return value;
    };

    bindMetaProperty(property, func);

    return layoutWidget;
}

QWidget *WidgetDialogGenerator::fontProperty(QMetaProperty property) {
    qCritical() << "Not implemented";
    return nullptr;
}

QWidget *WidgetDialogGenerator::stringProperty(QMetaProperty property) {
    // bruh
    QLineEdit *edit = new QLineEdit(this);
    /*if (isResize)*/ edit->setText(property.read(m_widget).toString());

    auto func = std::bind(&QLineEdit::text, edit);
    bindMetaProperty(property, func);

    return edit;
}

QWidget *WidgetDialogGenerator::shapeProperty(QMetaProperty property) {
    QComboBox *comboBox = new QComboBox(this);
    comboBox->addItems(Globals::shapeNameMap.keys());

    comboBox->setCurrentText(Globals::shapeNameMap.key(property.read(m_widget).value<Globals::FrameShape>()));

    Getter func = [comboBox]() -> QVariant {
        return QVariant::fromValue(Globals::shapeNameMap.value(comboBox->currentText()));
    };

    bindMetaProperty(property, func);

    return comboBox;
}
