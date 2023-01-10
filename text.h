#ifndef TEXT_H
#define TEXT_H

#include <QString>

enum NextLineWay {
    CRLF,  // \r\n
    LF,    // \n
    Unknown
};

NextLineWay getCrLf(QString);

QString toLf(NextLineWay, QString);

QString toCrLf(NextLineWay, QString);

QString toLf(QString);

QString toCrLf(QString);

const char* nextLineWayString(NextLineWay);

#endif // TEXT_H
