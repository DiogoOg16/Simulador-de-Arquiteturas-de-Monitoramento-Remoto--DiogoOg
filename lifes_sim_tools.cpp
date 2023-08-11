#include "lifes_sim_tools.h"

#include "mainwindow.h"

unsigned short _tt; // next storage position in queue[]
unsigned short _1st; // retrieve position in queue[]
unsigned short queue[FIFO_out_size]; // this array forms the queue

// inicia contadores da fila de saida
void _lifes_sim_tools::init_FIFO_out()
{
    _tt=0; //elements queued[]
    _1st=0; // oldest element in queue[]
}

//inclui elemento na fila de saida e inicia envio
unsigned short _lifes_sim_tools::fifo_out_push(unsigned short bla) //comando CMD_TYPE_SENSOR_DATA_ACC
{
    unsigned short usdado;
    unsigned char ucdado;

    if (_tt == FIFO_out_size) // fifo lotada? 1024
    {
        return 0;
    }
    else
    {
        queue[(_tt+_1st)%FIFO_out_size] = bla;   // coloca elemento na fila
        _tt++; // increaze elements queued[]
        if (fifo_out_pop(&usdado))			// usdado = proximo a ser enviado
        {
            ucdado = usdado&0xFF;

            socketTCP.SendTCP(ucdado);
        }
        return 1;
    }
}

//retira elemento da fila de saida
unsigned short _lifes_sim_tools::fifo_out_pop(unsigned short *value)
{
    if (_tt == 0)
    {
        return 0;
    }
    else
    {
        unsigned short ret;
        ret = queue[_1st];
        _1st++; // oldest queued
        _tt--;
        _1st%=FIFO_out_size; //% é o cara pra buffer cíclico
        *value = ret;
        return 1;
    }
}


//espaço disponível na fila de saida
unsigned short _lifes_sim_tools::fifo_out_free(void)
{
    return FIFO_out_size - _tt;
}

//espaço utilizado na fila
unsigned short _lifes_sim_tools::fifo_out_queued(void)
{
    return _tt;
}
