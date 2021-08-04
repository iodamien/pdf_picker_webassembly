#include "debouncer.h"
#include <QDateTime>

#include <time.h>


using namespace std::chrono;


Debouncer::Debouncer(long triggerTimeMs, QObject* parent): QObject(parent) {
    setTimeOut(triggerTimeMs);
    connect(&timer, &QTimer::timeout, this, &Debouncer::onTimeOut);
    timer.setSingleShot(true);
}

void Debouncer::send(float value) {
    if (value == lastValue) {
        return;
    }

    lastValue = value;
    timer.start();

    long currentTimeMs = time(NULL);
    long diffTms = currentTimeMs - lastTimeMs;

    if (diffTms >= timeoutMs) {
        lastTimeMs = currentTimeMs;
        timer.stop();
        onTimeOut();
    }
    else {
        timer.setInterval(diffTms);
        timer.start();
    }
}

void Debouncer::setTimeOut(long timeMs) {
   timeoutMs = timeMs;
}

void Debouncer::onTimeOut() {
    emit triggered(lastValue);
}
