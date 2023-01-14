#include "QtWidgets/qmenubar.h"

#include "io.h"
#include "mainwindow.h"
#include "shared.h"
#include "ui_mainwindow.h"
#include "myapplication.h"

#include <QApplication>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QShortcut>
#include <QLabel>

static QPlainTextEdit* txt = nullptr;

void setMenu(MainWindow *w, bool native = true) {
    w->menuBar()->setNativeMenuBar(native);
#ifdef Q_OS_MAC
    if (!native) {
        w->menuBar()->setFixedHeight(20);
    }
#endif

    //region Edit

    QMenu* menuEdit = w->menuBar()->addMenu("&Edit");

    auto *undoAct = new QAction("&Undo", w);
    MainWindow::connect(undoAct, &QAction::triggered, qApp, []{ txt->undo(); });

    auto *redoAct = new QAction("&Redo", w);
    MainWindow::connect(redoAct, &QAction::triggered, qApp, []{ txt->redo(); });

    auto *clearAct = new QAction("&Clear", w);
    MainWindow::connect(clearAct, &QAction::triggered, qApp, []{
        auto t = txt->toPlainText(); // prevent clear empty text cause modify
        if (t.isEmpty()) return;
        txt->setPlainText("");
    });

    auto *toLFAct  = new QAction("&To LF", w);
    MainWindow::connect(toLFAct, &QAction::triggered, qApp, [w]{
        if (w->getNextLine() == LF) return;
        w->setNextLine(LF);
        w->setModify(true);
    });
    auto *toCRLFAct  = new QAction("&To CRLF", w);
    MainWindow::connect(toCRLFAct, &QAction::triggered, qApp, [w]{
        if (w->getNextLine() == CRLF) return;
        w->setNextLine(CRLF);
        w->setModify(true);
    });


    //endregion

    menuEdit->addAction(undoAct);
    menuEdit->addAction(redoAct);
    menuEdit->addAction(clearAct);
    menuEdit->addSeparator();
    menuEdit->addAction(toLFAct);
    menuEdit->addAction(toCRLFAct);

    //region File

    QMenu* menuFile = w->menuBar()->addMenu("&File");

    auto *newAct = new QAction("&New", w);
    MainWindow::connect(newAct, &QAction::triggered, qApp, [w]{
        if (!w->preclose()) return;
        clearCurrentPath();
        txt->setPlainText("");
        w->setModify(false);
    });

    auto *openAct = new QAction("&Open", w);
    MainWindow::connect(openAct, &QAction::triggered, qApp, [w]{
        auto path = selectFile(nullptr);
        SetSignal setStat = setCurrentPath(path);
        if (setStat == Invalid) return;

        if (setStat == Same) {
            auto rst = QMessageBox::information(nullptr, "Reload", "Reload file?", QMessageBox::Ok | QMessageBox::Cancel);
            if (rst == QMessageBox::Cancel) return;
        }
        if (w->loadFile()) w->updateTitle();

    });

    auto *saveAct = new QAction("&Save", w);
    MainWindow::connect(saveAct, &QAction::triggered, qApp, [w]{
        if (!hasPath()) {
            w->saveAs();
            return;
        }
        if (w->save()) return;
        QMessageBox::critical(nullptr, "Write failed", "Cannot write to specific file!");

    });

    auto *saveAsAct = new QAction("&Save as", w);
    MainWindow::connect(saveAsAct, &QAction::triggered, qApp, [w]{ w->saveAs(); });

    auto *reloadAct = new QAction("&Reload", w);
    MainWindow::connect(reloadAct, &QAction::triggered, qApp, [w]{ w->loadFile(); });

    auto *exitAct = new QAction("&Exit", w);
    MainWindow::connect(exitAct, &QAction::triggered, qApp, [w]{ w->close(); });
    //endregion

    auto *zoomInAct = new QAction("&Zoom In", w);
    MainWindow::connect(zoomInAct, &QAction::triggered, qApp, [w]{
        w->increaseZoom(5);
    });

    auto *zoomOutAct = new QAction("&Zoom Out", w);
    MainWindow::connect(zoomOutAct, &QAction::triggered, qApp, [w]{
        w->increaseZoom(-5);
    });


    auto m = getActionMap();
    m->insert("crlf", toCRLFAct);
    m->insert("lf", toLFAct);

    newAct->setShortcut(QKeySequence::New);
    openAct->setShortcut(QKeySequence::Open);
    saveAct->setShortcut(QKeySequence::Save);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    reloadAct->setShortcut(QKeySequence::Refresh);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    auto *shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Equal), w);
    MainWindow::connect(shortcut, &QShortcut::activated, qApp, [w]{
        w->increaseZoom(5);
    });
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    exitAct->setShortcut(QKeySequence::Close);

    menuFile->addAction(newAct);
    menuFile->addAction(openAct);
    menuFile->addAction(saveAct);
    menuFile->addAction(saveAsAct);
    menuFile->addAction(reloadAct);
    menuFile->addAction(zoomInAct);
    menuFile->addAction(zoomOutAct);
    menuFile->addAction(exitAct);

    w->menuBar()->addMenu(menuFile);
    w->menuBar()->addMenu(menuEdit);
}

void setStatusBar(MainWindow *w) {
    auto *statusBar = new QStatusBar(w);
    // statusBar->setStyleSheet("QStatusBar::item { border: 0px solid black };");
    w->setStatusBar(statusBar);

    auto *count = new QLabel("count: 0", w);
    getLabelMap()->insert("count", count);

    auto *sep = new QLabel("|", w);

    auto *crlf = new QLabel("Unknown", w);
    getLabelMap()->insert("crlf", crlf);


    auto *sep2 = new QLabel("|", w);

    auto *pos = new QLabel("Col: 0, Row: 0", w);
    getLabelMap()->insert("pos", pos);

    auto *path = new QLabel("", w);
    getLabelMap()->insert("path", path);


    statusBar->addWidget(path);

    statusBar->addPermanentWidget(count);
    statusBar->addPermanentWidget(sep);
    statusBar->addPermanentWidget(crlf);
    statusBar->addPermanentWidget(sep2);
    statusBar->addPermanentWidget(pos);
}

int main(int argc, char *argv[])
{
    MyApplication a(argc, argv);
    MainWindow w;
    setMenu(&w, w.getCfg()->nativeTitleBar);
    setStatusBar(&w);

    QObject::connect (&a, &MyApplication::openFile, &w, &MainWindow::onOpenFile);

    if (argc > 1) {
        setCurrentPath(QString(argv[1]));
        w.loadFile();
    }

    w.updateTitle();
    
    txt = w.ui->txtContent;

    w.show();
    return a.exec();
}


