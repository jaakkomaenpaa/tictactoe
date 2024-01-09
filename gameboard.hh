//
// Gameboard coordinates will be interpreted so that the bottom left corner has
// the coordinate of (0, 0) (sw). The x coordinate increases towards the right
// side and the y coordinate increases towards bottom


#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH


#include <limits>
#include <unordered_map>
#include <array>

enum SquareValue {EMPTY, X, O, NO_SQUARE, I};
enum Direction {N, NE, E, SE, S, SW, W, NW, NO_DIRECTION};

using DirectionValue = std::array<int, 2>;

int const NO_VALUE = std::numeric_limits<unsigned int>::min();

struct Coord {
    unsigned int x = NO_VALUE;
    unsigned int y = NO_VALUE;
};

struct CoordHash {
    std::size_t operator()(Coord xy) const {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);

        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); }
inline bool operator<(Coord c1, Coord c2) {
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
};

Coord const NO_COORD = { NO_VALUE, NO_VALUE };


struct Square {

    SquareValue value = EMPTY;
    Coord location = NO_COORD;

    std::unordered_map<Direction, Square*> neighbors = {};

    Square(SquareValue i, Coord xy) : value(i), location(xy) {}
};


class GameBoard
{
public:
    GameBoard(unsigned int height, unsigned int width)
        : GameBoard(height, width, std::min(height, std::min(width, 5u))) {}

    GameBoard(const unsigned int height, const unsigned int width, unsigned int goal);

    ~GameBoard();

    // Sets all squares to empty
    void clearBoard();

    // Sets given value for the square on given coordinate
    bool setValue(SquareValue value, Coord coord);

    // Checks if game is won
    bool isGameWon();

    bool hasGameEnded();

    // Returns 1 or 2 depending on which player's turn it is
    unsigned int getPlayerInTurn();
    // Returns the value of square in given coordinate
    SquareValue getSquareValue(Coord coord);

private:

    // Counts how many of given value are on a row in given direction
    unsigned int countValuesInRow(Square* square, int valuesInRow, SquareValue valueToCheck, Direction dir);
    // Get direction of neighboring square
    Direction getNeighborDirection(Square* square, Square* neighbor);
    // Get direction opposite of the given one
    Direction getOppositeDirection(Direction dir);
    // Get a direction value by its code
    DirectionValue getValueByDirection(Direction dir);
    // Get a direction code by its value
    Direction getDirectionByValue(DirectionValue value);
    // Add all neighbors for a square
    void addNeighbors(Square* square);


    std::unordered_map<Coord, Square*, CoordHash> gameBoard_;

    const int DIRECTION_VALUES_[8][2] = {
        {0, 1},   // N
        {1, 1},   // NE
        {1, 0},   // E
        {1, -1},  // SE
        {0, -1},  // S
        {-1, -1}, // SW
        {-1, 0},  // W
        {-1, 1}   // NW
    };

    const Direction DIRECTIONS_[8] = {N, NE, E, SE, S, SW, W, NW};

    Square* latestSquare_ = nullptr;
    unsigned int turnsPlayed_ = 0;

    unsigned int BOARD_HEIGHT_ = NO_VALUE;
    unsigned int BOARD_WIDTH_ = NO_VALUE;
    unsigned int GOAL_ = NO_VALUE;
};

#endif // GAMEBOARD_HH
