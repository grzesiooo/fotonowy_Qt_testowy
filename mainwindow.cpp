#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    /* Setting up GUI and its connections */
    ui->setupUi(this);

    connect(ui->StartButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->StopButton, SIGNAL(clicked()), this, SLOT(stop()));

    connect(ui->DistributionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeParameters(int)));

    QRegExp positiveIntegers("[1-9][0-9]*");
    ui->SampleSizeBox->setValidator(new QRegExpValidator(positiveIntegers));


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

    delete ui;
}

void MainWindow::changeParameters(int index)
{
    QRegExp positiveDoubles("\\d+\\.?\\d*");

    switch (index)
    {
        case 0: //no distribution selected
            ui->Parameter1Name->setText("");
            ui->Parameter2Name->setText("");

            ui->Parameter1Value->setText("");
            ui->Parameter1Value->setEnabled(false);
            ui->Parameter2Value->setText("");
            ui->Parameter2Value->setEnabled(false);

            break;
        case 1: //normal distribution
            ui->Parameter1Name->setText("sigma");
            ui->Parameter2Name->setText("");

            ui->Parameter1Value->setEnabled(true);
            ui->Parameter1Value->setValidator(new QRegExpValidator(positiveDoubles));
            ui->Parameter1Value->setText("1");
            ui->Parameter2Value->setText("");
            ui->Parameter2Value->setEnabled(false);

        break;
    default:
        break;
    }
}

void MainWindow::start()
{
    this->stop();

    int index = ui->DistributionComboBox->currentIndex();
    if(index==0)
    {
        qDebug()<<"no disrtibution selected";
        //popup ""Please select distribution"

        return;
    }
    else
    {
        controller->selectDistribution(index);
    }

    unsigned long size = ui->SampleSizeBox->text().toInt();
    controller->setSampleSize(size);

    delete[] randomNumbers;
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

    //QPainter histogram = new QPainter(ui->HistogramWidget);

}
