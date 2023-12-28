/**
 * @file machinegraph.h
 * @author Keming Chen, Joshua Beatty
 * @brief The program editor panel.
 * @version 0.1
 * @date 2022-12-8
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MACHINEGRAPH_H
#define MACHINEGRAPH_H

#include "constants.h"
#include <QWidget>

class MachineGraph : public QWidget {
    Q_OBJECT
public:

    explicit MachineGraph(QWidget *parent = nullptr);

private:
    // Constants define the blocks size, blocks color.
    const int GENERAL_BLOCK_SIZE_X = 90, GENERAL_BLOCK_SIZE_Y = 30;
    const int CONDITIONAL_BLOCK_SIZE_X = 200, CONDITIONAL_BLOCK_SIZE_Y = 30;
    const int INNER_BLOCK_SIZE_X = 90, INNER_BLOCK_SIZE_Y = 20;
    const int INNER_BLOCK_SIZE_SMALLER_X = 40, INNER_BLOCK_SIZE_SMALLER_Y = 20;

    const QColor beginBlockColor = QColor::fromRgb(89, 255, 160);

    const QColor ifBlockColor = QColor::fromRgb(55, 114, 255);

    const QColor whileBlockColor = QColor::fromRgb(255, 252, 49);

    const QColor generalBlockColor = QColor::fromRgb(57, 62, 65);

    const QColor errorBlockColor = QColor::fromRgb(233, 79, 55);

    const QColor runningBlockColor = QColor::fromRgb(255, 255, 255);

    // Map from blockID to the block's info.
    std::map<int, std::tuple<ProgramBlock, QPointF, QPoint>> map;
    std::map<int, std::tuple<ProgramBlock, ProgramBlock>> condition;
    std::map<int, int> outputMap;

    // Manage connection between blocks.
    std::vector<int> blockTree;

    bool moving;
    bool connecting;
    bool selecting;
    bool mousePressing;
    int stateCount = 1;
    QPointF movingMousePosition, pressedMousePosition;
    std::string errorMessage;
    int hoverBlock;
    std::vector<int> selectedBlock;
    std::vector<QPointF> pressedBlockPosition;
    ProgramBlock type;
    int currentRunningBlock, errorBlock;

    /**
   * @brief paintEvent Paint blocks and connections.
   * @param event
   */
    void paintEvent(QPaintEvent *event);

    /**
   * @brief addBlock Add a block at given position.
   * @param blockType
   * @param position
   */
    void addBlock(ProgramBlock blockType, QPointF position);

    /**
   * @brief connectBlock Connects two blocks.
   * @param block1
   * @param block2
   */
    void connectBlock(int block1, int block2);

    /**
   * @brief drawBlock Helper method for paintEvent, draw a single block.
   * @param blockID
   * @param painter
   */
    void drawBlock(int blockID, QPainter &painter);

    /**
   * @brief drawConnection Draw connection between two points.
   * @param type
   * @param start
   * @param end
   * @param size
   * @param painter
   */
    void drawConnection(ProgramBlock type, QPointF start, QPointF end,
                        QPoint size, QPainter &painter);

    /**
   * @brief drawTextFromMid
   * @param position
   * @param text
   * @param painter
   */
    void drawTextFromMid(QPointF position, std::string text, QPainter &painter);

    /**
   * @brief setErrorMessage Print error message on a block.
   * @param blockId
   * @param message
   */
    void setErrorMessage(int blockId, std::string message);

    /**
   * @brief getBlock Get block at given point.
   * @param point
   * @return
   */
    int getBlock(QPointF point);

    /**
   * @brief getBlock Get block in the given rectangle.
   * @param start
   * @param end
   * @return
   */
    std::vector<int> getBlock(QPointF start, QPointF end);

    /**
   * @brief removeBlocks Remove all blocks.
   */
    void removeBlocks();

    /**
   * @brief clearSelected Clear selected blocks.
   */
    void clearSelected();

    /**
   * @brief reachable See whether block with id2 is reachable from block with id1.
   * @param id1
   * @param id2
   * @return
   */
    bool reachable(int id1, int id2);

    /**
   * @brief event Event triggered when mouse/key is pressed.
   * @param event
   * @return
   */
    bool event(QEvent *event);

    /**
   * @brief mousePressHandler Handling mouse press.
   * @param event
   */
    void mousePressHandler(QMouseEvent *event);

    /**
   * @brief mouseClickHandler Handling double click.
   * @param event
   */
    void mouseClickHandler(QMouseEvent *event);

    /**
   * @brief mouseReleaseHandler Handling mouse release.
   * @param event
   */
    void mouseReleaseHandler(QMouseEvent *event);

    /**
   * @brief mouseMoveHandler Handling mouse move.
   * @param event
   */
    void mouseMoveHandler(QMouseEvent *event);

    /**
   * @brief keyPressHandler Handling key press.
   * @param event
   */
    void keyPressHandler(QKeyEvent *event);

    /**
   * @brief keyReleaseHandler Handling key release.
   * @param event
   */
    void keyReleaseHandler(QKeyEvent *event);

    /**
   * @brief getText Get text for the given program block.
   * @param p
   * @return
   */
    const std::string getText(ProgramBlock p);

public slots:

    /**
   * @brief toggleConnecting Toggle the connecting state.
   */
    void toggleConnecting();

    /**
   * @brief setType Set current placing block's type.
   * @param type
   */
    void setType(ProgramBlock type);

    /**
   * @brief getProgram Get the program from the graph.
   * @return
   */
    std::vector<ProgramBlock> getProgram();

    /**
   * @brief setRunningBlock Set the current running block to blockID.
   * @param blockID
   */
    void setRunningBlock(int blockID);

signals:

    /**
   * @brief connectToggled Signal to tell gamewindow that the connecting state is toggled.
   * @param connecting
   */
    void connectToggled(bool connecting);

    /**
   * @brief programData Signal brings program data to the gamecanvas for running.
   */
    void programData(std::vector<ProgramBlock>);
};

#endif // MACHINEGRAPH_H
