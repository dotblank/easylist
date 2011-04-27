#ifndef CHOOSELISTFORM_H
#define CHOOSELISTFORM_H

#include <QWidget>
#include <QDebug>
#ifdef Q_WS_MAEMO_5
#include <QMaemo5InformationBox>
#endif
#include "globals.h"
#include "slidewidget.h"

namespace Ui {
    class ChooseListForm;
}

class ChooseListForm : public SlideWidget
{
    Q_OBJECT

public:
    explicit ChooseListForm(QWidget *parent = 0);
    ~ChooseListForm();

    virtual void shown();
    void saveList();
private:
    Ui::ChooseListForm *ui;

    QSettings * settings;

signals:
    void signalNavigate(int step);

private slots:
    void on_listNameLineEdit_returnPressed();
    void on_listWidget_activated(QModelIndex index);
    void on_loadPushButton_clicked();
    void on_removePushButton_clicked();
    void on_savePushButton_clicked();
    void on_cancelPushButton_clicked();
};

#endif // CHOOSELISTFORM_H
