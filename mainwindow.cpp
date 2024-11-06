#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"

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
    connect(openConfig, &QAction::triggered, this, &MainWindow::openConfig);
    projectMenu->addAction(openConfig);
    menuBar->addMenu(projectMenu);
    this->setMenuBar(menuBar);
}

void MainWindow::openConfig()
{
    Config* config = new Config(this);
    config->exec();
}
