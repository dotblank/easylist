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
    settings = new QSettings(WILLEM_LIU, EASY_LIST);

    // Set a default value for CHECKED_ITEMS_TO_BOTTOM
    if(settings->contains(CHECKED_ITEMS_TO_BOTTOM) == false)
    {
        settings->setValue(CHECKED_ITEMS_TO_BOTTOM, false);
    }

    // We always start in landscape mode.
    landscape = settings->value(LANDSCAPE).toBool();
    if(settings->contains(LANDSCAPE))
    {
        landscape = settings->value(LANDSCAPE).toBool();
    }
    settings->setValue(LANDSCAPE, landscape);
    // If keyboard is opened at start. We do landscape mode.
    // Otherwise we do what's read from the QSettings.
    if(isKeyboardClosed() == false)
    {
        setLandscapeMode(true);
    }
    else
    {
        setLandscapeMode(landscape);
    }
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
    if(isKeyboardClosed() == false)
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
    settings->setValue(LIST_TEXT, editUi->textEdit->toPlainText());
    showListWindow();
}

/**
 * Slot for action from Clear selected button in de the list window.
 * It clears all QCheckBoxes from the layout, removes all checked items from
 * the list text and finally calls generateList().
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

    settings->setValue(LIST_TEXT, listText);
    generateList();
}

/**
 * Prefix all checked items with ! in the list text and save it to QSettings.
 */
void MainWindow::slotListWindowSaveChecked()
{
    qDebug() << "Save selected";
    QString listText("");
    foreach(QCheckBox * cb, checkBoxes)
    {
        QString item(cb->text());
        if(cb->isChecked() == true)
        {
            if(item.startsWith("!") == false)
            {
                item.push_front("!");
            }
            listText.append(item);
        }
        else
        {
            if(item.startsWith("!") == true)
            {
                item = item.replace("!", "");
            }
            listText.append(item);
        }
        listText.append("\n");
    }

    settings->setValue(LIST_TEXT, listText);
}

/**
 * Is called when a checkbox has been clicked.
 *
 * @fn slotActionCheckBox
 * @param bool checked - true if checkbox is checked.
 */
void MainWindow::slotActionCheckBox(bool checked)
{
    qDebug() << "CheckBox checked=" << checked;
    slotListWindowSaveChecked();
    if(settings->value(CHECKED_ITEMS_TO_BOTTOM).toBool())
    {
        slotSortCheckedBottom();
        // Remove all the checkboxes from the screen.
        foreach(QCheckBox * cb, checkBoxes)
        {
            cb->deleteLater();
        }
        generateList();
    }
    else
    {
        qDebug() << "No need to sort items to bottom";
    }
}

/**
 * Sort checked items to bottom of the list text and returns the new list text.
 *
 * @fn slotSortCheckedBottom
 *
 * @return QString - the new list text.
 */
QString MainWindow::slotSortCheckedBottom()
{
    QString result(settings->value(LIST_TEXT).toString());
    if(settings->value(CHECKED_ITEMS_TO_BOTTOM).toBool())
    {
        QStringList list = result.split("\n");
        QString listText("");
        QString checkedListText("");
        foreach(QString item, list)
        {
            if(item.length() > 0)
            {
                if(item.startsWith("!"))
                {
                    checkedListText.append(item);
                    checkedListText.append("\n");
                }
                else
                {
                    listText.append(item);
                    listText.append("\n");
                }
            }
        }
        listText.append(checkedListText);
        qDebug() << "Sort checked items to bottom";
        settings->setValue(LIST_TEXT, listText);
        result = listText;
    }
    return result;
}

/**
 * Show the edit window.
 *
 * @fn showEditWindow
 */
void MainWindow::showEditWindow()
{
    slotListWindowSaveChecked();
    editUi->setupUi(this);
    editUi->textEdit->setText(settings->value(LIST_TEXT).toString());
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
    listUi->actionChecked_Bottom->setChecked(settings->value(CHECKED_ITEMS_TO_BOTTOM).toBool());
    slotSortCheckedBottom();
    generateList();
    connect(listUi->editListPushButton, SIGNAL(clicked()), this, SLOT(slotListWindowEdit()));
    connect(listUi->clearSelectedPushButton, SIGNAL(clicked()), this, SLOT(slotListWindowClearSelected()));
    connect(listUi->menuChecked_Bottom, SIGNAL(triggered(QAction*)), this, SLOT(slotActionCheckedBottom(QAction*)));
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
    QString text = settings->value(LIST_TEXT).toString();
    QStringList list = text.split("\n");

    foreach(QString item, list)
    {
        if(item.length() > 0)
        {
            QCheckBox * cb = new QCheckBox(item);
            connect(cb, SIGNAL(clicked(bool)), this, SLOT(slotActionCheckBox(bool)));
            if(item.startsWith("!"))
            {
                QString itemName(item.right(item.length()-1));
                cb->setText(itemName);
                cb->setChecked(true);
            }
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
    slotListWindowSaveChecked();
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

    landscape = !tempLandscapeMode;
    settings->setValue(LANDSCAPE, landscape);
    setLandscapeMode(landscape);
}



/**
 * Set landscape/portrait mode.
 */
void MainWindow::setLandscapeMode(bool landscape)
{
#if defined(Q_WS_MAEMO_5) || defined(Q_WS_HILDON)
    if(landscape)
    {
        tempLandscapeMode = true;
        qDebug() << LANDSCAPE;
        setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
        setAttribute(Qt::WA_Maemo5PortraitOrientation, false);
    }
    else
    {
        tempLandscapeMode = false;
        qDebug() << PORTRAIT;
        setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
        setAttribute(Qt::WA_Maemo5LandscapeOrientation, false);
    }
#endif
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

/**
 * Is called when the Checked Bottom menu item is triggered.
 * The menu item is a checkable item. So we need to check if it's checked or not.
 *
 * @fn slotActionCheckedBottom
 * @param QAction* action - the action.
 */
void MainWindow::slotActionCheckedBottom(QAction* action)
{
    qDebug() << "Checked Bottom" << action->text() << listUi->actionChecked_Bottom->isChecked();
    settings->setValue(CHECKED_ITEMS_TO_BOTTOM, listUi->actionChecked_Bottom->isChecked());
}
