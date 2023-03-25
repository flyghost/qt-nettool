#pragma execution_character_set("utf-8")

#include "tcpclient.h"
#include "quihelper.h"
#include "quihelperdata.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    this->initConfig();
    this->readConfig();

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

qint64 TcpClient::sendData(const QString &data)
{
    if (this->readConfigHexSendTcpClient()) {
        return this->writeHexString(data);
    } else if (this->readConfigAsciiTcpClient()) {
        return this->writeAsciiString(data);
    } else {
        return this->writeByteString(data);
    }
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

qsizetype TcpClient::readData(QString *string)
{
    qsizetype size;
    if (this->readConfigHexReceiveTcpClient()) {
        size = this->readHexString(string);
    } else if (this->readConfigAsciiTcpClient()) {
        size = this->readAsciiString(string);
    } else {
        size = this->readByteString(string);
    }

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

void TcpClient::initConfig()
{
    HexSendTcpClient = false;
    HexReceiveTcpClient = false;
    AsciiTcpClient = false;
    DebugTcpClient = false;
    AutoSendTcpClient = false;
    IntervalTcpClient = 1000;
    TcpBindIP = "127.0.0.1";
    TcpBindPort = 6001;
    TcpServerIP = "127.0.0.1";
    TcpServerPort = 6000;
}

bool TcpClient::readConfigHexSendTcpClient()
{
    return this->HexSendTcpClient;
}
bool TcpClient::readConfigHexReceiveTcpClient()
{
    return this->HexReceiveTcpClient;
}
bool TcpClient::readConfigAsciiTcpClient()
{
    return this->AsciiTcpClient;
}
bool TcpClient::readConfigDebugTcpClient()
{
    return this->DebugTcpClient;
}
bool TcpClient::readConfigAutoSendTcpClient()
{
    return this->AutoSendTcpClient;
}
int TcpClient::readConfigIntervalTcpClient()
{
    return this->IntervalTcpClient;
}
QString TcpClient::readConfigTcpBindIP()
{
    return this->TcpBindIP;
}
int TcpClient::readConfigTcpBindPort()
{
    return this->TcpBindPort;
}
QString TcpClient::readConfigTcpServerIP()
{
    return this->TcpServerIP;
}
int TcpClient::readConfigTcpServerPort()
{
    return this->TcpServerPort;
}
void TcpClient::writeConfigHexSendTcpClient(bool HexSendTcpClient)
{
    this->HexSendTcpClient = HexSendTcpClient;
}
void TcpClient::writeConfigHexReceiveTcpClient(bool HexReceiveTcpClient)
{
    this->HexReceiveTcpClient = HexReceiveTcpClient;
}
void TcpClient::writeConfigAsciiTcpClient(bool AsciiTcpClient)
{
    this->AsciiTcpClient = AsciiTcpClient;
}
void TcpClient::writeConfigDebugTcpClient(bool DebugTcpClient)
{
    this->DebugTcpClient = DebugTcpClient;
}
void TcpClient::writeConfigAutoSendTcpClient(bool AutoSendTcpClient)
{
    this->AutoSendTcpClient = AutoSendTcpClient;
}
void TcpClient::writeConfigIntervalTcpClient(int IntervalTcpClient)
{
    this->IntervalTcpClient = IntervalTcpClient;
}
void TcpClient::writeConfigTcpBindIP(QString TcpBindIP)
{
    this->TcpBindIP = TcpBindIP;
}
void TcpClient::writeConfigTcpBindPort(int TcpBindPort)
{
    this->TcpBindPort = TcpBindPort;
}
void TcpClient::writeConfigTcpServerIP(QString TcpServerIP)
{
    this->TcpServerIP = TcpServerIP;
}
void TcpClient::writeConfigTcpServerPort(int TcpServerPort)
{
    this->TcpServerPort = TcpServerPort;
}

void TcpClient::writeConfig()
{
    QSettings set(AppConfig::ConfigFile, QSettings::IniFormat);

    set.beginGroup("TcpClientConfig");
    set.setValue("HexSendTcpClient", this->HexSendTcpClient);
    set.setValue("HexReceiveTcpClient", this->HexReceiveTcpClient);
    set.setValue("DebugTcpClient", this->DebugTcpClient);
    set.setValue("AutoSendTcpClient", this->AutoSendTcpClient);
    set.setValue("IntervalTcpClient", this->IntervalTcpClient);
    set.setValue("TcpBindIP", this->TcpBindIP);
    set.setValue("TcpBindPort", this->TcpBindPort);
    set.setValue("TcpServerIP", this->TcpServerIP);
    set.setValue("TcpServerPort", this->TcpServerPort);
    set.endGroup();
}

void TcpClient::readConfig()
{
    QSettings set(AppConfig::ConfigFile, QSettings::IniFormat);

    set.beginGroup("TcpClientConfig");
    this->HexSendTcpClient = set.value("HexSendTcpClient", this->HexSendTcpClient).toBool();
    this->HexReceiveTcpClient = set.value("HexReceiveTcpClient", this->HexReceiveTcpClient).toBool();
    this->AsciiTcpClient = set.value("AsciiTcpClient", this->AsciiTcpClient).toBool();
    this->DebugTcpClient = set.value("DebugTcpClient", this->DebugTcpClient).toBool();
    this->AutoSendTcpClient = set.value("AutoSendTcpClient", this->AutoSendTcpClient).toBool();
    this->IntervalTcpClient = set.value("IntervalTcpClient", this->IntervalTcpClient).toInt();
    this->TcpBindIP = set.value("TcpBindIP", this->TcpBindIP).toString();
    this->TcpBindPort = set.value("TcpBindPort", this->TcpBindPort).toInt();
    this->TcpServerIP = set.value("TcpServerIP", this->TcpServerIP).toString();
    this->TcpServerPort = set.value("TcpServerPort", this->TcpServerPort).toInt();
    set.endGroup();
}









