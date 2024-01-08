#include "gameboard.hh"

using BoardIter = std::unordered_map<Coord, Square*, CoordHash>::iterator;
using BoardEntry = std::pair<const Coord, Square*>;
using NeighborEntry = std::pair<const Direction, Square*>;

GameBoard::GameBoard(const unsigned int height, const unsigned int width, unsigned int goal):
    BOARD_HEIGHT_(height), BOARD_WIDTH_(width), GOAL_(goal)
{
    // Init gameboard with empty squares
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            gameBoard_.emplace(Coord{i, j}, new Square{EMPTY, Coord{i, j}});
        }
    }

    // Add neighbors for squares
    for (BoardEntry& entry : gameBoard_) {
        Square* square = entry.second;
        addNeighbors(square);
    }
}

GameBoard::~GameBoard()
{
    for (BoardEntry& entry : gameBoard_) {
        delete entry.second;
    }
    gameBoard_.clear();
}

void GameBoard::clearBoard() {

    for (BoardEntry& boardEntry : gameBoard_) {
        Square* square = boardEntry.second;
        square->value = EMPTY;
    }
}

bool GameBoard::setValue(Coord coord, SquareValue value) {

    BoardIter square_it = gameBoard_.find(coord);

    if (square_it == gameBoard_.end()) {
        return false;
    }

    Square* square = square_it->second;
    square->value = value;

    turnsPlayed_ += 0.5;
    latestSquare_ = square;
    return true;
}

bool GameBoard::isGameWon() {

    if (turnsPlayed_ + 0.5 < GOAL_) {
        return false;
    }

    SquareValue valueToCheck = latestSquare_->value;

    // Check every neighbor of the latest modified square
    for (NeighborEntry entry : latestSquare_->neighbors) {

        Square* neighbor = entry.second;
        if (neighbor == nullptr || neighbor->value != valueToCheck) {
            continue;
        }

        Direction dir = getNeighborDirection(latestSquare_, neighbor);
        int valuesInDirection = countValuesInRow(latestSquare_, 0, valueToCheck, dir);

        Direction oppositeDir = getOppositeDirection(dir);
        int valuesInOpposite = countValuesInRow(latestSquare_, 0, valueToCheck, oppositeDir);

        // Sum of original square and values in both directions
        unsigned int sumOfValues = 1 + valuesInDirection + valuesInOpposite;

        if (sumOfValues >= GOAL_) {
            return true;
        }
    }

    return false;
}

unsigned int GameBoard::countValuesInRow(Square* square, int valuesInRow, SquareValue valueToCheck, Direction dir) {

    Square* neighbor = square->neighbors[dir];

    if (neighbor == nullptr || neighbor->value != valueToCheck) {
        return valuesInRow;
    }

    return countValuesInRow(neighbor, valuesInRow + 1, valueToCheck, dir);
}

Direction GameBoard::getNeighborDirection(Square* square, Square* neighbor) {

    Coord sqCoord = square->location;
    Coord nbCoord = neighbor->location;

    std::array<int, 2> diff = {
        static_cast<int>(nbCoord.x) - static_cast<int>(sqCoord.x),
        static_cast<int>(nbCoord.y) - static_cast<int>(sqCoord.y),
    };

    return getDirectionByValue(diff);
}

Direction GameBoard::getOppositeDirection(Direction dir) {

    std::array<int, 2> directionValue = getValueByDirection(dir);

    // Change to opposite direction value
    directionValue[0] *= -1;
    directionValue[1] *= -1;

    // Find direction by new value
    return getDirectionByValue(directionValue);
}

std::array<int, 2> GameBoard::getValueByDirection(Direction dir) {

    std::array<int, 2> directionValue = {};

    for (unsigned int i = 0; i < sizeof(DIRECTIONS_); i++) {
        if (dir == DIRECTIONS_[i]) {
            directionValue[0] = DIRECTION_VALUES_[i][0];
            directionValue[1] = DIRECTION_VALUES_[i][1];
        }
    }

    return directionValue;
}

Direction GameBoard::getDirectionByValue(std::array<int, 2> value) {

    for (unsigned int i = 0; i < sizeof(DIRECTIONS_); i++) {
        if (value[0] == DIRECTION_VALUES_[i][0] && value[1] == DIRECTION_VALUES_[i][1]) {
            return DIRECTIONS_[i];
        }
    }

    // This should never actually happen
    return NO_DIRECTION;
}

void GameBoard::addNeighbors(Square* square) {

    Coord sqCoord = square->location;

    for (unsigned int i = 0; i < sizeof(DIRECTIONS_); i++) {
        int xCoord = sqCoord.x + DIRECTION_VALUES_[i][0];
        int yCoord = sqCoord.y + DIRECTION_VALUES_[i][1];

        if (xCoord < 0 || yCoord < 0) {
            continue;
        }

        unsigned int newX = static_cast<unsigned int>(xCoord);
        unsigned int newY = static_cast<unsigned int>(yCoord);
        Coord newCoord = {newX, newY};

        BoardIter it = gameBoard_.find(newCoord);
        square->neighbors[DIRECTIONS_[i]] = (it != gameBoard_.end()) ? it->second : nullptr;
    }
}
