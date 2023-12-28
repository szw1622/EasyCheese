/**
 * @file levelselectwindow.h
 * @author Joshua Beatty, Zhuowen Song, Keming Chen, Matthew Whitaker
 * @brief Header file for levelselectwindow.cpp
 * @version 1.0
 * @date 2022-12-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LEVELSELECTWINDOW_H
#define LEVELSELECTWINDOW_H

#include "constants.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class LevelSelectWindow; }
QT_END_NAMESPACE

class LevelSelectWindow : public QMainWindow
{
    Q_OBJECT

public:
    LevelSelectWindow(QWidget *parent = nullptr);
    ~LevelSelectWindow();

signals:
    // Called when a custom map should be loaded
    void selectMap(std::vector<std::vector<MapTile>>);

    // Called when a new level should be loaded
    void selectLevel(int);

private:
    Ui::LevelSelectWindow *ui;

private slots:
    // Opens the level given by the index passed in the QModelIndex
    void openLevel(const QModelIndex& level);
};
#endif // LEVELSELECTWINDOW_H
