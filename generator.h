#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtCore>
#include <QMutex>

#include "pcg-cpp-0.98/include/pcg_random.hpp"


/* Qt class providing interface between GUI and generator */
class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = 0);
    ~Controller();

    bool stop(); // returns true if called during generation and false if called when idle

public slots:
    void selectDistribution(int);
    void setSampleSize(unsigned long);
    void start();

signals:
    void finished();
    void newNumber(double);

private:
    bool running;
    bool abort;
    int index;
    unsigned long sample;
    QMutex mutex;
    pcg32 randomNumberGenerator;

    template< class DIST >
    double generate( DIST &distribution )
    {
        return (double) distribution(randomNumberGenerator);
    }
};

#endif // GENERATOR_H
