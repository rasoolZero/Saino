#include "gui/led.h"
#include <QDebug>

static int currentSize = 0;

void LED::updateStyle()
{
    switch (current) {
    case StateOk:
    default:
        setStyleSheet(greenSS.arg(currentSize / 2));
        break;
    case StateError:
        setStyleSheet(redSS.arg(currentSize / 2));
        break;
    }
}

LED::LED(QWidget *parent)
    : QLabel(parent)
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
    setState(state ? StateError : StateOk);
}

void LED::resizeEvent(QResizeEvent *event)
{
    currentSize = std::min(event->size().height(), event->size().width());
    this->resize(currentSize, currentSize);
    updateStyle();
}
