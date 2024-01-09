#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createUI();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gameBoard_;
}

void MainWindow::createUI() {

    turnLabel_ = new QLabel("Game not yet started", this);
    QLabel* heightLabel = new QLabel("Height", this);
    QLabel* widthLabel = new QLabel("Width", this);
    QLabel* goalLabel = new QLabel("Goal", this);
    QComboBox* heightComboBox = new QComboBox(this);
    QComboBox* widthComboBox = new QComboBox(this);
    QComboBox* goalComboBox = new QComboBox(this);
    QPushButton* startButton = new QPushButton("Start", this);

    heightLabel->setGeometry(10, 10, 80, 20);
    widthLabel->setGeometry(90, 10, 80, 20);
    goalLabel->setGeometry(170, 10, 80, 20);
    heightComboBox->setGeometry(10, 30, 80, 30);
    widthComboBox->setGeometry(90, 30, 80, 30);
    goalComboBox->setGeometry(170, 30, 80, 30);
    startButton->setGeometry(50, 70, 100, 30);
    turnLabel_->setGeometry(100, 100, 200, 30);

    for (const int& size : BOARD_SIZES_) {
        heightComboBox->addItem(QString::number(size));
        widthComboBox->addItem(QString::number(size));
    }

    goalComboBox->addItem("default");
    for (const int& goal : GOALS_) {
        goalComboBox->addItem(QString::number(goal));
    }

    connect(startButton, &QPushButton::clicked, this, [=]() {

        unsigned int height = heightComboBox->currentText().toInt();
        unsigned int width = widthComboBox->currentText().toInt();
        QString goalChoice = goalComboBox->currentText();
        unsigned int goal = (goalChoice == "default") ? 0 : goalChoice.toInt();

        turnLabel_->setText("Started");
        createGameBoard(height, width, goal);
    });
}

void MainWindow::changeTurnLabel() {

    unsigned int playerInTurn = gameBoard_->getPlayerInTurn();
    turnLabel_->setText("Player " + QString::number(playerInTurn) + "'s turn");
}

void MainWindow::createGameBoard(unsigned int height, unsigned int width, unsigned int goal) {

    if (goal == 0 || (goal > height && goal > width)) {
        gameBoard_ = new GameBoard(height, width);
    } else {
        gameBoard_ = new GameBoard(height, width, goal);
    }

    for (unsigned int row = 0; row < height; row++) {
        for (unsigned int col = 0; col < width; col++) {

            QPushButton* square = new QPushButton(this);

            unsigned int square_x = BOARD_X_COORD_ + col * SQUARE_SIZE_;
            unsigned int square_y = BOARD_Y_COORD_ + row * SQUARE_SIZE_;

            square->setGeometry(square_x, square_y, SQUARE_SIZE_, SQUARE_SIZE_);

            connect(square, &QPushButton::clicked, this, [=]() {
                setSquareValue({col,row}, square);
            });

            square->show();
        }
    }

    resize(BOARD_X_COORD_ + width * SQUARE_SIZE_ + MARGIN_,
           BOARD_Y_COORD_ + height * SQUARE_SIZE_ + MARGIN_);

    changeTurnLabel();
    return;
}

void MainWindow::setSquareValue(Coord coord, QPushButton* square) {

    SquareValue value = gameBoard_->getPlayerInTurn() == 1 ? X : O;
    gameBoard_->setValue(value, coord);

    std::string stringValue = value == X ? "X" : "O";

    square->setText(QString::fromStdString(stringValue));

    changeTurnLabel();
}
