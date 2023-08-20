#include "myserver.h"
#include "ui_myserver.h"
#include <QMessageBox>
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>
MyServer::MyServer() : m_nNextBlockSize(0),
    ui(new Ui::MyServer)
{
    ui->setupUi(this);
    m_ptcpServer = new QTcpServer(this);
    if(!m_ptcpServer->listen(QHostAddress::Any, 2323)){
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:" + m_ptcpServer->errorString());
        m_ptcpServer->close();
        return;
    }
    //connect(m_ptcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection));
    connect(m_ptcpServer, &QTcpServer::newConnection, this, &MyServer::slotNewConnection);
    ui->textEdit->setReadOnly(true);
}

MyServer::~MyServer()
{
    m_ptcpServer->close();
    m_ptcpServer->deleteLater();
}
//среда в 9-10
void MyServer::slotNewConnection()
{
    QTcpSocket *pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, &QTcpSocket::disconnected, pClientSocket, &QTcpSocket::deleteLater);
    connect(pClientSocket, &QTcpSocket::readyRead, this, &MyServer::slotReadClient);
    sendToClient(pClientSocket, "Server Respone: Connected");
}

void MyServer::slotReadClient()
{
    QTcpSocket *ClientSoc = (QTcpSocket*)sender();
    QDataStream in(ClientSoc);
    in.setVersion(QDataStream::Qt_5_3);
    for(;;)
    {
        if(!m_nNextBlockSize)
        {
            if(ClientSoc->bytesAvailable() < sizeof(quint16))
                break;
            in >> m_nNextBlockSize;
        }
        if(ClientSoc->bytesAvailable() < m_nNextBlockSize)
            break;
        QTime time;
        QString str;
        in >> time >> str;
        QString strMessage = time.toString() + " " + "Client has send - " + str;
        ui->textEdit->append(strMessage);

        m_nNextBlockSize = 0;
        sendToClient(ClientSoc, "Server Respone: Received \"" + str + "\"");
    }
}

void MyServer::sendToClient(QTcpSocket *pSocket, const QString &str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}

QMenu *MyServer::getMenu()
{
    return NULL;
}
