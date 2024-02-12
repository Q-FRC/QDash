#include "dialogs/NTSettingsDialog.h"

#include "dialogs/NewWidgetTreeDialog.h"
#include "stores/FilterStore.h"

#include <QScreen>

NTSettingsDialog::NTSettingsDialog(QWidget *parent) : QDialog(parent), Ui::NTSettingsDialog()
{
    setupUi(this);
    teamNumber->setChecked(Globals::server.teamNumber);
    server->setText(QString::fromStdString(Globals::server.server));
    port->setValue(Globals::server.port);
    topic->setText(Globals::server.switchTopic);
}

NTSettingsDialog::~NTSettingsDialog() {}

void NTSettingsDialog::serializeData() {
    emit dataReady(ServerData{teamNumber->isChecked(), server->text().toStdString(), port->value(), topic->text()});
}

void NTSettingsDialog::putTopic() {
    NewWidgetTreeDialog *dialog = new NewWidgetTreeDialog(true, this);
    auto filteredTopics = NewWidgetTreeDialog::filterStringTypes(FilterStore::UnfilteredTopics);

    dialog->constructList(filteredTopics);
    dialog->setWindowTitle("Select a Topic");

    QRect screenSize = qApp->primaryScreen()->geometry();
    dialog->resize(screenSize.width() / 2., screenSize.height() / 2.);

    dialog->show();

    connect(dialog, &NewWidgetTreeDialog::topicReady, this, [this](const Globals::Topic &topic) {
            this->topic->setText(topic.Name);
        }, Qt::SingleShotConnection);
}
