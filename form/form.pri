FORMS += $$PWD/frmmain.ui

HEADERS += $$PWD/frmmain.h

SOURCES += $$PWD/frmmain.cpp

INCLUDEPATH += $$PWD

include ($$PWD/frmtcpclient/frmtcpclient.pri)
include ($$PWD/frmtcpserver/frmtcpserver.pri)
include ($$PWD/frmudpclient/frmudpclient.pri)
include ($$PWD/frmudpserver/frmudpserver.pri)
include ($$PWD/frmwebclient/frmwebclient.pri)
include ($$PWD/frmwebserver/frmwebserver.pri)
