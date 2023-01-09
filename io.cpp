#include "io.h"
#include <QFileDialog>

QString selectFile(QWidget *parent) {
    QString file = QFileDialog::getOpenFileName(
                parent,
                QFileDialog::tr("Open File"),
                QDir::currentPath());
    return file;
}

QString selectNewFile(QWidget *parent) {
    QString file = QFileDialog::getSaveFileName(
                parent,
                QFileDialog::tr("Save to"),
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

bool writeAllText(QString path, QString content, Encoding encoding) {
    QFile f(path);
    if (!f.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }
    f.write(content.toUtf8());
    f.close();
    return true;
}
