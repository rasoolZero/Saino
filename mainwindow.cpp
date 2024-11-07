#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "serialcontroller.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupMenuBar();
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
