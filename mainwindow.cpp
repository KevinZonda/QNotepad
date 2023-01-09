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
