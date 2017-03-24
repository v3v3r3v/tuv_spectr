#ifndef CHARTSRENDERTHREAD_H
#define CHARTSRENDERTHREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include "comportreader.h"

class ChartsRenderThread : public QThread
{
    Q_OBJECT

public:
    ChartsRenderThread(QObject *parent = 0);
    ~ChartsRenderThread();

    void readAndMakeCalculationsFor(int);

signals:
    void newDataProcessed(QVector<double>,
                          QVector<double>,
                          double,
                          QVector<double>,
                          QVector<double>,
                          double);

protected:
    void run() override;

private:
    ComPortReader* reader;

    QMutex mutex;
    QWaitCondition condition;

    int pointsCount;

    bool restart;
    bool abort;
};

#endif // CHARTSRENDERTHREAD_H
