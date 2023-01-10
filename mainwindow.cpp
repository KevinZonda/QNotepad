#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "shared.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateTitle() {
    this->setWindowTitle(hasPath()
                      ? "QNotepad - " + getCurrentPath()
                      : "QNotepad");
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
