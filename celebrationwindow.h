/**
 * @file celebrationwindow.h
 * @author Matthew Whitaker
 * @brief Header file for celebrationwindow.cpp
 * @version 1.0
 * @date 2022-12-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CELEBRATIONWINDOW_H
#define CELEBRATIONWINDOW_H

#include <QMainWindow>
#include <Box2D/Box2D.h>
#include <QTimer>
#include <QPainter>
#include <QPaintDevice>

const float WORLD_WIDTH = 10.0f;
const float WORLD_HEIGHT = 7.0f;

const float WALL_THICKNESS = 0.001f;

const int NUM_CONFETTI = 300;
const float CONFETTI_RADIUS = 0.06f;

namespace Ui {
class CelebrationWindow;
}

class CelebrationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CelebrationWindow(int nextLevelIndex, QWidget *parent = nullptr);
    ~CelebrationWindow();

public slots:
    // Called when the physics engine should progress to the next state.
    void updatePhysics();

private:
    Ui::CelebrationWindow *ui;
    b2World physicsWorld;
    std::array<b2Body* , NUM_CONFETTI> confettiBodies;
    QTimer timer;
    QPainter painter;
    int nextLevelIndex;
private slots:
    // Closes this window and shows the main menu window
    void showMainMenu();

    // Closes this window and opens up a new game window with the next level!
    void nextLevel();

signals:

};

/// A widget with a custom canvas for drawing some celebratory confetti!
class CelebrationWidget : public QWidget {
    Q_OBJECT

public:
    explicit CelebrationWidget(QWidget *parent = nullptr);

    // Passes in the physics bodies so that the canvas can draw them on the screen
    void setBodies(std::array<b2Body*, NUM_CONFETTI>* bodiesReference);

private:
    std::array<b2Body*, NUM_CONFETTI>* confettiBodiesReference;
    std::array<QColor, NUM_CONFETTI> confettiColors;

    // Called when the Canvas needs to repaint
    void paintEvent(QPaintEvent *);
};

#endif // CELEBRATIONWINDOW_H
