#include "io.h"
#include <QFileDialog>

QString selectFile(QWidget *parent) {
    QString file = QFileDialog::getOpenFileName(
                parent,
                QFileDialog::tr("Open File"),
                QDir::currentPath());
    return file;
}
