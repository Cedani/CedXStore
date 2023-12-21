#include "ClickableLabel.hpp"
#include <iostream>

lau::ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f): QLabel(parent)
{
}

void lau::ClickableLabel::mouseClickEvent(QMouseEvent *)
{
    std::cout << "Clicked" << std::endl;
    emit clicked();
}