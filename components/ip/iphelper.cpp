#include "iphelper.h"

IpHelper::IpHelper()
{

}

QStringList IpHelper::loadIP()
{
    //获取本机所有IP
    static QStringList ips;
    if (ips.count() == 0) {
#ifdef Q_OS_WASM
        ips << "127.0.0.1";
#else
        QList<QNetworkInterface> netInterfaces = QNetworkInterface::allInterfaces();
        foreach (const QNetworkInterface  &netInterface, netInterfaces) {
            //移除虚拟机和抓包工具的虚拟网卡
            QString humanReadableName = netInterface.humanReadableName().toLower();
            if (humanReadableName.startsWith("vmware network adapter") || humanReadableName.startsWith("npcap loopback adapter")) {
                continue;
            }

            //过滤当前网络接口
            bool flag = (netInterface.flags() == (QNetworkInterface::IsUp | QNetworkInterface::IsRunning | QNetworkInterface::CanBroadcast | QNetworkInterface::CanMulticast));
            if (flag) {
                QList<QNetworkAddressEntry> addrs = netInterface.addressEntries();
                foreach (QNetworkAddressEntry addr, addrs) {
                    //只取出IPV4的地址
                    if (addr.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                        QString ip4 = addr.ip().toString();
                        if (ip4 != "127.0.0.1") {
                            ips << ip4;
                        }
                    }
                }
            }
        }
#endif
    }

    if (!ips.contains("127.0.0.1")) {
        ips << "127.0.0.1";
    }

    return ips;
}

void IpHelper::comboboxAddIP(QComboBox *cbox)
{
    QStringList ips = loadIP();

    if (!ips.contains("127.0.0.1")) {
        ips << "127.0.0.1";
    }

    cbox->clear();
    cbox->addItems(ips);
}
