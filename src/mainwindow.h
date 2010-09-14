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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFrame>
#include <QCloseEvent>
#include <QSettings>
#include <QCheckBox>
#include <QMessageBox>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

namespace Ui {
    class EditWindow;
    class ListWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QVector<QCheckBox * > checkBoxes;
    QSettings *settings;
    bool landscape;
    bool tempLandscapeMode;

    Ui::EditWindow *editUi;
    Ui::ListWindow *listUi;

    void closeEvent(QCloseEvent *event);

    void showEditWindow();
    void showListWindow();
    void generateList();
    void setLandscapeMode(bool landscape);
    bool isKeyboardClosed();

public slots:
    void slotEditWindowSave();
    void slotEditWindowCancel();
    void slotListWindowEdit();
    void slotListWindowClearSelected();
    void slotListWindowSaveChecked();

    void slotKeyboardSlide();

private slots:
    void slotActionAbout(QAction* action);
    void slotActionRotate(QAction* action);
};

#endif // MAINWINDOW_H
