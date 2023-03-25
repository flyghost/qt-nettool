#include "tcpclient.h"
#include "quihelper.h"
#include "quihelperdata.h"

TcpClient::TcpClient(QTcpSocket *socket, QObject *parent) : QObject(parent)
{
    this->socket = socket;
}
