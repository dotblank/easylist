#include "editform.h"
#include "ui_editform.h"

EditForm::EditForm(QWidget *parent) :
    SlideWidget(parent),
    ui(new Ui::EditForm)
{
    ui->setupUi(this);
    settings = new QSettings(WILLEM_LIU, EASY_LIST);
    shown();
}

EditForm::~EditForm()
{
    delete ui;
}

void EditForm::shown()
{
    ui->textEdit->setText(MyCheckBoxContainer::getInstance()->getListText());
    ui->textEdit->setFocus(Qt::ActiveWindowFocusReason);
}

void EditForm::on_savePushButton_clicked()
{
    qDebug() << "Save";
    settings->setValue(LIST_TEXT, ui->textEdit->toPlainText());
    SystemSettings::getInstance()->saveCurrentList();
    emit signalNavigate(0);
}

void EditForm::on_cancelPushButton_clicked()
{
    qDebug() << "Cancel";
    emit signalNavigate(0);
}
