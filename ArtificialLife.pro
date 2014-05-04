#---------------------------------------------------------------------------------------------------------------
#    ArtificialLife - A small self contained c++ library for alife
#    Chad McKinney, 2013-2014
#
#    Licensed under GNU GPL 3.0
#    See COPYING for licensing details
#---------------------------------------------------------------------------------------------------------------

linux-g++* {
    TEMPLATE = app
    QT += core gui opengl
    CONFIG += qt
    QMAKE_CXXFLAGS += -std=c++11
    #CONFIG -= qt

    CONFIG += x86_64
    TARGET = ArtificialLife

    ###########################################################################################################################
    # Cinder path variable CHANGE THIS TO THE CORRECT PATHS
    ###########################################################################################################################

    # Point to your cinder folder so that we know where everything is
    CINDER_HOME = /home/octopian/Documents/source/libraries/Cinder
    CINDER_LIB = /home/octopian/Documents/source/libraries/Cinder/Qt/CinderQt4-build
    ############################################################################################################################

    INCLUDEPATH += $${CINDER_HOME}/include
    INCLUDEPATH += $${CINDER_HOME}/lib


    DEFINES += __LINUX__
    DEFINES += CINDER_LINUX
    DEFINES += linux
    DEFINES += SC_AUDIO_API=SC_AUDIO_API_JACK
    DEFINES += OSC_HOST_LITTLE_ENDIAN
    DEFINES += BOOST_HAS_TR1
    DEFINES += CINDER_VERSION=8500
    # DEFINES += PURE_GL_SHADER_SETUP

    LIBS += -lpthread
    LIBS += $${CINDER_LIB}/libcinder.a
    LIBS += -lz -lpng -lpixman-1 -lcairo -lX11 -lcurl
    # boost
    LIBS += -lboost_date_time -lboost_filesystem -lboost_system -lboost_thread -lboost_program_options -lboost_chrono
}

CONFIG += opengl

TEMPLATE = app

HEADERS += \
    src/abstractca.h \
    src/ArtificialLife.h \
    src/CellularAutomata.h \
    src/Flocking.h \
    src/gameoflife.h \
    src/wolframca.h \
    src/Camera.h \
    src/Array.h \
    src/Euler.h \
    src/geometry.h \
    src/Vector.h \
    src/Math.h \
    src/LSystem.h \
    src/Strange.h

SOURCES += \
    src/abstractca.cpp \
    src/ArtificialLife.cpp \
    src/Flocking.cpp \
    src/gameoflife.cpp \
    src/wolframca.cpp \
    src/Camera.cpp \
    src/Array.cpp \
    src/Vector.cpp \
    src/LSystem.cpp \
    src/Strange.cpp
