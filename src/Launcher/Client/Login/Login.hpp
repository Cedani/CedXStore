#pragma once
#include <QWidget>

class QPushButton;
class QFormLayout;
class QLineEdit;
class QBoxLayout;
class QLabel;
class QTcpSocket;
namespace lau {
    class ClickableLabel;
    class Login : public QWidget{
        Q_OBJECT
        public:
            explicit Login(QWidget *parent = Q_NULLPTR);

        signals:

        public slots:
            void switchLoginSignup();
            void onErrorConnection();
            void onReadyRead();

        private:
            void init();
            void initNetwork();
            void connectHandlers();

            // form
            QBoxLayout *_mainLayout;
            QLabel *_titleWindowLabel;


            QFormLayout *_loginFormLayout;

            QLineEdit *_pseudoLineEdit;
            QLineEdit *_passwordLineEdit;
            QLineEdit *_confirmPasswordLineEdit;

            QPushButton *_switch;


            bool _signUp;

            QPushButton *_sendButton;

        private:
            void handleMessage(QByteArray);
            //Network
            QTcpSocket *_socket;

            

    };
}