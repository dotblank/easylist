#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QCryptographicHash>
#include <QMessageBox>
#include "slidewidget.h"
#include "globals.h"

namespace Ui {
    class SettingsForm;
}

class SettingsForm : public SlideWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = 0);
    ~SettingsForm();

    virtual void shown();

signals:
    void signalNavigate(int step);
protected:
    bool eventFilter(QObject *obj, QEvent *e);

private slots:
    void on_restoreSyncPushButton_clicked();
    void on_applyPushButton_clicked();
    void on_savePushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::SettingsForm *ui;

    QSettings * settings;

    void saveSync();
};

#endif // SETTINGSFORM_H
