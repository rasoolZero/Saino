#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "serialcontroller.h"
#include "datastorage.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupMenuBar();
    setupGauge();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *projectMenu = new QMenu("Project", menuBar);
    QAction *openConfig = new QAction("Open Config", projectMenu);
    QAction *start = new QAction("Start", projectMenu);
    QAction *stop = new QAction("Stop", projectMenu);
    connect(openConfig, &QAction::triggered, this, &MainWindow::openConfig);
    connect(start, &QAction::triggered, this, &MainWindow::startSerial);
    connect(stop, &QAction::triggered, this, &MainWindow::stopSerial);
    projectMenu->addAction(openConfig);
    projectMenu->addSeparator();
    projectMenu->addAction(start);
    projectMenu->addAction(stop);
    menuBar->addMenu(projectMenu);
    this->setMenuBar(menuBar);
}

void MainWindow::setupGauge()
{
    using ids = DataStorage::DataID;
    QcGaugeWidget* const allGauges[] = {ui->gaugeFuel,ui->gaugeMotorSpeed,ui->gaugeOilPressure,ui->gaugeOilTempreture,ui->gaugeTorque};
    const ids gaugeIDs[] = {ids::FUEL,ids::MOTOR_SPEED,ids::OIL_PRESSURE,ids::OIL_TEMPERATURE,ids::TORQUE};
    auto info = DataStorage::getInstance().getInfo();
    int i=0;
    for(QcGaugeWidget* gauge : allGauges){
        auto bg2 = gauge->addBackground(92);
        bg2->clearrColors();
        bg2->addColor(0.1f,Qt::blue);
        bg2->addColor(1.f,Qt::darkBlue);
        gauge->addArc(90);
        auto bg = gauge->addBackground(90);
        bg->clearrColors();
        bg->addColor(0.1f,Qt::lightGray);
        bg->addColor(1.f,Qt::darkGray);
        auto values = gauge->addValues(80);
        values->setValueRange(info[gaugeIDs[i]].minValue,info[gaugeIDs[i]].maxValue);
        values->setStep((info[gaugeIDs[i]].maxValue - info[gaugeIDs[i]].minValue)/10);
        gauge->addLabel(40)->setText(info[gaugeIDs[i]].name);
        gauge->addNeedle(70)->setValueRange(info[gaugeIDs[i]].minValue,info[gaugeIDs[i]].maxValue);
        i++;
    }
}

void MainWindow::openConfig()
{
    Config* config = new Config(this);
    config->exec();
}

void MainWindow::startSerial()
{
    bool result;
    try{
        result = SerialController::getInstance().connect();
    }
    catch(EmptyPortName &){
        QMessageBox::critical(this,"Failure","Please choose a port name in config menu");
        return;
    }
    if(result){
        QMessageBox::information(this,"Success","Serial connection has been opened successfully");
    }
    else{
        QMessageBox::critical(this,"Failure","Failed to open serial connect\n"+SerialController::getInstance().error());
    }
}

void MainWindow::stopSerial()
{
    SerialController::getInstance().disconnect();
}
