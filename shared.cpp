#include "shared.h"

QString currentPath = "";

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
