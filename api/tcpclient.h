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
    explicit TcpClient(QTcpSocket *socket, QObject *parent = 0);

private:
    QTcpSocket *socket;
    
};

#endif // TCPCLIENT_H
