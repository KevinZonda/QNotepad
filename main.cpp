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

    QMenu* menuFile = w->menuBar()->addMenu("&File");
    auto *saveAct = new QAction("&Save", w);
    MainWindow::connect(saveAct, &QAction::triggered, qApp, []{
        if (txt == nullptr) return;
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
        auto rs = readAllText(getCurrentPath());
        if (rs.ok) {
            txt->setPlainText(rs.text);
            w->updateTitle();
        } else {
            QMessageBox::critical(nullptr, "Read failed", "Cannot read specific file!");
        }
    });


    auto *saveAsAct = new QAction("&Save as", w);
    MainWindow::connect(saveAsAct, &QAction::triggered, qApp, []{
        if (txt == nullptr) return;
        auto path = selectNewFile(nullptr);
        bool isOk = writeAllText(path, txt->toPlainText());
        if (!isOk) {
            QMessageBox::critical(nullptr, "Write failed", "Cannot write to specific file!");
        }
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


