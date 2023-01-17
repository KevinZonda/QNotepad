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

QString fileSize(qint64 size) {
    if (size < 1024) {
        return QString::number(size) + " B";
    }
    double kb = size / 1024.0;
    if (kb < 1024) {
        return QString::number(kb, 'f', 2) + " KB";
    }
    double mb = kb / 1024.0;
    if (mb < 1024) {
        return QString::number(mb, 'f', 2) + " MB";
    }
    double gb = mb / 1024.0;
    return QString::number(gb, 'f', 2) + " GB";
}

struct ReadStatus readAllText(QString path) {
    struct ReadStatus x;
    x.text = "";
    x.ok = false;

    QFile f(path);
    x.exists = f.exists();
    if (!x.exists){
        return x;
    }
    if (!f.open(QFile::ReadOnly)) {
        return x;
    }

    QTextStream in(&f);
    x.text = in.readAll();
    x.ok = true;
    x.size = fileSize(f.size());
    f.close();
    return x;
}

bool writeAllText(QString path, QString& content, Encoding encoding) {
    QFile f(path);
    if (!f.open(QFile::WriteOnly)) {
        return false;
    }
    f.write(content.toUtf8());
    f.close();
    return true;
}
