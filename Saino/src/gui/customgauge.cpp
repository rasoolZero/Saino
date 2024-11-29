#include "gui/customgauge.h"

void CustomGauge::paintEvent(QPaintEvent *event)
{
    QcGaugeWidget::paintEvent(event);
}

CustomGauge::CustomGauge(QWidget *parent)
    : QcGaugeWidget(parent)
{
    this->value = 0.f;
}
void CustomGauge::setGaugeProperties(QString name,
                                     float min,
                                     float max,
                                     QColor textColor,
                                     QColor accentColor,
                                     QColor backgroundColor,
                                     int valueRanges)
{
    // setting up backgrounds, leave a border with the accent color
    // the numbers 92 and 90 are size scales
    auto bg1 = this->addBackground(92);
    bg1->clearrColors();
    bg1->addColor(0.0f, accentColor);
    auto bg2 = this->addBackground(90);
    bg2->clearrColors();
    bg2->addColor(0.0f, backgroundColor);

    // setting up values, separated in 10 ranges
    // the number 80 is size scales (distance from center)
    auto values = this->addValues(80);
    values->setValueRange(min, max);
    values->setStep((max - min) / valueRanges);
    values->setColor(textColor);

    // name of the value represented by this widget
    auto nameLabel = this->addLabel(40);
    nameLabel->setText(name);
    nameLabel->setColor(textColor);

    // add needle with the accent color
    // the number 80 is size scales
    needle = this->addNeedle(70);
    needle->setValueRange(min, max);
    needle->setColor(accentColor);

    // add the value label
    valueLabel = this->addLabel(20);
    valueLabel->setText("0");
    valueLabel->setColor(textColor);
}

void CustomGauge::setValue(float newValue)
{
    if (this->value == newValue)
        return;
    this->value = newValue;
    this->needle->setCurrentValue(this->value);
    this->valueLabel->setText(QString::number(this->value));
}
