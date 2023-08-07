#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDir>
#include "Logger.h"
#include "Lifes_Protocol.h"
#include "protocol.h"
#include "Socket_TCP.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString fileName;
    Logger *logger;
    QTimer *timerTCP;
    _Lifes_Protocol Lifes_Protocol;
private slots:
    void on_connectbutton_clicked();
    void SendInFreq();

    void on_startbutton_clicked();
    void on_stopbutton_clicked();

    void on_loadLabel_clicked();
    void on_loadSensors_clicked();

private:
    Ui::MainWindow *ui;
    _dataset_parser *parser = nullptr;
    double last_timestamp;
    unsigned int parsed_index_sent;
};
#endif // MAINWINDOW_H
