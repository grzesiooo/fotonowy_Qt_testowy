#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "generator.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QThread *generatorThread;
    Controller *controller;
    double* randomNumbers;
    unsigned long count;

private slots:
    void changeParameters(int);
    void start();
    void stop();
    void takeNumber(double);
    void draw();

signals:
    void startGenerator();

};

#endif // MAINWINDOW_H
