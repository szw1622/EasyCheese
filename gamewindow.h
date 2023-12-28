/**
 * @file gamewindow.h
 * @author Joshua Beatty, Zhuowen Song, Keming Chen, Matthew Whitaker
 * @brief Header file for gamewindow.cpp.
 * @version 0.1
 * @date 2022-12-8
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "constants.h"
#include "gamecanvas.h"

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit GameWindow(std::vector<std::vector<MapTile>> map, int levelNumber,
                        QMainWindow *parent = nullptr);
    ~GameWindow();

private:
    Ui::GameWindow *ui;
    int gameAreaX;
    int gameAreaY;
    int levelNumber;

    GameCanvas *canvas;

private slots:
    /**
   * @brief changeMap Change the map in the new level
   */
    void changeMap(std::vector<std::vector<MapTile>>);

    /**
   * @brief gameStartTest Test for the elements display
   */
    void gameStartTest();

    /**
   * @brief runningTest
   */
    void runningTest();

    /**
   * @brief showIdleRobot Show the robot is wait commands
   */
    void showIdleRobot(QPoint, int size);

    /**
   * @brief showCheese Show the cheese
   */
    void showCheese(QPoint, int size);

    /**
   * @brief showRobotMovie change the robot movie
   */
    void showRobotMovie(QMovie *);

    /**
   * @brief showEducationalMessage Shows an educational message to the user.
   */
    void showEducationalMessage();

    /**
   * @brief hideRobot hide the robot if lost the game
   */
    void lost();

    /**
   * @brief restart When the user creat a new program and try again
   */
    void restart();

    /**
   * @brief gameWon When the player won in one level
   */
    void gameWon();

    /**
   * @brief Those are events for buttons for switching the block to be placed
   */
    void ifButtonPushed();
    void notButtonPushed();
    void facingWallButtonPushed();
    void moveForwardButtonPushed();
    void turnLeftButtonPushed();
    void turnRightButtonPushed();
    void facingPitButtonPushed();
    void eatCheeseButtonPushed();
    void endWhileButtonPushed();
    void endIfButtonPushed();
    void whilePushed();
    void facingBlockButtonPushed();
    void facingCheeseButtonPushed();

    /**
   * @brief connectToggled Toggle the connection mode.
   * @param connecting
   */
    void connectToggled(bool connecting);

signals:
    /**
   * @brief viewReady Signals for ready to display the view.
   */
    void viewReady();

    /**
   * @brief changeType Change the current placing block type.
   * @param p
   */
    void changeType(ProgramBlock p);
};

#endif // GAMEWINDOW_H
