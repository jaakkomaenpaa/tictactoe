#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameboard.hh"

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

const int BOARD_SIZES_[10] = {
    3, 4, 5, 6, 7, 8, 9, 10, 11, 12
};

const int GOALS_[4] = {
    3, 4, 5, 6
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void createUI();

    void changeTurnLabel();

    void createGameBoard(unsigned int height, unsigned int width, unsigned int goal = 0);

    void setSquareValue(Coord coord, QPushButton* square);

private:

    Ui::MainWindow *ui;

    QLabel* turnLabel_;

    GameBoard* gameBoard_;

    const unsigned int BOARD_X_COORD_ = 30;
    const unsigned int BOARD_Y_COORD_ = 170;
    const unsigned int SQUARE_SIZE_ = 70;
    const unsigned int MARGIN_ = 30;

};
#endif // MAINWINDOW_HH
