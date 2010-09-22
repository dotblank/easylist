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
}

void EditForm::on_savePushButton_clicked()
{
    qDebug() << "Save";
    settings->setValue(LIST_TEXT, ui->textEdit->toPlainText());
    emit signalSavePushButtonClicked(this);
}

void EditForm::on_cancelPushButton_clicked()
{
    qDebug() << "Cancel";
    emit signalCancelPushButtonClicked(this);
}
