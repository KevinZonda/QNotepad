#include "shared.h"

QString currentPath = "";

QString getCurrentPath(){
    return currentPath;
}

bool setCurrentPath(QString p){
    auto np = p.trimmed();
    if (np == currentPath || np.isEmpty()) return false;
    currentPath = np;
    return true;
}

bool hasPath(){
    return !currentPath.isEmpty();
}
