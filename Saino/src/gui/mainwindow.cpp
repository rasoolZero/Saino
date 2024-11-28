#include "gui/mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "gui/config.h"
#include "logic/datastorage.h"
#include "logic/excelhelper.h"
#include "logic/parser.h"
#include "serial/serialcontroller.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&Parser::getInstance(), &Parser::packetGenerated, this, &MainWindow::newPacket);
    setupMenuBar();
    setupGauge();
    setupTables();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newPacket(SPL::Packet packet)
{
    updateDetailTables(packet);
    updateMainIndicators(packet);
}

void MainWindow::setupMenuBar()
{
    connect(this->ui->actionOpen_Config, &QAction::triggered, this, &MainWindow::openConfig);
    connect(this->ui->actionStart, &QAction::triggered, this, &MainWindow::startSerial);
    connect(this->ui->actionStop, &QAction::triggered, this, &MainWindow::stopSerial);
    connect(this->ui->actionSave_into_Excel, &QAction::triggered, this, &MainWindow::saveExcel);
}

void MainWindow::setupGauge()
{
    using ids = SPL::DataID;
    CustomGauge *const allGauges[] = {ui->gauge1, ui->gauge2, ui->gauge3, ui->gauge4, ui->gauge5};
    auto info = SPL::getInfo();
    int i = 0;
    QColor mainColor(0xef, 0xf0, 0xf1);
    QColor accent(0x3d, 0xae, 0xe9);
    auto background = QColor(0x31, 0x36, 0x3b).lighter(150);
    for (CustomGauge *gauge : allGauges) {
        gauge->setGaugeProperties(info[gaugeIDs[i]].name,
                                  info[gaugeIDs[i]].minValue,
                                  info[gaugeIDs[i]].maxValue,
                                  mainColor,
                                  accent,
                                  background);
        this->gauges[i] = gauge;
        i++;
    }
}

void MainWindow::setupTables()
{
    const auto &allInfo = SPL::getInfo();

    auto tblError = this->ui->tblError;
    auto tblValue = this->ui->tblValue;

    auto allErrors = SPL::allErrorCodes();
    tblError->setRowCount(allErrors.size());
    int i = 0;
    foreach (auto &errorCode, allErrors) {
        tblError->setItem(i, 0, new QTableWidgetItem(allInfo[errorCode].name));
        tblError->setItem(i, 1, new QTableWidgetItem("NaN"));
        ++i;
    }
    tblError->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto allDataCodes = SPL::allDataCodes();
    tblValue->setRowCount(allDataCodes.size());
    i = 0;
    foreach (auto &dataCode, allDataCodes) {
        tblValue->setItem(i, 0, new QTableWidgetItem(allInfo[dataCode].name));
        tblValue->setItem(i, 1, new QTableWidgetItem("NaN"));
        ++i;
    }
    tblValue->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::updateDetailTables(SPL::Packet &packet)
{
    const auto &allInfo = SPL::getInfo();
    auto tblError = this->ui->tblError;
    auto tblValue = this->ui->tblValue;
    auto allErrors = SPL::allErrorCodes();
    auto allDataCodes = SPL::allDataCodes();
    foreach (const auto &packetData, packet.getAllPackets()) {
        auto id = packetData.getId();
        auto value = packetData.getValue();
        auto name = allInfo[id].name;
        if (allErrors.contains(static_cast<SPL::DataID>(id))) {
            auto items = tblError->findItems(name, Qt::MatchExactly);
            auto rowIndex = tblError->row(items[0]);
            QString str = static_cast<bool>(value) ? "Error" : "Ok";
            tblError->item(rowIndex, 1)->setText(QString(str));
        } else if (allDataCodes.contains(static_cast<SPL::DataID>(id))) {
            auto items = tblValue->findItems(name, Qt::MatchExactly);
            auto rowIndex = tblValue->row(items[0]);
            tblValue->item(rowIndex, 1)->setText(QString::number(value));
        }
    }
}

void MainWindow::updateMainIndicators(SPL::Packet &packet)
{
    using ids = SPL::DataID;
    auto allErrors = SPL::allErrorCodes();
    this->ui->ledLast->setState(false);
    foreach (const auto &packetData, packet.getAllPackets()) {
        auto id = packetData.getId();
        auto idEnum = static_cast<ids>(id);
        auto value = packetData.getValue();
        if (gaugeIDs.contains(idEnum)) {
            auto index = gaugeIDs.indexOf(idEnum);
            this->gauges[index]->setValue(value);
        } else if (allErrors.contains(idEnum) && static_cast<bool>(value)) {
            this->ui->ledLast->setState(true);
            this->ui->ledAll->setState(true);
        }
    }
}

void MainWindow::resetUI()
{
    for (int i = 0; i < this->ui->tblError->rowCount(); i++)
        this->ui->tblError->item(i, 1)->setText("NaN");
    for (int i = 0; i < this->ui->tblValue->rowCount(); i++)
        this->ui->tblValue->item(i, 1)->setText("NaN");
    foreach (auto ptr, this->gauges) {
        ptr->setValue(0);
    }
    this->ui->ledAll->setState(false);
    this->ui->ledLast->setState(false);
}

void MainWindow::openConfig()
{
    Config *config = new Config(this);
    config->exec();
}

void MainWindow::startSerial()
{
    DataStorage::getInstance().reset();
    Parser::getInstance().reset();
    resetUI();
    bool result;
    try {
        result = SerialController::getInstance().connect();
    } catch (EmptyPortName &) {
        QMessageBox::critical(this, "Failure", "Please choose a port name in config menu");
        return;
    }
    if (result) {
        this->ui->actionOpen_Config->setDisabled(true);
        this->ui->actionStart->setDisabled(true);
        this->ui->actionSave_into_Excel->setDisabled(true);
        QMessageBox::information(this, "Success", "Serial connection has been opened successfully");
    } else {
        QMessageBox::critical(this,
                              "Failure",
                              "Failed to open serial connect\n"
                                  + SerialController::getInstance().error());
    }
}

void MainWindow::stopSerial()
{
    this->ui->actionOpen_Config->setDisabled(false);
    this->ui->actionStart->setDisabled(false);
    this->ui->actionSave_into_Excel->setDisabled(false);
    if (SerialController::getInstance().disconnect())
        QMessageBox::information(this, "Port Closed", "serial port has been closed");
}

void MainWindow::saveExcel()
{
    bool saved = false;
    try {
        ExcelHelper saver;
        QString filename = QFileDialog::getSaveFileName(this,
                                                        tr("Save as Excel"),
                                                        QDir::homePath(),
                                                        tr("Excel (*.xlsx)"));
        if (filename.isEmpty())
            return;
        saved = saver.save(filename);
    } catch (const EmptyStorage &) {
        QMessageBox::critical(this, "No Data", "No data in the storage to be saved");
    } catch (const QException &e) {
        QMessageBox::critical(this, "Error", QString("Error while saving data\n") + e.what());
    }
    if (saved)
        QMessageBox::information(this, "Success", "Data has been saved in excel file");
    else
        QMessageBox::critical(this, "Error", "Could not save the excel file");
}
