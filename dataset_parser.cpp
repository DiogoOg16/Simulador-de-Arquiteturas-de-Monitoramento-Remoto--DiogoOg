

#include "dataset_parser.h"

_dataset_parser::_dataset_parser(QObject *parent, Logger *logger, QString filename)
    : QObject{parent}
{
    QString temp_string;
    QStringList raw_line;

    this->logger = logger;

    label_list.begin();

    if(filename.isEmpty())
    {
        return;
    }

    /* Open file */
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        this->logger->write("Cannot open " + filename + " file!");
        return;
    }

    /* Get labels */
    while (!file.atEnd())
    {
        temp_string = file.readLine();
        raw_line = temp_string.split('\n');
        label_list.append(raw_line[0]);
    }

    file.close();
}

QStringList _dataset_parser::get_personactivity_list(void)
{
    return label_list;
}

void _dataset_parser::load_sensor_data(QString path, int person_activity)
{
    unsigned int sensor, data_pos, max_size = UINT_MAX;
    QStringList temp_string;
    QList<QStringList> sensor_data;
    _life_sim_csv_dataset parsed_data;


    /* Check the sizes */
    if (csv_filenames.size() != LIFE_SIM_CSV_DATASET_SIZE)
        return;

    /* If not selected, pick the first one */
    if (person_activity < 0)
        person_activity = 0;

    /*  Get data from CSV */
    sensor_data.begin();
    for(sensor = 0; sensor < csv_filenames.size(); sensor++)
    {
        temp_string = this->get_sensor_info(path, sensor, person_activity).split(';');
        if (temp_string.size() < max_size)
            max_size = temp_string.size();

        sensor_data.append(temp_string);
    }

    /* Compose parsed list */
    parser_list.begin();
    for (data_pos = 0; data_pos < max_size; data_pos++)
    {
        for (sensor = 0; sensor < LIFE_SIM_CSV_DATASET_SIZE; sensor++)
        {
            parsed_data.raw[sensor] = sensor_data[sensor][data_pos].toDouble();
        }
        parser_list.append(parsed_data);
    }
}

QString _dataset_parser::get_sensor_info(QString path, unsigned int sensor, unsigned int line)
{
    QString temp_string = "";

    QFile file(path + this->csv_filenames[sensor] + ".csv");
    if (!file.open(QIODevice::ReadOnly))
    {
        this->logger->write("Cannot open " + this->csv_filenames[sensor] + " file!");
        goto get_sensor_info_end;
    }

    while (line--)
    {
        file.readLine();
        if(file.atEnd())
            goto get_sensor_info_end;

    }
    temp_string = file.readLine();

get_sensor_info_end:
    file.close();
    return temp_string;
}

bool _dataset_parser::get_item(unsigned int pos, _life_sim_csv_dataset* values)
{
    if (pos < parser_list.count())
    {
        *values = parser_list[pos];
        return true;
    }
    return false;
}

size_t _dataset_parser::get_size(void)
{
    return parser_list.count();
}

