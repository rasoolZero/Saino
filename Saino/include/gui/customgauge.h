#ifndef CUSTOMGAUGE_H
#define CUSTOMGAUGE_H
#include "gui/qcgaugewidget.h"

class CustomGauge : public QcGaugeWidget
{
public:
    CustomGauge(QWidget *parent = 0);
    void setGaugeProperties(QString name,
                            float min,
                            float max,
                            QColor textColor,
                            QColor accentColor,
                            QColor backgroundColor);
    void setValue(float newValue);

private:
    QcLabelItem *valueLabel;
    QcNeedleItem *needle;

    float value;
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // CUSTOMGAUGE_H
