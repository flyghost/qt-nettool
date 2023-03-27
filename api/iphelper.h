#ifndef IPHELPER_H
#define IPHELPER_H

#include "head.h"

class IpHelper
{
public:
    IpHelper();
    static QStringList loadIP();
    //添加网卡IP地址到下拉框
    static void comboboxAddIP(QComboBox *cbox);
};

#endif // IPHELPER_H
