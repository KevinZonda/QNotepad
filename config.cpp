#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "config.h"

config::config() {
    font = new QStringList();
    fontSize = -1;
    nativeTitleBar = false;
}

config::~config() {
    delete font;
}

QString config::toJson() {
    QJsonObject j;
    j.insert("font", QJsonArray::fromStringList(*font));
    j.insert("fontSize", fontSize);
    j.insert("nativeTitleBar", nativeTitleBar);
    QJsonDocument doc(j);
    return doc.toJson();
}

config::config(QString json) {
    font = new QStringList();
    fontSize = -1;
    nativeTitleBar = false;
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
}