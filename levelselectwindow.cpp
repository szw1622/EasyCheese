/**
 * @file levelselectwindow.cpp
 * @author Joshua Beatty, Zhuowen Song, Keming Chen, Matthew Whitaker
 * @brief Header file that contains essential game data
 * @version 1.0
 * @date 2022-12-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "levelselectwindow.h"
#include "ui_levelselectwindow.h"
#include "constants.h"
#include "gamewindow.h"

LevelSelectWindow::LevelSelectWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LevelSelectWindow)
{
    ui->setupUi(this);

    for(size_t i = 1; i <= (sizeof(levels) / sizeof(std::vector<std::vector<MapTile>>)); i++) {
        ui->levelListWidget->addItem(QString("Level %1").arg(i));
    }

    connect(ui->levelListWidget, &QListWidget::clicked, this, &LevelSelectWindow::openLevel);

}

LevelSelectWindow::~LevelSelectWindow()
{
    delete ui;
}

void LevelSelectWindow::openLevel(const QModelIndex& level) {
    int selectedLevel = level.row();
    GameWindow* window = new GameWindow(levels[selectedLevel], selectedLevel);
    this->close();
    window->show();
}

