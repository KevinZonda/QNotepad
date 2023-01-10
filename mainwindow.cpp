#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "io.h"
#include "shared.h"
#include <QMessageBox>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->txtContent, &QPlainTextEdit::modificationChanged, this,
                [this](bool m) {
                    this->setModify(m);
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
    QFileInfo info(getCurrentPath());

    QString title = hasPath()
            ? "QNotepad - " + info.fileName()
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
        msg = "Unknown (LF)";
        break;
    }

    this->ui->statusbar->showMessage(msg);

}

NextLineWay MainWindow::getNextLine() {
    return this->currentNextLine;
}

bool MainWindow::loadFile() {
    if (!hasPath()) return false;
    auto rs = readAllText(getCurrentPath());
    if (rs.ok) {
        ui->txtContent->setPlainText(rs.text);
        auto x = getCrLf(rs.text);
        this->setNextLine(x);
        return true;
    }
    QMessageBox::critical(nullptr, "Read failed", "Cannot read specific file!");
    return false;
}

bool MainWindow::save(QString path) {
    auto t = ui->txtContent->toPlainText();
    t = this->getNextLine() == CRLF ? toCrLf(t) : toLf(t);
    bool isOk = writeAllText(path, t);
    if (!isOk) return isOk;
    this->setModify(false);
    auto cursor = this->ui->txtContent->textCursor();
    this->loadFile();
    this->ui->txtContent->setTextCursor(cursor);
    return isOk;
}

bool MainWindow::save() {
    return this->save(getCurrentPath());
}

// True: process ok, go close
// False: process failed, block close
bool MainWindow::preclose() {
    if (!this->isModified) return true;
    if (!hasPath() && this->ui->txtContent->toPlainText().trimmed().isEmpty()) return true;

    auto rst = QMessageBox::information(this,
                                        "Change Not Saved",
                                        "Change not saved. Save?",
                                        QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel,
                                        QMessageBox::Cancel);
    if (rst == QMessageBox::Cancel) return false;
    if (rst == QMessageBox::No) return true;
    if (hasPath()) {
        this->save();
        return true;
    }

    auto path = selectNewFile(nullptr);
    if (path.isEmpty()) {
        QMessageBox::critical(nullptr, "Cancelled", "User cancelled proecess. Not saved!");
        return false;
    }

    bool isOk = this->save(path);
    if (isOk) return true;

    QMessageBox::critical(nullptr, "Write failed", "Cannot write to specific file!");
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (this->preclose()) event->accept();
    else event->ignore();
}

void MainWindow::setModify(bool modifyState, bool updateTitle) {
    if (this->isModified == modifyState) return;
    this->isModified = modifyState;
    if (!updateTitle) return;
    if (this->isModified) this->updateTitle(true);
    else this->updateTitle(false);
}

bool MainWindow::getModify() {
    return this->isModified;
}
