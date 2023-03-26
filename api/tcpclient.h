#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QtNetwork>

//为了方便并发和单独处理数据所以单独写个类专门处理
#if 1
//小数据量可以不用线程,收发数据本身默认异步的
class TcpClient : public QObject
#else
//线程方便处理密集运算比如解析图片
class TcpClient : public QThread
#endif
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    void connectToHost(QString TcpServerIP, int TcpServerPort);
    void abort();
    QString errorString() const;
    qint64 sendData(const QString &data);
    qint64 writeHexString(const QString &data);
    qint64 writeAsciiString(const QString &data);
    qint64 writeByteString(const QString &data);
    qsizetype readData(QString *string);
    qsizetype readHexString(QString *string);
    qsizetype readAsciiString(QString *string);
    qsizetype readByteString(QString *string);
    QString localAddressString();
    quint16 localPortInt();
    QString remoteAddressString();
    quint16 remotePortInt();

private:
    bool isConnected;
    QTcpSocket *socket;
    QTimer *timer;
    QSettings *set;
    QString autoSendString;

    //TCP客户端配置参数
    bool HexSendTcpClient;       //16进制发送
    bool HexReceiveTcpClient;    //16进制接收
    bool AsciiTcpClient;         //ASCII模式
    bool DebugTcpClient;         //启用数据调试
    bool AutoSendTcpClient;      //自动发送数据
    int IntervalTcpClient;       //发送数据间隔
    QString TcpBindIP;           //绑定地址
    int TcpBindPort;             //绑定端口
    QString TcpServerIP;         //服务器地址
    int TcpServerPort;           //服务器端口

public:
    void setAutoSendString(QString &str);
    void startAutoSendTimer();
    void stopAutoSendTimer();

    void initConfig();
    void readConfig();

    bool readConfigHexSendTcpClient();
    bool readConfigHexReceiveTcpClient();
    bool readConfigAsciiTcpClient();
    bool readConfigDebugTcpClient();
    bool readConfigAutoSendTcpClient();
    int readConfigIntervalTcpClient();
    QString readConfigTcpBindIP();
    int readConfigTcpBindPort();
    QString readConfigTcpServerIP();
    int readConfigTcpServerPort();

    void writeConfigHexSendTcpClient(bool HexSendTcpClient);
    void writeConfigHexReceiveTcpClient(bool HexReceiveTcpClient);
    void writeConfigAsciiTcpClient(bool AsciiTcpClient);
    void writeConfigDebugTcpClient(bool DebugTcpClient);
    void writeConfigAutoSendTcpClient(bool AutoSendTcpClient);
    void writeConfigIntervalTcpClient(int IntervalTcpClient);
    void writeConfigTcpBindIP(QString TcpBindIP);
    void writeConfigTcpBindPort(int TcpBindPort);
    void writeConfigTcpServerIP(QString TcpServerIP);
    void writeConfigTcpServerPort(int TcpServerPort);


signals:
    void autoSendOk(QString &str);
    void connected();
    void disconnected();
    void readyRead();
    void errorOccurred(QString errorString);

private slots:
    void slot_timer();
    void slot_connected();
    void slot_disconnected();
    void slot_readyRead();
    void slot_errorOccurred();
    
};

#endif // TCPCLIENT_H
