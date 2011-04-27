#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include <QSettings>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QStateMachine>
#include <QState>
#include <QAbstractTransition>
#include <QPropertyAnimation>
#include <QCloseEvent>
#include "slidewidget.h"
#include "listform.h"
#include "editform.h"
#include "chooselistform.h"
#include "systemsettings.h"
#include "mycheckboxcontainer.h"
#include "globals.h"

namespace Ui {
    class MainForm;
}

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

    void setLandscapeMode(bool landscape);

public slots:
    void keyboardClosed(bool closed);
    void changeWidget(int step);
    void stateOutFinished();
private:
    QSettings *settings;
    bool landscape;
    bool tempLandscapeMode;
    int newIndex;
    Ui::MainForm *ui;

    ListForm * listForm;
    EditForm * editForm;
    ChooseListForm * chooseListForm;
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionLists_triggered();
    void on_actionSort_A_Z_triggered();
    void on_actionAuto_Orientation_triggered();
    void on_actionChecked_bottom_triggered();
    void on_actionAbout_triggered();
    void on_actionRotate_triggered();
};

#endif // MAINFORM_H
