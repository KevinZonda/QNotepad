#ifndef QNOTEPAD_CONFIG_H
#define QNOTEPAD_CONFIG_H

#include <QString>

class config {
public:
    config();
    config(QString json);
    ~config();
    QStringList* font;
    int fontSize;
    bool nativeTitleBar;
    bool lineWrap;
    bool wordWrap;
    bool isConstructWithParameter;
    QString toJson();
};


#endif //QNOTEPAD_CONFIG_H
