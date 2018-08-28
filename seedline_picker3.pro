#-------------------------------------------------
#
# Project created by QtCreator 2012-05-12T13:31:32
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = seedline_picker3
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    GLWidget.cpp \
    JdsVtkScalar.cpp \
    JdsVtkScalarGfx.cpp \
    JdsVtkFile.cpp \
    JdsVtkScalarFile.cpp \
    SeedLine.cpp \
    SeedLineGfx.cpp \
    jds_shader_program.cpp \
    jds_shader.cpp \
    jds_opengl.cpp \
    jds_counter.cpp \
    dragonfly_wing.cpp \
    GfxBase.cpp

HEADERS  += MainWindow.h \
    GLWidget.h \
    JdsVtkScalar.h \
    JdsVtkScalarGfx.h \
    JdsVtkFile.h \
    JdsVtkScalarFile.h \
    SeedLine.h \
    SeedLineGfx.h \
    jds_shader_program.hpp \
    jds_shader.hpp \
    jds_opengl.hpp \
    jds_counter.hpp \
    dragonfly_wing.h \
    GfxBase.h

unix:LIBS += -lrt -lGLEW -lGLU
# bdver1 = bulldozer version 1
# btver1 = brazos t version 1
# barcelona = AMD family 10h with SSE4A (AMD family 12h)
# native = hopefully picks the right version for anything
unix:QMAKE_CFLAGS_RELEASE += -fsingle-precision-constant -march=native
unix:QMAKE_CFLAGS_DEBUG += -fsingle-precision-constant -march=native
unix:QMAKE_CXXFLAGS_RELEASE += -fsingle-precision-constant -march=native
unix:QMAKE_CXXFLAGS_DEBUG += -fsingle-precision-constant -march=native

win32:INCLUDEPATH += $$quote(C:/vc_lib/x64/include)
win32:QMAKE_LFLAGS += /LIBPATH:$$quote("\"c:/vc_lib/x64/lib\"")
# these options should be OK for VS 2010, AVX can be used also if SSE2 isn't supported
# /Oi enables intrinsics, /Ot is favor fast code
win32:QMAKE_CFLAGS_RELEASE += $$quote(/Oi /Ot)
win32:QMAKE_CFLAGS_DEBUG += $$quote(/Oi)
win32:QMAKE_CXXFLAGS_RELEASE += $$quote(/Oi /Ot)
win32:QMAKE_CXXFLAGS_DEBUG += $$quote(/Oi)
win32:LIBS += -lkernel32 -lglew32_d -lglu32
#win32:LIBS += -lkernel32 -lglew32
win32:CONFIG += console # used for debugging output, delete when done

OTHER_FILES += \
    prim_render_v330.vert \
    prim_render_v330.frag \
    smooth_ads_v330.vert \
    smooth_ads_v330.frag \
    smooth_ads_twoside_v330.vert \
    smooth_ads_twoside_v330.frag
