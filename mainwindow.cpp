#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "serialcontroller.h"
#include "datastorage.h"
#include "parser.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "excelhelper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&Parser::getInstance(),&Parser::packetGenerated,this,&MainWindow::newPacket);
    setupMenuBar();
    setupGauge();
    setupTables();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newPacket(Packet packet)
{
    updateDetailTables(packet);
    updateMainIndicators(packet);
}

void MainWindow::setupMenuBar()
{
    connect(this->ui->actionOpen_Config, &QAction::triggered, this, &MainWindow::openConfig);
    connect(this->ui->actionStart, &QAction::triggered, this, &MainWindow::startSerial);
    connect(this->ui->actionStop, &QAction::triggered, this, &MainWindow::stopSerial);
    connect(this->ui->actionSave_into_Excel,&QAction::triggered,this,&MainWindow::saveExcel);
}

void MainWindow::setupGauge()
{
    using ids = DataStorage::DataID;
    QcGaugeWidget* const allGauges[] = {ui->gauge1,ui->gauge2,ui->gauge3,ui->gauge4,ui->gauge5};
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
        auto needle = gauge->addNeedle(70);
        needle->setValueRange(info[gaugeIDs[i]].minValue,info[gaugeIDs[i]].maxValue);
        gaugeNeedles[i] = needle;
        auto label = gauge->addLabel(20);
        label->setText("0");
        gaugeLabels[i] = label;
        i++;
    }
}

void MainWindow::setupTables()
{
    const DataStorage& instance = DataStorage::getInstance();
    const auto & allInfo = instance.getInfo();

    auto tblError = this->ui->tblError;
    auto tblValue = this->ui->tblValue;

    auto allErrors = instance.allErrorCodes();
    tblError->setRowCount(allErrors.size());
    int i=0;
    foreach(auto&errorCode,allErrors){
        tblError->setItem(i,0,new QTableWidgetItem(allInfo[errorCode].name));
        tblError->setItem(i,1,new QTableWidgetItem("NaN"));
        ++i;
    }
    tblError->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto allDataCodes = instance.allDataCodes();
    tblValue->setRowCount(allDataCodes.size());
    i=0;
    foreach(auto&dataCode,allDataCodes){
        tblValue->setItem(i,0,new QTableWidgetItem(allInfo[dataCode].name));
        tblValue->setItem(i,1,new QTableWidgetItem("NaN"));
        ++i;
    }
    tblValue->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::updateDetailTables(Packet &packet)
{
    const DataStorage& instance = DataStorage::getInstance();
    const auto & allInfo = instance.getInfo();
    auto tblError = this->ui->tblError;
    auto tblValue = this->ui->tblValue;
    auto allErrors = instance.allErrorCodes();
    auto allDataCodes = instance.allDataCodes();
    foreach(const auto& packetData,packet.getAllPackets()){
        auto id = packetData.getId();
        auto value = packetData.getValue();
        auto name = allInfo[id].name;
        if(allErrors.contains(static_cast<DataStorage::DataID>(id))){
            auto items = tblError->findItems(name,Qt::MatchExactly);
            auto rowIndex = tblError->row(items[0]);
            QString str = static_cast<bool>(value)?"Error":"Ok";
            tblError->item(rowIndex,1)->setText(QString(str));
        }
        else if(allDataCodes.contains(static_cast<DataStorage::DataID>(id))){
            auto items = tblValue->findItems(name,Qt::MatchExactly);
            auto rowIndex = tblValue->row(items[0]);
            tblValue->item(rowIndex,1)->setText(QString::number(value));
        }
    }
}

void MainWindow::updateMainIndicators(Packet &packet)
{
    using ids = DataStorage::DataID;
    const DataStorage& instance = DataStorage::getInstance();
    auto allErrors = instance.allErrorCodes();
    this->ui->ledLast->setState(false);
    foreach(const auto& packetData,packet.getAllPackets()){
        auto id = packetData.getId();
        auto idEnum = static_cast<ids>(id);
        auto value = packetData.getValue();
        if(gaugeIDs.contains(idEnum)){
            auto index = gaugeIDs.indexOf(idEnum);
            QcNeedleItem* needle = dynamic_cast<QcNeedleItem*>(gaugeNeedles[index]);
            needle->setCurrentValue(value);
            QcLabelItem* label = dynamic_cast<QcLabelItem*>(gaugeLabels[index]);
            label->setText(QString::number(value));
        }
        else if(allErrors.contains(idEnum) && static_cast<bool>(value)){
            this->ui->ledLast->setState(true);
            this->ui->ledAll->setState(true);
        }
    }
}

void MainWindow::resetUI()
{
    for(int i=0;i<this->ui->tblError->rowCount();i++)
        this->ui->tblError->item(i,1)->setText("NaN");
    for(int i=0;i<this->ui->tblValue->rowCount();i++)
        this->ui->tblValue->item(i,1)->setText("NaN");
    foreach(auto ptr,this->gaugeLabels){
        QcLabelItem* label = dynamic_cast<QcLabelItem*>(ptr);
        label->setText("0");
    }
    foreach(auto ptr,this->gaugeNeedles){
        QcNeedleItem* needle = dynamic_cast<QcNeedleItem*>(ptr);
        needle->setCurrentValue(0);
    }
    this->ui->ledAll->setState(false);
    this->ui->ledLast->setState(false);
}

void MainWindow::openConfig()
{
    Config* config = new Config(this);
    config->exec();
}

void MainWindow::startSerial()
{
    DataStorage::getInstance().reset();
    Parser::getInstance().reset();
    resetUI();
    bool result;
    try{
        result = SerialController::getInstance().connect();
    }
    catch(EmptyPortName &){
        QMessageBox::critical(this,"Failure","Please choose a port name in config menu");
        return;
    }
    if(result){
        this->ui->actionOpen_Config->setDisabled(true);
        this->ui->actionStart->setDisabled(true);
        this->ui->actionSave_into_Excel->setDisabled(true);
        QMessageBox::information(this,"Success","Serial connection has been opened successfully");
    }
    else{
        QMessageBox::critical(this,"Failure","Failed to open serial connect\n"+SerialController::getInstance().error());
    }
}

void MainWindow::stopSerial()
{
    this->ui->actionOpen_Config->setDisabled(false);
    this->ui->actionStart->setDisabled(false);
    this->ui->actionSave_into_Excel->setDisabled(false);
    if(SerialController::getInstance().disconnect())
        QMessageBox::information(this,"Port Closed","serial port has been closed");
}

void MainWindow::saveExcel()
{
    bool saved = false;
    try{
        ExcelHelper saver;
        QString filename = QFileDialog::getSaveFileName(this,tr("Save as Excel"),QDir::homePath(),tr("Excel (*.xlsx)"));
        if(filename.isEmpty())
            return;
        saver.save(filename);
        saved = true;
    }
    catch(const EmptyStorage &){
        QMessageBox::critical(this,"No Data","No data in the storage to be saved");
    }
    catch(const QException & e){
        QMessageBox::critical(this,"Error",QString("Error while saving data\n")+e.what());
    }
    if(saved)
        QMessageBox::information(this,"Success","Data has been saved in excel file");
}
