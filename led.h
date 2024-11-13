#ifndef QLEDLABEL_H
#define QLEDLABEL_H

#include <QLabel>
#include <QResizeEvent>

class LED : public QLabel
{
    Q_OBJECT
public:
    explicit LED(QWidget *parent = 0);

    enum State{
        StateOk,
        StateError
    };

signals:

public slots:
    void setState(State state);
    void setState(bool state);


    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    State current;

    void updateStyle();
};

#endif // QLEDLABEL_H

