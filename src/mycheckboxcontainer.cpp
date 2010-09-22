#include "mycheckboxcontainer.h"

MyCheckBoxContainer * MyCheckBoxContainer::instance = 0;
int MyCheckBoxContainer::instances = 0;

MyCheckBoxContainer::MyCheckBoxContainer()
{
    ++instances;
    qDebug() << "MyCheckBoxContainer instances:" << instances;
    sortToBottom = false;
}

MyCheckBoxContainer::~MyCheckBoxContainer()
{
    --instances;
    qDebug() << "MyCheckBoxContainer instances left:" << instances;
    clear();
}

MyCheckBoxContainer * MyCheckBoxContainer::getInstance()
{
    if(instance == 0)
    {
        instance = new MyCheckBoxContainer();
    }
    return instance;
}

void MyCheckBoxContainer::add(QString item)
{
    QStringList list = item.split("\n");
    foreach(QString item, list)
    {
        if(item.length() > 0)
        {
            MyCheckBox * checkBox;
            if(item.startsWith("!"))
            {
                checkBox = new MyCheckBox(item.right(item.length()-1));
                checkBox->setChecked(true);
            }
            else
            {
                checkBox = new MyCheckBox(item);
            }
            connect(checkBox, SIGNAL(clicked()), this, SLOT(clickedAction()));
            checkBoxes.append(checkBox);
        }
    }
    sortCheckedToBottom();
}

void MyCheckBoxContainer::set(QString item)
{
    clear();
    add(item);
}

void MyCheckBoxContainer::clear()
{
    foreach(MyCheckBox * item, checkBoxes)
    {
        item->setParent(0);
        item->deleteLater();
    }
    checkBoxes.clear();
}

void MyCheckBoxContainer::sortCheckedToBottom()
{
    if(sortToBottom)
    {
        qDebug() << "Sort checked to bottom";
        QList<MyCheckBox * > tempList;
        QList<MyCheckBox * > tempListChecked;
        foreach(MyCheckBox * item, checkBoxes)
        {
            if(item->isChecked())
            {
                tempListChecked.append(item);
            }
            else
            {
                tempList.append(item);
            }
        }
        checkBoxes.clear();
        checkBoxes.append(tempList);
        checkBoxes.append(tempListChecked);
        emit signalSorted();
    }
}

QList<MyCheckBox * > const & MyCheckBoxContainer::getCheckBoxes()
{
    return checkBoxes;
}

QString MyCheckBoxContainer::getListText()
{
    QString listText("");
    foreach(MyCheckBox * item, checkBoxes)
    {
        if(item->isChecked())
        {
            listText.append("!");
        }
        listText.append(item->text());
        listText.append("\n");
    }
    return listText;
}

void MyCheckBoxContainer::uncheckAll()
{
    foreach(MyCheckBox * item, checkBoxes)
    {
        item->setChecked(false);
    }
}

void MyCheckBoxContainer::removeChecked()
{
    QMutableListIterator<MyCheckBox * > i(checkBoxes);
    while (i.hasNext())
    {
        MyCheckBox * item = i.next();
        if (item->isChecked())
        {
            item->setParent(0);
            item->deleteLater();
            i.remove();
        }
    }
}

void MyCheckBoxContainer::setSortCheckedToBottom(bool sortToBottom)
{
    this->sortToBottom = sortToBottom;
    sortCheckedToBottom();
}

void MyCheckBoxContainer::clickedAction()
{
    qDebug() << "Clicked a MyCheckBox";
    sortCheckedToBottom();
}
