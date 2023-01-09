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


QString selectNewFile(QWidget *parent);

enum Encoding {
    UTF8,
    ASNI,
    UTF16
};

bool writeAllText(QString path, QString content, Encoding encoding = UTF8);

#endif // IO_H
