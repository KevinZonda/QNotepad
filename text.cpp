#include "text.h"

NextLineWay getCrLf(QString s) {
    if (!s.contains("\n")) {
        return Unknown;
    }
    if (s.contains("\r\n")) return CRLF;
    return LF;
}
