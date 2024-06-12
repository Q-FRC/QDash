#ifndef TABMAXSIZEDIALOG_H
#define TABMAXSIZEDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QFormLayout>
#include <QDialogButtonBox>

class TabMaxSizeDialog : public QDialog
{
    Q_OBJECT
private:
    QSpinBox *m_xBox;
    QSpinBox *m_yBox;

    QDialogButtonBox *m_buttonBox;

    QFormLayout *m_layout;
public:
    TabMaxSizeDialog(QWidget *parent = nullptr, const QPoint &maxSize = QPoint());
    ~TabMaxSizeDialog();
signals:
    void dataReady(QPoint point);
};

#endif // TABMAXSIZEDIALOG_H
