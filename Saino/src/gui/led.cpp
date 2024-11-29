#include "gui/led.h"
#include <QDebug>
#include <QPainter>
static int currentSize = 0;

void LED::updateStyle()
{
    switch (current) {
    case StateOk:
    default:
        colorGradient = QLinearGradient();
        colorGradient.setColorAt(0.1, QColor(12, 230, 113, 255));
        colorGradient.setColorAt(1, QColor(0, 218, 101, 255));
        break;
    case StateError:
        colorGradient = QLinearGradient();
        colorGradient.setColorAt(0.1, QColor(255, 68, 14, 255));
        colorGradient.setColorAt(1, QColor(255, 57, 0, 255));
        break;
    }
}

void LED::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(painter.Antialiasing);
    auto rect = this->rect();
    auto mPen = Qt::NoPen;
    painter.setBrush(Qt::NoBrush);
    painter.setPen(mPen);
    painter.setBrush(this->colorGradient);
    painter.drawEllipse(rect);
}

LED::LED(QWidget *parent)
    : QWidget(parent)
{
    setState(false);
    updateStyle();
    update();
}

void LED::setState(State state)
{
    if (current != state) {
        current = state;
        updateStyle();
        update();
    }
}

void LED::setState(bool state)
{
    setState(state ? StateError : StateOk);
}

void LED::resizeEvent(QResizeEvent *event)
{
    currentSize = std::min(event->size().height(), event->size().width());
    this->resize(currentSize, currentSize);
    update();
}
