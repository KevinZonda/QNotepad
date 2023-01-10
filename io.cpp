#include "io.h"
#include "shared.h"
#include <QFileDialog>

QString getTargetPath() {
    if (hasPath()) {
        QFileInfo fi(getCurrentPath());
        return fi.absolutePath();
    }
    return QDir::currentPath();
}

QString selectFile(QWidget *parent) {
    QString file = QFileDialog::getOpenFileName(
                parent,
                QFileDialog::tr("Open File"),
                getTargetPath());
    return file;
}

QString selectNewFile(QWidget *parent) {
    QString file = QFileDialog::getSaveFileName(
                parent,
                QFileDialog::tr("Save to"),
                getTargetPath());
    return file;
}


struct ReadStatus readAllText(QString path) {
    struct ReadStatus x;
    x.text = "";

    QFile f(path);
    if (!f.open(QFile::ReadOnly)) {
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
    if (!f.open(QFile::WriteOnly)) {
        return false;
    }
    f.write(content.toUtf8());
    f.close();
    return true;
}
