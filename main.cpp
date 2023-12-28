/**
 * @file main.cpp
 * @author Joshua Beatty, Zhuowen Song, Keming Chen, Matthew Whitaker
 * @brief Starts the educational game project by showing the level select window
 * @version 1.0
 * @date 2022-12-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "levelselectwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    LevelSelectWindow levelSelectWindow;
    levelSelectWindow.show();
    return a.exec();

    return 0;
}
