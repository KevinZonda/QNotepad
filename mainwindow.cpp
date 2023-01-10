#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "shared.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->txtContent, &QPlainTextEdit::modificationChanged, this,
                [this](bool m) {
                    if (isLoading) return;
                    this->isModified = true;
                    updateTitle();
                }
    );

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateTitle(bool isModifyCalled) {
    if (isModifyCalled) {
        auto title = this->windowTitle();
        if (this->isModified && title.startsWith("*"))
            return;
        this->setWindowTitle("*" + title);
    }
    QString title = hasPath()
            ? "QNotepad - " + getCurrentPath()
            : "QNotepad";
    title = this->isModified ? "*" + title : title;
    this->setWindowTitle(title);
}

void MainWindow::setNextLine(NextLineWay x) {
    this->currentNextLine = x;
    const char* msg;
    switch(x) {
    case CRLF:
        msg = "CRLF";
        break;
    case LF:
        msg = "LF";
        break;
    default:
        msg = "Unknown";
        break;
    }

    this->ui->statusbar->showMessage(msg);

}

NextLineWay MainWindow::getNextLine() {
    return this->currentNextLine;
}
