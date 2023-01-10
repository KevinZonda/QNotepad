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
        msg = "Unknown";
        break;
    }

    this->ui->statusbar->showMessage(msg);

}

NextLineWay MainWindow::getNextLine() {
    return this->currentNextLine;
}

bool MainWindow::loadFile() {
    this->isLoading = true;
    auto rs = readAllText(getCurrentPath());
    if (rs.ok) {
        auto x = getCrLf(rs.text);
        this->setNextLine(x);
        ui->txtContent->setPlainText(rs.text);
        this->isLoading = false;
        this->isModified = false;
        return true;
    }
    this->isLoading = false;
    QMessageBox::critical(nullptr, "Read failed", "Cannot read specific file!");
    return false;
}

bool MainWindow::save(QString path) {
    bool isOk = writeAllText(path, ui->txtContent->toPlainText());
    if (isOk) {
        this->isModified = false;
        this->updateTitle();
    }
    auto cursor = this->ui->txtContent->textCursor();
    this->loadFile();
    this->ui->txtContent->setTextCursor(cursor);
    return isOk;
}

bool MainWindow::save() {
    return this->save(getCurrentPath());
}


void MainWindow::closeEvent(QCloseEvent *event) {
    if (!this->isModified) return;
    if (!hasPath() && this->ui->txtContent->toPlainText().trimmed().isEmpty()) return;

    auto rst = QMessageBox::information(this,
                                        "Change Not Saved",
                                        "Change not saved. Save?",
                                        QMessageBox::Cancel | QMessageBox::Save | QMessageBox::No,
                                        QMessageBox::Cancel);
    if (rst == QMessageBox::Cancel) {event->ignore(); return;}
    if (rst == QMessageBox::No) {event->accept(); return;}
    if (hasPath()) {
        this->save();
        event->accept();
        return;
    }

    auto path = selectNewFile(nullptr);
    if (path.isEmpty()) {
        QMessageBox::critical(nullptr, "Cancelled", "User cancelled proecess. Not saved!");
        event->ignore();
        return;
    }

    bool isOk = this->save(path);
    if (isOk) {
        event->accept();
        return;
    }

    QMessageBox::critical(nullptr, "Write failed", "Cannot write to specific file!");
    event->ignore();
}
