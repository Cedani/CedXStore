#include "Login.hpp"
#include <QBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QTcpSocket>
#include <iostream>
#include <fstream>
#include <QApplication>
#include <QTextCodec>
#include "config.h"

lau::Login::Login(QWidget *parent): QWidget(parent), _signUp(false)
{
    setFixedSize(275, 200);
    init();
    connectHandlers();
    initNetwork();
}

void lau::Login::init()
{
    // Label With Login
    _mainLayout = new QVBoxLayout(this);
    _titleWindowLabel = new QLabel(tr("Login"));
    _mainLayout->addWidget(_titleWindowLabel, 0, Qt::AlignHCenter);

    // Initializing form
    _loginFormLayout = new QFormLayout();
    _mainLayout->addLayout(_loginFormLayout);

    _pseudoLineEdit = new QLineEdit();
    _pseudoLineEdit->setPlaceholderText(tr("Enter your pseudo"));
    _loginFormLayout->addRow(tr("&Pseudo"), _pseudoLineEdit);

    _passwordLineEdit = new QLineEdit();
    _passwordLineEdit->setEchoMode(QLineEdit::Password);
    _passwordLineEdit->setPlaceholderText(tr("Enter your password"));
    _loginFormLayout->addRow(tr("&Password"), _passwordLineEdit);

    // Switch between Login and Sign up
    _switch = new QPushButton();
    _switch->setFlat(true);
    _switch->setText(tr("Do not have an account? Click here"));
    _mainLayout->addWidget(_switch, 0, Qt::AlignHCenter);

    _sendButton = new QPushButton();
    _sendButton->setText(tr("Login"));
    _mainLayout->addWidget(_sendButton, 0, Qt::AlignHCenter);

    // creating socket
    _socket = new QTcpSocket(this);
}

void lau::Login::connectHandlers()
{
    connect(_switch, &QPushButton::clicked, this, &Login::switchLoginSignup);
    connect(_socket, &QTcpSocket::errorOccurred, this, &Login::onErrorConnection);
    connect(_socket, &QTcpSocket::readyRead, this, &Login::onReadyRead);
}

void lau::Login::switchLoginSignup()
{
    _signUp = !_signUp;

    if (_signUp) {
        _confirmPasswordLineEdit = new QLineEdit();
        _confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);
        _confirmPasswordLineEdit->setPlaceholderText(tr("Confirm your password"));
        _loginFormLayout->addRow(tr("&Confirm"), _confirmPasswordLineEdit);
        _switch->setText(tr("Already have an account? Click here"));
        _titleWindowLabel->setText(tr("Signup"));
        _sendButton->setText("Signup");
    } else {
        _loginFormLayout->removeRow(_confirmPasswordLineEdit);
        _switch->setText(tr("Do not have an account? Click here"));
        _titleWindowLabel->setText(tr("Login"));
        _sendButton->setText("Login");
    }
}

void lau::Login::onErrorConnection()
{
    int ret = QMessageBox::critical(this, tr("Connection failed"),
             tr("Could not connect with the server.\nYou can try to connect to the server again by clicking on retry, or close the app and try later"),
             QMessageBox::Retry | QMessageBox::Cancel);
    
    switch (ret) {
        case QMessageBox::Retry:
            initNetwork();
            break;
        case QMessageBox::Cancel:
            QApplication::exit(84);
    }
}

void lau::Login::initNetwork()
{
    std::ifstream endPointFile(TEMP_ENDPOINT);

    if (!endPointFile.is_open())
        exit(84);
    std::string port;
    std::string hostName;
    std::getline(endPointFile, hostName);
    std::getline(endPointFile, port);
    
    _socket->connectToHost(hostName.c_str(), std::stoi(port));
}

void lau::Login::onReadyRead()
{
    QByteArray Data = _socket->readAll();

    std::cout << QTextCodec::codecForMib(106)->toUnicode(Data).toStdString() << std::endl;
}