#ifndef QNOTEPAD_MYAPPLICATION_H
#define QNOTEPAD_MYAPPLICATION_H

#include <QApplication>
#include <QFileOpenEvent>

class MyApplication : public QApplication
{
    Q_OBJECT
public:
    MyApplication(int &argc, char **argv)
            : QApplication(argc, argv)
    {
    }

signals:
    void openFile( const QString & fileName );
};


#endif //QNOTEPAD_MYAPPLICATION_H
