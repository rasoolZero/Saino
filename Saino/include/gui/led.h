#ifndef QLEDLABEL_H
#define QLEDLABEL_H

#include <QLabel>
#include <QResizeEvent>

class LED : public QWidget
{
    Q_OBJECT
public:
    explicit LED(QWidget *parent = 0);

    enum State { StateOk, StateError };

signals:

public slots:
    void setState(State state);
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
