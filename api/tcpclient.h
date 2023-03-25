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
    QByteArray readAll();
    qint64 writeHexString(const QString &data);
    qint64 writeAsciiString(const QString &data);
    qint64 writeByteString(const QString &data);
    qsizetype readHexString(QString *string);
    qsizetype readAsciiString(QString *string);
    qsizetype readByteString(QString *string);
    QString localAddressString();
    quint16 localPortInt();
    QString remoteAddressString();
    quint16 remotePortInt();

private:
    QTcpSocket *socket;

signals:
    void connected();
    void disconnected();
    void readyRead();
    void errorOccurred(QString errorString);

private slots:
    void slot_connected();
    void slot_disconnected();
    void slot_readyRead();
    void slot_errorOccurred();
    
};

#endif // TCPCLIENT_H
