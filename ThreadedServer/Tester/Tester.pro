TARGET += Tester
TEMPLATE      = app
QT           += core gui sql network widgets
INCLUDEPATH += BO \
    DAO
HEADERS       = \
    Protocol.h \
    Dialog.h \
    Tester.h
SOURCES       = main.cpp \
    Protocol.cpp \
    Dialog.cpp \
    Tester.cpp
FORMS         = \
    Dialog.ui \
    Tester.ui
