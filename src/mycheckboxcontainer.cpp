#include "mycheckboxcontainer.h"

MyCheckBoxContainer * MyCheckBoxContainer::instance = 0;
int MyCheckBoxContainer::instances = 0;

MyCheckBoxContainer::MyCheckBoxContainer()
{
    ++instances;
    qDebug() << "MyCheckBoxContainer instances:" << instances;
    sortToBottom = false;
    sortAlphabetically = false;
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
    qDebug() << "add" << item;
    QStringList list2;
    if(sortAlphabetically)
    {
        int listSize = list.size();
        int pos = 0;
        for(int i = 0; i < listSize; ++i)
        {
            int list2Size = list2.size();
            for(int j = 0; j < list2Size; ++j)
            {
                pos = j;
                if(list[i].toUpper() > list2[j].toUpper())
                {
                    break;
                }
            }
            qDebug() << "append" << list[i];
            list2.append(list[i]);
        }
        list = list2;
    }
    qDebug() << "list" << list;
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
    qDebug() << "set" << item;
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
                if(sortAlphabetically)
                {
                    int size = tempListChecked.size();
                    int pos = 0;
                    for(pos = 0; pos < size; ++pos)
                    {
                        if(tempListChecked[pos]->text().toUpper() > item->text().toUpper())
                        {
                            break;
                        }
                    }
                    tempListChecked.insert(pos, item);
                }
                else
                {
                    tempListChecked.append(item);
                }
            }
            else
            {
                if(sortAlphabetically)
                {
                    int size = tempList.size();
                    int pos = 0;
                    for(pos = 0; pos < size; ++pos)
                    {
                        if(tempList[pos]->text().toUpper() > item->text().toUpper())
                        {
                            break;
                        }
                    }
                    tempList.insert(pos, item);
                }
                else
                {
                    tempList.append(item);
                }
            }
        }
        checkBoxes.clear();
        checkBoxes.append(tempList);
        checkBoxes.append(tempListChecked);
    }
    emit signalSorted();
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
    if(sortAlphabetically)
    {
        set(getListText());
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

void MyCheckBoxContainer::setSortAlphabetically(bool sortAlphabetically)
{
    this->sortAlphabetically = sortAlphabetically;
    if(sortAlphabetically)
    {
        set(getListText());
    }
}

void MyCheckBoxContainer::clickedAction()
{
    qDebug() << "Clicked a MyCheckBox";
    sortCheckedToBottom();
}
