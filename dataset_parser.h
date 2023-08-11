#ifndef DATASET_PARSER_H
#define DATASET_PARSER_H

#include <QObject>
#include <QList>

#include "Logger.h"
#include "protocol.h"

class _dataset_parser : public QObject
{
    Q_OBJECT
public:
    explicit _dataset_parser(QObject *parent = nullptr, Logger *logger = nullptr, QString dirpath = "");

private:
    Logger* logger;
    QStringList label_list;
    QList<_life_sim_csv_dataset> parser_list;
    QList<QString> csv_filenames = {
                                    "ACC-X_TESTE",
                                    "ACC-Y_TESTE",
                                    "ACC-Z_TESTE",
                                    "BVP_TESTE",
                                    "EDA_TESTE",
                                    "HR_TESTE",
                                    "TEMP_TESTE",
                                 };
    QString get_sensor_info(QString path, unsigned int sensor, unsigned int line);

signals:

public slots:
    QStringList get_personactivity_list(void);
    void load_sensor_data(QString path, int person_activity);
    bool get_item(unsigned int pos, _life_sim_csv_dataset* values);
    size_t get_size(void);

};

#endif // DATASET_PARSER_H
