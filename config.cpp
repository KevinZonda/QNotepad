#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "config.h"

config::config() {
    font = new QStringList();
    fontSize = -1;
    nativeTitleBar = false;
    lineWrap = false;
    wordWrap = false;
    isConstructWithParameter = false;
    scaleFactor = 5;
}

config::~config() {
    delete font;
}

QString config::toJson() {
    QJsonObject j;
    j.insert("font", QJsonArray::fromStringList(*font));
    j.insert("fontSize", fontSize);
    j.insert("nativeTitleBar", nativeTitleBar);
    j.insert("scaleFactor", scaleFactor);
    j.insert("lineWrap", lineWrap);
    if (lineWrap) j.insert("wordWrap", wordWrap);
    QJsonDocument doc(j);
    return doc.toJson();
}

config::config(QString json) {
    font = new QStringList();
    fontSize = -1;
    nativeTitleBar = false;
    lineWrap = false;
    wordWrap = false;
    isConstructWithParameter = true;
    scaleFactor = 5;
    if (json.isEmpty()) return;
    auto j = QJsonDocument::fromJson(json.toUtf8());
    if (j.isNull()) return;
    auto o = j.object();
    if (o.contains("font")) {
        auto f = o.value("font");
        if (!f.isArray())
            (*font) << o.value("font").toString();
        else {
            auto a = f.toArray();
            for (auto i = a.begin(); i != a.end(); i++) {
                (*font) << i->toString();
            }
        }
    }
    if (o.contains("fontSize")) {
        fontSize = o.value("fontSize").toInt();
    }
    if (o.contains("nativeTitleBar")) {
        nativeTitleBar = o.value("nativeTitleBar").toBool();
    }
    if (o.contains("lineWrap")) {
        lineWrap = o.value("lineWrap").toBool();
    }
    if (lineWrap && o.contains("wordWrap")) {
        wordWrap = o.value("wordWrap").toBool();
    }
    if (o.contains("scaleFactor")) {
        scaleFactor = o.value("scaleFactor").toInt();
        if (scaleFactor <= 0) scaleFactor = 5;
    }
}