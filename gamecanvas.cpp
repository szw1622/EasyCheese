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

#include "gamecanvas.h"
#include "constants.h"
#include "qtimer.h"
#include "simulation.h"
#include <QMovie>
#include <QPainter>

GameCanvas::GameCanvas(QWidget *parent, std::vector<std::vector<MapTile>> map)
    : QWidget{parent}, map(map) {
    this->setMinimumSize(QSize(1000, 2000));

    // Store the initial map and set the map if restart the game
    resetMap = map;

    // Varibles for drawing the map
    brickSize = 100;
    interval = 1000;
    robotSize = 90;
    preDir = east;

    // Set pixel art icons for the map elements
    QPixmap wallMap(":/elements/brickwall.jpg");
    scaledWallMap = wallMap.scaled(brickSize, brickSize, Qt::KeepAspectRatio);
    QPixmap blockMap(":/elements/block.png");
    scaledBlockMap = blockMap.scaled(brickSize, brickSize, Qt::KeepAspectRatio);
    QPixmap pitMap(":/elements/pit.png");
    scaledPitMap = pitMap.scaled(brickSize, brickSize, Qt::KeepAspectRatio);

    // Set robot movies
    rightWaiting = new QMovie(":/elements/robot-right.gif");
    rightWaiting->setScaledSize(QSize(robotSize, robotSize));
    leftWaiting = new QMovie(":/elements/robot-left.gif");
    leftWaiting->setScaledSize(QSize(robotSize, robotSize));
    upWaiting = new QMovie(":/elements/robot-up.gif");
    upWaiting->setScaledSize(QSize(robotSize, robotSize));
    downWaiting = new QMovie(":/elements/robot-down.gif");
    downWaiting->setScaledSize(QSize(robotSize, robotSize));
    rightRunning = new QMovie(":/elements/robot-run-right.gif");
    rightRunning->setScaledSize(QSize(robotSize, robotSize));
    leftRunning = new QMovie(":/elements/robot-run-right.gif");
    leftRunning->setScaledSize(QSize(robotSize, robotSize));

    // Set cheese icon
    QPixmap cheeseMap(":/elements/cheese.png");
    scaledCheeseMap = cheeseMap.scaled(robotSize, robotSize, Qt::KeepAspectRatio);

    // Set the color of ground
    groundColor = QColor::fromRgb(222, 184, 135);

    // Timer for running the program
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &GameCanvas::step);
}

void GameCanvas::paintEvent(QPaintEvent * /* event */) {
    QPainter painter(this);

    // Got through the map vector
    for (unsigned long long y = 0; y < map.size(); y++) {
        for (unsigned long long x = 0; x < map[y].size(); x++) {
            // Draw the robot
            if (map[y][x] == start) {
                painter.fillRect(x * brickSize, y * brickSize, brickSize, brickSize,
                                 groundColor);
                QPoint pos;
                pos.setX(x * brickSize);
                pos.setY(y * brickSize);
                emit robotMovie(rightWaiting);
                emit showRobot(pos, robotSize);
            }
            // Draw the cheese
            if (map[y][x] == cheese) {
                painter.fillRect(x * brickSize, y * brickSize, brickSize, brickSize,
                                 groundColor);
                QPoint pos;
                pos.setX(x * brickSize);
                pos.setY(y * brickSize);
                emit showCheese(pos, robotSize);
            }
            // Draw the gournd
            if (map[y][x] == ground) {
                painter.fillRect(x * brickSize, y * brickSize, brickSize, brickSize,
                                 groundColor);
            }
            // Draw the pit
            if (map[y][x] == pit) {
                painter.fillRect(x * brickSize, y * brickSize, brickSize, brickSize,
                                 scaledPitMap);
            }
            // Draw the wall
            if (map[y][x] == wall) {
                painter.fillRect(x * brickSize, y * brickSize, brickSize, brickSize,
                                 scaledWallMap);
            }
            // Draw the block
            if (map[y][x] == block) {
                painter.fillRect(x * brickSize, y * brickSize, brickSize, brickSize,
                                 scaledBlockMap);
            }
        }
    }
}

void GameCanvas::setMap(std::vector<std::vector<MapTile>> map) {
    this->map = map;
    update();
}

void GameCanvas::simulate(std::vector<ProgramBlock> program) {
    // Stop running the program
    stop();
    s = new Simulation(map, program);
    // Run the block
    connect(s, &Simulation::runningBlock, this, &GameCanvas::emitRunningBlock);
    emit restartGame();
    run(interval);
}

void GameCanvas::step() {
    s->step();
    // lost in the game
    if(s->getGameState() == lost){
        // reset the robot and map
        emit robotMovie(rightWaiting);
        setMap(resetMap);
        stop();
        return;
    }
    if(s->getGameState() == won){
        // tell the game window the user won
        emit gameWon();
        stop();
        return;
    }
    // Refresh the robot
    emit showRobot(s->getRobotPos() * brickSize, robotSize);
    direction currentDir = s->getRobotDirection();
    // If the robot turn
    if (preDir != currentDir) {
        switch (currentDir) {
        case north:
            emit robotMovie(upWaiting);
            preDir = north;
            break;
        case south:
            emit robotMovie(downWaiting);
            preDir = south;
            break;
        case east:
            emit robotMovie(rightWaiting);
            preDir = east;
            break;
        case west:
            emit robotMovie(leftWaiting);
            preDir = west;
            break;
        }
    }
    // Refresh the map
    setMap(s->getMap());
}

void GameCanvas::run(int newInterval) { timer->start(newInterval); }

void GameCanvas::setInterval(int newInterval){
    interval = newInterval;
    if(timer->isActive()){
        timer->setInterval(interval);
    }
}
void GameCanvas::stop() { timer->stop(); }

void GameCanvas::emitRunningBlock(int block){
    emit currentBlock(block);
}
