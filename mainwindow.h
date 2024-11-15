#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "packet.h"
#include <QMap>
#include "qcgaugewidget.h"
#include "datastorage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    using ids = DataStorage::DataID;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void newPacket(Packet packet);

private:
    Ui::MainWindow *ui;
    void setupMenuBar();
    void setupGauge();
    void setupTables();
    void updateDetailTables(Packet& packet);
    void updateMainIndicators(Packet& packet);
    QMap<int,QcItem*> gaugeLabels;
    QMap<int,QcItem*> gaugeNeedles;
    const QVector<ids> gaugeIDs = {ids::OIL_PRESSURE,ids::OIL_TEMPERATURE,ids::FUEL,ids::TORQUE,ids::MOTOR_SPEED};

private slots:
    void openConfig();
    void startSerial();
    void stopSerial();
};
#endif // MAINWINDOW_H
