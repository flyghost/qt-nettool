TARGET      = nettool
TEMPLATE    = app
DESTDIR     = $$PWD/build/bin


QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4) {
QT += widgets
#判断是否有websocket模块
qtHaveModule(websockets) {
QT += websockets
DEFINES += websocket
}}

greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat




CONFIG      += warn_off

INCLUDEPATH += $$PWD

include ($$PWD/applications/nettool/nettool.pri)
include ($$PWD/qrc/qrc.pri)
include ($$PWD/api/api.pri)
include ($$PWD/examples/examples.pri)
include ($$PWD/components/components.pri)
