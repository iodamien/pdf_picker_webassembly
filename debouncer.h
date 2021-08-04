#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include <QTimer>
#include <QObject>

class Debouncer : public QObject {

Q_OBJECT

public:
    Debouncer(long trigger, QObject* parent = nullptr);
    void setTimeOut(long timeMs);
    void send(float value);

signals:
    void triggered(float val);

public slots:
    void onTimeOut();

private:
    QTimer timer;
    long lastTimeMs{0};
    long timeoutMs;
    float lastValue{1.0f};
};

#endif // DEBOUNCER_H
