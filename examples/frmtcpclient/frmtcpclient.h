﻿#ifndef FRMTCPCLIENT_H
#define FRMTCPCLIENT_H

#include <QWidget>
#include <QtNetwork>
#include "tcpclient.h"

namespace Ui {
class frmTcpClient;
}

class frmTcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit frmTcpClient(QWidget *parent = 0);
    ~frmTcpClient();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::frmTcpClient *ui;

    bool isOk;              // 是否已经连接
    TcpClient *client;
    QTimer *timer;

private slots:
    void initForm();
    void initConfig();
    void saveConfig();
    void append(int type, const QString &data, bool clear = false);

private slots:
    void connected();
    void disconnected();
    void error(QString errorString);
    void readData();
    void sendData(const QString &data);
    void autoSend(QString &data);

private slots:
    void on_btnConnect_clicked();
    void on_btnSave_clicked();
    void on_btnClear_clicked();
    void on_btnSend_clicked();
};

#endif // FRMTCPCLIENT_H
