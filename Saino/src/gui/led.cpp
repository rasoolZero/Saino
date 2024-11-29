#include "gui/led.h"
#include <QDebug>
#include <QPainter>
static int currentSize = 0;

void LED::updateStyle()
{
    const static auto red1 = QColor(255, 68, 14, 255);
    const static auto red2 = QColor(255, 57, 0, 255);
    const static auto green1 = QColor(12, 230, 113, 255);
    const static auto green2 = QColor(0, 218, 101, 255);

    switch (current) {
    case StateOk:
    default:
        colorGradient = QLinearGradient();
        colorGradient.setColorAt(0.1, green1);
        colorGradient.setColorAt(1, green2);
        break;
    case StateError:
        colorGradient = QLinearGradient();
        colorGradient.setColorAt(0.1, red1);
        colorGradient.setColorAt(1, red2);
        break;
    }
}

void LED::paintEvent(QPaintEvent *)
{
    // paint a circle
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
    // keep the widget as a square
    currentSize = std::min(event->size().height(), event->size().width());
    this->resize(currentSize, currentSize);
    update();
}
