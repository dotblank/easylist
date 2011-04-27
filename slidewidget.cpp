#include "slidewidget.h"

SlideWidget::SlideWidget(QWidget *parent) :
    QWidget(parent)
{
    initStates();

    QAbstractTransition * t1 = stateIn.addTransition(this, SIGNAL(signalDoStateOut()), &stateOut);
    QAbstractTransition * t2 = stateOut.addTransition(this, SIGNAL(signalDoStateIn()), &stateIn);

    t1->addAnimation(&animPosOut);
    t2->addAnimation(&animPosIn);

    connect(&animPosIn, SIGNAL(finished()), this, SIGNAL(signalTransitionInFinished()));
    connect(&animPosIn, SIGNAL(finished()), this, SLOT(slotStateInFinished()));
    connect(&animPosOut, SIGNAL(finished()), this, SIGNAL(signalTransitionOutFinished()));
    connect(&animPosOut, SIGNAL(finished()), this, SLOT(slotStateOutFinished()));

    machine.addState(&stateIn);
    machine.addState(&stateOut);
    machine.setInitialState(&stateOut);
    machine.start();
}

SlideWidget::~SlideWidget()
{
}

void SlideWidget::initStates()
{
    animPosIn.setTargetObject(this);
    animPosIn.setPropertyName("pos");
    animPosIn.setDuration(500);
    animPosIn.setEasingCurve(QEasingCurve::OutExpo);

    animPosOut.setTargetObject(this);
    animPosOut.setPropertyName("pos");
    animPosOut.setDuration(500);
    animPosOut.setEasingCurve(QEasingCurve::OutExpo);

    stateIn.assignProperty(this, "pos", QPoint(0, 0));

    stateOut.assignProperty(this, "pos", QPoint(0, -this->height()));
}

void SlideWidget::setStateIn()
{
    qDebug() << "State In";
    emit signalDoStateIn();
}

void SlideWidget::setStateOut()
{
    qDebug() << "State Out";
    emit signalDoStateOut();
}

void SlideWidget::slotStateInFinished()
{
    qDebug() << "State in finished" << pos();
}

void SlideWidget::slotStateOutFinished()
{
    qDebug() << "State out finished" << pos();
}
