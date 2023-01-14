#ifndef SHARED_H
#define SHARED_H


#include "QString"
#include "QtGui/qaction.h"
#include <QMap>

QString getCurrentPath();

enum SetSignal {
    Ok,
    Same,
    Invalid
};

SetSignal setCurrentPath(QString);

bool hasPath();

void clearCurrentPath();

QString getTitle();

QMap<QString, QAction*>* getActionMap();


#endif // SHARED_H
