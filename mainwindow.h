#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "text.h"
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
private:
    NextLineWay currentNextLine = Unknown;
    void closeEvent(QCloseEvent *event);
    bool isModified = false;
};
#endif // MAINWINDOW_H
