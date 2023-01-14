#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "io.h"
#include "shared.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUnifiedTitleAndToolBarOnMac(true);


    QObject::connect(ui->txtContent, &QPlainTextEdit::modificationChanged, this,
                [this](bool m) {
                    this->setModify(m);
                }
    );
    loadConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (cfg != nullptr) delete cfg;
}


void MainWindow::updateTitle(bool isModifyCalled) {
    if (isModifyCalled) {
        auto title = this->windowTitle();
        if (this->isModified && title.startsWith("*"))
            return;
        this->setWindowTitle("*" + title);
    }
    QString title = "QNotepad - " + getTitle();
    title = this->isModified ? "*" + title : title;
    this->setWindowTitle(title);
}

void MainWindow::setNextLine(NextLineWay x) {
    this->currentNextLine = x;
    auto m = getActionMap();
    auto lf = m->value("lf");
    auto crlf = m->value("crlf");
    lf->setDisabled(false);
    crlf->setDisabled(false);
    if (x == LF) {
        lf->setDisabled(true);
    }
    else if (x == CRLF) {
        crlf->setDisabled(true);
    }
    this->ui->statusbar->showMessage(nextLineWayString(x));

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
    if (!hasPath() && this->ui->txtContent->toPlainText().trimmed().isEmpty())
        return true;

    auto rst = QMessageBox::information(this,
                                        "Change Not Saved",
                                        "Change not saved. Save?",
                                        QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel,
                                        QMessageBox::Cancel);
    if (rst == QMessageBox::Cancel) return false;
    if (rst == QMessageBox::No)     return true;
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
    else                  event->ignore();
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

void MainWindow::setFont(QString font, int size) {
    QFont f;
    if (font.isEmpty()) {
        f = ui->txtContent->font();
    } else {
        f.setFamily(font);
    }

    if (size > 0) f.setPointSize(size);
    ui->txtContent->setFont(f);
}

void MainWindow::setFont(QStringList & families, int size) {
    QFont f;
    if (families.isEmpty()) {
        f = ui->txtContent->font();
    } else {
        f.setFamilies(families);
    }

    if (size > 0) f.setPointSize(size);
    ui->txtContent->setFont(f);
}

void MainWindow::onOpenFile(const QString & fileName) {
    setCurrentPath(QString(fileName));
    loadFile();
}

config* MainWindow::getCfg() {
    if (cfg == nullptr) loadConfig();
    return cfg;
}

QString getConfigPath() {
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}

QString getConfigFile(const QString filename)
{
    return QDir::cleanPath(getConfigPath() + QDir::separator() + filename);
}

void MainWindow::swapCfg(config *newCfg) {
    config* c = cfg;
    cfg = newCfg;
    if (c != nullptr) delete c;
}

void MainWindow::loadConfig() {
    auto path = getConfigFile("config.json");
    qDebug() << path;
    if (!QFile::exists(path)) {
        swapCfg(new config());
        return;
    }

    auto x = readAllText(path);
    if (!x.ok) {
        QMessageBox::critical(nullptr, "Read config failed", "Cannot read config file! Use default config.");
        swapCfg(new config());
        return;
    }

    swapCfg(new config(x.text));
    setFont(*cfg->font, cfg->fontSize);
    if (cfg->lineWrap && cfg->wordWrap) {
        ui->txtContent->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    }
}