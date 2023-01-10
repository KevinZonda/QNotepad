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
    bool isModified = false;
    void setNextLine(NextLineWay);
    NextLineWay getNextLine();
    bool isLoading = false;
private:
    NextLineWay currentNextLine = Unknown;
};
#endif // MAINWINDOW_H
