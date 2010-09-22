#ifndef MYCHECKBOXCONTAINER_H
#define MYCHECKBOXCONTAINER_H

#include <QString>
#include <QList>
#include <QObject>
#include <QLayout>
#include <QVBoxLayout>
#include "mycheckbox.h"

class MyCheckBoxContainer : public QObject
{
    Q_OBJECT
public:
    QList<MyCheckBox * > const & getCheckBoxes();
    QString getListText();

    virtual ~MyCheckBoxContainer();
    static MyCheckBoxContainer * getInstance();

    void add(QString item);
    void set(QString item);
    void clear();
    void uncheckAll();
    void removeChecked();
    void setSortCheckedToBottom(bool sortToBottom);
    void sortCheckedToBottom();

private:
    QList<MyCheckBox * > checkBoxes;
    static MyCheckBoxContainer * instance;
    static int instances;

    bool sortToBottom;

    MyCheckBoxContainer();
signals:
    void signalSorted();
public slots:
    void clickedAction();

};

#endif // MYCHECKBOXCONTAINER_H
