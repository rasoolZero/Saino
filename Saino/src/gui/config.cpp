#include "gui/config.h"
#include "serial/serialmanager.h"
#include "ui_config.h"

Config::Config(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Config)
{
    ui->setupUi(this);
    foreach (auto &port, SerialManager::getPorts()) {
        ui->cmbPorts->addItem(port.portName());
    }
    foreach (auto &rate, SerialManager::getBaudRates()) {
        ui->cmbRates->addItem(QString::number(rate), rate);
    }
    foreach (auto &parity, SerialManager::getParities()) {
        ui->cmbParity->addItem(QtEnumToString(parity), parity);
    }
    foreach (auto &stopbit, SerialManager::getStopbits()) {
        ui->cmbStopbits->addItem(QtEnumToString(stopbit), stopbit);
    }
    ui->cmbParity->setCurrentText(QtEnumToString(SerialManager::getInstance().getParity()));
    ui->cmbRates->setCurrentText(QString::number(SerialManager::getInstance().getRate()));
    ui->cmbStopbits->setCurrentText(QtEnumToString(SerialManager::getInstance().getStopbit()));
    ui->cmbPorts->setCurrentText(SerialManager::getInstance().getPort());

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

Config::~Config()
{
    delete ui;
}

void Config::onAccept()
{
    SerialManager::getInstance().setPort(ui->cmbPorts->currentText());
    SerialManager::getInstance().setParity(
        ui->cmbParity->currentData().value<SerialManager::Parity>());
    SerialManager::getInstance().setRate(ui->cmbRates->currentData().value<qint32>());
    SerialManager::getInstance().setStopbit(
        ui->cmbStopbits->currentData().value<SerialManager::StopBits>());
    this->accept();
}
