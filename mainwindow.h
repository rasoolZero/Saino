#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    void setupMenuBar();
    void setupGauge();
    void setupTables();
private slots:
    void openConfig();
    void startSerial();
    void stopSerial();
};
#endif // MAINWINDOW_H
