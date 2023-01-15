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

    bool event(QEvent *event) override
    {
        if (event->type() == QEvent::FileOpen)
        {
            QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
            emit openFile(openEvent->file());
        }
        return QApplication::event(event);;
    }

signals:
    void openFile(const QString & fileName);
};


#endif //QNOTEPAD_MYAPPLICATION_H
