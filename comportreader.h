#ifndef COMPORTREADER_H
#define COMPORTREADER_H


#include <QVector>
#include "measuredata.h"

typedef QVector<double> QVectorDouble;

class ComPortReader
{
public:
    ComPortReader();
    QVectorDouble ReadCOM(int n);
    double timePassed;
};

#endif // COMPORTREADER_H
