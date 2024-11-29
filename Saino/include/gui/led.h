#ifndef QLEDLABEL_H
#define QLEDLABEL_H

#include <QLabel>
#include <QResizeEvent>

// LED indicator with 2 states: ok (green) and error (red)
class LED : public QWidget
{
    Q_OBJECT
public:
    explicit LED(QWidget *parent = 0);

    enum State { StateOk, StateError };

signals:

public slots:
    void setState(State state);
    // true means error, false means ok
    void setState(bool state);

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    State current;
    QLinearGradient colorGradient;
    void updateStyle();

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // QLEDLABEL_H
