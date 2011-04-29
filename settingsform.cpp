#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    SlideWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
    ui->syncUsernameLineEdit->setInputMethodHints(Qt::ImhNoAutoUppercase);
    ui->syncPasswordLineEdit->setInputMethodHints(Qt::ImhNoAutoUppercase);
    ui->syncUrlLineEdit->setInputMethodHints(Qt::ImhNoAutoUppercase);
    settings = new QSettings(WILLEM_LIU, EASY_LIST);
    shown();
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::shown()
{
    ui->syncUsernameLineEdit->clear();
    ui->syncPasswordLineEdit->clear();
    ui->syncUrlLineEdit->setText(settings->value(SYNC_URL, DEFAULT_SYNC_URL).toString());
    ui->syncUsernameLineEdit->setFocus(Qt::ActiveWindowFocusReason);
}

void SettingsForm::on_restoreSyncPushButton_clicked()
{
    ui->syncUrlLineEdit->setText(DEFAULT_SYNC_URL);
}

void SettingsForm::on_applyPushButton_clicked()
{
    saveSync();
}

void SettingsForm::on_savePushButton_clicked()
{
    saveSync();
    emit signalNavigate(0);
}

void SettingsForm::saveSync()
{
    settings->setValue(USERNAME, ui->syncUsernameLineEdit->text());
    QVariant passString = ui->syncPasswordLineEdit->text();
    QString passMd5 = QCryptographicHash::hash(passString.toByteArray(), QCryptographicHash::Md5).toHex().constData();
    // We save the md5 hash of the password instead of the real password so it won't be retrievable.
    settings->setValue(PASSWORD, passMd5);
    settings->setValue(SYNC_URL, ui->syncUrlLineEdit->text());
}

void SettingsForm::on_cancelPushButton_clicked()
{
    emit signalNavigate(0);
}
