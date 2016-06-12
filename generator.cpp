#include <QDebug>

#include <random>

#include "generator.h"

Controller::Controller(QObject *parent): QObject(parent), randomNumberGenerator(pcg_extras::seed_seq_from<std::random_device>())
{
/* do not use new here, as it will be allocated in the heap of the main thread, because the object is constructed before it is moved to thread */
/* unless creating QObject and point this as the parent */

    qDebug()<<"constructing controller from thread "<<thread()->currentThreadId();

    running = false;
    abort = false;
}

Controller::~Controller()
{
    qDebug()<<"destructing controller from thread "<<thread()->currentThreadId();
}


bool Controller::stop()
{
    qDebug()<<"stopping generator from thread "<<thread()->currentThreadId();

    mutex.lock();
    bool isRunning = running;
    mutex.unlock();

    if(isRunning)
    {
        mutex.lock();
        abort = true;
        mutex.unlock();

        return true;
    }
    else
    {
        return false;
    }
}

void Controller::selectDistribution(int Iindex)
{
    index = Iindex;

    qDebug()<<"selected distribution "<<index;
}

void Controller::setSampleSize(unsigned long size)
{
    sample = size;

    qDebug()<<"sample size "<<sample;
}

void Controller::start()
{
    qDebug()<<"starting generation from thread "<<thread()->currentThreadId();

    mutex.lock();
    running = true;
    mutex.unlock();

   std::normal_distribution<> distribution(0.0, 2.0);

   /*switch (index)
    {
        case 1:
            std::normal_distribution<> distribution(0.0, 2.0);
        break;
    }*/

    bool breakLoop;

    for(unsigned long i = 0UL; i < sample; ++i)
    {
        mutex.lock();
        breakLoop = abort;
        mutex.unlock();

        if(breakLoop)
            break;

        emit newNumber(distribution(randomNumberGenerator));
    }

    mutex.lock();
    running = false;
    abort = false;
    mutex.unlock();

    emit finished();
}
