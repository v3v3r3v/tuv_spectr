#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot/qcustomplot.h"
#include "chartsrenderthread.h"
//#include "measuredata.h"
//#include "comportreader.h"

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
  void start();
protected:
  ChartsRenderThread thread;
public slots:
    void processGraphicData();
    void emittedSignal(QVector<double>,
                       QVector<double>,
                       double,
                       QVector<double>,
                       QVector<double>,
                       double);
private:
  Ui::MainWindow *ui;
  QCPAxis * xSignalAxis;
  QCPAxis * ySignalAxis;
  QCPAxis * xSpectrAxis;
  QCPAxis * ySpectrAxis;

  QCPGraph * signalGraph;
  QCPGraph * spectrGraph;


};

#endif // MAINWINDOW_H
