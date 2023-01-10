#include "text.h"
#include "QtCore/qdebug.h"

NextLineWay getCrLf(QString s) {
    if (!s.contains("\n")) {
        return Unknown;
    }
    if (s.contains("\r\n")) return CRLF;
    return LF;
}

QString toLf(NextLineWay ori, QString str) {
    if (ori != CRLF) return str;
    while (str.contains("\r\n")) {
        str = str.replace("\r\n", "\n");
    }
    return str;
}

QString toLf(QString str) {
    while (str.contains("\r\n")) {
        str = str.replace("\r\n", "\n");
    }
    return str;
}

QString toCrLf(NextLineWay ori, QString str) {
    if (ori == CRLF) return str;
    return str.replace("\n", "\r\n").replace("\r\r\n", "\r\n");;
}


QString toCrLf(QString str) {
    return str.replace("\n", "\r\n").replace("\r\r\n", "\r\n");;
}
