#ifndef SHARED_H
#define SHARED_H


#include "QString"

QString getCurrentPath();

enum SetSignal {
    Ok,
    Same,
    Invalid
};

SetSignal setCurrentPath(QString);

bool hasPath();

#endif // SHARED_H
