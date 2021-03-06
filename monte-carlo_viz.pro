QT       += core gui
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += \
    main.cpp \
    mainwindow_context.cpp \
    renderarea_context.cpp \
    playbutton.cpp

HEADERS += \
    mainwindow_context.h \
    renderarea_context.h \
    playbutton.h \
    drawing_role.h

unix:!macx:!symbian: LIBS += -L$$PWD/../monte_carlo_techs/lib/ -lmonte_carlo

INCLUDEPATH += $$PWD/../monte_carlo_techs/lib/
DEPENDPATH += $$PWD/../monte_carlo_techs/lib/
