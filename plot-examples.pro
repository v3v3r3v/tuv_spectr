#
#  QCustomPlot Plot Examples
#

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = plot-examples
TEMPLATE = app

SOURCES += main.cpp\
           mainwindow.cpp \
    fftw/Array.cc \
    fftw/Complex.cc \
    fftw/convolution.cc \
    fftw/fftw++.cc \
    qcustomplot/qcustomplot.cpp \
    comportreader.cpp \
    chartsrenderthread.cpp

HEADERS  += mainwindow.h \
    fftw/align.h \
    fftw/Array.h \
    fftw/cmult-sse2.h \
    fftw/Complex.h \
    fftw/convolution.h \
    fftw/fftw++.h \
    fftw/seconds.h \
    fftw/statistics.h \
    fftw/transposeoptions.h \
    qcustomplot/qcustomplot.h \
    comportreader.h \
    chartsrenderthread.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/fftwdll32/ -lfftw3-3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/fftwdll32/ -lfftw3-3
else:unix: LIBS += -L$$PWD/fftwdll32/ -lfftw3-3

INCLUDEPATH += $$PWD/fftwdll32
DEPENDPATH += $$PWD/fftwdll32

DISTFILES +=

