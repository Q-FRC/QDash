#include "misc/WidgetDialogGenerator.h"

#include "misc/NewWidgetTreeDialog.h"

#include "qscreen.h"
#include "stores/FilterStore.h"

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
#include <QApplication>
#include <QRandomGenerator>
#include <QClipboard>
#include <QMessageBox>

bool operator<(QMetaProperty a, QMetaProperty b) {
    return a.name() < b.name();
}

WidgetDialogGenerator::WidgetDialogGenerator(BaseWidget *widget, QWidget *parent, bool isResize, WidgetData data) : QDialog(parent)
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

    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &WidgetDialogGenerator::reject);
    connect(this, &WidgetDialogGenerator::rejected, this, [this, widget] {
        emit cancelled(m_widget);
    });

    // only properties at BaseWidget and above
    int offset = BaseWidget::staticMetaObject.propertyOffset();
    int propertyCount = widget->metaObject()->propertyCount();

    for (int i = offset; i < propertyCount; ++i) {
        QMetaProperty property = widget->metaObject()->property(i);
        if (!property.isRequired() || (m_isResize && property.isConstant())) {
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
        PROPERTY_FUNCTION(CustomMetaTypes::TopicList, topicListProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::XAxis, xAxisProperty)
        PROPERTY_FUNCTION(CustomMetaTypes::TopicColorMap, topicColorMapProperty)
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
        widgetData.tabIdx = 0;
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

        m_widget->setReady(true);
        emit widgetReady(m_widget, widgetData);
        accept();
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

void WidgetDialogGenerator::serializeMap(QHash<Globals::Topic, QColor> map, QTableWidget *widget) {
    QHashIterator iter(map);

    int i = 0;
    while (iter.hasNext()) {
        iter.next();

        widget->insertRow(i);
        widget->setItem(i, 0, new QTableWidgetItem(iter.key().name));
        widget->setItem(i, 1, new QTableWidgetItem(iter.value().name()));
        ++i;
    }
}

void WidgetDialogGenerator::serializeMap(QVariantMap map, QTableWidget *widget) {
    QMapIterator iter(map);

    int i = 0;
    while (iter.hasNext()) {
        iter.next();

        widget->insertRow(i);
        widget->setItem(i, 0, new QTableWidgetItem(iter.key()));
        widget->setItem(i, 1, new QTableWidgetItem(iter.value().toString()));
        ++i;
    }
}

QPushButton *WidgetDialogGenerator::selectTopicButton() {
    QPushButton *topicButton = new QPushButton("Select Topic...", this);
    connect(topicButton, &QPushButton::clicked, this, [this, topicButton] {
        NewWidgetTreeDialog *dialog = new NewWidgetTreeDialog(true, this);
        auto filteredTopics = NewWidgetTreeDialog::filterNumberTypes(FilterStore::UnfilteredTopics);

        dialog->constructList(filteredTopics);
        dialog->setWindowTitle("Select a Topic");

        QRect screenSize = qApp->primaryScreen()->geometry();
        dialog->resize(screenSize.width() / 2., screenSize.height() / 2.);

        dialog->show();

        QObject *receiver = new QObject(this);
        connect(dialog, &NewWidgetTreeDialog::topicReady, receiver, [this, receiver, topicButton](const Globals::Topic &topic) {
            emit topicSelected(topic, topicButton);
            receiver->deleteLater();
        });
    });

    return topicButton;
}

QPushButton *WidgetDialogGenerator::selectColorButton() {
    QPushButton *colorButton = new QPushButton("Select Color...", this);
    connect(colorButton, &QPushButton::clicked, this, [this] {
        QColor random = QColor::fromRgb(QRandomGenerator::global()->generate());
        QColor color = QColorDialog::getColor(random, this, "Select Color");

        if (color.isValid()) emit colorSelected(color);
    });

    return colorButton;
}

std::tuple<QTableWidget *, QPushButton *, QPushButton *> WidgetDialogGenerator::setupTable(const QStringList &headers) {
    QTableWidget *table = new QTableWidget(0, headers.count(), this);

    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(false);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    QPushButton *addButton = new QPushButton("Add", this);
    connect(addButton, &QPushButton::clicked, this, [table, headers] {
        int rowCount = table->rowCount();

        table->insertRow(rowCount);
        for (int i = 0; i < headers.count(); ++i) {
            table->setItem(rowCount, i, new QTableWidgetItem(""));
        }
    });

    QPushButton *removeButton = new QPushButton("Delete", this);
    removeButton->setShortcut(Qt::Key_Delete);
    connect(removeButton, &QPushButton::clicked, this, [table] {
        QList<QTableWidgetItem *> selectedItems = table->selectedItems();

        if (selectedItems.empty()) return;

        table->removeRow(selectedItems.at(0)->row());
    });

    return {table, addButton, removeButton};
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
    auto [table, addButton, removeButton] = setupTable({"Key", "Value"});

    serializeMap(property.read(m_widget).toMap(), table);

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
}    QVariantMap variantMap{};

QWidget *WidgetDialogGenerator::topicListProperty(QMetaProperty property) {
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    QListWidget *list = new QListWidget(widget);
    layout->addWidget(list, 1);

    auto topics = property.read(m_widget).value<QList<Globals::Topic>>();

    QStringList names = FilterStore::topicNames(topics);
    list->addItems(names);

    QPushButton *topicButton = selectTopicButton();
    connect(this, &WidgetDialogGenerator::topicSelected, this, [this, list](const Globals::Topic &topic, QWidget *) {
        list->addItem(topic.name);
    });

    QPushButton *addButton = new QPushButton("Add", this);
    connect(addButton, &QPushButton::clicked, this, [list] {
        QListWidgetItem *item = new QListWidgetItem("Topic", list);
        item->setFlags(Qt::ItemFlag::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        list->addItem(item);
    });

    QPushButton *removeButton = new QPushButton("Delete", this);
    removeButton->setShortcut(Qt::Key_Delete);
    connect(removeButton, &QPushButton::clicked, this, [list] {
        QList<QListWidgetItem *> selectedItems = list->selectedItems();

        for (QListWidgetItem * item : selectedItems) {
            delete item;
        }
    });

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    buttonLayout->addWidget(topicButton);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);

    layout->addLayout(buttonLayout);

    auto stringListToTopicList = [](const QStringList &list) -> QList<Globals::Topic> {
        QList<Globals::Topic> topicList{};

        for (const QString & name : list) {
            Globals::Topic topic = FilterStore::topicFromName(name, FilterStore::FilteredTopics);
            if (topic.name == name) {
                topicList.append(topic);
            }
        }

        return topicList;
    };;

    auto func = [list, stringListToTopicList]() -> QVariant {
        QStringList names{};
        for (int i = 0; i < list->count(); ++i) {
            names.append(list->item(i)->text());
        }

        return QVariant::fromValue(stringListToTopicList(names));
    };

    bindMetaProperty(property, func);

    return widget;
}

QWidget *WidgetDialogGenerator::xAxisProperty(QMetaProperty property) {
    Globals::GraphXAxis xAxis = property.read(m_widget).value<Globals::GraphXAxis>();
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QCheckBox *checkbox = new QCheckBox("Use Time?", widget);
    checkbox->setChecked(xAxis.useTime);

    QLineEdit *lineEdit = new QLineEdit(widget);
    lineEdit->setText(xAxis.topic);

    QPushButton *topicButton = selectTopicButton();
    connect(this, &WidgetDialogGenerator::topicSelected, this, [this, lineEdit, topicButton](const Globals::Topic &topic, QWidget *receiver) {
        if (topicButton == receiver) lineEdit->setText(topic.name);
    });

    layout->addWidget(checkbox, 0);
    layout->addWidget(lineEdit, 1);
    layout->addWidget(topicButton, 0);

    auto showLineEdit = [lineEdit, topicButton](bool checked) {
        lineEdit->setVisible(!checked);
        topicButton->setVisible(!checked);
    };

    showLineEdit(checkbox->isChecked());

    connect(checkbox, &QCheckBox::clicked, this, showLineEdit);

    auto func = [lineEdit, checkbox]() -> QVariant {
        Globals::GraphXAxis axis{};
        axis.useTime = checkbox->isChecked();
        axis.topic = lineEdit->displayText();

        return QVariant::fromValue(axis);
    };

    bindMetaProperty(property, func);
    return widget;
}

QWidget *WidgetDialogGenerator::topicColorMapProperty(QMetaProperty property) {
    auto [table, addButton, removeButton] = setupTable({"Topic", "Color"});

    serializeMap(property.read(m_widget).value<QHash<Globals::Topic, QColor>>(), table);

    QPushButton *topicButton = selectTopicButton();
    connect(this, &WidgetDialogGenerator::topicSelected, this, [this, table, topicButton](const Globals::Topic &topic, QWidget *receiver) {
        if (topicButton != receiver) return;
        QList<QTableWidgetItem *> selectedItems = table->selectedItems();

        if (selectedItems.empty()) {
            qApp->clipboard()->setText(topic.name);
            QMessageBox::information(this, "Copied to Clipboard", "Copied topic selection to clipboard.");
            return;
        }

        int row = table->row(selectedItems.at(0));

        table->item(row, 0)->setText(topic.name);
    });

    QPushButton *colorButton = selectColorButton();
    connect(this, &WidgetDialogGenerator::colorSelected, this, [this, table](const QColor &color) {
        QList<QTableWidgetItem *> selectedItems = table->selectedItems();

        if (selectedItems.empty()) {
            qApp->clipboard()->setText(color.name());
            QMessageBox::information(this, "Copied to Clipboard", "Copied color selection to clipboard.");
            return;
        }

        int row = table->row(selectedItems.at(0));

        table->item(row, 1)->setText(color.name());
    });

    QWidget *tableLayoutWidget = new QWidget;

    QVBoxLayout *tableLayout = new QVBoxLayout(tableLayoutWidget);

    QHBoxLayout *buttons = new QHBoxLayout;

    buttons->addWidget(addButton);
    buttons->addWidget(removeButton);
    buttons->addWidget(topicButton);
    buttons->addWidget(colorButton);

    tableLayout->addWidget(table);
    tableLayout->addLayout(buttons);

    auto func = [this, table]() -> QVariant {
        QHash<Globals::Topic, QColor> map{};

        for (int i = 0; i < table->rowCount(); ++i) {
            QString name = table->item(i, 0)->text();
            Globals::Topic key = FilterStore::topicFromName(name, FilterStore::UnfilteredTopics);
            QString value = table->item(i, 1)->text();

            if (!key.name.isEmpty() && !value.isEmpty()) map.insert(key, QColor(value));
        }

        return QVariant::fromValue(map);
    };

    bindMetaProperty(property, func);

    return tableLayoutWidget;
}
