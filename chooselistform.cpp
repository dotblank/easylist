#include "chooselistform.h"
#include "ui_chooselistform.h"

ChooseListForm::ChooseListForm(QWidget *parent) :
    SlideWidget(parent),
    ui(new Ui::ChooseListForm)
{
    ui->setupUi(this);
    settings = new QSettings(WILLEM_LIU, EASY_LIST);
}

ChooseListForm::~ChooseListForm()
{
    delete ui;
}

void ChooseListForm::shown()
{
    qDebug() << "ChooseListForm shown";
    ui->listNameLineEdit->setText(settings->value(SELECTED_LIST_NAME, "").toString());
    ui->listWidget->clear();
    QStringList listNames = settings->value(LIST_NAMES, "").toStringList();
    bool addList = false;
    foreach(QString item, listNames)
    {
        if(item.length() > 0)
        {
            addList = true;
            break;
        }
    }
    qDebug() << "List names:" << settings->value(LIST_NAMES, "").toString().size();
    if(addList)
    {
        ui->listWidget->addItems(listNames);
    }
    ui->listNameLineEdit->setFocus(Qt::ActiveWindowFocusReason);
}

void ChooseListForm::on_cancelPushButton_clicked()
{
    emit signalNavigate(0);
}

void ChooseListForm::on_savePushButton_clicked()
{
    QList<QListWidgetItem * > items = ui->listWidget->findItems(ui->listNameLineEdit->text(), Qt::MatchExactly);
    QString listName = ui->listNameLineEdit->text();
    if(listName.isEmpty() == false)
    {
        if(items.count() == 0)
        {
            ui->listWidget->addItem(listName);
        }
        settings->setValue(listName, settings->value(LIST_TEXT, ""));
        QString information;
        information.append("List saved ");
        information.append(listName);
        settings->setValue(SELECTED_LIST_NAME, listName);
#ifdef Q_WS_MAEMO_5
        QMaemo5InformationBox::information(this, information, QMaemo5InformationBox::DefaultTimeout);
#endif
        saveList();
    }
    else
    {
#ifdef Q_WS_MAEMO_5
        QMaemo5InformationBox::information(this, "List name can't be empty", QMaemo5InformationBox::DefaultTimeout);
#endif
    }
}

void ChooseListForm::on_removePushButton_clicked()
{
    if(ui->listWidget->selectedItems().size() > 0)
    {
        qDebug() << "Remove" << ui->listWidget->currentRow();
        QListWidgetItem * item = ui->listWidget->takeItem(ui->listWidget->currentRow());
        settings->remove(item->text());
        settings->setValue(SELECTED_LIST_NAME, LIST_TEXT);
        saveList();
    }
}

void ChooseListForm::saveList()
{
    ui->listWidget->sortItems();
    QStringList list;
    int size = ui->listWidget->count();
    for(int i = 0; i < size; ++i)
    {
        list.append(ui->listWidget->item(i)->text());
    }
    settings->setValue(LIST_NAMES, QVariant(list));
}

void ChooseListForm::on_loadPushButton_clicked()
{
    if(ui->listWidget->selectedItems().size() > 0)
    {
        QListWidgetItem * item = ui->listWidget->item(ui->listWidget->currentRow());
        settings->setValue(LIST_TEXT, settings->value(item->text(), LIST_TEXT).toString());
        settings->setValue(SELECTED_LIST_NAME, item->text());
        emit signalNavigate(0);
    }
}

void ChooseListForm::on_listWidget_activated(QModelIndex index)
{
    ui->listNameLineEdit->setText(ui->listWidget->item(index.row())->text());
}

void ChooseListForm::on_listNameLineEdit_returnPressed()
{
    qDebug() << "Enter pressed";
    on_savePushButton_clicked();
}
