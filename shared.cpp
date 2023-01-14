#include <QFileInfo>
#include "shared.h"

QString currentPath = "";

void clearCurrentPath() {
    currentPath = "";
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
    return Ok;
}

bool hasPath(){
    return !currentPath.isEmpty();
}

QMap<QString, QAction*> actMap;

QMap<QString, QAction*>* getActionMap() {
    return &actMap;
}
