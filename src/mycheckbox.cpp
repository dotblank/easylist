#include "mycheckbox.h"

int MyCheckBox::instances = 0;

MyCheckBox::MyCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    ++instances;
    qDebug() << "MyCheckBox instances:" << instances;
    uncheckedColor = palette();
    checkedColor.setColor(QPalette::WindowText, Qt::gray);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuActivated(QPoint)));
    connect(this, SIGNAL(toggled(bool)), this, SLOT(slotTriggered(bool)));
}

MyCheckBox::MyCheckBox(const QString & text, QWidget *parent) :
        QCheckBox(text, parent)
{
    ++instances;
    qDebug() << "MyCheckBox instances:" << instances;

    uncheckedColor = palette();
    checkedColor.setColor(QPalette::WindowText, Qt::gray);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenuActivated(QPoint)));
    connect(this, SIGNAL(toggled(bool)), this, SLOT(slotTriggered(bool)));
}

MyCheckBox::~MyCheckBox()
{
    --instances;
    qDebug() << "MyCheckBox instances left:" << instances;
}

void MyCheckBox::slotDeleteClicked()
{
    qDebug() << "Delete clicked";
    emit signalDeleteClicked(this);
}

void MyCheckBox::slotContextMenuActivated(QPoint point)
{
    qDebug() << "Context menu activated";
    QMenu menu(this);
    menu.addAction(tr("Delete"), this, SLOT(slotDeleteClicked()));
    menu.exec(this->mapToGlobal(point));
}

void MyCheckBox::slotTriggered(bool checked)
{
    if(checked)
    {
        setPalette(checkedColor);
    }
    else
    {
        setPalette(uncheckedColor);
    }
    QFont f(font());
    f.setStrikeOut(checked);
    setFont(f);
}
