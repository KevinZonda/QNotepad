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
#include <QQueue>
#include <QFileInfo>

static QPlainTextEdit* txt = nullptr;

void setMenu(MainWindow *w, bool native = true) {
    w->menuBar()->setNativeMenuBar(native);
#ifdef Q_OS_MAC
    if (!native) {
        w->menuBar()->setFixedHeight(20);
    }
#endif

    //region Edit
    auto q = getAction();

    QMenu* menuEdit = w->menuBar()->addMenu("&Edit");

    auto *undoAct = new QAction("&Undo", w);
    q->enqueue(undoAct);
    MainWindow::connect(undoAct, &QAction::triggered, qApp, []{ txt->undo(); });

    auto *copyAct = new QAction("&Copy", w);
    q->enqueue(copyAct);
    MainWindow::connect(copyAct, &QAction::triggered, qApp, []{ txt->copy(); });

    auto *pasteAct = new QAction("&Paste", w);
    q->enqueue(pasteAct);
    MainWindow::connect(pasteAct, &QAction::triggered, qApp, []{ txt->paste(); });

    auto *cutAct = new QAction("&Cut", w);
    q->enqueue(cutAct);
    MainWindow::connect(cutAct, &QAction::triggered, qApp, []{ txt->cut(); });


    auto *redoAct = new QAction("&Redo", w);
    q->enqueue(redoAct);
    MainWindow::connect(redoAct, &QAction::triggered, qApp, []{ txt->redo(); });

    auto *clearAct = new QAction("&Clear", w);
    q->enqueue(clearAct);
    MainWindow::connect(clearAct, &QAction::triggered, qApp, []{
        auto t = txt->toPlainText(); // prevent clear empty text cause modify
        if (t.isEmpty()) return;
        txt->setPlainText("");
    });

    auto *toLFAct  = new QAction("&To LF", w);
    q->enqueue(toLFAct);
    MainWindow::connect(toLFAct, &QAction::triggered, qApp, [w]{
        if (w->getNextLine() == LF) return;
        w->setNextLine(LF);
        w->setModify(true);
    });
    auto *toCRLFAct  = new QAction("&To CRLF", w);
    q->enqueue(toCRLFAct);
    MainWindow::connect(toCRLFAct, &QAction::triggered, qApp, [w]{
        if (w->getNextLine() == CRLF) return;
        w->setNextLine(CRLF);
        w->setModify(true);
    });


    //endregion

    //region File

    QMenu* menuFile = w->menuBar()->addMenu("&File");

    auto *newAct = new QAction("&New", w);
    q->enqueue(newAct);
    MainWindow::connect(newAct, &QAction::triggered, qApp, [w]{
        if (!w->preclose()) return;
        clearCurrentPath();
        txt->setPlainText("");
        w->setModify(false);
        getLabelMap()->value("size")->setText("");
        getLabelMap()->value("sizeSep")->setVisible(false);
    });

    auto *openAct = new QAction("&Open", w);
    q->enqueue(openAct);
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
    q->enqueue(saveAct);
    MainWindow::connect(saveAct, &QAction::triggered, qApp, [w]{
        if (!hasPath()) {
            w->saveAs();
            return;
        }
        if (w->save()) return;
        QMessageBox::critical(nullptr, "Write failed", "Cannot write to specific file!");

    });

    auto *saveAsAct = new QAction("&Save as", w);
    q->enqueue(saveAsAct);
    MainWindow::connect(saveAsAct, &QAction::triggered, qApp, [w]{ w->saveAs(); });

    auto *reloadAct = new QAction("&Reload", w);
    q->enqueue(reloadAct);
    MainWindow::connect(reloadAct, &QAction::triggered, qApp, [w]{ w->loadFile(); });

    auto *exitAct = new QAction("&Exit", w);
    q->enqueue(exitAct);
    MainWindow::connect(exitAct, &QAction::triggered, qApp, [w]{ w->close(); });
    //endregion

    auto *zoomInAct = new QAction("&Zoom In", w);
    q->enqueue(zoomInAct);
    MainWindow::connect(zoomInAct, &QAction::triggered, qApp, [w]{
        w->increaseZoom(true);
    });

    auto *zoomOutAct = new QAction("&Zoom Out", w);
    q->enqueue(zoomOutAct);
    MainWindow::connect(zoomOutAct, &QAction::triggered, qApp, [w]{
        w->increaseZoom(false);
    });


    QMenu* menuHelp = w->menuBar()->addMenu("&Help");

    auto *aboutAct = new QAction("&About", w);
    q->enqueue(aboutAct);
    MainWindow::connect(aboutAct, &QAction::triggered, qApp, [w]{
        QMessageBox::about(w, "About", w->getAboutText());
    });

    auto *editConfigAct = new QAction("&Edit Config", w);
    q->enqueue(editConfigAct);
    MainWindow::connect(editConfigAct, &QAction::triggered, qApp, [w]{
        if (!w->preclose()) return;
        setCurrentPath(getConfigFile("config.json"));
        w->loadFile();
    });

    auto *reloadCfgAct = new QAction("&Reload Config", w);
    q->enqueue(reloadCfgAct);
    MainWindow::connect(reloadCfgAct, &QAction::triggered, qApp, [w]{
        if (getCurrentPath() == getConfigFile("config.json") && w->getModify()) {
            w->preclose();
        }
        w->loadConfig();
    });


    QMenu* menuView = w->menuBar()->addMenu("&View");

    auto m = getActionMap();
    m->insert("crlf", toCRLFAct);
    m->insert("lf", toLFAct);
    m->insert("undo", undoAct);
    undoAct->setEnabled(false);
    m->insert("redo", redoAct);
    redoAct->setEnabled(false);

    newAct->setShortcut(QKeySequence::New);
    openAct->setShortcut(QKeySequence::Open);
    saveAct->setShortcut(QKeySequence::Save);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    reloadAct->setShortcut(QKeySequence::Refresh);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);

    auto *shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Equal), w);
    MainWindow::connect(shortcut, &QShortcut::activated, qApp, [w]{
        w->increaseZoom(true);
    });
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    exitAct->setShortcut(QKeySequence::Close);
    copyAct->setShortcut(QKeySequence::Copy);
    cutAct->setShortcut(QKeySequence::Cut);
    pasteAct->setShortcut(QKeySequence::Paste);
    undoAct->setShortcut(QKeySequence::Undo);
    redoAct->setShortcut(QKeySequence::Redo);

    menuFile->addAction(newAct);
    menuFile->addAction(openAct);
    menuFile->addSeparator();
    menuFile->addAction(saveAct);
    menuFile->addAction(saveAsAct);
    menuFile->addSeparator();
    menuFile->addAction(reloadAct);
    menuFile->addAction(exitAct);

    menuEdit->addAction(copyAct);
    menuEdit->addAction(cutAct);
    menuEdit->addAction(pasteAct);
    menuEdit->addSeparator();
    menuEdit->addAction(undoAct);
    menuEdit->addAction(redoAct);
    menuEdit->addAction(clearAct);
    menuEdit->addSeparator();
    menuEdit->addAction(toLFAct);
    menuEdit->addAction(toCRLFAct);

    menuView->addAction(zoomInAct);
    menuView->addAction(zoomOutAct);

    menuHelp->addAction(editConfigAct);
    menuHelp->addAction(reloadCfgAct);
    menuHelp->addAction(aboutAct);

    w->menuBar()->addMenu(menuFile);
    w->menuBar()->addMenu(menuEdit);
    w->menuBar()->addMenu(menuView);
    w->menuBar()->addMenu(menuHelp);
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

    auto *pos = new QLabel("Ln: 1, Col: 1", w);
    getLabelMap()->insert("pos", pos);
    auto *sizeSep = new QLabel("|", w);
    getLabelMap()->insert("sizeSep", sizeSep);
    sizeSep->setVisible(false);

    auto *size = new QLabel("", w);
    getLabelMap()->insert("size", size);

    auto *path = new QLabel("", w);
    getLabelMap()->insert("path", path);


    statusBar->addWidget(path);

    statusBar->addPermanentWidget(size);
    statusBar->addPermanentWidget(sizeSep);
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
        QFileInfo fi(argv[1]);
        setCurrentPath(fi.absoluteFilePath());
        w.loadFile();
    }

    w.updateTitle();

    txt = w.ui->txtContent;

    w.show();
    return a.exec();
}


