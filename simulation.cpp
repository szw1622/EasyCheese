/**
 * @file simulation.h
 * @author Joshua Beatty, Keming Chen
 * @brief The tiny simulation program.
 * @version 0.1
 * @date 2022-12-8
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "simulation.h"
#include "constants.h"
#include <QDebug>
#include <QPoint>
#include <stack>
#include <string>
#include <vector>
Simulation::Simulation(std::vector<std::vector<MapTile>> newMap,
                       std::vector<ProgramBlock> newProgram, QObject *parent)
    : QObject(parent), gameState(notEnded), robotDirection(east),
      program(newProgram), map(newMap) {
    height = map.size();
    width = map[0].size();
    for (unsigned long long y = 0; y < map.size(); y++) {
        for (unsigned long long x = 0; x < map[y].size(); x++) {
            if (map[y][x] == start) {
                robotPos = QPoint(x, y);
                map[y][x] = ground;
            }
            if (map[y][x] == cheese) {
                cheesePos = QPoint(x, y);
                map[y][x] = ground;
            }
        }
    }

    tickCount = 0;
    currentBlock = 0;

    std::stack<int> ifWhileStack;
    for (unsigned long long index = 0; index < program.size(); index++) {
        switch (program[index]) {
        case ifStatement:
        case whileLoop:
            ifWhileStack.push(index);
            break;
        case endIf:
        case endWhile: {
            int start = ifWhileStack.top();
            ifWhileStack.pop();
            ifWhileToEnd.insert({start, index});
            endToIfWhile.insert({index, start});
            break;
        }
        default:
            break;
        }
    }
}

void Simulation::step() {
    if (gameState != notEnded)
        return;
    tickCount++;
    currentBlock++;
    emit runningBlock(currentBlock);
    if (currentBlock == (int)program.size()) {
        setLost();
        return;
    }

    switch (program[currentBlock]) {
    case beginBlock:
        break;
    case moveForward: {
        QPoint newPos = getFacingPoint(1);
        QPoint newBoxPos = getFacingPoint(2);

        // qDebug() << newPos;
        // qDebug() << checkInBounds(newPos);
        if (!checkInBounds(newPos)) {
            return;
        }
        // qDebug() << "should move";
        // qDebug() << newPos;

        switch (map[newPos.y()][newPos.x()]) {
        case ground:
            robotPos = newPos;
            break;
        case pit:
            setLost();
            break;
        case block:
            if (!checkInBounds(newBoxPos)) {
                return;
            }
            switch (map[newBoxPos.y()][newBoxPos.x()]) {
            case ground:
                robotPos = newPos;
                map[newBoxPos.y()][newBoxPos.x()] = block;
                map[newPos.y()][newPos.x()] = ground;
                break;
            case pit:
                robotPos = newPos;
                map[newPos.y()][newPos.x()] = ground;
                break;
            case block:
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    }
    case turnLeft:
        switch (robotDirection) {
        case north:
            robotDirection = west;
            break;
        case south:
            robotDirection = east;
            break;
        case east:
            robotDirection = north;
            break;
        case west:
            robotDirection = south;
            break;
        }
        break;
    case turnRight:
        switch (robotDirection) {
        case north:
            robotDirection = east;
            break;
        case south:
            robotDirection = west;
            break;
        case east:
            robotDirection = south;
            break;
        case west:
            robotDirection = north;
            break;
        }
        break;
    case eatCheese:
        if (cheesePos == robotPos) {
            cheesePos = QPoint(-1, -1);
            gameState = won;
        }
        break;
    case ifStatement: {
        bool flag = checkCondition(program[currentBlock + 1] == conditionNot,
                program[currentBlock + 2]);
        if (!flag) {
            currentBlock = ifWhileToEnd[currentBlock];
        } else {
            currentBlock += 2;
        }
    } break;
    case whileLoop: {
        bool flag = checkCondition(program[currentBlock + 1] == conditionNot,
                program[currentBlock + 2]);
        if (!flag) {
            currentBlock = ifWhileToEnd[currentBlock];
        } else {
            currentBlock += 2;
        }
    } break;
    case endWhile:
        currentBlock = ifWhileToEnd[currentBlock];
        break;
    case endIf:
        break;
    case conditionNot:
    case conditionFacingBlock:
    case conditionFacingWall:
    case conditionFacingPit:
    case conditionFacingCheese:
    case blank:
        break;
    }
}

void Simulation::setLost() {
    gameState = lost;
    robotPos = QPoint(-1, -1);
    emit runningBlock(-1);
}

QPoint Simulation::getFacingPoint(int offset) {
    QPoint newPos(robotPos);
    switch (robotDirection) {
    case north:
        newPos.setY(newPos.y() - offset);
        break;
    case south:
        newPos.setY(newPos.y() + offset);
        break;
    case west:
        newPos.setX(newPos.x() - offset);
        break;
    case east:
        newPos.setX(newPos.x() + offset);
        break;
    }
    return newPos;
}

bool Simulation::checkInBounds(QPoint point) {
    return point.x() != -1 && point.y() != -1 && point.x() != width &&
            point.y() != height;
}

bool Simulation::checkCondition(bool isNot, ProgramBlock condition) {
    QPoint facing = getFacingPoint(1);
    if (!checkInBounds(QPoint(facing.x(), facing.y())))
        return false;
    MapTile facingTile = map[facing.y()][facing.x()];
    bool flag = false;
    switch (condition) {
    case conditionFacingBlock:
        flag = facingTile == block;
        break;
    case conditionFacingWall:
        flag = facingTile == wall;
        break;
    case conditionFacingPit:
        flag = facingTile == pit;
        break;
    case conditionFacingCheese:
        flag = facing == cheesePos;
        break;
    default:
        break;
    }
    return isNot ? !flag : flag;
}

QPoint Simulation::getCheesePos() { return cheesePos; }
QPoint Simulation::getRobotPos() { return robotPos; }
int Simulation::getCurrentBlock() { return currentBlock; }
void Simulation::printGameState() {
    switch (gameState) {
    case lost:
        break;
    case won:
        break;
    case notEnded:
        break;
    }
    std::string mapString = "";
    for (unsigned long long y = 0; y < map.size(); y++) {
        mapString.append("\n");
        for (unsigned long long x = 0; x < map[y].size(); x++) {
            if (robotPos == QPoint(x, y)) {
                switch (robotDirection) {
                case north:
                    mapString.append("^");
                    continue;
                    break;
                case south:
                    mapString.append("v");
                    continue;
                    break;
                case east:
                    mapString.append(">");
                    continue;
                    break;
                case west:
                    mapString.append("<");
                    continue;
                    break;
                }
            }
            if (cheesePos == QPoint(x, y)) {
                mapString.append("C");
                continue;
            }
            switch (map[y][x]) {
            case ground:
                mapString.append("*");
                break;
            case pit:
                mapString.append("0");
                break;
            case block:
                mapString.append("@");
                break;
            case wall:
                mapString.append("#");
                break;
            default:
                break;
            }
        }
    }
}

std::vector<std::vector<MapTile>> Simulation::getMap() {
    QPoint pos = this->getCheesePos();
    std::vector<std::vector<MapTile>> newMap = map;
    newMap[pos.y()][pos.x()] = MapTile::cheese;
    return newMap;
}

direction Simulation::getRobotDirection() { return robotDirection; }

enum gameState Simulation::getGameState() { return gameState; }
