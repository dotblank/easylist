#ifndef LISTFORM_H
#define LISTFORM_H

#include <QWidget>
#include <QSettings>
#include "mycheckboxcontainer.h"
#include "systemsettings.h"
#include "slidewidget.h"
#include "globals.h"

namespace Ui {
    class ListForm;
}

class ListForm : public SlideWidget
{
    Q_OBJECT

public:
    explicit ListForm(QWidget *parent = 0);
    ~ListForm();

    virtual void shown();
    virtual void saveList();

private:
    QSettings * settings;

    Ui::ListForm *ui;
signals:
    void signalNavigate(int step);

private slots:
    void on_clearCheckedPushButton_clicked();
    void on_uncheckAllPushButton_clicked();
    void on_editListPushButton_clicked();
    void addCheckBoxes();
};

#endif // LISTFORM_H
