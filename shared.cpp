#include <QFileInfo>
#include <QLabel>
#include <QQueue>
#include "shared.h"

QString currentPath = "";

void setPathLabel() {
    auto path = currentPath;
    auto pth = getLabelMap()->value("path");
    if (currentPath.length() > 50) {
        path = "..." + currentPath.right(50);
    }
    pth->setToolTip(currentPath);
    pth->setText(path);
}

void clearCurrentPath() {
    currentPath = "";
    setPathLabel();
}

QString getTitle() {
    if (!hasPath()) return "untitled";
    QFileInfo fi(getCurrentPath());
    return fi.fileName();
}

QString getCurrentPath(){
    return currentPath;
}

SetSignal setCurrentPath(QString p){
    auto np = p.trimmed();
    if (np.isEmpty()) return Invalid;
    if (np == currentPath) return Same;
    currentPath = np;
    setPathLabel();
    return Ok;
}

bool hasPath(){
    return !currentPath.isEmpty();
}

QMap<QString, QAction*> actMap;

QMap<QString, QAction*>* getActionMap() {
    return &actMap;
}
QMap<QString, QLabel*> lblMap;

QMap<QString, QLabel*>* getLabelMap() {
    return &lblMap;
}

QQueue<QAction*> act;

QQueue<QAction*>* getAction() {
    return &act;
}
