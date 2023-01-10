#include "text.h"

const char* _lf = "LF";
const char* _crlf = "CRLF";
const char* _unknown = "Unknown (LF)";

const char* nextLineWayString(NextLineWay n) {
    if (n == LF) return _lf;
    if (n == CRLF) return _crlf;
    return _unknown;
}

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
