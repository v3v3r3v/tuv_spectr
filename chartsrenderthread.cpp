#include "chartsrenderthread.h"

#include <QtWidgets>
#include <cmath>

#include "fftw/Array.h"
#include "fftw/fftw++.h"

using namespace Array;
using namespace fftwpp;

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
        mutex.unlock();

        QVector<double> points(this->pointsCount);
        points = this->reader->ReadCOM(this->pointsCount);
        double timePassed = this->reader->timePassed;

        double timeShift = timePassed / this->pointsCount;

        QVector<double> signalX(pointsCount), signalY(pointsCount);

    //    for (int i = 0; i < pointsCount; ++i) было так
        for (int i = 0; i < pointsCount; i++)
        {
            signalX[i] = i*timeShift; // x goes from 0 to  timePassed to take points amount
            signalY[i] = points[i];        // let's plot a quadratic function
        }





        int n = points.count();
        unsigned int np = n / 2 + 1;
        size_t align = sizeof(Complex);

        array1<Complex> F(np, align);
        array1<double> f(n, align);

        rcfft1d Forward(n, f, F);

        for(int i = 0; i < n; i++) {
            f[i] = points[i];
        }

        Forward.fft(f, F);



        QVector<double> spectrX(np), spectrY(np);
        for (int i = 0; i < np; i++)
        {
          spectrX[i] = i;
          spectrY[i] = abs(F[i]) / np;
        }

        if (!restart) {
            emit newDataProcessed(signalX,
                                  signalY,
                                  timePassed,
                                  spectrX,
                                  spectrY,
                                  np);
        }

        mutex.lock();
        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();
    }
}
