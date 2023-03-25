HEADERS += $$PWD/appconfig.h \
    $$PWD/appdata.h \
    $$PWD/quihelper.h \
    $$PWD/quihelperdata.h \
    $$PWD/tcpserverclient.h

HEADERS += $$PWD/tcpserver.h
HEADERS += $$PWD/tcpclient.h

SOURCES += $$PWD/appconfig.cpp \
    $$PWD/appdata.cpp \
    $$PWD/quihelper.cpp \
    $$PWD/quihelperdata.cpp \
    $$PWD/tcpserverclient.cpp

SOURCES += $$PWD/tcpserver.cpp
SOURCES += $$PWD/tcpclient.cpp

contains(DEFINES, websocket) {
HEADERS += $$PWD/webclient.h
HEADERS += $$PWD/webserver.h

SOURCES += $$PWD/webclient.cpp
SOURCES += $$PWD/webserver.cpp
}
