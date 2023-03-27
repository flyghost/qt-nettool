#include "frmtcpclient.h"
#include "ui_frmtcpclient.h"
#include "quihelper.h"
#include "quihelperdata.h"
#include "iphelper.h"

frmTcpClient::frmTcpClient(QWidget *parent) : QWidget(parent), ui(new Ui::frmTcpClient)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();
}

frmTcpClient::~frmTcpClient()
{
    delete ui;
}

bool frmTcpClient::eventFilter(QObject *watched, QEvent *event)
{
    //双击清空
    if (watched == ui->txtMain->viewport()) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            on_btnClear_clicked();
        }
    }

    return QWidget::eventFilter(watched, event);
}

void frmTcpClient::initForm()
{
    QFont font;
    font.setPixelSize(16);
    ui->txtMain->setFont(font);
    ui->txtMain->viewport()->installEventFilter(this);

    isOk = false;

    //实例化对象并绑定信号槽
    client = new TcpClient(this);
    connect(client, SIGNAL(connected()), this, SLOT(connected()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(client, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(client, SIGNAL(errorOccurred(QString)), this, SLOT(error(QString)));
    connect(client, SIGNAL(autoSendOk(QString&)), this, SLOT(autoSend(QString&)));

    //填充数据到下拉框
    ui->cboxInterval->addItems(AppData::Intervals);
    ui->cboxData->addItems(AppData::Datas);

    QStringList ips = IpHelper::loadIP();
    ui->cboxBindIP->clear();
    ui->cboxBindIP->addItems(ips);
}

void frmTcpClient::initConfig()
{
    ui->ckHexSend->setChecked(client->readConfigHexSendTcpClient());
    connect(ui->ckHexSend, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckHexReceive->setChecked(client->readConfigHexReceiveTcpClient());
    connect(ui->ckHexReceive, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckAscii->setChecked(client->readConfigAsciiTcpClient());
    connect(ui->ckAscii, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckDebug->setChecked(client->readConfigDebugTcpClient());
    connect(ui->ckDebug, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckAutoSend->setChecked(client->readConfigAutoSendTcpClient());
    connect(ui->ckAutoSend, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->cboxInterval->setCurrentIndex(ui->cboxInterval->findText(QString::number(client->readConfigIntervalTcpClient())));
    connect(ui->cboxInterval, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

    ui->cboxBindIP->setCurrentIndex(ui->cboxBindIP->findText(client->readConfigTcpBindIP()));
    connect(ui->cboxBindIP, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

    ui->txtBindPort->setText(QString::number(client->readConfigTcpBindPort()));
    connect(ui->txtBindPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtServerIP->setText(client->readConfigTcpServerIP());
    connect(ui->txtServerIP, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtServerPort->setText(QString::number(client->readConfigTcpServerPort()));
    connect(ui->txtServerPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
}

void frmTcpClient::saveConfig()
{    
    client->writeConfigHexSendTcpClient(ui->ckHexSend->isChecked());
    client->writeConfigHexReceiveTcpClient(ui->ckHexReceive->isChecked());
    client->writeConfigAsciiTcpClient(ui->ckAscii->isChecked());
    client->writeConfigDebugTcpClient(ui->ckDebug->isChecked());
    client->writeConfigTcpBindIP(ui->cboxBindIP->currentText());
    client->writeConfigTcpBindPort(ui->txtBindPort->text().trimmed().toInt());
    client->writeConfigTcpServerIP(ui->txtServerIP->text().trimmed());
    client->writeConfigTcpServerPort(ui->txtServerPort->text().trimmed().toInt());

    client->writeConfigAutoSendTcpClient(ui->ckAutoSend->isChecked());
    client->writeConfigIntervalTcpClient(ui->cboxInterval->currentText().toInt());

    if(ui->ckAutoSend->isChecked()){
        QString data = ui->cboxData->currentText();
        client->setAutoSendString(data);
        client->startAutoSendTimer();
    }else{
        client->stopAutoSendTimer();
    }

    QString data = ui->cboxData->currentText();
    client->setAutoSendString(data);
}

void frmTcpClient::append(int type, const QString &data, bool clear)
{
    static int currentCount = 0;
    static int maxCount = 100;

    if (clear) {
        ui->txtMain->clear();
        currentCount = 0;
        return;
    }

    if (currentCount >= maxCount) {
        ui->txtMain->clear();
        currentCount = 0;
    }

    if (ui->ckShow->isChecked()) {
        return;
    }

    //过滤回车换行符
    QString strData = data;
    strData = strData.replace("\r", "");
    strData = strData.replace("\n", "");

    //不同类型不同颜色显示
    QString strType;
    if (type == 0) {
        strType = "发送";
        ui->txtMain->setTextColor(QColor("#22A3A9"));
    } else if (type == 1) {
        strType = "接收";
        ui->txtMain->setTextColor(QColor("#753775"));
    } else {
        strType = "错误";
        ui->txtMain->setTextColor(QColor("#D64D54"));
    }

    strData = QString("时间[%1] %2: %3").arg(TIMEMS).arg(strType).arg(strData);
    ui->txtMain->append(strData);
    currentCount++;
}

void frmTcpClient::connected()
{
    isOk = true;
    ui->btnConnect->setText("断开");
    append(0, "服务器连接");
    append(0, QString("本地地址: %1  本地端口: %2").arg(client->localAddressString()).arg(client->localPortInt()));
    append(0, QString("远程地址: %1  远程端口: %2").arg(client->remoteAddressString()).arg(client->remotePortInt()));
}

void frmTcpClient::disconnected()
{
    isOk = false;
    ui->btnConnect->setText("连接");
    append(1, "服务器断开");
}

void frmTcpClient::error(QString errorString)
{
    append(2, errorString);
}

void frmTcpClient::readData()
{
    QString buffer;
    if(client->readData(&buffer) <= 0){
        return;
    }

    append(1, buffer);

    //自动回复数据,可以回复的数据是以;隔开,每行可以带多个;所以这里不需要继续判断
    if (client->readConfigDebugTcpClient()) {
        int count = AppData::Keys.count();
        for (int i = 0; i < count; i++) {
            if (AppData::Keys.at(i) == buffer) {
                qDebug() << "recv key : " << AppData::Keys.at(i) << ", send value : " << AppData::Values.at(i);
                sendData(AppData::Values.at(i));
                break;
            }
        }
    }
}

void frmTcpClient::sendData(const QString &data)
{
    client->sendData(data);

    append(0, data);
}

void frmTcpClient::on_btnConnect_clicked()
{
    if (ui->btnConnect->text() == "连接") {
        client->connectToHost(client->readConfigTcpServerIP(), client->readConfigTcpServerPort());
    } else {
        client->abort();
    }
}

void frmTcpClient::on_btnSave_clicked()
{
    QString data = ui->txtMain->toPlainText();
    AppData::saveData(data);
    on_btnClear_clicked();
}

void frmTcpClient::on_btnClear_clicked()
{
    append(0, "", true);
}

void frmTcpClient::on_btnSend_clicked()
{
    if (!isOk) {
        return;
    }

    QString data = ui->cboxData->currentText();
    if (data.length() <= 0) {
        return;
    }

    sendData(data);
}

void frmTcpClient::autoSend(QString &data)
{
    if (!isOk) {
        return;
    }

    if (data.length() <= 0) {
        return;
    }

    append(0, data);
}
