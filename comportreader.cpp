#include "comportreader.h"
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <QTime>

using namespace std;
HANDLE hSerial;

ComPortReader::ComPortReader()
{
    LPCTSTR sPortName = L"COM1";

    hSerial = ::CreateFile(sPortName,GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);

    if(hSerial==INVALID_HANDLE_VALUE)
    {
            if(GetLastError()==ERROR_FILE_NOT_FOUND)
        {
            cout << "serial port does not exist.\n";
        }
        cout << "some other error occurred.\n";
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        cout << "getting state error\n";
    }
    dcbSerialParams.BaudRate=CBR_4800;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(hSerial, &dcbSerialParams))
    {
        cout << "error setting serial port state\n";
    }
}

QVector<double> ComPortReader::ReadCOM(int n)
{
    DWORD iSize;
    char sReceivedChar;

    int i = 0;
    char highb, lowb;

    highb = 'a'; // 'a' означает, что пора бы записать high bit, проставляем после добавления нового числа в массив
    QVector<double> values(n);

    this->timePassed = 0.0;
    QTime timer;
    timer.start();

    for(unsigned int i=0; i < n; i++) values[i]=sin(628/2*i*0.001);
//    while (i!=n) //сколько значений, удовлетворяющих всем условиям, будет записано в массив
//    {
//          ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);  // получаем 1 байт
//          if (iSize > 0) {  // если что-то принято, выводим
//              if (highb == 'a')
//              {
//                  highb = sReceivedChar;
//              } else
//              {
//                  lowb = sReceivedChar;
//                  double number = (highb << 8) + lowb;

//                  if (0 < number && number < 4095)
//                  {
//                      double doubleNumber = number / 1000;
//                      values[i] = doubleNumber;
//                      i++;
//                  }
//                  highb = 'a';
//              }
//          }
//    }

    double newTime = timer.elapsed() / 1000.0;
    this->timePassed = newTime;

    return values;
}
