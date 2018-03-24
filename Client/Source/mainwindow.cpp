#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setLayout();
    setConnectionThread();
    connect(setConnection->tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::readMessages);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setLayout()
{
    setWindowTitle("For English Learners(Client)");

    mainWindowWidget = new QWidget(this);
    setCentralWidget(mainWindowWidget);

    mainWindowLayout = new QVBoxLayout;
    mainWindowWidget->setLayout(mainWindowLayout);

    contentPlace = new QTextEdit(mainWindowWidget);
    contentPlace->setMinimumSize(400, 300);
    contentPlace->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    contentPlace->setReadOnly(true);
    mainWindowLayout->addWidget(contentPlace);

    bottomBarLayout = new QHBoxLayout;
    mainWindowLayout->addLayout(bottomBarLayout);

    for(int i = 0; i < 3; i++) {
        bottomButtons[i] = new QPushButton(mainWindowWidget);
        bottomBarLayout->setSpacing(0);
        bottomBarLayout->addWidget(bottomButtons[i]);
        connect(bottomButtons[i], &QPushButton::clicked, [this, i](){disabledButtonNumber = i;});
        connect(bottomButtons[i], &QPushButton::clicked, this, &MainWindow::disableBottomButtons);
    }
    bottomButtons[0]->setText("Contents");
    bottomButtons[0]->setEnabled(false);
    disabledButtonNumber = 0;
    bottomButtons[1]->setText("Favorites");
    bottomButtons[2]->setText("About");
}

void MainWindow::setConnectionThread()
{
    connectionThread = new QThread;
    setConnection = new Connection();
    setConnection->moveToThread(connectionThread);
    connect(connectionThread, &QThread::started, setConnection, &Connection::connectToServer);
    connect(connectionThread, &QThread::finished, connectionThread, &QThread::deleteLater);
    connect(connectionThread, &QThread::finished, setConnection, &QObject::deleteLater);
    connectionThread->start();
    connect(setConnection, &Connection::failedToConnect, this, &MainWindow::showReconnectMsBox);
}

void MainWindow::disableBottomButtons()
{
    for(int i = 0; i < 3; i++) {
        bottomButtons[i]->setEnabled(true);
        if(i == disabledButtonNumber) {
            bottomButtons[i]->setEnabled(false);
        }
    }
}

void MainWindow::showReconnectMsBox()
{
    setMsBox.msBox.setWindowTitle("Failed to connect to server");
    connect(&setMsBox, &MessageBoxes::reconnect, setConnection, &Connection::connectToServer);
    setMsBox.showReconnectMsBox();
}

void MainWindow::readMessages()
{
    QDataStream readData(setConnection->tcpSocket);
    QString messagesFromServer;
    readData >> messagesFromServer;
    qDebug() << messagesFromServer;
}