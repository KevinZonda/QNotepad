#include "io.h"
#include <QFileDialog>

QString selectFile(QWidget *parent) {
    QString file = QFileDialog::getOpenFileName(
                parent,
                QFileDialog::tr("Open File"),
                QDir::currentPath());
    return file;
}


struct ReadStatus readAllText(QString path) {
    struct ReadStatus x;
    x.text = "";

    QFile f(path);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        x.ok = false;
        return x;
    }
    QTextStream in(&f);
    x.text = in.readAll();
    x.ok = true;
    f.close();
    return x;
}
