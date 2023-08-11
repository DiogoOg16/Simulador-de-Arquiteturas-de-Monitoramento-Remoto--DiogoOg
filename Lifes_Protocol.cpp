#include "Lifes_Protocol.h"


_lifes_sim lifes_sim;
_lifes_sim lifes_sim_ant;

unsigned char bufferIn[255];
unsigned char bufferOUT[255];
unsigned short crcOUT;
unsigned short iOUT;

//CRC Look-up table
const unsigned char crcTable[256] = {
    0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15,
    0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d,
    0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65,
    0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d,
    0xe0, 0xe7, 0xee, 0xe9, 0xfc, 0xfb, 0xf2, 0xf5,
    0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
    0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85,
    0xa8, 0xaf, 0xa6, 0xa1, 0xb4, 0xb3, 0xba, 0xbd,
    0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2,
    0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea,
    0xb7, 0xb0, 0xb9, 0xbe, 0xab, 0xac, 0xa5, 0xa2,
    0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
    0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32,
    0x1f, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0d, 0x0a,
    0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42,
    0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a,
    0x89, 0x8e, 0x87, 0x80, 0x95, 0x92, 0x9b, 0x9c,
    0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
    0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec,
    0xc1, 0xc6, 0xcf, 0xc8, 0xdd, 0xda, 0xd3, 0xd4,
    0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c,
    0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44,
    0x19, 0x1e, 0x17, 0x10, 0x05, 0x02, 0x0b, 0x0c,
    0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
    0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b,
    0x76, 0x71, 0x78, 0x7f, 0x6a, 0x6d, 0x64, 0x63,
    0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b,
    0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13,
    0xae, 0xa9, 0xa0, 0xa7, 0xb2, 0xb5, 0xbc, 0xbb,
    0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
    0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb,
    0xe6, 0xe1, 0xe8, 0xef, 0xfa, 0xfd, 0xf4, 0xf3 };

void _Lifes_Protocol::Init_Lifes_SIM(Logger* logger_pointer)
{
    logger = logger_pointer;

    //tamanhos estruturas
    lifes_sim.size[CMD_TYPE_SYNC_CLOCK]         = (int) sizeof(_Relogio);
    lifes_sim.size[CMD_TYPE_SENSOR_DATA_ACC]    = (int) sizeof(_DataACC);
    lifes_sim.size[CMD_TYPE_SENSOR_DATA_BVP]    = (int) sizeof(_DataBVP);
    lifes_sim.size[CMD_TYPE_SENSOR_DATA_EDA]    = (int) sizeof(_DataEDA);
    lifes_sim.size[CMD_TYPE_SENSOR_DATA_HR]     = (int) sizeof(_DataHR);
    lifes_sim.size[CMD_TYPE_SENSOR_DATA_TEMP]   = (int) sizeof(_DataTemp);
    lifes_sim.size[CMD_TYPE_PARAM_ACC]          = (int) sizeof(_ParametersACC);
    lifes_sim.size[CMD_TYPE_PARAM_BVP]          = (int) sizeof(_ParametersBVP);
    lifes_sim.size[CMD_TYPE_PARAM_EDA]          = (int) sizeof(_ParametersEDA);
    lifes_sim.size[CMD_TYPE_PARAM_HR]           = (int) sizeof(_ParametersHR);
    lifes_sim.size[CMD_TYPE_PARAM_TEMP]         = (int) sizeof(_ParametersTemp);
    lifes_sim.size[CMD_TYPE_CONFIG_ACC]         = (int) sizeof(_ConfigACC);
    lifes_sim.size[CMD_TYPE_CONFIG_BVP]         = (int) sizeof(_ConfigBVP);
    lifes_sim.size[CMD_TYPE_CONFIG_EDA]         = (int) sizeof(_ConfigEDA);
    lifes_sim.size[CMD_TYPE_CONFIG_HR]          = (int) sizeof(_ConfigHR);
    lifes_sim.size[CMD_TYPE_CONFIG_TEMP]        = (int) sizeof(_ConfigTemp);
    lifes_sim.size[CMD_TYPE_CONFIG_PUB]         = (int) sizeof(_ConfigPub);

    //ponteiros para estruturas
    lifes_sim.address[CMD_TYPE_SYNC_CLOCK]      = &lifes_sim.cmddata.relogio;
    lifes_sim.address[CMD_TYPE_SENSOR_DATA_ACC] = &lifes_sim.cmddata.sensor_data.acc;
    lifes_sim.address[CMD_TYPE_SENSOR_DATA_BVP] = &lifes_sim.cmddata.sensor_data.bvp;
    lifes_sim.address[CMD_TYPE_SENSOR_DATA_EDA] = &lifes_sim.cmddata.sensor_data.eda;
    lifes_sim.address[CMD_TYPE_SENSOR_DATA_HR]  = &lifes_sim.cmddata.sensor_data.hr;
    lifes_sim.address[CMD_TYPE_SENSOR_DATA_TEMP]= &lifes_sim.cmddata.sensor_data.temp;
    lifes_sim.address[CMD_TYPE_PARAM_ACC]       = &lifes_sim.cmddata.param.acc;
    lifes_sim.address[CMD_TYPE_PARAM_BVP]       = &lifes_sim.cmddata.param.bvp;
    lifes_sim.address[CMD_TYPE_PARAM_EDA]       = &lifes_sim.cmddata.param.eda;
    lifes_sim.address[CMD_TYPE_PARAM_HR]        = &lifes_sim.cmddata.param.hr;
    lifes_sim.address[CMD_TYPE_PARAM_TEMP]      = &lifes_sim.cmddata.param.temp;
    lifes_sim.address[CMD_TYPE_CONFIG_ACC]      = &lifes_sim.cmddata.config.acc;
    lifes_sim.address[CMD_TYPE_CONFIG_BVP]      = &lifes_sim.cmddata.config.bvp;
    lifes_sim.address[CMD_TYPE_CONFIG_EDA]      = &lifes_sim.cmddata.config.eda;
    lifes_sim.address[CMD_TYPE_CONFIG_HR]       = &lifes_sim.cmddata.config.hr;
    lifes_sim.address[CMD_TYPE_CONFIG_TEMP]     = &lifes_sim.cmddata.config.temp;
    lifes_sim.address[CMD_TYPE_CONFIG_PUB]      = &lifes_sim.cmddata.config.publisher;

    lifes_sim_tools.init_FIFO_out();

    // conecta evento de recebimento com funcao de tratamento dos dados
    connect(&lifes_sim_tools.socketTCP._socket, SIGNAL(readyRead()), this, SLOT(lifes_sim_in()));

    lifes_sim.st = lifes_sim.ACTIVE;  //Stream constante de dados
}

unsigned char _Lifes_Protocol::TestaCRC(unsigned char crc, unsigned char data)
{
    return (crcTable[crc ^ data]);
}

/*	Mandar estrutura 'comando' para fifo de saida*/
unsigned char _Lifes_Protocol::lifes_SIM_comando(_command_types comando) //comando teste CMD_TYPE_SENSOR_DATA_ACC
{
    static unsigned char block_function = 0;

    if (block_function) return 0;
    else block_function = 1;

    //insere na fila COMANDO
    //n entra nos calculos de crc0/1
    lifes_sim_tools.fifo_out_push(comando);		// Manda comando pra fila
    crcOUT = 0;
    crcOUT = TestaCRC(crcOUT, comando);	// inicia CRC

    //calcula tamanho do pacote de dados (=tamanho estrutura mais crc)
    lifes_sim_tools.fifo_out_push(lifes_sim.size[comando]);	// Manda tamanho+crc
    crcOUT = TestaCRC(crcOUT, lifes_sim.size[comando]); // Segue no CRC

    //envia para fifo estrutura de dados
    memcpy(bufferOUT, (void *) lifes_sim.address[comando], lifes_sim.size[comando]);

    for (iOUT = 0; iOUT < lifes_sim.size[comando]; iOUT++)
    {
        lifes_sim_tools.fifo_out_push(bufferOUT[iOUT]);
        crcOUT = TestaCRC(crcOUT, bufferOUT[iOUT]);
    }

    //coloca crc na fifo
    lifes_sim_tools.fifo_out_push(crcOUT);

    block_function = 0;

    return 1;
}



_ConfigPub _Lifes_Protocol::Configuracoes_de_Publicacao(void)
{
    return lifes_sim.cmddata.config.publisher;
}

void _Lifes_Protocol::Configuracoes_pela_Interface(_ConfigPub config)
{
    lifes_sim.cmddata.config.publisher = config;
}

unsigned char _Lifes_Protocol::Atualiza_Curvas(_command_types cmd, double* sensor_data, unsigned int pos)
{
    switch(cmd)
    {
        case CMD_TYPE_SENSOR_DATA_ACC:
            memcpy(lifes_sim.cmddata.sensor_data.acc.data, sensor_data, sizeof(lifes_sim.cmddata.sensor_data.acc.data));
            lifes_sim.cmddata.sensor_data.acc.pos = pos;
        break;

        case CMD_TYPE_SENSOR_DATA_BVP:
            lifes_sim.cmddata.sensor_data.bvp.data = *sensor_data;
            lifes_sim.cmddata.sensor_data.bvp.pos = pos;
        break;

        case CMD_TYPE_SENSOR_DATA_EDA:
            lifes_sim.cmddata.sensor_data.eda.data = *sensor_data;
            lifes_sim.cmddata.sensor_data.eda.pos = pos;
        break;

        case CMD_TYPE_SENSOR_DATA_HR:
            lifes_sim.cmddata.sensor_data.hr.data = *sensor_data;
            lifes_sim.cmddata.sensor_data.hr.pos = pos;
        break;

        case CMD_TYPE_SENSOR_DATA_TEMP:
            lifes_sim.cmddata.sensor_data.temp.data = *sensor_data;
            lifes_sim.cmddata.sensor_data.temp.pos = pos;
        break;

        default:
            return 1;
    }
    return 0;
}

/* - Verifica CRC dos dados recebidos pela central
 * - Aplica Comando recebido pela central
 */
void _Lifes_Protocol::lifes_sim_in(void)
{
    unsigned int size = lifes_sim_tools.socketTCP._socket.read((char*) bufferIn, sizeof(bufferIn));
    //recepção de dados
    if (--size > 0) // remove ultimo byte do crc
    {
        unsigned char crc = 0;
        // Verifica CRC
        for (unsigned short pos=0; pos < size; pos++)
        {
            crc = TestaCRC(crc, bufferIn[pos]);
        }
        // CRC valido (CRC-8/MAXIM)
        if (crc == bufferIn[size] && size >= 3)
        {
            _command_types command = (_command_types) bufferIn[0];
            size = bufferIn[1];

            switch (command)
            {
                case CMD_TYPE_CONFIG_ACC:
                    if (size == sizeof(_ConfigACC))
                    {
                        memcpy((unsigned char*) &lifes_sim.cmddata.config.acc, &bufferIn[2], size);
                        logger->write("acc config received");
                    }
                break;

                case CMD_TYPE_CONFIG_BVP:
                    if (size == sizeof(_ConfigBVP))
                    {
                        memcpy((unsigned char*) &lifes_sim.cmddata.config.bvp, &bufferIn[2], size);
                        logger->write("bvp config received");
                    }
                break;

                case CMD_TYPE_CONFIG_EDA:
                    if (size == sizeof(_ConfigEDA))
                    {
                        memcpy((unsigned char*) &lifes_sim.cmddata.config.eda, &bufferIn[2], size);
                        logger->write("eda config received");
                    }
                break;

                case CMD_TYPE_CONFIG_HR:
                    if (size == sizeof(_ConfigHR))
                    {
                        memcpy((unsigned char*) &lifes_sim.cmddata.config.hr, &bufferIn[2], size);
                        logger->write("hr config received");
                    }
                break;

                case CMD_TYPE_CONFIG_TEMP:
                    if (size == sizeof(_ConfigTemp))
                    {
                        memcpy((unsigned char*) &lifes_sim.cmddata.config.temp, &bufferIn[2], size);
                        logger->write("temp config received");
                    }
                break;

                case CMD_TYPE_CONFIG_PUB:
                    if (size == sizeof(_ConfigPub))
                    {
                        memcpy((unsigned char*) &lifes_sim.cmddata.config.publisher, &bufferIn[2], size);
                        logger->write("Publisher config received");
                    }
                break;
            }
        }
    }
}
