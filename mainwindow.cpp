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


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);

  connect(&thread,
          SIGNAL(newDataProcessed(QVector<double>, QVector<double>, doubleQVector<double>, QVector<double>, double)),
          this,
          SLOT(emittedSignal(QVector<double>, QVector<double>, doubleQVector<double>, QVector<double>, double)));
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
  this->xSpectrAxis->setRange(0, 100);
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

  this->start();
}

void MainWindow::start()
{
    int interval = 5000;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(processGraphicData()));
    timer->start(interval);
}

void MainWindow::processGraphicData()
{
    this->thread.readAndMakeCalculationsFor(200);
    //в инициализации нужно подписаться на слот класса для асинхронного чтения и анализа
    //тут вызов класса для асинхронного чтения и анализа
}

void MainWindow::emittedSignal(QVector<double> signalX,
                                     QVector<double> signalY,
                                     double timePassed,
                                     QVector<double> spectrX,
                                     QVector<double> spectrY,
                                     double spectrRange)
{
    this->xSignalAxis->setRange(0, timePassed);
    this->signalGraph->setData(signalX, signalY);

    this->xSignalAxis->setRange(0, spectrRange);
    this->spectrGraph->setData(spectrX, spectrY);
}

MainWindow::~MainWindow()
{
  delete ui;
}
