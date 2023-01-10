#ifndef TEXT_H
#define TEXT_H

#include <QString>

enum NextLineWay {
    CRLF, // \r\n
    LF,    // \n
    Unknown
};

NextLineWay getCrLf(QString);

#endif // TEXT_H
