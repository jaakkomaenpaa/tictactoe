//
// Gameboard coordinates will be interpreted so that the bottom left corner has
// the coordinate of (0, 0) (sw). The x coordinate increases towards the right
// side and the y coordinate increases towards bottom


#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH


#include <limits>
#include <unordered_map>
#include <array>

enum SquareValue {EMPTY, X, O};
enum Direction {n, ne, e, se, s, sw, w, nw, NO_DIRECTION};

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

    void clearBoard();

    bool setValue(Coord coord, SquareValue value);

    bool isGameWon();



private:

    void addNeighbors(Square* square);
    unsigned int countValuesInRow(Square* square, int valuesInRow, SquareValue valueToCheck, Direction direction);
    Direction getNeighborDirection(Square* square, Square* neighbor);
    Direction getOppositeDirection(Direction dir);
    std::array<int, 2> getValueByDirection(Direction dir);
    Direction getDirectionByValue(std::array<int, 2> value);


    std::unordered_map<Coord, Square*, CoordHash> gameBoard_;

    const int DIRECTION_VALUES_[8][2] = {
        {0, 1},   // n
        {1, 1},   // ne
        {1, 0},   // e
        {1, -1},  // se
        {0, -1},  // s
        {-1, -1}, // sw
        {-1, 0},  // w
        {-1, 1}   // nw
    };

    const Direction DIRECTIONS_[8] = {n, ne, e, se, s, sw, w, nw};

    Square* latestSquare_ = nullptr;

    int turnsPlayed_ = 0;

    unsigned int BOARD_HEIGHT_ = NO_VALUE;
    unsigned int BOARD_WIDTH_ = NO_VALUE;
    unsigned int GOAL_ = NO_VALUE;
};

#endif // GAMEBOARD_HH
