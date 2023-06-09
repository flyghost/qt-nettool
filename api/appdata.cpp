﻿#include "appdata.h"
#include "quihelper.h"

QStringList AppData::Intervals = QStringList();
QStringList AppData::Datas = QStringList();

// txt文档的数据格式
// 左边是接收到的key，右边是发送的value
// 第一行：16 FF 01 01 E0 E1;11 22 33 44 55 66
// 第二行：16 FF 01 01 E0 E2;aa bb cc dd ee ff
QStringList AppData::Keys = QStringList();      // 会和接收到的数据进行比对，如果相同，则会发送下面的值
QStringList AppData::Values = QStringList();    // 如果key和接收到的数据相同，则会发送这个数据

QString AppData::SendFileName = "send.txt";
void AppData::readSendData()
{
    //读取发送数据列表
    AppData::Datas.clear();
    QString fileName = QString("%1/%2").arg(QUIHelper::appPath()).arg(AppData::SendFileName);
    QFile file(fileName);
    if (file.size() > 0 && file.open(QFile::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QString line = file.readLine();
            line = line.trimmed();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            if (!line.isEmpty()) {
                AppData::Datas.append(line);
            }
        }

        file.close();
    }

    //没有的时候主动添加点免得太空
    if (AppData::Datas.count() == 0) {
        AppData::Datas << "16 FF 01 01 E0 E1" << "16 FF 01 01 E1 E2";
    }
}

QString AppData::DeviceFileName = "device.txt";
void AppData::readDeviceData()
{
    //读取转发数据列表
    AppData::Keys.clear();
    AppData::Values.clear();
    QString fileName = QString("%1/%2").arg(QUIHelper::appPath()).arg(AppData::DeviceFileName);
    QFile file(fileName);
    if (file.size() > 0 && file.open(QFile::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QString line = file.readLine();
            line = line.trimmed();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            if (!line.isEmpty()) {
                QStringList list = line.split(";");
                QString key = list.at(0);
                QString value;
                for (int i = 1; i < list.count(); i++) {
                    value += QString("%1;").arg(list.at(i));
                }

                //去掉末尾分号
                value = value.mid(0, value.length() - 1);
                AppData::Keys.append(key);
                AppData::Values.append(value);
            }
        }

        file.close();
    }
}

void AppData::saveData(const QString &data)
{
    if (data.length() <= 0) {
        return;
    }

    QString fileName = QString("%1/%2.txt").arg(QUIHelper::appPath()).arg(STRDATETIME);
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(data.toUtf8());
        file.close();
    }
}


