#ifndef PROTOCOL_H
#define PROTOCOL_H

//comandos simples Gateway para LIFES_SIM
#define GATEWAY_INFORMA_LINK_ON 			254
#define GATEWAY_INFORMA_LINK_OFF 			253

//comandos compostos Gateway para LIFES_SIM
#define GATEWAY_INFORMA_IP                  219
#define GAEWAY_SERVER_CLOCK                 218

//comandos
typedef enum {
    CMD_TYPE_SYNC_CLOCK = 1,

    CMD_TYPE_SENSOR_DATA_ACC,
    CMD_TYPE_SENSOR_DATA_BVP,
    CMD_TYPE_SENSOR_DATA_EDA,
    CMD_TYPE_SENSOR_DATA_HR,
    CMD_TYPE_SENSOR_DATA_TEMP,

    CMD_TYPE_PARAM_ACC,
    CMD_TYPE_PARAM_BVP,
    CMD_TYPE_PARAM_EDA,
    CMD_TYPE_PARAM_HR,
    CMD_TYPE_PARAM_TEMP,

    CMD_TYPE_CONFIG_ACC,
    CMD_TYPE_CONFIG_BVP,
    CMD_TYPE_CONFIG_EDA,
    CMD_TYPE_CONFIG_HR,
    CMD_TYPE_CONFIG_TEMP,

    CMD_TYPE_CONFIG_PUB,

    CMD_TYPE_END

} _command_types;

///////////////////////////////////////////////////////////////////////////////

#define X_Y_Z   3

#pragma pack(push,2)
// acc data
typedef struct
{
    unsigned short pos;
    double data[X_Y_Z];
} _DataACC;
#pragma pack(pop)

#pragma pack(push,2)
//_bvp_data
typedef struct
{
    unsigned short pos;
    double data;
} _DataBVP;
#pragma pack(pop)

#pragma pack(push,2)
//_eda_data
typedef struct
{
    unsigned short pos;
    double data;
} _DataEDA;

#pragma pack(push,2)
//_hr_data
typedef struct
{
    unsigned short pos;
    double data;
} _DataHR;
#pragma pack(pop)

#pragma pack (2)
//_temp_data
typedef struct
{
    unsigned short pos;
    double data;
} _DataTemp;
#pragma pack(pop)

///////////////////////////////////////////////////////////////////////////////
//parameters

//_Parameters acc
typedef struct
{
    //reference values
    unsigned short example_value;
} _ParametersACC;

//_Parameters BVP
typedef struct
{
    //reference values
    unsigned short example_value;
} _ParametersBVP;

//_Parameters EDA
typedef struct
{
    //reference values
    unsigned short example_value;
} _ParametersEDA;

//_Parameters HR
typedef struct
{
    //reference values
    unsigned short example_value;
} _ParametersHR;

//_Parameters Temperature
typedef struct
{
    //reference values
    unsigned short example_value;
} _ParametersTemp;

///////////////////////////////////////////////////////////////////////////////
//configurations

typedef enum
{
    CONFIG_ENABLED_ACC      = 0b00000001, // x, y and z
    CONFIG_ENABLED_BVP      = 0b00000010,
    CONFIG_ENABLED_EDA      = 0b00000100,
    CONFIG_ENABLED_HR       = 0b00001000,
    CONFIG_ENABLED_TEMP     = 0b00010000,
} _ConfigEnabledSensors;

//_Config acceloremeter
typedef struct
{
    unsigned char example_value;
} _ConfigACC;

//_Config BVP
typedef struct
{
    unsigned char example_value;
} _ConfigBVP;

//_Config EDA
typedef struct
{
    unsigned char example_value;
} _ConfigEDA;

//_Config HR
typedef struct
{
    unsigned char example_value;
} _ConfigHR;

//_Config Temperature
typedef struct
{
    unsigned char example_value;
} _ConfigTemp;

//_Configuracoes de publicacao (intervalo e dados)
typedef struct
{
    unsigned char bitarray_enabled_sensors;
    unsigned short sampleRate;
} _ConfigPub;

/* composição estruturas protocolo */

//_curvas
typedef struct
{
    _DataACC  acc;
    _DataBVP  bvp;
    _DataEDA  eda;
    _DataHR   hr;
    _DataTemp temp;
} _sensors_data;

//_parameters
typedef struct
{
    _ParametersACC  acc;
    _ParametersBVP  bvp;
    _ParametersEDA  eda;
    _ParametersHR   hr;
    _ParametersTemp temp;
} _parameters;

//_configurations
typedef struct
{
    _ConfigACC  acc;
    _ConfigBVP  bvp;
    _ConfigEDA  eda;
    _ConfigHR   hr;
    _ConfigTemp temp;
    _ConfigPub  publisher;
} _configurations;


//_Relogio
typedef struct
{
    unsigned char Segundo;
    unsigned char Minuto;
    unsigned char Hora;
    unsigned char Dia;
    unsigned char Mes;
    unsigned char Ano;
} _Relogio;

//Time_Sync
typedef struct
{
    unsigned char comando;
    unsigned char tamanho;
    unsigned char Hora;
    unsigned char Minuto;
    unsigned char Segundo;
    unsigned char Dia;
    unsigned char Mes;
    unsigned char Ano[2];
    unsigned char crc;
} Time_Sync;


//_dados
typedef struct
{
    _Relogio relogio;
    _sensors_data sensor_data;
    _parameters param;
    _configurations config;
    Time_Sync rel;
} _cmddata;

//LIFE SIM protocol
typedef struct
{
    enum
    {
        DESL, INI, ACTIVE
    } st;
    _cmddata cmddata;
    unsigned short size[CMD_TYPE_END]; //último comando mais 1
    void *address[CMD_TYPE_END];
} _lifes_sim;

///////////////////////////////////////////////////////////////////////////////
//csv structure

#define LIFE_SIM_CSV_DATASET_SIZE   7


typedef union
{
    double raw[LIFE_SIM_CSV_DATASET_SIZE];
    struct // should match csv_filenames size
    {
        double acc[X_Y_Z]; // x, y and z
        double bvp;
        double eda;
        double hr;
        double temp;
    } data;
} _life_sim_csv_dataset;

#endif // PROTOCOL_H
