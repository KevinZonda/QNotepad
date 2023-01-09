#include "QtWidgets/qmenubar.h"
#include "QStatusBar"
#include "io.h"
#include "mainwindow.h"
#include "QPlainTextEdit"
#include "shared.h"
#include "ui_mainwindow.h"

#include <QApplication>

static QPlainTextEdit* txt = nullptr;

void setMenu(MainWindow *w) {
    w->menuBar()->setNativeMenuBar(false);

    QMenu* menuEdit = w->menuBar()->addMenu("&Edit");
    auto *undoAct = new QAction("&Undo", w);
    MainWindow::connect(undoAct, &QAction::triggered, qApp, []{
        if (txt == nullptr) return;
        txt->undo();
    });
    menuEdit->addAction(undoAct);

    QMenu* menuFile = w->menuBar()->addMenu("&File");
    auto *saveAct = new QAction("&Save", w);
    MainWindow::connect(saveAct, &QAction::triggered, qApp, []{
        if (txt == nullptr) return;
    });

    auto *openAct = new QAction("&Open", w);
    MainWindow::connect(openAct, &QAction::triggered, qApp, []{
        if (txt == nullptr) return;
        auto path = selectFile(nullptr);
        txt->setPlainText(path);
    });


    auto *saveAsAct = new QAction("&Open", w);
    MainWindow::connect(saveAsAct, &QAction::triggered, qApp, []{
        if (txt == nullptr) return;
    });

    menuFile->addAction(openAct);
    menuFile->addAction(saveAct);
    menuFile->addAction(saveAsAct);


    w->menuBar()->addMenu(menuFile);
    w->menuBar()->addMenu(menuEdit);
}


bool isModified;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    setMenu(&w);

    if (argc > 1) {
        setCurrentPath(QString(argv[1]));
    }

    w.updateTitle();
    txt = w.ui->txtContent;
    txt->setPlainText("OK!");

    w.show();
    return a.exec();
}


