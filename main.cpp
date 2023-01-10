#include "QtWidgets/qmenubar.h"
#include "QStatusBar"
#include "io.h"
#include "mainwindow.h"
#include "QPlainTextEdit"
#include "shared.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QMessageBox>

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
    auto *redoAct = new QAction("&Redo", w);
    MainWindow::connect(redoAct, &QAction::triggered, qApp, []{
        if (txt == nullptr) return;
        txt->redo();
    });
    menuEdit->addAction(redoAct);

    QMenu* menuFile = w->menuBar()->addMenu("&File");
    auto *saveAct = new QAction("&Save", w);
    MainWindow::connect(saveAct, &QAction::triggered, qApp, [w]{
        if (txt == nullptr) return;
        bool isOk = w->save();
        if (!isOk) {
            QMessageBox::critical(nullptr, "Write failed", "Cannot write to specific file!");
        }
    });

    auto *newAct = new QAction("&New", w);
    MainWindow::connect(newAct, &QAction::triggered, qApp, [w]{
        // TODO: Save current
        clearCurrentPath();
        txt->setPlainText("");
        w->isModified = false;
        w->updateTitle();
    });

    auto *openAct = new QAction("&Open", w);
    MainWindow::connect(openAct, &QAction::triggered, qApp, [w]{
        if (txt == nullptr) return;
        auto path = selectFile(nullptr);
        SetSignal setStat = setCurrentPath(path);
        if (setStat == Invalid) return;

        if (setStat == Same) {
            auto rst = QMessageBox::information(nullptr, "Reload", "Reload file?", QMessageBox::Ok | QMessageBox::Cancel);
            if (rst == QMessageBox::Cancel) return;
        }
        bool isOk = w->loadFile();
        if (isOk) w->updateTitle();

    });


    auto *saveAsAct = new QAction("&Save as", w);
    MainWindow::connect(saveAsAct, &QAction::triggered, qApp, [w]{
        if (txt == nullptr) return;
        auto path = selectNewFile(nullptr);
        if (path.isEmpty()) return;
        bool isOk = w->save(path);
        if (!isOk) {
            setCurrentPath(path);
            w->updateTitle();
            QMessageBox::critical(nullptr, "Write failed", "Cannot write to specific file!");
        }
    });

    auto *reloadAct = new QAction("&Reload", w);
    MainWindow::connect(reloadAct, &QAction::triggered, qApp, [w]{
        if (txt == nullptr || !hasPath()) return;
        w->loadFile();
    });

    auto *exitAct = new QAction("&Exit", w);
    MainWindow::connect(exitAct, &QAction::triggered, qApp, [w]{
        if (txt == nullptr || !hasPath()) return;
        // TODO

    });

    menuFile->addAction(newAct);
    menuFile->addAction(openAct);
    menuFile->addAction(saveAct);
    menuFile->addAction(saveAsAct);
    menuFile->addAction(reloadAct);
    menuFile->addAction(exitAct);

    w->menuBar()->addMenu(menuFile);
    w->menuBar()->addMenu(menuEdit);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    setMenu(&w);

    if (argc > 1) {
        setCurrentPath(QString(argv[1]));
        w.loadFile();
    }

    w.updateTitle();
    txt = w.ui->txtContent;

    w.show();
    return a.exec();
}


