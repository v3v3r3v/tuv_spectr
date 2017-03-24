#ifndef COMPORTREADER_H
#define COMPORTREADER_H


#include <QVector>
#include "measuredata.h"

class ComPortReader
{
public:
    ComPortReader();
    QVector<double> ReadCOM(int n);
    double timePassed;
};

#endif // COMPORTREADER_H
