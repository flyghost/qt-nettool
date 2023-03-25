#include "tcpclient.h"
#include "quihelper.h"
#include "quihelperdata.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    this->socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(slot_connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slot_readyRead()));
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(slot_errorOccurred()));
#else
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slot_errorOccurred()));
#endif
}

void TcpClient::connectToHost(QString TcpServerIP, int TcpServerPort)
{
    //断开所有连接和操作
    //socket->abort();
    //绑定网卡和端口
    //有个后遗症,客户端这边断开连接后还会保持几分钟导致不能重复绑定
    //如果是服务器断开则可以继续使用
    //提示 The bound address is already in use
    //参考 https://www.cnblogs.com/baiduboy/p/7426822.html
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    //socket->bind(QHostAddress(AppConfig::TcpBindIP), AppConfig::TcpBindPort);
#endif
    //连接服务器
    this->socket->connectToHost(TcpServerIP, TcpServerPort);
}

void TcpClient::abort()
{
    this->socket->abort();
}

QString TcpClient::errorString() const
{
    return this->socket->errorString();
}

qint64 TcpClient::writeHexString(const QString &data)
{
    QByteArray buffer = QUIHelperData::hexStrToByteArray(data);
    return this->socket->write(buffer);
}

qint64 TcpClient::writeAsciiString(const QString &data)
{
    QByteArray buffer = QUIHelperData::asciiStrToByteArray(data);
    return this->socket->write(buffer);
}

qint64 TcpClient::writeByteString(const QString &data)
{
    QByteArray buffer = data.toUtf8();
    return this->socket->write(buffer);
}

qsizetype TcpClient::readHexString(QString *string)
{
    QByteArray data = socket->readAll();
    qsizetype size  = data.length();
    if (size <= 0) {
        return size;
    }

    *string = QUIHelperData::byteArrayToHexStr(data);
    return size;
}

qsizetype TcpClient::readAsciiString(QString *string)
{
    QByteArray data = socket->readAll();
    qsizetype size  = data.length();
    if (size <= 0) {
        return size;
    }

    *string = QUIHelperData::byteArrayToAsciiStr(data);
    return size;
}

qsizetype TcpClient::readByteString(QString *string)
{
    QByteArray data = socket->readAll();
    qsizetype size  = data.length();
    if (size <= 0) {
        return size;
    }

    *string = QString(data);
    return size;
}

QString TcpClient::localAddressString()
{
    return this->socket->localAddress().toString();
}

quint16 TcpClient::localPortInt()
{
    return this->socket->localPort();
}

QString TcpClient::remoteAddressString()
{
    return this->socket->peerAddress().toString();
}

quint16 TcpClient::remotePortInt()
{
    return this->socket->peerPort();
}

void TcpClient::slot_connected()
{
    emit connected();
}

void TcpClient::slot_disconnected()
{
    emit disconnected();
}

void TcpClient::slot_readyRead()
{
    emit readyRead();
}

void TcpClient::slot_errorOccurred()
{
    emit errorOccurred(this->socket->errorString());
}













