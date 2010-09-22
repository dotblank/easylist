#ifndef SLIDEWIDGET_H
#define SLIDEWIDGET_H

#include <QStateMachine>
#include <QState>
#include <QPropertyAnimation>
#include <QAbstractTransition>
#include <QtCore>
#include <QWidget>
#include <QPoint>
#include <qDebug>

class SlideWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SlideWidget(QWidget *parent = 0);
    virtual ~SlideWidget();

    virtual void shown() = 0;

    void initStates();

    void setStateIn();
    void setStateOut();

private:
    QStateMachine machine;
    QState stateIn;
    QState stateOut;

    QPropertyAnimation animPosIn;
    QPropertyAnimation animPosOut;

protected:

signals:
    void signalDoStateIn();
    void signalDoStateOut();
    void signalTransitionOutFinished();
    void signalTransitionInFinished();
public slots:
    void slotStateInFinished();
    void slotStateOutFinished();
};

#endif // SLIDEWIDGET_H
