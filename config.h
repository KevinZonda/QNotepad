#ifndef QNOTEPAD_CONFIG_H
#define QNOTEPAD_CONFIG_H


#include <QString>

class config {
public:
    config();
    config(QString json);
    QString font;
    int fontSize;
    QString toJson();
};


#endif //QNOTEPAD_CONFIG_H
