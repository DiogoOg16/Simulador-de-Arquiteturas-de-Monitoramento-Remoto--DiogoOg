#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QHostAddress>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //LOG File
    fileName = "logger.txt";
    logger = new Logger(this, fileName, this->ui->logplaintextedit);
    logger->write("Hello Qt");

    //Init Protocol
    Lifes_Protocol.Init_Lifes_SIM(logger);

    timerTCP = new QTimer(this);
    connect(timerTCP, SIGNAL(timeout()), this, SLOT(SendInFreq()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectbutton_clicked()
{
    //socket
    //socketTCP.Connect();
    Lifes_Protocol.lifes_sim_tools.socketTCP.Connect(this->ui->ipTextBox->text(), this->ui->portTextBox->text().toUInt());
    logger->write("Connected!");
}

void MainWindow::on_startbutton_clicked()
{
    _ConfigPub config = Lifes_Protocol.Configuracoes_de_Publicacao();

    // Caso o gateway nao tenha configurado, utiliza as configuracoes da interface
    if (config.sampleRate == 0)
    {
        config.bitarray_enabled_sensors = 0;
        if (this->ui->enableACC->isChecked()) config.bitarray_enabled_sensors |= CONFIG_ENABLED_ACC;
        if (this->ui->enableBVP->isChecked()) config.bitarray_enabled_sensors |= CONFIG_ENABLED_BVP;
        if (this->ui->enableEDA->isChecked()) config.bitarray_enabled_sensors |= CONFIG_ENABLED_EDA;
        if (this->ui->enableHR->isChecked()) config.bitarray_enabled_sensors |= CONFIG_ENABLED_HR;
        if (this->ui->enableTemp->isChecked()) config.bitarray_enabled_sensors |= CONFIG_ENABLED_TEMP;
        config.sampleRate = this->ui->sampleRate->value();
        Lifes_Protocol.Configuracoes_pela_Interface(config);
    }

    if (parser->get_size())
    {
        parsed_index_sent = 0;
        logger->write("Started!");
        timerTCP->start(config.sampleRate * this->ui->pubInterval->value());
    }
    else
    {
        logger->write("Any data parsed to send!");
    }
}

void MainWindow::on_stopbutton_clicked()
{
   logger->write("Stoped!");
   timerTCP->stop();
}

void MainWindow::SendInFreq()
{
    static unsigned char result;
    static _life_sim_csv_dataset value;
    _ConfigPub config = Lifes_Protocol.Configuracoes_de_Publicacao();

    if (parser->get_item(parsed_index_sent, &value))
    {
        /*  Update data to send */
        result = Lifes_Protocol.Atualiza_Curvas(CMD_TYPE_SENSOR_DATA_ACC, value.data.acc, parsed_index_sent);
        result += Lifes_Protocol.Atualiza_Curvas(CMD_TYPE_SENSOR_DATA_BVP, &value.data.bvp, parsed_index_sent);
        result += Lifes_Protocol.Atualiza_Curvas(CMD_TYPE_SENSOR_DATA_EDA, &value.data.eda, parsed_index_sent);
        result += Lifes_Protocol.Atualiza_Curvas(CMD_TYPE_SENSOR_DATA_HR, &value.data.hr, parsed_index_sent);
        result += Lifes_Protocol.Atualiza_Curvas(CMD_TYPE_SENSOR_DATA_TEMP, &value.data.temp, parsed_index_sent);

        /*  Error */
        if (result != 0)
            logger->write("Fail updating data structure");
    }
    else
    {
        logger->write("Get CSV parsed information fails");
        logger->write("Stopped!");
        timerTCP->stop();
        return;
    }

    /*  Enabled sensors to send */
    if (config.bitarray_enabled_sensors & CONFIG_ENABLED_ACC)
    {
        Lifes_Protocol.lifes_SIM_comando(CMD_TYPE_SENSOR_DATA_ACC);
        logger->write("sent acc data");
    }
    if (config.bitarray_enabled_sensors & CONFIG_ENABLED_BVP)
    {
        Lifes_Protocol.lifes_SIM_comando(CMD_TYPE_SENSOR_DATA_BVP);
        logger->write("sent bvp data");
    }
    if (config.bitarray_enabled_sensors & CONFIG_ENABLED_EDA)
    {
        Lifes_Protocol.lifes_SIM_comando(CMD_TYPE_SENSOR_DATA_EDA);
        logger->write("sent eda data");
    }
    if (config.bitarray_enabled_sensors & CONFIG_ENABLED_HR)
    {
        Lifes_Protocol.lifes_SIM_comando(CMD_TYPE_SENSOR_DATA_HR);
        logger->write("sent hr data");
    }
    if (config.bitarray_enabled_sensors & CONFIG_ENABLED_TEMP)
    {
        Lifes_Protocol.lifes_SIM_comando(CMD_TYPE_SENSOR_DATA_TEMP);
        logger->write("sent temp data");
    }

    // move pointer
    parsed_index_sent += config.sampleRate;
}

void MainWindow::on_loadLabel_clicked()
{
    parser = new _dataset_parser(this, logger, QFileDialog::getOpenFileName(this,
                                                              tr("Open LABEL file"),
                                                              "..\\lifesenior-dataset",
                                                              tr("Dataset label file (*csv)")));

    this->ui->personActivity_Selector->addItems(parser->get_personactivity_list());
}

void MainWindow::on_loadSensors_clicked()
{
    QString path;


    path = QFileDialog::getExistingDirectory(this,
                                 tr("Open Dataset directory"),
                                 "..\\lifesenior-dataset",
                                 QFileDialog::ShowDirsOnly);
    if (parser == nullptr)
    {
        parser = new _dataset_parser(this, logger);
        logger->write("Loading first line from sensors");
    }
    else
    {
        logger->write("Loading data of " + this->ui->personActivity_Selector->currentText());
    }
    parser->load_sensor_data(path + '/', this->ui->personActivity_Selector->currentIndex());
}

