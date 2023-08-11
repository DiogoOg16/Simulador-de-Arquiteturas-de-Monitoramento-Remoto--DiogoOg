#include "Socket_TCP.h"


void _SocketTCP::Connect(QString ip, unsigned int port)
{
    if (ip.isEmpty() || port == 0)
        //modificado
        //_socket.connectToHost(QHostAddress("10.32.162.125"), 23);
        _socket.connectToHost(QHostAddress("127.0.0.1"), 23);
    else
        _socket.connectToHost(QHostAddress(ip), port);
}

void _SocketTCP::SendTCP(unsigned char dado)
{
    QByteArray buf;
    buf.append(dado);

    _socket.write(buf);
}

