#include <QDebug>

#include "generator.h"

Controller::Controller(QObject *parent): QObject(parent)
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

    generator = new Rand();
    //generator->seed(123UL);

    bool breakLoop;

    for(unsigned long i = 0UL; i < sample; ++i)
    {
        mutex.lock();
        breakLoop = abort;
        mutex.unlock();

        if(breakLoop)
            break;

        emit newNumber(generator->generate());
    }

    delete generator;

    mutex.lock();
    running = false;
    abort = false;
    mutex.unlock();

    emit finished();
}

/* Generator */

//Generator::Generator() {}

Generator::~Generator() {}


/* Generators */

//Rand::Rand() {}

Rand::~Rand() {}

/*void Rand::seed(unsigned long iSeed)
{
    qDebug()<<"seed"<<iSeed;
}*/

double Rand::generate()
{
    return 3.14;
}
