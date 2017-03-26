#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <thread>
#include <stdlib.h>

#include <QTimer>
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

#include "fftw/Array.h"
#include "fftw/fftw++.h"

using namespace std;
using namespace utils;
using namespace Array;
using namespace fftwpp;

#define N 100
typedef QVector<double> QVectorDouble;



MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{

  qRegisterMetaType<QVectorDouble>("QVectorDouble");
  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);

  connect(&thread,
          SIGNAL(newDataProcessed(QVectorDouble,
                                  QVectorDouble,
                                  double,
                                  QVectorDouble,
                                  QVectorDouble,
                                  double)),
          this,
          SLOT(emittedSignal(QVectorDouble,
                             QVectorDouble,
                             double,
                             QVectorDouble,
                             QVectorDouble,
                             double)));
\
  setWindowTitle("Спектр сигнала");

  statusBar()->clearMessage();

  setupGraphics(ui->customPlot);
}

void MainWindow::setupGraphics(QCustomPlot *customPlot)
{

  fftw::maxthreads=get_max_threads();

  //create plots

  customPlot->plotLayout()->clear(); // clear default axis rect
  // add two axis rects to rows
  QCPAxisRect* spectrAxisRect = new QCPAxisRect(customPlot);
  customPlot->plotLayout()->addElement(0, 0, spectrAxisRect);
  QCPAxisRect* signalAxisRect = new QCPAxisRect(customPlot);
  customPlot->plotLayout()->addElement(1, 0, signalAxisRect);


  //setup spectr axis
  this->xSpectrAxis = spectrAxisRect->axis(QCPAxis::atBottom);
  this->ySpectrAxis = spectrAxisRect->axis(QCPAxis::atLeft);
  this->xSpectrAxis->setRange(0, 1000);
  this->xSpectrAxis->setLabel("Частота");
  this->ySpectrAxis->setRange(0, 1);
  this->ySpectrAxis->setLabel("");
  this->spectrGraph = customPlot->addGraph(xSpectrAxis,ySpectrAxis);


  // create graph and assign data to it:
  this->xSignalAxis = signalAxisRect->axis(QCPAxis::atBottom);
  this->ySignalAxis = signalAxisRect->axis(QCPAxis::atLeft);
  this->xSignalAxis->setLabel("t,с");
  this->ySignalAxis->setRange(0, 5);
  this->ySignalAxis->setLabel("V,В");

  this->signalGraph = customPlot->addGraph(xSignalAxis,ySignalAxis);

  // set layers for axis
  QList<QCPAxis*> allAxes;
  allAxes << spectrAxisRect->axes() << signalAxisRect->axes();
  foreach (QCPAxis *axis, allAxes)
  {
    axis->setLayer("axes");
    axis->grid()->setLayer("grid");
  }

  ui->customPlot->replot();

  this->processGraphicData();
}

void MainWindow::processGraphicData()
{
    this->thread.readAndMakeCalculationsFor(100);
}

void MainWindow::emittedSignal(QVectorDouble signalX,
                                     QVectorDouble signalY,
                                     double timePassed,
                                     QVectorDouble spectrX,
                                     QVectorDouble spectrY,
                                     double spectrRange)
{
    qDebug() << "emitted signal";
    qDebug() << "timePassed" << timePassed;
    qDebug() << "spectrRange" << spectrRange;
    qDebug() << "spectrY" << spectrY;
    qDebug() << "signalY" << signalY;


    this->xSignalAxis->setRange(0, timePassed);
    this->signalGraph->setData(signalX, signalY);

    this->spectrGraph->setData(spectrX, spectrY);
    ui->customPlot->replot();

    this->processGraphicData();
}

MainWindow::~MainWindow()
{
  delete ui;
}
