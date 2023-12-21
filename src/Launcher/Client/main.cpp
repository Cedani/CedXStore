#include <iostream>
#include <QApplication>
#include "Login.hpp"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    lau::Login login;

    login.show();
    return app.exec();
}