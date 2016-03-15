#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    /* Setting up GUI and its connections */
    ui->setupUi(this);

    connect(ui->StartButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->StopButton, SIGNAL(clicked()), this, SLOT(stop()));

    QRegExp positiveIntegers("[1-9][0-9]*");
    ui->sampleSizeBox->setValidator(new QRegExpValidator(positiveIntegers));

    /* Setting up QThread and its connections */
    generatorThread = new QThread(this);

    connect(generatorThread, SIGNAL(finished()), generatorThread, SLOT(deleteLater()));
    //QThread object lives in main thread and will be deleted after a thread wrapped by it, ends execution

    generatorThread->start();


    /* Creating Controler, moving it to separate thread and setting its connections */
    controller = new Controller();
    controller->moveToThread(generatorThread);

    connect(this, SIGNAL(startGenerator()), controller, SLOT(start()));
    connect(controller, SIGNAL(newNumber(double)), this, SLOT(takeNumber(double)));
    connect(controller, SIGNAL(finished()), this, SLOT(draw()));
    connect(generatorThread, SIGNAL(finished()), controller, SLOT(deleteLater()));

    randomNumbers = new double[1];//quick fix for first call of start() to have something to delete
}

MainWindow::~MainWindow()
{
    disconnect(controller, SIGNAL(finished()), this, SLOT(draw()));
    this->stop();
    delete[] randomNumbers;

    generatorThread->quit();
    generatorThread->wait();

    //delete randomNumbers;
    delete ui;
}

void MainWindow::start()
{
    this->stop();
    delete[] randomNumbers;

    controller->selectDistribution(ui->DistributionComboBox->currentIndex());

    unsigned long size = ui->sampleSizeBox->text().toInt();
    controller->setSampleSize(size);
    randomNumbers = new double[size];
    count = 0;

    emit startGenerator();
}

void MainWindow::stop()
{
    bool status = controller->stop();
    if(status)
    {
        qDebug()<<"generator was running and stopped";
    }
    else
    {
        qDebug()<<"generator was not running";
    }
}

void MainWindow::takeNumber(double number)
{
    randomNumbers[count++] = number;

    qDebug()<<"got number "<<number;
}

void MainWindow::draw()
{
    qDebug()<<"generation ended, will now draw from thread ";
}
