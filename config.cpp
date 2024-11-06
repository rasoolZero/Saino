#include "config.h"
#include "ui_config.h"
#include <serialmanager.h>

Config::Config(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Config)
{
    ui->setupUi(this);
    foreach(auto& port,SerialManager::getPorts()){
        ui->cmbPorts->addItem(port.portName());
    }
    foreach(auto& rate, SerialManager::getBaudRates()){
        ui->cmbRates->addItem( QString::number(rate) ,rate);
    }
    foreach(auto& parity, SerialManager::getParities()){
        ui->cmbParity->addItem(QtEnumToString(parity),parity);
    }
    foreach(auto& stopbit, SerialManager::getStopbits()){
        ui->cmbStopbits->addItem(QtEnumToString(stopbit),stopbit);
    }
    ui->cmbParity->setCurrentText(QtEnumToString(SerialManager::getInstance().getParity()));
    ui->cmbRates->setCurrentText(QString::number(SerialManager::getInstance().getRate()));
    ui->cmbStopbits->setCurrentText(QtEnumToString(SerialManager::getInstance().getStopbit()));

}

Config::~Config()
{
    delete ui;
}
