#ifndef EDITFORM_H
#define EDITFORM_H

#include <QWidget>
#include <QDebug>
#include <QSettings>
#include "mycheckboxcontainer.h"
#include "slidewidget.h"
#include "globals.h"

namespace Ui {
    class EditForm;
}

class EditForm : public SlideWidget
{
    Q_OBJECT

public:
    explicit EditForm(QWidget *parent = 0);
    ~EditForm();

    virtual void shown();

private:
    Ui::EditForm *ui;

    QSettings * settings;

signals:
    void signalNavigate(int step);

private slots:
    void on_cancelPushButton_clicked();
    void on_savePushButton_clicked();
};

#endif // EDITFORM_H
