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
    ui->syncPasswordLineEdit->installEventFilter(this);
    shown();
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::shown()
{
    ui->syncUsernameLineEdit->setText(settings->value(USERNAME,"").toString());
    ui->syncPasswordLineEdit->setText(settings->value(PASSWORD,"").toString());
    if(settings->contains(PASSWORD))
        ui->syncPasswordLineEdit->setReadOnly(true);

    ui->syncUrlLineEdit->setText(settings->value(SYNC_URL, DEFAULT_SYNC_URL).toString());
    ui->syncUsernameLineEdit->setFocus(Qt::ActiveWindowFocusReason);
}
bool SettingsForm::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == ui->syncPasswordLineEdit && ui->syncPasswordLineEdit->isReadOnly() && e->type() == QEvent::MouseButtonRelease)
    {
        int res = QMessageBox::warning(this, "Clear Password",
                                       "Are you sure you would like to change the password?",
                                       QMessageBox::Yes, QMessageBox::Cancel);
        if(res==QMessageBox::Yes)
        {
            ui->syncPasswordLineEdit->setReadOnly(false);
            ui->syncPasswordLineEdit->clear();
        }
    }
    else
        return QWidget::eventFilter(obj,e);
    return true;
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

    if(ui->syncPasswordLineEdit->isReadOnly()) //Password is already a hash
    {
        settings->setValue(PASSWORD,passString);
    }
    else
    {
        QString passMd5 = QCryptographicHash::hash(passString.toByteArray(), QCryptographicHash::Md5).toHex().constData();
        // We save the md5 hash of the password instead of the real password so it won't be retrievable.
        settings->setValue(PASSWORD, passMd5);
    }
    settings->setValue(SYNC_URL, ui->syncUrlLineEdit->text());
    settings->sync();
}

void SettingsForm::on_cancelPushButton_clicked()
{
    emit signalNavigate(0);
}
