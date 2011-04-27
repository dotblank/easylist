#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H

#include <QCheckBox>
#include <QDebug>
#include <QMenu>
#include <QObject>

class MyCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    MyCheckBox(QWidget *parent = 0);
    MyCheckBox(const QString & text, QWidget *parent = 0);
    virtual ~MyCheckBox();
private:
    static int instances;

    QPalette checkedColor;
    QPalette uncheckedColor;
signals:
    void signalDeleteClicked(MyCheckBox * myCheckBox);
public slots:
    void slotContextMenuActivated(QPoint point);
    void slotDeleteClicked();
    void slotTriggered(bool checked);
};

#endif // MYCHECKBOX_H
