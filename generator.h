#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtCore>
#include <QMutex>

/* Abstract class providing interface for implemented generators */
class Generator
{
public:
    //Generator()
    virtual ~Generator();

    //virtual void seed(unsigned long) = 0;
    virtual double generate() = 0;
};

class Rand : public Generator
{
public:
    //Rand();
    ~Rand();

    //void seed(unsigned long);
    double generate();
};


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
    Generator* generator;

};

#endif // GENERATOR_H
