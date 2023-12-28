/**
 * @file mainwindow.h
 * @author Joshua Beatty, Zhuowen Song, Keming Chen, Matthew Whitaker
 * @brief Header file for mainwindow.cpp.
 * @version 0.1
 * @date 2022-11-15
 * @reviewer Joshua Beatty
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef GAMECANVAS_H
#define GAMECANVAS_H

#include "simulation.h"
#include "constants.h"
#include "qmovie.h"
#include <QWidget>
#include <QPainter>


class GameCanvas : public QWidget {
    Q_OBJECT
    QPainter painter;

public:
    explicit GameCanvas(QWidget* parent, std::vector<std::vector<MapTile>> map);

private:
    // a simulation to control the robot moving
    Simulation *s;

    // varibles for drawing the map
    QPixmap scaledWallMap;
    QPixmap scaledBlockMap;
    QPixmap scaledPitMap;
    QColor groundColor;

    // record the robot position
    int robotX;
    int robotY;
    int robotSize;
    int interval;
    direction preDir;

    // robot movies
    QTimer * timer;
    QMovie* rightWaiting;
    QMovie* leftWaiting;
    QMovie* upWaiting;
    QMovie* downWaiting;
    QMovie* rightRunning;
    QMovie* leftRunning;

    // cheese icon
    QPixmap scaledCheeseMap;

    // current map
    std::vector<std::vector<MapTile>> map;
    std::vector<std::vector<MapTile>> resetMap;

    // size of map blocks
    int brickSize;

public slots:
    /**
     * @brief setMap Draw the map on the canvas
     * @param map
     */
    void setMap(std::vector<std::vector<MapTile>> map);
    /**
     * @brief simulate Create a simulation by using the user's program and current map
     * @param program
     */
    void simulate(std::vector<ProgramBlock> program);
    /**
     * @brief step Get next step from simulation
     */
    void step();
    /**
     * @brief run Start the timer for run the progame
     * @param interval Interval of the timer
     */
    void run(int interval);
    /**
     * @brief setInterval Set the interval for the run program timer
     * @param interval
     */
    void setInterval(int interval);
    /**
     * @brief stop Stop the timer
     */
    void stop();
    /**
     * @brief emitRunningBlock Send the current program block signal
     * @param block
     */
    void emitRunningBlock(int block);

protected:
    /**
     * @brief paintEvent Paint the whole map on the canvas
     * @param event
     */
    void paintEvent(QPaintEvent *event) override;

signals:
    /**
     * @brief robotMovie Send the robot movie to the game window
     */
    void robotMovie(QMovie*);

    /**
     * @brief showRobot Send the position and the size of the robot to the game window
     */
    void showRobot(QPoint, int);

    /**
     * @brief showCheese Send the position and the size of the cheese to the game window
     */
    void showCheese(QPoint, int);

    /**
     * @brief gameWon Send the signal to game window if the user won the game
     */
    void gameWon();

    /**
     * @brief restartGame Send the signal to the simulation if restart the game
     */
    void restartGame();

    /**
     * @brief currentBlock Send the current program block to the simulation
     */
    void currentBlock(int);
};

#endif // GAMECANVAS_H
