#ifndef EDITFORM_H
#define EDITFORM_H

#include <QWidget>
#include <QDebug>
#include <QSettings>
#include "mycheckboxcontainer.h"
#include "slidewidget.h"

#define WILLEM_LIU "WillemLiu"
#define EASY_LIST "easylist"
#define LIST_TEXT "ListText"

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
    void signalSavePushButtonClicked(SlideWidget * thisWidget);
    void signalCancelPushButtonClicked(SlideWidget * thisWidget);

private slots:
    void on_cancelPushButton_clicked();
    void on_savePushButton_clicked();
};

#endif // EDITFORM_H
