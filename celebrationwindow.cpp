/**
 * @file celebrationwindow.cpp
 * @author Matthew Whitaker
 * @brief Defines a celebration window complete with Box2D confetti!
 * @version 1.0
 * @date 2022-12-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "celebrationwindow.h"
#include "gamewindow.h"
#include "ui_celebrationwindow.h"
#include "levelselectwindow.h"
#include <Box2D/Box2D.h>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QRandomGenerator>

CelebrationWindow::CelebrationWindow(int nextLevelIndex, QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::CelebrationWindow)
  , physicsWorld(b2World(b2Vec2(0.0f, 0.0f)))
  , timer(QTimer(this))
  , painter(QPainter(this))
  , nextLevelIndex(nextLevelIndex)
{
    ui->setupUi(this);

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 0.0f);
    b2Body* groundBody = physicsWorld.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(WORLD_WIDTH, WALL_THICKNESS);

    groundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef ceilingBodyDef;
    ceilingBodyDef.position.Set(0.0f, WORLD_HEIGHT);
    b2Body* ceilingBody = physicsWorld.CreateBody(&ceilingBodyDef);
    b2PolygonShape ceilingBox;
    ceilingBox.SetAsBox(WORLD_WIDTH, WALL_THICKNESS);
    ceilingBody->CreateFixture(&ceilingBox, 0.0f);

    b2BodyDef leftWallBodyDef;
    leftWallBodyDef.position.Set(0.0f, 0.0f);
    b2Body* leftWallBody = physicsWorld.CreateBody(&leftWallBodyDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(WALL_THICKNESS, WORLD_HEIGHT);
    leftWallBody->CreateFixture(&leftWallBox, 0.0f);

    b2BodyDef rightWallBodyDef;
    rightWallBodyDef.position.Set(WORLD_WIDTH, 0.0f);
    b2Body* rightWallBody = physicsWorld.CreateBody(&rightWallBodyDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(WALL_THICKNESS, WORLD_HEIGHT);
    rightWallBody->CreateFixture(&rightWallBox, 0.0f);


    confettiBodies = std::array<b2Body*, NUM_CONFETTI>();
    for(b2Body*& body : confettiBodies) {
        // Define the dynamic body. We set its position and call the body factory.
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(QRandomGenerator::system()->generateDouble() * WORLD_WIDTH, QRandomGenerator::system()->generateDouble() * WORLD_HEIGHT);

        body = physicsWorld.CreateBody(&bodyDef);

        // Define another box shape for our dynamic body.
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(CONFETTI_RADIUS, CONFETTI_RADIUS);

        // Define the dynamic body fixture.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;

        // Set the box density to be non-zero, so it will be dynamic.
        fixtureDef.density = 1.0f;

        // Override the default friction.
        fixtureDef.friction = 0.3f;

        fixtureDef.restitution = 0.5;

        body->CreateFixture(&fixtureDef);
    }

    ui->centralwidget->setBodies(&confettiBodies);

    connect(ui->mainMenuButton, &QPushButton::pressed, this, &CelebrationWindow::showMainMenu);
    connect(ui->nextLevelButton, &QPushButton::pressed, this, &CelebrationWindow::nextLevel);

    connect(&timer, &QTimer::timeout, this, &CelebrationWindow::updatePhysics);
    timer.setInterval(10);
    timer.start();
}

CelebrationWindow::~CelebrationWindow()
{
    delete ui;
}

void CelebrationWindow::showMainMenu() {
    LevelSelectWindow* mainMenu = new LevelSelectWindow();
    mainMenu->show();
    this->close();
}

void CelebrationWindow::nextLevel() {
    GameWindow* window = new GameWindow(levels[nextLevelIndex], nextLevelIndex, this);
    this->close();
    window->show();
}

const float32 timeStep = 1.0f / 60.0f;
const int32 velocityIterations = 6;
const int32 positionIterations = 2;

void CelebrationWindow::updatePhysics() {

    confettiBodies.at(QRandomGenerator::system()->bounded(0, NUM_CONFETTI))->ApplyForceToCenter(b2Vec2(0.5f, 0.5f), true);
    confettiBodies.at(QRandomGenerator::system()->bounded(0, NUM_CONFETTI))->ApplyForceToCenter(b2Vec2(0.5f, -0.5f), true);
    confettiBodies.at(QRandomGenerator::system()->bounded(0, NUM_CONFETTI))->ApplyForceToCenter(b2Vec2(-0.5f, 0.5f), true);
    confettiBodies.at(QRandomGenerator::system()->bounded(0, NUM_CONFETTI))->ApplyForceToCenter(b2Vec2(-0.5f, -0.5f), true);

    physicsWorld.Step(timeStep, velocityIterations, positionIterations);

    ui->centralwidget->repaint();
}


CelebrationWidget::CelebrationWidget(QWidget* parent): QWidget(parent) {
    confettiColors = std::array<QColor, NUM_CONFETTI>();
    for(QColor& color : confettiColors) {
        color.setRed(QRandomGenerator::system()->bounded(0, 255));
        color.setGreen(QRandomGenerator::system()->bounded(0, 255));
        color.setBlue(QRandomGenerator::system()->bounded(0, 255));
        color.setAlpha(QRandomGenerator::system()->bounded(100, 255));
    }
}

void CelebrationWidget::setBodies(std::array<b2Body*, NUM_CONFETTI>* bodies) {
    this->confettiBodiesReference = bodies;
}

void CelebrationWidget::paintEvent(QPaintEvent* event) {
      QPainter p(this);
      int i = 0;
      for(auto body : *confettiBodiesReference) {
          p.setBrush(QBrush(confettiColors[i]));
          p.setPen(QPen(confettiColors[i]));
          auto point = body->GetPosition();
          QPointF scaledCoordinates = QPointF(point.x * event->rect().width() / WORLD_WIDTH, event->rect().height() - (point.y * event->rect().height() / WORLD_HEIGHT));
          p.translate(scaledCoordinates);
          p.rotate(body->GetAngle() * 180.0 / 3.14159);
          p.drawRect(-CONFETTI_RADIUS * event->rect().width() / WORLD_WIDTH, -CONFETTI_RADIUS * event->rect().height() / WORLD_HEIGHT, CONFETTI_RADIUS * 2 * event->rect().width() / WORLD_WIDTH, CONFETTI_RADIUS * 2 * event->rect().height() / WORLD_HEIGHT);
          p.resetTransform();
          i++;
      }
}
