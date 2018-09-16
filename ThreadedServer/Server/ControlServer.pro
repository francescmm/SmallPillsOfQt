TARGET += Server
TEMPLATE = app

CONFIG += console qt c++14

QT += network widgets

INCLUDEPATH += ../QLogger

HEADERS = \
    Server.h \
    Protocol.h \
    CommandLineApp.h \
    Client.h \
    version.h \
    ClientThread.h \
    ../QLogger/QLogger.h

SOURCES = \
    main.cpp \
    Server.cpp \
    Protocol.cpp \
    CommandLineApp.cpp \
    Client.cpp \
    ClientThread.cpp \
    ../QLogger/QLogger.cpp
