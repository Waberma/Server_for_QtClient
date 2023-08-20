#ifndef MYSERVER_H
#define MYSERVER_H

#include <QWidget>
#include "MenuInterface.h"
class QTcpServer;
class QTextEdit;
class QTcpSocket;

namespace Ui {
class MyServer;
}

class MyServer : public MenuWidgetInterface
{
    Q_OBJECT

public:
    MyServer();
    ~MyServer();
public slots:
    virtual void slotNewConnection();
    void slotReadClient ();
    //explicit MyServer(QWidget *parent = nullptr);
    //~MyServer();

private:
    Ui::MyServer *ui;
    QTcpServer* m_ptcpServer;
    //QTextEdit* m_ptxt;
    quint16 m_nNextBlockSize;
    private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);
    QVector<QTcpSocket*> sockets;


    // MenuWidgetInterface interface
public:
    QMenu *getMenu();
};

#endif // MYSERVER_H
