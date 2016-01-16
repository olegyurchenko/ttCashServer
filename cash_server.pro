# This project demonstrates how to use QtWebAppLib by including the
# sources into this project.

TARGET = cash_server
TEMPLATE = app
QT = core network xml
CONFIG += console

HEADERS += \
           src/startup.h \
           src/requesthandler.h \
    src/cash_requesthandler.h \
    src/web_requesthandler.h \
    src/xmlrequest.h \
    src/xmlresponse.h \
    src/xmlserver.h

SOURCES += src/main.cpp \
           src/startup.cpp \
           src/requesthandler.cpp \
    src/cash_requesthandler.cpp \
    src/web_requesthandler.cpp \
    src/xmlrequest.cpp \
    src/xmlresponse.cpp \
    src/xmlserver.cpp


#---------------------------------------------------------------------------------------
# The following lines include the sources of the QtWebAppLib library
#---------------------------------------------------------------------------------------

include(QtWebApp/qtservice/qtservice.pri)
include(QtWebApp/httpserver/httpserver.pri)
include(QtWebApp/logging/logging.pri)
#include(QtWebApp/templateengine/templateengine.pri)


INCLUDEPATH += src
