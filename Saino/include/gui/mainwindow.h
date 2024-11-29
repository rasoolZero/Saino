#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "gui/customgauge.h"
#include "logic/datastorage.h"
#include <SPL/common.h>
#include <SPL/packet.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    using ids = SPL::DataID;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void newPacket(SPL::Packet packet);

private:
    Ui::MainWindow *ui;
    void setupMenuBar();
    void setupGauge();
    void setupTables();
    void updateDetailTables(SPL::Packet &packet);
    void updateMainIndicators(SPL::Packet &packet);
    // resets UI, used when a new serial connection is opened
    void resetUI();
    QMap<int, CustomGauge *> gauges;

    // list of data IDs showen with gauge widgets
    const QVector<ids> gaugeIDs = {ids::OIL_PRESSURE,
                                   ids::OIL_TEMPERATURE,
                                   ids::FUEL,
                                   ids::TORQUE,
                                   ids::MOTOR_SPEED};

    void updateValueTable(QString name, qreal value);
    void updateErrorTable(QString name, qreal value);

    void setupErrorTable();

    void setupValueTable();

private slots:
    void openConfig();
    void startSerial();
    void stopSerial();
    void saveExcel();
};
#endif // MAINWINDOW_H
