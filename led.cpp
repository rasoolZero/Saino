#include "led.h"
#include <QDebug>

static int currentSize = 0;

static const QString greenSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 rgba(20, 252, 7, 255), stop:1 rgba(25, 134, 5, 255));");
static const QString redSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:0.92, y2:0.988636, stop:0 rgba(255, 12, 12, 255), stop:0.869347 rgba(103, 0, 0, 255));");

void LED::updateStyle()
{
    switch(current){
        case StateOk:
        default:
            setStyleSheet(greenSS.arg(currentSize/2));
        break;
        case StateError:
            setStyleSheet(redSS.arg(currentSize/2));
        break;
    }
}

LED::LED(QWidget *parent) :
    QLabel(parent)
{
    setState(false);
}

void LED::setState(State state)
{
    current = state;
    updateStyle();
}

void LED::setState(bool state)
{
    setState(state ? StateOk : StateError);
}

void LED::resizeEvent(QResizeEvent *event)
{
    currentSize = std::min(event->size().height(),event->size().width());
    updateStyle();
}


