#include "generalstate.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
GeneralState::GeneralState(QWidget *parent, ProgramBlock type)
    : QWidget{parent} {
    button = new QPushButton("Start");
    button->setParent(this);
    this->setGeometry(QRect(200, 200, 200, 200));
    button->setGeometry(QRect(200, 200, 200, 200));

}

