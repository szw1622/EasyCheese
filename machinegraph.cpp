/**
 * @file machinegraph.cpp
 * @author Keming Chen, Joshua Beatty
 * @brief The program editor panel.
 * @version 0.1
 * @date 2022-12-8
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "machinegraph.h"
#include <QEvent>
#include <QKeyEvent>
#include <QLine>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include <QPen>
#include <tuple>
#include <vector>
MachineGraph::MachineGraph(QWidget *parent) : QWidget{parent} {
    blockTree.push_back(-1);
    this->setAttribute(Qt::WA_Hover, true);

    map[0] = std::tuple<ProgramBlock, QPointF, QPoint>(
                ProgramBlock::beginBlock,
                QPointF(this->width() / 2 - GENERAL_BLOCK_SIZE_X / 2,
                        this->height() / 2 - GENERAL_BLOCK_SIZE_Y / 2),
                QPoint(GENERAL_BLOCK_SIZE_X, GENERAL_BLOCK_SIZE_Y));
    type = ProgramBlock::moveForward;
    this->setFocusPolicy(Qt::StrongFocus);
    hoverBlock = -1;
    errorBlock = -1;
    connecting = false;
    selecting = false;
    mousePressing = false;
    moving = false;

    update();
}

void MachineGraph::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    // Draw backgournd.
    painter.drawRect(QRect(0, 0, this->width() - 10, this->height() - 10));

    for (const auto &[key, value] : map) {
        bool visited[blockTree.size()];

        for (unsigned long i = 0; i < blockTree.size(); i++) {
            visited[i] = false;
        }

        painter.setPen(QPen(QColor::fromRgb(0, 0, 0), 2, Qt::SolidLine,
                            Qt::RoundCap, Qt::RoundJoin));

        int currentBlock = key;
        while (currentBlock != -1) {
            int next = blockTree[currentBlock];
            if (!visited[currentBlock]) {
                QPointF startPoint = std::get<QPointF>(map[currentBlock]);
                QPointF startSize = std::get<QPoint>(map[currentBlock]);
                visited[currentBlock] = true;
                if (next != -1) {
                    ProgramBlock nextType = std::get<ProgramBlock>(map[next]);

                    QPoint endSize = std::get<QPoint>(map[next]);
                    QPointF endPoint = std::get<QPointF>(map[next]);
                    drawConnection(nextType, startPoint + startSize / 2,
                                   endPoint + endSize / 2, endSize, painter);
                }
            }
            drawBlock(currentBlock, painter);
            currentBlock = next;
        }
    }

    if (selecting) {
        painter.fillRect(QRect(pressedMousePosition.x(), pressedMousePosition.y(),
                               movingMousePosition.x() - pressedMousePosition.x(),
                               movingMousePosition.y() - pressedMousePosition.y()),
                         QColor::fromRgbF(255, 255, 255, 0.3));
    }
}

void MachineGraph::drawBlock(int blockID, QPainter &painter) {
    QPointF startPoint = std::get<QPointF>(map[blockID]);
    ProgramBlock type = std::get<ProgramBlock>(map[blockID]);
    QPoint size = std::get<QPoint>(map[blockID]);

    bool lighter = false;

    int midY = startPoint.y() + size.y() / 2;
    int midX = startPoint.x() + size.x() / 2;

    QColor blockColor;
    QColor innerBlockColor;

    if (selectedBlock.size() > 0) {
        if (blockID == hoverBlock ||
                std::find(selectedBlock.begin(), selectedBlock.end(), blockID) !=
                selectedBlock.end()) {
            lighter = true;
        }
        if (blockID == hoverBlock && reachable(blockID, selectedBlock[0]) &&
                blockID != selectedBlock[0]) {
            lighter = false;
        }
    }

    if (type == ProgramBlock::beginBlock) {
        blockColor = beginBlockColor;

    } else if (type == ProgramBlock::ifStatement || type == ProgramBlock::endIf) {
        blockColor = ifBlockColor;

    } else if (type == ProgramBlock::whileLoop ||
               type == ProgramBlock::endWhile) {
        blockColor = whileBlockColor;

    } else {
        blockColor = generalBlockColor;
    }

    // Draw error message
    if (errorBlock == blockID) {
        blockColor = errorBlockColor;
        painter.setPen(errorBlockColor);
        painter.drawLine(QLine(startPoint.x() + size.x(), midY,
                               startPoint.x() + size.x() + 20, midY));
        painter.drawText(startPoint.x() + size.x() + 30, midY + 5,
                         errorMessage.c_str());
    }

    if (outputMap.count(currentRunningBlock) &&
            outputMap[currentRunningBlock] == blockID) {
        blockColor = runningBlockColor;
    }

    painter.setPen(Qt::black);
    if (lighter) {
        blockColor = blockColor.lighter();
        innerBlockColor = innerBlockColor.lighter();
    }
    innerBlockColor = blockColor.lighter().lighter();

    QColor borderAndTextColorOuter;
    QColor borderAndTextColorInner;

    if (blockColor.lightness() > 150) {
        borderAndTextColorOuter = Qt::black;
    } else {
        borderAndTextColorOuter = Qt::white;
    }

    if (innerBlockColor.lightness() > 150) {
        borderAndTextColorInner = Qt::black;
    } else {
        borderAndTextColorInner = Qt::white;
    }

    QPen outerPen(borderAndTextColorOuter, 1);
    QPen innerPen(borderAndTextColorInner, 1);

    switch (type) {
    case ProgramBlock::whileLoop: {
    }
    case ProgramBlock::ifStatement: {
        QPainterPath path;
        int firstConstGap = 50;
        int secondConstGap = 10;
        path.addRoundedRect(QRectF(startPoint.x(), startPoint.y(),
                                   CONDITIONAL_BLOCK_SIZE_X,
                                   CONDITIONAL_BLOCK_SIZE_Y),
                            5, 5);
        painter.setPen(outerPen);
        painter.fillPath(path, blockColor);
        painter.drawPath(path);
        drawTextFromMid(QPointF(startPoint.x() + firstConstGap / 2, midY + 5),
                        this->getText(type), painter);

        ProgramBlock firstConst = std::get<0>(condition[blockID]);
        ProgramBlock secondConst = std::get<1>(condition[blockID]);

        QPainterPath pathInner1;
        QPainterPath pathInner2;
        painter.setPen(innerPen);

        pathInner1.addRoundedRect(QRectF(startPoint.x() + firstConstGap,
                                         midY - INNER_BLOCK_SIZE_SMALLER_Y / 2,
                                         INNER_BLOCK_SIZE_SMALLER_X,
                                         INNER_BLOCK_SIZE_SMALLER_Y),
                                  5, 5);

        painter.fillPath(pathInner1, innerBlockColor);
        drawTextFromMid(
                    QPointF(startPoint.x() + firstConstGap + INNER_BLOCK_SIZE_SMALLER_X / 2,
                            midY + 5),
                    this->getText(firstConst), painter);

        pathInner2.addRoundedRect(QRectF(startPoint.x() + firstConstGap +
                                         secondConstGap +
                                         INNER_BLOCK_SIZE_SMALLER_X,
                                         midY - INNER_BLOCK_SIZE_Y / 2,
                                         INNER_BLOCK_SIZE_X, INNER_BLOCK_SIZE_Y),
                                  5, 5);
        painter.fillPath(pathInner2, innerBlockColor);
        drawTextFromMid(QPointF(startPoint.x() + firstConstGap +
                                INNER_BLOCK_SIZE_SMALLER_X + secondConstGap +
                                INNER_BLOCK_SIZE_X / 2,
                                midY + 5),
                        this->getText(secondConst), painter);
        break;
    }
        //  case ProgramBlock::whileLoop: {
        //    painter.fillRect(startPoint.x(), startPoint.y(),
        //    CONDITIONAL_BLOCK_SIZE_X,
        //                     CONDITIONAL_BLOCK_SIZE_Y, blockColor);
        //    painter.drawText(startPoint.x() + 5, midY,
        //    this->getText(type).c_str()); ProgramBlock firstConst =
        //    std::get<0>(condition[blockID]); ProgramBlock secondConst =
        //    std::get<1>(condition[blockID]); painter.fillRect(startPoint.x() + 50,
        //    midY - INNER_BLOCK_SIZE_SMALLER_Y / 2,
        //                     INNER_BLOCK_SIZE_SMALLER_X,
        //                     INNER_BLOCK_SIZE_SMALLER_Y, innerBlockColor);
        //    painter.drawText(startPoint.x() + 50, midY,
        //                     this->getText(firstConst).c_str());

        //    painter.fillRect(startPoint.x() + 60 + INNER_BLOCK_SIZE_SMALLER_X,
        //                     midY - INNER_BLOCK_SIZE_Y / 2, INNER_BLOCK_SIZE_X,
        //                     INNER_BLOCK_SIZE_Y, innerBlockColor);
        //    painter.drawText(startPoint.x() + 60 + INNER_BLOCK_SIZE_SMALLER_X,
        //    midY,
        //                     this->getText(secondConst).c_str());
        //    break;
        //  }
    default: {
        QPainterPath path;
        path.addRoundedRect(
                    QRectF(startPoint.x(), startPoint.y(), size.x(), GENERAL_BLOCK_SIZE_Y),
                    5, 5);
        painter.setPen(outerPen);
        painter.fillPath(path, blockColor);
        painter.drawPath(path);
        drawTextFromMid(QPointF(midX, midY + 5), this->getText(type), painter);
    }
    }
}

void MachineGraph::drawConnection(ProgramBlock type, QPointF start, QPointF end,
                                  QPoint size, QPainter &painter) {

    painter.setPen(QPen(Qt::black, 2));
    float distX = qAbs(end.x() - start.x());
    float distY = qAbs(end.y() - start.y());
    int arrowSize = 5;
    if (type == ProgramBlock::endIf || type == ProgramBlock::endWhile) {
        if (distY < distX) {
            painter.drawLine(QLine(start.x(), start.y(), start.x(), end.y()));
            painter.drawLine(QLine(start.x(), end.y(), end.x(), end.y()));
            if (end.x() < start.x()) {
                QPainterPath path;
                path.moveTo(end.x() + size.x() / 2, end.y());
                path.lineTo(end.x() + size.x() / 2 + arrowSize, end.y() - arrowSize);
                path.lineTo(end.x() + size.x() / 2 + arrowSize, end.y() + arrowSize);
                painter.fillPath(path, QColor::fromRgb(0, 0, 0));
            } else {
                QPainterPath path;
                path.moveTo(end.x() - size.x() / 2, end.y());
                path.lineTo(end.x() - size.x() / 2 - arrowSize, end.y() - arrowSize);
                path.lineTo(end.x() - size.x() / 2 - arrowSize, end.y() + arrowSize);
                painter.fillPath(path, QColor::fromRgb(0, 0, 0));
            }
        } else {
            painter.drawLine(QLine(start.x(), start.y(), end.x(), start.y()));
            painter.drawLine(QLine(end.x(), start.y(), end.x(), end.y()));
            if (end.y() < start.y()) {
                QPainterPath path;
                path.moveTo(end.x(), end.y() + size.y() / 2);
                path.lineTo(end.x() + arrowSize, end.y() + size.y() / 2 + arrowSize);
                path.lineTo(end.x() - arrowSize, end.y() + size.y() / 2 + arrowSize);
                painter.fillPath(path, QColor::fromRgb(0, 0, 0));
            } else {
                QPainterPath path;
                path.moveTo(end.x(), end.y() - size.y() / 2);
                path.lineTo(end.x() + arrowSize, end.y() - size.y() / 2 - arrowSize);
                path.lineTo(end.x() - arrowSize, end.y() - size.y() / 2 - arrowSize);
                painter.fillPath(path, QColor::fromRgb(0, 0, 0));
            }
        }
        return;
    }

    if (distY > distX) {
        painter.drawLine(QLine(start.x(), start.y(), start.x(), end.y()));
        painter.drawLine(QLine(start.x(), end.y(), end.x(), end.y()));
        // Arrow points left.
        if (start.x() > end.x() - size.x() / 2 &&
                start.x() < end.x() + size.x() / 2) {
            if (end.y() > start.y()) {
                QPainterPath path;
                path.moveTo(start.x(), end.y() - size.y() / 2);
                path.lineTo(start.x() - arrowSize, end.y() - size.y() / 2 - arrowSize);
                path.lineTo(start.x() + arrowSize, end.y() - size.y() / 2 - arrowSize);
                painter.fillPath(path, QColor::fromRgb(0, 0, 0));
            } else {
                QPainterPath path;
                path.moveTo(start.x(), end.y() + size.y() / 2);
                path.lineTo(start.x() - arrowSize, end.y() + size.y() / 2 + arrowSize);
                path.lineTo(start.x() + arrowSize, end.y() + size.y() / 2 + arrowSize);
                painter.fillPath(path, QColor::fromRgb(0, 0, 0));
            }
            return;
        }
        if (end.x() < start.x()) {
            QPainterPath path;
            path.moveTo(end.x() + size.x() / 2, end.y());
            path.lineTo(end.x() + size.x() / 2 + arrowSize, end.y() - arrowSize);
            path.lineTo(end.x() + size.x() / 2 + arrowSize, end.y() + arrowSize);
            painter.fillPath(path, QColor::fromRgb(0, 0, 0));
        } else {
            QPainterPath path;
            path.moveTo(end.x() - size.x() / 2, end.y());
            path.lineTo(end.x() - size.x() / 2 - arrowSize, end.y() - arrowSize);
            path.lineTo(end.x() - size.x() / 2 - arrowSize, end.y() + arrowSize);
            painter.fillPath(path, QColor::fromRgb(0, 0, 0));
        }
    } else {
        painter.drawLine(QLine(start.x(), start.y(), end.x(), start.y()));
        painter.drawLine(QLine(end.x(), start.y(), end.x(), end.y()));

        if (start.y() > end.y() - size.y() / 2 &&
                start.y() < end.y() + size.y() / 2) {
            if (end.x() > start.x()) {
                QPainterPath path;
                path.moveTo(end.x() - size.x() / 2, start.y());
                path.lineTo(end.x() - size.x() / 2 - arrowSize, start.y() - arrowSize);
                path.lineTo(end.x() - size.x() / 2 - arrowSize, start.y() + arrowSize);
                painter.fillPath(path, QColor::fromRgb(0, 0, 0));
            } else {
                QPainterPath path;
                path.moveTo(end.x() + size.x() / 2, start.y());
                path.lineTo(end.x() + size.x() / 2 + arrowSize, start.y() - arrowSize);
                path.lineTo(end.x() + size.x() / 2 + arrowSize, start.y() + arrowSize);
                painter.fillPath(path, QColor::fromRgb(0, 0, 0));
            }
            return;
        }
        if (end.y() < start.y()) {
            QPainterPath path;
            path.moveTo(end.x(), end.y() + size.y() / 2);
            path.lineTo(end.x() + arrowSize, end.y() + size.y() / 2 + arrowSize);
            path.lineTo(end.x() - arrowSize, end.y() + size.y() / 2 + arrowSize);
            painter.fillPath(path, QColor::fromRgb(0, 0, 0));
        } else {
            QPainterPath path;
            path.moveTo(end.x(), end.y() - size.y() / 2);
            path.lineTo(end.x() + arrowSize, end.y() - size.y() / 2 - arrowSize);
            path.lineTo(end.x() - arrowSize, end.y() - size.y() / 2 - arrowSize);
            painter.fillPath(path, QColor::fromRgb(0, 0, 0));
        }
    }
}

void MachineGraph::drawTextFromMid(QPointF position, std::string text,
                                   QPainter &painter) {
    QFontMetrics fm(painter.font());
    const std::string str = text;
    int xoffset = fm.boundingRect(str.c_str()).width() / 2;
    painter.drawText(position.x() - xoffset, position.y(), str.c_str());
}

void MachineGraph::connectBlock(int block1, int block2) {
    if (!reachable(block2, block1)) {
        blockTree[block1] = block2;
    }
}

void MachineGraph::toggleConnecting() {
    connecting = !connecting;
    if (connecting)
        setCursor(Qt::CrossCursor);
    else {
        setCursor(Qt::ArrowCursor);
    }
    emit connectToggled(connecting);
}

bool MachineGraph::event(QEvent *event) {
    switch (event->type()) {
    case QEvent::MouseButtonDblClick:
        mouseClickHandler(static_cast<QMouseEvent *>(event));
        return false;
        break;
    case QEvent::MouseButtonPress:
        mousePressHandler(static_cast<QMouseEvent *>(event));
        return false;
        break;
    case QEvent::MouseButtonRelease:
        mouseReleaseHandler(static_cast<QMouseEvent *>(event));
        return false;
        break;
    case QEvent::MouseMove:
        mouseMoveHandler(static_cast<QMouseEvent *>(event));
        return false;
        break;
    case QEvent::KeyPress:
        keyPressHandler(static_cast<QKeyEvent *>(event));
        return false;
        break;
    case QEvent::KeyRelease:
        keyPressHandler(static_cast<QKeyEvent *>(event));
        return false;
        break;
    default:
        break;
    }
    QWidget::event(event);
    return false;
}
void MachineGraph::mouseMoveHandler(QMouseEvent *event) {
    if (selecting) {
        movingMousePosition = event->position();
        // Update selected boxes.
        selectedBlock = getBlock(pressedMousePosition, movingMousePosition);

        std::vector<QPointF> newPositionList;
        for (int blockId : selectedBlock) {
            newPositionList.push_back(std::get<QPointF>(map[blockId]));
        }

        pressedBlockPosition = newPositionList;
    }
    if (moving && selectedBlock.size() > 0) {
        for (unsigned long i = 0; i < selectedBlock.size(); i++) {
            QPointF newPosition =
                    event->position() - pressedMousePosition + pressedBlockPosition[i];
            if (newPosition.x() < 1) {
                newPosition.setX(1);
            }
            if (newPosition.y() < 1) {
                newPosition.setY(1);
            }
            QPoint size = std::get<QPoint>(map[selectedBlock[i]]);
            if (newPosition.x() + size.x() > 760) {
                newPosition.setX(760 - size.x());
            }
            if (newPosition.y() + size.y() > 721) {
                newPosition.setY(721 - size.y());
            }

            std::get<QPointF>(map[selectedBlock[i]]) = newPosition;
        }
    }
    if (connecting) {
        int blockId = getBlock(event->position());
        hoverBlock = blockId;
    }
    update();
}
void MachineGraph::clearSelected() {
    if (selectedBlock.size() > 0) {
        selectedBlock.clear();
    }
    if (pressedBlockPosition.size() > 0) {
        pressedBlockPosition.clear();
    }
    update();
}
void MachineGraph::mouseReleaseHandler(QMouseEvent *event) {
    if (connecting) {
        int blockId = getBlock(event->position());
        if (selectedBlock.size() > 0 && blockTree[blockId] != selectedBlock[0]) {
            connectBlock(selectedBlock[0], blockId);
        }
        clearSelected();
    }

    for (unsigned long i = 0; i < selectedBlock.size(); i++) {
        pressedBlockPosition[i] = std::get<QPointF>(map[selectedBlock[i]]);
    }

    hoverBlock = -1;
    mousePressing = false;
    moving = false;
    selecting = false;
    update();
}
void MachineGraph::mousePressHandler(QMouseEvent *event) {
    int blockId = getBlock(event->position());
    pressedMousePosition = event->position();
    movingMousePosition = event->position();
    mousePressing = true;
    errorBlock = -1;

    if (!connecting && selectedBlock.size() != 0 &&
            std::find(selectedBlock.begin(), selectedBlock.end(), blockId) !=
            selectedBlock.end()) {
        moving = true;
        return;
    }

    if (connecting) {
        clearSelected();
        if (blockId != -1) {
            selectedBlock.push_back(blockId);
        }
    } else {
        clearSelected();
        if (blockId != -1) {
            moving = true;
            selectedBlock.push_back(blockId);
            pressedBlockPosition.push_back(std::get<QPointF>(map[blockId]));
        } else {
            selecting = true;
        }
    }
}

void MachineGraph::mouseClickHandler(QMouseEvent *event) {
    if (Qt::LeftButton && event->position().x() > 0 &&
            event->position().y() > 0) {
        addBlock(type, event->position());
        update();
    }
}

void MachineGraph::keyPressHandler(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) {
        if (!mousePressing) {
            removeBlocks();
        }
    }
    if (event->key() == Qt::Key_Space) {
        toggleConnecting();
    }
}

void MachineGraph::keyReleaseHandler(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        toggleConnecting();
    }
}

const std::string MachineGraph::getText(ProgramBlock p) {
    switch (p) {
    case ProgramBlock::conditionFacingBlock:
        return "Facing Block";
    case ProgramBlock::conditionNot:
        return "Not";
    case ProgramBlock::conditionFacingPit:
        return "Facing Pit";
    case ProgramBlock::conditionFacingWall:
        return "Facing Wall";
    case ProgramBlock::conditionFacingCheese:
        return "Facing Cheese";
    case ProgramBlock::beginBlock:
        return "Begin";
    case ProgramBlock::blank:
        return "";
    case ProgramBlock::eatCheese:
        return "Eat Chese";
    case ProgramBlock::endIf:
        return "End If";
    case ProgramBlock::endWhile:
        return "End While";
    case ProgramBlock::whileLoop:
        return "While";
    case ProgramBlock::ifStatement:
        return "If";
    case ProgramBlock::turnLeft:
        return "Turn Left";
    case ProgramBlock::turnRight:
        return "Turn Right";
    case ProgramBlock::moveForward:
        return "Move Forward";
    }
    return "";
}

bool MachineGraph::reachable(int id1, int id2) {
    int current = id1;
    while (current != -1) {
        if (current == id2) {
            return true;
        }
        current = blockTree[current];
    }
    return false;
}

void MachineGraph::addBlock(ProgramBlock type, QPointF position) {
    if (type == ProgramBlock::conditionFacingBlock ||
            type == ProgramBlock::conditionFacingPit ||
            type == ProgramBlock::conditionFacingWall ||
            type == ProgramBlock::conditionFacingCheese ||
            type == ProgramBlock::conditionNot) {

        int blockId = getBlock(position);
        ProgramBlock blockType = std::get<ProgramBlock>(map[blockId]);
        if (blockType == ProgramBlock::whileLoop ||
                blockType == ProgramBlock::ifStatement) {
            if (type == ProgramBlock::conditionNot) {
                std::get<0>(condition[blockId]) = type;
            } else {
                std::get<1>(condition[blockId]) = type;
            }
        }
        return;
    }
    int id = blockTree.size();
    blockTree.push_back(-1);
    if (type == ProgramBlock::ifStatement || type == ProgramBlock::whileLoop) {
        map[id] = std::tuple<ProgramBlock, QPointF, QPoint>(
                    type, position,
                    QPoint(CONDITIONAL_BLOCK_SIZE_X, CONDITIONAL_BLOCK_SIZE_Y));
        condition[id] = std::tuple<ProgramBlock, ProgramBlock>(ProgramBlock::blank,
                                                               ProgramBlock::blank);
    } else {
        map[id] = std::tuple<ProgramBlock, QPointF, QPoint>(
                    type, position, QPoint(GENERAL_BLOCK_SIZE_X, GENERAL_BLOCK_SIZE_Y));
    }
}

int MachineGraph::getBlock(QPointF point) {
    for (const auto &[key, value] : map) {
        QPointF startPoint = std::get<QPointF>(map[key]);
        QPoint size = std::get<QPoint>(map[key]);
        if (point.x() < (startPoint.x() + size.x()) && point.x() > startPoint.x() &&
                point.y() < (startPoint.y() + size.y()) && point.y() > startPoint.y()) {
            return key;
        }
    }

    return -1;
}

std::vector<int> MachineGraph::getBlock(QPointF start, QPointF end) {
    std::vector<int> blocks;
    if (start.x() > end.x()) {
        float temp = start.x();
        start.setX(end.x());
        end.setX(temp);
    }
    if (start.y() > end.y()) {
        float temp = start.y();
        start.setY(end.y());
        end.setY(temp);
    }
    for (const auto &[key, value] : map) {
        QPointF startPoint = std::get<QPointF>(map[key]);
        QPoint size = std::get<QPoint>(map[key]);
        if ((start.x() < startPoint.x()) && (start.y() < startPoint.y()) &&
                (end.x() > startPoint.x() + size.x()) &&
                (end.y() > startPoint.y() + size.y())) {
            blocks.push_back(key);
        }
    }
    return blocks;
}

void MachineGraph::removeBlocks() {
    if (selectedBlock.size() > 0) {
        for (int id : selectedBlock) {
            if (id == 0)
                continue;
            for (unsigned long i = 0; i < blockTree.size(); i++) {
                if (blockTree[i] == id) {
                    blockTree[i] = -1;
                }
            }
            map.erase(id);
        }
        clearSelected();
    }
    update();
}

void MachineGraph::setType(ProgramBlock type) { this->type = type; }

void MachineGraph::setErrorMessage(int blockId, std::string message) {
    errorBlock = blockId;
    errorMessage = message;
    update();
}

std::vector<ProgramBlock> MachineGraph::getProgram() {
    int currentBlock = 0;
    std::vector<int> blockId;
    std::vector<ProgramBlock> program;
    std::vector<ProgramBlock> grammaStack;
    outputMap.clear();
    while (currentBlock != -1) {
        int next = blockTree[currentBlock];
        ProgramBlock type = std::get<ProgramBlock>(map[currentBlock]);
        outputMap[program.size()] = currentBlock;
        program.push_back(type);
        if (type == ProgramBlock::ifStatement || type == ProgramBlock::whileLoop) {
            grammaStack.push_back(type);
            blockId.push_back(currentBlock);
            ProgramBlock firstConst = std::get<0>(condition[currentBlock]);
            ProgramBlock secondConst = std::get<1>(condition[currentBlock]);
            if (secondConst == ProgramBlock::blank) {
                setErrorMessage(currentBlock, "Incomplete conditinal statement");
                return std::vector<ProgramBlock>(ProgramBlock::blank);
            } else {
                program.push_back(firstConst);
                program.push_back(secondConst);
            }
        }

        if (type == ProgramBlock::endIf) {
            if (grammaStack.empty() ||
                    grammaStack.back() != ProgramBlock::ifStatement) {
                setErrorMessage(currentBlock, "No matched If statement for End If");
                return std::vector<ProgramBlock>(ProgramBlock::blank);
            } else {
                grammaStack.pop_back();
                blockId.pop_back();
            }
        }

        if (type == ProgramBlock::endWhile) {
            if (grammaStack.empty() ||
                    grammaStack.back() != ProgramBlock::whileLoop) {
                setErrorMessage(currentBlock, "No matched If statement for End While");
                return std::vector<ProgramBlock>(ProgramBlock::blank);
            } else {
                grammaStack.pop_back();
                blockId.pop_back();
            }
        }
        currentBlock = next;
    }

    if (!grammaStack.empty()) {
        setErrorMessage(blockId.back(), "Needs end statement");
        return std::vector<ProgramBlock>(ProgramBlock::blank);
    }

    emit programData(program);

    return program;
}

void MachineGraph::setRunningBlock(int blockID) {
    this->currentRunningBlock = blockID;
    update();
}
