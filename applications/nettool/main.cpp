#include "frmmain.h"
#include "quihelper.h"

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    QApplication::setAttribute(Qt::AA_Use96Dpi);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5,14,0))
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);
#endif

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/main.ico"));

    //设置编码+字体+中文翻译文件
    QUIHelper::initAll();

    frmMain w;
    w.setWindowTitle("网络调试助手 V2022 (copyright by creekwater)");
    w.resize(950, 700);
    QUIHelper::setFormInCenter(&w);
    w.show();

    return a.exec();
}
