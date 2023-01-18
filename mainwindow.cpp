#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "io.h"
#include "shared.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QQueue>

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

    QObject::connect(ui->txtContent, &QPlainTextEdit::textChanged, this,
                     [this]() {
                         getLabelMap()->value("count")->setText("count: " + QString::number(ui->txtContent->toPlainText().length()));
                     }
    );

    QObject::connect(ui->txtContent, &QPlainTextEdit::cursorPositionChanged, this,
                     [this]() {
                         auto c = ui->txtContent->textCursor();
                         getLabelMap()->value("pos")->setText(
                                 "Ln: " + QString::number(c.blockNumber() + 1) + ", Col: " + QString::number(c.columnNumber() + 1));
                     }
    );
    loadConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cfg;
    auto q = getAction();
    while (!q->isEmpty()) {
        delete q->dequeue();
    }
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
    getLabelMap()->value("crlf")->setText(nextLineWayString(x));
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
        getLabelMap()->value("size")->setText(rs.size);
        getLabelMap()->value("sizeSep")->setVisible(true);
        return true;
    }
    if (!rs.exists) {
        auto rst = QMessageBox::information(nullptr, "New file", "File not found. Create new one?",
                                 QMessageBox::Ok | QMessageBox::Discard, QMessageBox::Discard);
        if (rst == QMessageBox::Discard) return false;
        ui->txtContent->clear();
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
                                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                        QMessageBox::Cancel);
    if (rst == QMessageBox::Cancel)  return false;
    if (rst == QMessageBox::Discard) return true;
    if (hasPath()) {
        this->save();
        return true;
    }

    auto path = selectNewFile(nullptr);
    if (path.isEmpty()) {
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

void MainWindow::increaseZoom(int v) {
    auto f = ui->txtContent->font();
    auto s = f.pointSize();
    s += v;
    if (s < 1) s = 1;
    f.setPointSize(s);
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

void MainWindow::swapCfg(config *newCfg) {
    config* c = cfg;
    cfg = newCfg;
    delete c;
}

void MainWindow::loadConfig() {
    auto path = getConfigFile("config.json");
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
    ui->txtContent->setLineWrapMode(
            cfg->lineWrap
                ? QPlainTextEdit::WidgetWidth
                : QPlainTextEdit::NoWrap);

    if (cfg->lineWrap && cfg->wordWrap) {
        ui->txtContent->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    }
}

void MainWindow::saveAs() {
    auto path = selectNewFile(nullptr);
    if (path.isEmpty()) return;
    bool isOk = save(path);
    if (!isOk) {
        QMessageBox::critical(nullptr, "Write failed", "Cannot write to specific file!");
        return;
    }
    setCurrentPath(path);
}

QString MainWindow::getAboutText() {
    // config path
    auto configPath = getConfigFile("config.json");
    QString about = "QNotepad\n";
    about += "by KevinZonda\nhttps://github.com/KevinZonda/QNotepad\n";
    about += "Config Path: " + configPath + "\n";
    about += "Config Load: ";
    about += cfg->isConstructWithParameter ? "Yes": "No";
    return about;
}
