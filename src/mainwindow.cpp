/*
 *  Copyright (c) 2010 Willem Liu
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_listwindow.h"

#define DBUS_KEYBOARD_SLIDE "/org/freedesktop/Hal/devices/platform_slide"

/**
 * Constructor.
 * Settings are initialised here.
 * Landscape mode is set to true.
 * List window is loaded.
 * @fn MainWindow
 * @param parent - The parent widget.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    editUi(new Ui::EditWindow),
    listUi(new Ui::ListWindow)
{
    // Connect to DBUS of keyboard slider.
    QDBusConnection::systemBus().connect(QString("org.freedesktop.Hal"),
                                         DBUS_KEYBOARD_SLIDE,
                                         QString("org.freedesktop.Hal.Device"),
                                         QString("PropertyModified"),
                                         this, SLOT(slotKeyboardSlide()));
    // Initialise the settings.
    settings = new QSettings("WillemLiu", "easylist");
    // We always start in landscape mode.
    landscape = settings->value("Landscape").toBool();
    if(settings->contains("Landscape"))
    {
        landscape = settings->value("Landscape").toBool();
    }
    settings->setValue("Landscape", landscape);
    // If keyboard is opened at start.
    if(isKeyboardClosed() == false)
    {
        landscape = true;
    }
    setLandscapeMode(landscape);
    // Auto-detect portrait/landscape mode. Only works on top widget.
//    setAttribute(Qt::WA_Maemo5AutoOrientation, true);
    showListWindow();
}

/**
 * Destructor.
 * User interface pointers are deleted here.
 *
 * @fn ~MainWindow
 */
MainWindow::~MainWindow()
{
    delete editUi;
    delete listUi;
}

/**
 * Check the DBUS property to see if the keyboard is closed or opened.
 */
bool MainWindow::isKeyboardClosed()
{
    QDBusInterface propertyInterface("org.freedesktop.Hal",
                    DBUS_KEYBOARD_SLIDE,
                    "org.freedesktop.Hal.Device",
                    QDBusConnection::systemBus());
    bool result = propertyInterface.call("GetProperty", "button.state.value").arguments().at(0).toBool();
    qDebug() << "Keyboard is closed:" << result;
    return result;
}

/**
 * Slot is called when a DBUS event has been received caused by opening/closing the keyboard.
 */
void MainWindow::slotKeyboardSlide()
{
    // When keyboard is opened.
    if(false == isKeyboardClosed())
    {
        setLandscapeMode(true);
    }
    else
    {
        setLandscapeMode(landscape);
    }
}

/**
 * Slot for action from Edit list button in de the list window.
 *
 * @fn slotListWindowEdit
 */
void MainWindow::slotListWindowEdit()
{
    qDebug() << "Edit list";
    showEditWindow();
}

/**
 * Slot for action from Cancel button in de the edit window.
 *
 * @fn slotEditWindowCancel
 */
void MainWindow::slotEditWindowCancel()
{
    qDebug() << "Cancel";
    showListWindow();
}

/**
 * Slot for action from Save button in de the edit window.
 *
 * @fn slotEditWindowSave
 */
void MainWindow::slotEditWindowSave()
{
    qDebug() << "Save";
    settings->setValue("ListText", editUi->textEdit->toPlainText());
    showListWindow();
}

/**
 * Slot for action from Clear selected button in de the list window.
 *
 * @fn slotListWindowClearSelected
 */
void MainWindow::slotListWindowClearSelected()
{
    qDebug() << "Clear selected";
    QString listText("");
    foreach(QCheckBox * cb, checkBoxes)
    {
        cb->deleteLater();
        if(cb->isChecked() == false)
        {
            listText.append(cb->text());
            listText.append("\n");
        }
    }

    settings->setValue("ListText", listText);
    generateList();
}

/**
 * Show the edit window.
 *
 * @fn showEditWindow
 */
void MainWindow::showEditWindow()
{
    editUi->setupUi(this);
    editUi->textEdit->setText(settings->value("ListText").toString());
    connect(editUi->savePushButton, SIGNAL(clicked()), this, SLOT(slotEditWindowSave()));
    connect(editUi->cancelPushButton, SIGNAL(clicked()), this, SLOT(slotEditWindowCancel()));
}

/**
 * Show the list window.
 *
 * @fn showListWindow
 */
void MainWindow::showListWindow()
{
    listUi->setupUi(this);
    listUi->listVerticalLayout->setAlignment(Qt::AlignTop);
    generateList();
    connect(listUi->editListPushButton, SIGNAL(clicked()), this, SLOT(slotListWindowEdit()));
    connect(listUi->clearSelectedPushButton, SIGNAL(clicked()), this, SLOT(slotListWindowClearSelected()));
    connect(listUi->menuAbout, SIGNAL(triggered(QAction*)), this, SLOT(slotActionAbout(QAction*)));
    connect(listUi->menuRotate, SIGNAL(triggered(QAction*)), this, SLOT(slotActionRotate(QAction*)));
}

/**
 * Generate the list of checkboxes. A pointer to such a checkbox will be stored
 * in a container for later use.
 *
 * @fn generateList
 */
void MainWindow::generateList()
{
    qDebug() << "Generate List";

    checkBoxes.clear();
    QString text = settings->value("ListText").toString();
    QStringList list = text.split("\n");

    foreach(QString item, list)
    {
        if(item.length() > 0)
        {
            QCheckBox * cb = new QCheckBox(item);
            checkBoxes.append(cb);
            listUi->listVerticalLayout->addWidget(cb);
        }
    }
}

/**
 * Is called when the application terminates.
 *
 * @fn closeEvent
 * @param event - the QCloseEvent.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "Closed";
    event->accept();
}

/**
 * Is called when the Rotate menu item is triggered. Display orientation is saved in the QSettings.
 *
 * @fn slotActionRotate
 * @param QAction* action - the action.
 */
void MainWindow::slotActionRotate(QAction* action)
{
    qDebug() << "Rotate" << action->text();
    landscape = !landscape;
    settings->setValue("Landscape", landscape);
    setLandscapeMode(landscape);
}

/**
 * Set landscape/portrait mode.
 */
void MainWindow::setLandscapeMode(bool landscape)
{
    if(landscape)
    {
        setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
        setAttribute(Qt::WA_Maemo5PortraitOrientation, false);
    }
    else
    {
        setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
        setAttribute(Qt::WA_Maemo5LandscapeOrientation, false);
    }
}

/**
 * Is called when the About menu item is triggered.
 *
 * @fn slotActionAbout
 * @param QAction* action - the action.
 */
void MainWindow::slotActionAbout(QAction* action)
{
    qDebug() << "About" << action->text();
    QString aboutText;
    aboutText.append("EasyList (c) 2010\n\n");
    aboutText.append("Created by Willem Liu.\n");
    aboutText.append("Created with QtCreator.\n");
    QMessageBox::about(this, "EasyList", aboutText);
}
