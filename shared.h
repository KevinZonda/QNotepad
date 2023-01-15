#ifndef SHARED_H
#define SHARED_H


#include "QString"
#include "QtGui/qaction.h"
#include <QMap>
#include <QLabel>

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

QMap<QString, QLabel*>* getLabelMap();

QQueue<QAction*>* getAction();

QString getConfigPath();

QString getConfigFile(const QString filename);

#endif // SHARED_H
