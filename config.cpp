#include <QJsonDocument>
#include <QJsonObject>
#include "config.h"

config::config() {
    font = "";
    fontSize = -1;
}

QString config::toJson() {
    QJsonObject j;
    j.insert("font", font);
    j.insert("fontSize", fontSize);
    QJsonDocument doc(j);
    return doc.toJson();
}

config::config(QString json) {
    font = "";
    fontSize = -1;
    if (json.isEmpty()) return;
    auto j = QJsonDocument::fromJson(json.toUtf8());
    if (j.isNull()) return;
    auto o = j.object();
    if (o.contains("font")) {
        font = o.value("font").toString();
    }
    if (o.contains("fontSize")) {
        fontSize = o.value("fontSize").toInt();
    }
}