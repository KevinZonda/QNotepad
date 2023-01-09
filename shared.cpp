#include "shared.h"

QString currentPath = "";

QString getCurrentPath(){
    return currentPath;
}

QString setCurrentPath(QString p){
    currentPath = p.trimmed();
    return p;
}

bool hasPath(){
    return !currentPath.isEmpty();
}
