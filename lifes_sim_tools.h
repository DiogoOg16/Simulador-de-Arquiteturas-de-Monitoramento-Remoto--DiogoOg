#ifndef LIFES_SIM_TOOLS_H
#define LIFES_SIM_TOOLS_H

#include "Socket_TCP.h"
#include <QObject>

#define FIFO_out_size 1024
#define FIFO_in_size  50

class _lifes_sim_tools : public QObject {

Q_OBJECT
public:
_SocketTCP socketTCP;

private:


signals:

public slots:
void init_FIFO_out();
unsigned short fifo_out_push(unsigned short bla);
unsigned short fifo_out_pop(unsigned short *value);
unsigned short fifo_out_free(void);
unsigned short fifo_out_queued(void);

};

#endif // LIFES_SIM_TOOLS_H
