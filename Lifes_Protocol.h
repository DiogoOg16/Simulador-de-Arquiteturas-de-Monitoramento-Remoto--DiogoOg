#ifndef LIFES_PROTOCOL_H
#define LIFES_PROTOCOL_H


#include <QObject>
#include <QPlainTextEdit>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "lifes_sim_tools.h"
#include "Logger.h"
#include "protocol.h"
#include "dataset_parser.h"

class _Lifes_Protocol : public QObject {

Q_OBJECT
public:

_lifes_sim_tools lifes_sim_tools;
private:
Logger* logger;


public slots:
void Init_Lifes_SIM(Logger*);
unsigned char TestaCRC(unsigned char crc, unsigned char data);
unsigned char lifes_SIM_comando(_command_types comando);
void Configuracoes_pela_Interface(_ConfigPub config);
_ConfigPub Configuracoes_de_Publicacao(void);
unsigned char Atualiza_Curvas(_command_types cmd, double* sensor_data, unsigned int pos);
void lifes_sim_in(void);
};


#endif // LIFES_PROTOCOL_H
