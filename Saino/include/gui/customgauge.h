#ifndef CUSTOMGAUGE_H
#define CUSTOMGAUGE_H
#include "gui/qcgaugewidget.h"

// custom gauge which uses QcGaugeWidget, has better and easier API
class CustomGauge : public QcGaugeWidget
{
public:
    CustomGauge(QWidget *parent = 0);
    void setGaugeProperties(QString name,
                            float min,
                            float max,
                            QColor textColor,
                            QColor accentColor,
                            QColor backgroundColor,
                            int valueRanges = 10);
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
