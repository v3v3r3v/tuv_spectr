#include "chartsrenderthread.h"

#include <QtWidgets>
#include <cmath>

#include <QDebug>

#include "fftw/Array.h"
#include "fftw/fftw++.h"

using namespace Array;
using namespace fftwpp;
typedef QVector<double> QVectorDouble;

ChartsRenderThread::ChartsRenderThread(QObject *parent)
    : QThread(parent)
{
    this->reader = new ComPortReader();
    restart = false;
    abort = false;
}

ChartsRenderThread::~ChartsRenderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void ChartsRenderThread::readAndMakeCalculationsFor(int pointsCount)
{
    QMutexLocker locker(&mutex);
    this->pointsCount = pointsCount;
    if (!isRunning()) {
        start(LowPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }
}

void ChartsRenderThread::run()
{
    forever {
        mutex.lock();
        QVectorDouble points(this->pointsCount);
        points = this->reader->ReadCOM(this->pointsCount);
        double timePassed = this->reader->timePassed;
        mutex.unlock();

        unsigned int n = points.count();
        double timeShift = timePassed / n;

        QVectorDouble signalX(n), signalY(n);

        for (unsigned int i = 0; i < n; i++)
        {
            signalX[i] = i*timeShift; // x goes from 0 to  timePassed to take points amount
            signalY[i] = points[i];        // let's plot a quadratic function
        }





        fftw::maxthreads = get_max_threads();
        unsigned int np = n / 2 + 1;
        size_t align = sizeof(Complex);

        array1<Complex> F(np, align);
        array1<double> f(n, align);

        rcfft1d Forward(n, f, F);
//        crfft1d Backward(n, F, f);

        for(unsigned int i = 0; i < n; i++) {
            f[i] = points[i];
        }

        Forward.fft(f, F);

        //вывести и узнать, сколько точек лежит в F. Вроде бы np
        qDebug() << F.count();

        QVectorDouble spectrX(np), spectrY(np);
        for (int i = 0; i < np; i++)
        {
          spectrX[i] = i * 20; //если снимаем 100 точек, то преобразование отдает 50 точек, которые нужно распеределить по графику с максимально частотой 1000 => соответственно усножаем на 20.
          spectrY[i] = abs(F[i]) / np;
        }

        qDebug() << "data received";
        qDebug() << timePassed;

        emit newDataProcessed(signalX,
                              signalY,
                              timePassed,
                              spectrX,
                              spectrY,
                              np);

//        if (!restart) {

//        }

        mutex.lock();
        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();
    }
}
