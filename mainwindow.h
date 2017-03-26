#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot/qcustomplot.h"
#include "chartsrenderthread.h"
//#include "measuredata.h"
//#include "comportreader.h"

typedef QVector<double> QVectorDouble;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void setupGraphics(QCustomPlot *customPlot);
public slots:
    void processGraphicData();   
private slots:
    void emittedSignal(QVectorDouble,
                       QVectorDouble,
                       double,
                       QVectorDouble,
                       QVectorDouble,
                       double);
private:
  Ui::MainWindow *ui;
  QCPAxis * xSignalAxis;
  QCPAxis * ySignalAxis;
  QCPAxis * xSpectrAxis;
  QCPAxis * ySpectrAxis;

  QCPGraph * signalGraph;
  QCPGraph * spectrGraph;

  ChartsRenderThread thread;
};

#endif // MAINWINDOW_H
