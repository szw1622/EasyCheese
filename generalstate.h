#ifndef GENERALSTATE_H
#define GENERALSTATE_H

#include <QWidget>
#include "constants.h"
#include <QPushButton>
class GeneralState : public QWidget
{
    Q_OBJECT
public:
    QPushButton* button;

    explicit GeneralState(QWidget *parent = nullptr, ProgramBlock type = ProgramBlock::begin);
    bool event(QEvent* event);
    void mousePressHandler(QMouseEvent* event);
    void mouseClickHandler(QMouseEvent* event);
    void mouseReleaseHandler(QMouseEvent* event);
    void mouseMoveHandler(QMouseEvent* event);
private:
    bool moving;


signals:
    void mousePressed(QWidget* widget);

};

#endif // GENERALSTATE_H
