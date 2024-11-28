#ifndef QLEDLABEL_H
#define QLEDLABEL_H

#include <QLabel>
#include <QResizeEvent>

class LED : public QLabel
{
    inline static const QString greenSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:1, y2:1, stop:0 rgba(12, 230, 113, 255), stop:1 rgba(0, 218, 101, 255));");
    inline static const QString redSS = QString("color: white;border-radius: %1;background-color: qlineargradient(spread:pad, x1:0.145, y1:0.16, x2:0.92, y2:0.988636, stop:0 rgba(255, 68, 14, 255), stop:0.869347 rgba(255, 57, 0, 255));");

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

