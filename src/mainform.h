#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QStateMachine>
#include <QState>
#include <QAbstractTransition>
#include <QPropertyAnimation>
#include "slidewidget.h"
#include "listform.h"
#include "editform.h"
#include "systemsettings.h"
#include "mycheckboxcontainer.h"

#define WILLEM_LIU "WillemLiu"
#define EASY_LIST "easylist"
#define DBUS_KEYBOARD_SLIDE "/org/freedesktop/Hal/devices/platform_slide"
#define CHECKED_ITEMS_TO_BOTTOM "CheckedItemsToBottom"
#define LANDSCAPE "Landscape"
#define PORTRAIT "Portrait"
#define LIST_TEXT "ListText"

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
    void changeWidget(SlideWidget * currentWidget);
    void stateOutFinished();
private:
    QSettings *settings;
    bool landscape;
    bool tempLandscapeMode;
    int newIndex;
    Ui::MainForm *ui;

    ListForm * listForm;
    EditForm * editForm;

private slots:
    void on_actionChecked_bottom_triggered();
    void on_actionAbout_triggered();
    void on_actionRotate_triggered();
};

#endif // MAINFORM_H
