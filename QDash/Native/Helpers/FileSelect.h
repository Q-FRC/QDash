#pragma once

#include <QMetaObject>
#include <QObject>
#include <QQmlProperty>
#include <QFileDialog>

class FileSelect : public QObject {
    Q_OBJECT

private:
    QWidget *m_parent;
public:
    FileSelect(QWidget *parent) : QObject((QObject *)parent) {
        m_parent = parent;
    }

    Q_INVOKABLE QString getOpenFileName(const QString& title,
                                        const QString& dir,
                                        const QString& filter,
                                        QString* selectedFilter = nullptr,
                                        QFileDialog::Options options = QFileDialog::Options()) {
        return QFileDialog::getOpenFileName(m_parent, title, dir, filter, selectedFilter,
                                            options);
    }

    Q_INVOKABLE QString getSaveFileName(const QString& title,
                                        const QString& dir,
                                        const QString& filter,
                                        QString* selectedFilter = nullptr,
                                        QFileDialog::Options options = QFileDialog::Options()) {
        return QFileDialog::getSaveFileName(m_parent, title, dir, filter, selectedFilter,
                                            options);
    }

    Q_INVOKABLE QString getExistingDirectory(const QString& caption,
                                             const QString& dir,
                                             QFileDialog::Options options = QFileDialog::Options()) {
        return QFileDialog::getExistingDirectory(m_parent, caption, dir,
                                                 options);
    }
};
