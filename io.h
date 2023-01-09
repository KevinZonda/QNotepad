#ifndef IO_H
#define IO_H

#include <QString>
#include <QWidget>

QString selectFile(QWidget *);

struct ReadStatus readAllText(QString path);

struct ReadStatus {
    QString text;
    bool ok;
};

#endif // IO_H
