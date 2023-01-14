#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "text.h"
#include "config.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;
    void updateTitle(bool isModifyCalled = false);
    void setNextLine(NextLineWay);
    NextLineWay getNextLine();
    void setModify(bool modifyState, bool updateTitle = true);
    bool getModify();
    bool loadFile();
    bool save(QString path);
    bool save();
    bool preclose();
    void setFont(QStringList & families, int fontSize = -1);
    void setFont(QString font = "", int fontSize = -1);
    config* getCfg();
    void loadConfig();
    void increaseZoom(int v);
    void saveAs();
public slots:
    void onOpenFile(const QString & fileName);
private:
    NextLineWay currentNextLine = Unknown;
    void closeEvent(QCloseEvent *event) override;
    bool isModified = false;
    config* cfg = nullptr;
    void swapCfg(config* newCfg);
};
#endif // MAINWINDOW_H
