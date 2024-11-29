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

    // these colors are based on dark.qss stylesheet
    QColor mainColor(0xef, 0xf0, 0xf1);
    QColor accent(0x3d, 0xae, 0xe9);
    auto background = QColor(0x31, 0x36, 0x3b).lighter(150);
    for (CustomGauge *gauge : allGauges) {
        // set the properties of each gauge widget based on the data info
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

void MainWindow::setupErrorTable()
{
    const auto &allInfo = SPL::getInfo();
    auto tblError = this->ui->tblError;
    // get all of error codes, loop through each row
    // set the 1st column to the error code's name
    // set the 2nd column to NaN
    auto allErrors = SPL::allErrorCodes();
    tblError->setRowCount(allErrors.size());
    int i = 0;
    foreach (auto &errorCode, allErrors) {
        tblError->setItem(i, 0, new QTableWidgetItem(allInfo[errorCode].name));
        tblError->setItem(i, 1, new QTableWidgetItem("NaN"));
        ++i;
    }
    tblError->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::setupValueTable()
{
    auto tblValue = this->ui->tblValue;
    const auto &allInfo = SPL::getInfo();
    // get all of value codes, loop through each row
    // set the 1st column to the value data's name
    // set the 2nd column to NaN
    auto allDataCodes = SPL::allDataCodes();
    tblValue->setRowCount(allDataCodes.size());
    int i = 0;
    foreach (auto &dataCode, allDataCodes) {
        tblValue->setItem(i, 0, new QTableWidgetItem(allInfo[dataCode].name));
        tblValue->setItem(i, 1, new QTableWidgetItem("NaN"));
        ++i;
    }
    tblValue->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::setupTables()
{
    setupErrorTable();
    setupValueTable();
}

void MainWindow::updateValueTable(QString name, qreal value)
{
    // find the row containg the name and update second column
    auto items = ui->tblValue->findItems(name, Qt::MatchExactly);
    auto rowIndex = ui->tblValue->row(items[0]);
    ui->tblValue->item(rowIndex, 1)->setText(QString::number(value));
}

void MainWindow::updateErrorTable(QString name, qreal value)
{
    // find the row containg the name and update second column
    auto items = ui->tblError->findItems(name, Qt::MatchExactly);
    auto rowIndex = ui->tblError->row(items[0]);
    QString str = static_cast<bool>(value) ? "Error" : "Ok";
    ui->tblError->item(rowIndex, 1)->setText(QString(str));
}

void MainWindow::updateDetailTables(SPL::Packet &packet)
{
    const auto &allInfo = SPL::getInfo();
    auto allErrors = SPL::allErrorCodes();
    auto allDataCodes = SPL::allDataCodes();

    // loop through all of data in the received packet
    foreach (const auto &packetData, packet.getAllPackets()) {
        auto id = packetData.getId();
        auto value = packetData.getValue();
        auto name = allInfo[id].name;

        // if the data is an error, update tblError
        // otherwise update tblValue
        if (allErrors.contains(static_cast<SPL::DataID>(id))) {
            updateErrorTable(name, value);
        } else if (allDataCodes.contains(static_cast<SPL::DataID>(id))) {
            updateValueTable(name, value);
        }
    }
}

void MainWindow::updateMainIndicators(SPL::Packet &packet)
{
    using ids = SPL::DataID;
    auto allErrors = SPL::allErrorCodes();
    this->ui->ledLast->setState(false);

    // loop through all data in the packet
    // if it contains an error, update LEDs
    // if it contains a value shown in gauge widgets, update the widget
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
    // reset tables, set the 2nd column to Nan
    for (int i = 0; i < this->ui->tblError->rowCount(); i++)
        this->ui->tblError->item(i, 1)->setText("NaN");
    for (int i = 0; i < this->ui->tblValue->rowCount(); i++)
        this->ui->tblValue->item(i, 1)->setText("NaN");

    // reset all gauge widgets
    foreach (auto ptr, this->gauges) {
        ptr->setValue(0);
    }
    // reset LEDs
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

    // try to open a new connection
    try {
        result = SerialController::getInstance().connect();
    } catch (EmptyPortName &) {
        QMessageBox::critical(this, "Failure", "Please choose a port name in config menu");
        return;
    }
    if (result) {
        // if connection was opened, disable menubar items
        // user should not temper serial config ordata while connected
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
    // enable menubar items
    this->ui->actionOpen_Config->setDisabled(false);
    this->ui->actionStart->setDisabled(false);
    this->ui->actionSave_into_Excel->setDisabled(false);
    if (SerialController::getInstance().disconnect())
        QMessageBox::information(this, "Port Closed", "serial port has been closed");
}

void MainWindow::saveExcel()
{
    // try to save data in excel
    // if storage is empty or export process fails, show an error
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
