/**
 * @file simulation.h
 * @author Joshua Beatty, Keming Chen
 * @brief Header file for simulation.cpp.
 * @version 0.1
 * @date 2022-12-8
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SIMULATION_H
#define SIMULATION_H

#include "constants.h"
#include <QObject>
#include <QPoint>
#include <vector>

class Simulation : public QObject {
    Q_OBJECT
private:
    gameState gameState;
    int width;
    int height;
    QPoint cheesePos;

    QPoint robotPos;
    direction robotDirection;

    std::vector<ProgramBlock> program;
    int tickCount;
    int currentBlock;
    int level;
    std::map<int, int> ifWhileToEnd;
    std::map<int, int> endToIfWhile;

public:
    std::vector<std::vector<MapTile>> map;

    /**
   * @brief Simulation Constructs a new simulation.
   * @param newMap
   * @param newProgram
   * @param parent
   */
    Simulation(std::vector<std::vector<MapTile>> newMap,
               std::vector<ProgramBlock> newProgram, QObject *parent = 0);

    /**
   * @brief step Execute next block.
   */
    void step();

    /**
   * @brief getRobotPos Get robot's position.
   * @return
   */
    QPoint getRobotPos();

    /**
   * @brief getCheesePos Get cheese's position.
   * @return
   */
    QPoint getCheesePos();

    /**
   * @brief getRobotDirection Get the robot's direction.
   * @return
   */
    direction getRobotDirection();

    /**
   * @brief getGameState Get the current game state.
   * @return
   */
    enum gameState getGameState();

    /**
   * @brief getCurrentBlock Get the current running block.
   * @return
   */
    int getCurrentBlock();

    /**
   * @brief printGameState Print the game state by printing the map to console,
   * used for debugging.
   */
    void printGameState();

    /**
   * @brief getMap Get the current map.
   * @return
   */
    std::vector<std::vector<MapTile>> getMap();

private:
    /**
   * @brief setLost Set the game state to lost.
   */
    void setLost();

    /**
   * @brief checkInBounds Check whether the given point is in the bound of map.
   * @return
   */
    bool checkInBounds(QPoint);

    /**
   * @brief checkCondition Check if the conditional statement satisfied.
   * @return
   */
    bool checkCondition(bool, ProgramBlock);

    /**
   * @brief getFacingPoint Get the facing point.
   * @param offset
   * @return
   */
    QPoint getFacingPoint(int offset = 1);
signals:

    /**
   * @brief runningBlock Singal notify machinegraph that running block has
   * changed, so it can update the graph.
   */
    void runningBlock(int);
};

#endif // SIMULATION_H
