#include "listform.h"
#include "ui_listform.h"

ListForm::ListForm(QWidget *parent) :
    SlideWidget(parent),
    ui(new Ui::ListForm)
{
    ui->setupUi(this);
    ui->checkBoxVerticalLayout->setAlignment(Qt::AlignTop);

    settings = new QSettings(WILLEM_LIU, EASY_LIST);

    // Add the list text to the MyCheckBoxContainer. It will create a list of MyCheckBox objects.
    MyCheckBoxContainer::getInstance()->add(settings->value(LIST_TEXT, "").toString());
    connect(MyCheckBoxContainer::getInstance(), SIGNAL(signalSorted()), this, SLOT(addCheckBoxes()));
    addCheckBoxes();
}

ListForm::~ListForm()
{
    delete ui;
}

void ListForm::shown()
{
    MyCheckBoxContainer::getInstance()->set(settings->value(LIST_TEXT, "").toString());
    addCheckBoxes();
}

void ListForm::addCheckBoxes()
{
    // Add the MyCheckBox items to the view.
    foreach(MyCheckBox * cb, MyCheckBoxContainer::getInstance()->getCheckBoxes())
    {
        // Reset the parent.
        ui->checkBoxVerticalLayout->addWidget(cb);
    }
}

void ListForm::on_editListPushButton_clicked()
{
    saveList();
    emit signalNavigate(1);
}

void ListForm::saveList()
{
    settings->setValue(LIST_TEXT, MyCheckBoxContainer::getInstance()->getListText());
    SystemSettings::getInstance()->saveCurrentList();
}

void ListForm::on_uncheckAllPushButton_clicked()
{
    int res = QMessageBox::warning(this, "Uncheck All", "All items will be unchecked.", QMessageBox::Ok, QMessageBox::Cancel);

    if(res == QMessageBox::Ok)
    {
        MyCheckBoxContainer::getInstance()->uncheckAll();
    }
}

void ListForm::on_clearCheckedPushButton_clicked()
{
    int res = QMessageBox::warning(this, "Clear selected", "All checked items will be cleared.", QMessageBox::Ok, QMessageBox::Cancel);

    if(res == QMessageBox::Ok)
    {
        MyCheckBoxContainer::getInstance()->removeChecked();
        settings->setValue(LIST_TEXT, MyCheckBoxContainer::getInstance()->getListText());
        SystemSettings::getInstance()->saveCurrentList();
    }
}

