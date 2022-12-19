#include<iostream>
using namespace std;
enum CellState { Empty, Deck, Miss, HitDeck }; // множество состояний одной клетки
enum ShipState { Destroyed, ShipHit, Safe }; // множество состояний корабля
class GameBoardCell { // класс - клетка игрового поля
private:
    int _x, _y;         // координаты клетки
    CellState _state;   // состояние клетки
public:
    GameBoardCell(int x = 0, int y = 0, CellState state = Empty)
    {
        _x = x;
        _y = y;
        _state = state;
    }
    void SetX(int x) // функция устанавливает x-кординату клети
    {
        _x = x;
    }
    void SetY(int y) // функция устанавливает y-кординату клети
    {
        _y = y;
    }
    void SetState(CellState state)    // функция устанавливает состояние клети
    {
        _state = state;
    }
    CellState GetState()    // функция возвращает состояние текуйщей клетки
    {
        return _state;
    }
    int GetX()    // функция возвращает x текуйщей клетки
    {
        return _x;
    }
    int GetY()    // функция возвращает y текуйщей клетки
    {
        return _y;
    }
    bool TryHit(int x, int y)    // функция - проверка попадания в клетку
    {
        return x == _x && y == _y && _state != HitDeck;
    }
    void Print()    // функция печати одной клетки, в зависимости от ее состояния
    {
        if (_state == Empty)
            cout << ' ';
        else if (_state == HitDeck)
            cout << 'x';
        else if (_state == Deck)
            cout << 'd';
        else
            cout << '*';
    }
};
class GameBoard; //аргумент функций класса Ship
class Ship // класс - корабль
{
private:
    GameBoardCell* _cells;  // клетки корабля
    ShipState _state;   // состояние корабля
    int _size;
public:
    Ship()
    {
        _cells = NULL;
        _size = 0;
    }
    void Create(GameBoard& gameBoard, int size, int x, int y, bool horizontal);
    ~Ship();
    ShipState GetState();     // функция возвращает состояние корабля, выполнив проверку целостности всех палуб
    void Shoot(GameBoard& gameBoard, int x, int y);    // функция выстрела по корабрю, возвращает состояние корябля, после выстрела
    bool TryHit(int x, int y);
};
class GameBoard { // класс - игровое поле
private:
    static const int _size = 10;            // размер игорового поля
    static const int _4DeckShipCount = 1;   // число 4-х палубных
    static const int _3DeckShipCount = 2;   // число 3-х палубных
    static const int _2DeckShipCount = 3;   // число 2-х палубных
    static const int _1DeckShipCount = 4;   // число 1-х палубных
    static const int _shipsCount = _4DeckShipCount + _3DeckShipCount + _2DeckShipCount + _1DeckShipCount;   // число кораблей
    GameBoardCell _cells[_size][_size]; // клетки игрового поля
    Ship _ships[_shipsCount];   // корабли
    void Generate();    // функция создание игрового поля
public:
    GameBoard()
    {
        Generate();
    }
    ~GameBoard() { }
    void SetState(int x, int y, CellState state)    // функция установки статуса клетки игровго поля
    {
        _cells[y][x].SetState(state);
    }
    bool IsDeck(int x, int y)    // функция возвращает является ли клетка палубой
    {
        return _cells[y][x].GetState() == Deck || _cells[y][x].GetState() == HitDeck;
    }
    void Print();    // функция печати игрового поля
    int GetCount()    // функция возвращает число клеток игровой области
    {
        return _size * _size;
    }
    int GetSize()   // функция возвращает размер игровой области
    {
        return _size;
    }
    void Shoot(int x, int y);    // функция - выстрел в игрове поле
    bool AllShipsDestroyed();    // функция провекри - уничтожены ли все корабли на поле
};
int main(int argc, char** argv)
{
    GameBoard gameBoard;
    gameBoard.Print();
    do {
        cout << "Enter x coord: " << endl;
        int x;
        cin >> x;
        cout << "Enter y coord: " << endl;
        int y;
        cin >> y;
        gameBoard.Shoot(x, y);
        gameBoard.Print();
    } while (!gameBoard.AllShipsDestroyed());
    return 0;
}
void Ship::Create(GameBoard& gameBoard, int size, int x, int y, bool horizontal)
{
    _size = size;
    _cells = new GameBoardCell[size];
    for (int i = 0; i < size; i++)    // заполняем клетки в зависимости от начала координат корябля и его направления
    {
        if (horizontal) {
            _cells[i].SetX(x + i);
            _cells[i].SetY(y);
            gameBoard.SetState(x + i, y, Deck);
        }
        else {
            _cells[i].SetX(x);
            _cells[i].SetY(y + i);
            gameBoard.SetState(x, y + i, Deck);
        }
    }
}
Ship::~Ship()
{
    // очищение памяти
    if (_size)
        delete _cells;
}
ShipState Ship::GetState()
{
    // число попаданий
    int hitCount = 0;
    for (int i = 0; i < _size; i++)
        if (_cells[i].GetState() == HitDeck)
            hitCount++;
    // проверка числа попаданий с числом палуб
    if (hitCount == 0)  // если не попали - корабль цел
        _state = Safe;
    else if (hitCount < _size)  // если попали но меньше раз, чем число палуб, то ранен
        _state = ShipHit;
    else
        _state = Destroyed; // иначе уничтожен
    return _state;
}
void Ship::Shoot(GameBoard& gameBoard, int x, int y) // с 168 строчки до 203 это я взял из интернета
{
    for (int i = 0; i < _size; i++)
        if (_cells[i].TryHit(x, y))
        {
            _cells[i].SetState(HitDeck);
            gameBoard.SetState(x, y, HitDeck);
            break;
        }
    if (GetState() == Destroyed)
    {
        for (int i = 0; i < _size; i++)
        {
            int localX = _cells[i].GetX();
            int localY = _cells[i].GetY();
            if (localX - 1 >= 0 && localY - 1 >= 0 && !gameBoard.IsDeck(localX - 1, localY - 1))
                gameBoard.SetState(localX - 1, localY - 1, Miss);
            if (localX - 1 >= 0 && !gameBoard.IsDeck(localX - 1, localY))
                gameBoard.SetState(localX - 1, localY, Miss);
            if (localX - 1 >= 0 && localY + 1 < gameBoard.GetSize() && !gameBoard.IsDeck(localX - 1, localY + 1))
                gameBoard.SetState(localX - 1, localY + 1, Miss);

            if (localY - 1 >= 0 && !gameBoard.IsDeck(localX, localY - 1))
                gameBoard.SetState(localX, localY - 1, Miss);
            if (localY + 1 < gameBoard.GetSize() && !gameBoard.IsDeck(localX, localY + 1))
                gameBoard.SetState(localX, localY + 1, Miss);

            if (localX + 1 < gameBoard.GetSize() && localY - 1 >= 0 && !gameBoard.IsDeck(localX + 1, localY - 1))
                gameBoard.SetState(localX + 1, localY - 1, Miss);
            if (localX + 1 < gameBoard.GetSize() && !gameBoard.IsDeck(localX + 1, localY))
                gameBoard.SetState(localX + 1, localY, Miss);
            if (localX + 1 < gameBoard.GetSize() && localY + 1 < gameBoard.GetSize() && !gameBoard.IsDeck(localX + 1, localY + 1))
                gameBoard.SetState(localX + 1, _cells[i].GetY() + 1, Miss);
        }
    }
}
bool Ship::TryHit(int x, int y)
{
    for (int i = 0; i < _size; i++)
        if (_cells[i].TryHit(x, y))
            return true;
    return false;
}
void GameBoard::Generate()
{
    for (int i = 0; i < _size; i++)    // заполнение игрового поля пустыми клетками
        for (int j = 0; j < _size; j++)
            _cells[i][j].SetState(Empty);
    int idx = 0;

    _ships[idx++].Create(*this, 4, 0, 0, true);    // расставление 4-х палубных
    _ships[idx++].Create(*this, 3, 0, 2, false);    // расставление 3-х палубных
    _ships[idx++].Create(*this, 3, 8, 1, false);
    _ships[idx++].Create(*this, 2, 3, 9, true);    // расставление 2-х палубных
    _ships[idx++].Create(*this, 2, 2, 6, true);
    _ships[idx++].Create(*this, 2, 6, 5, true);
    _ships[idx++].Create(*this, 1, 4, 2, true);    // расставление 1-о палубных
    _ships[idx++].Create(*this, 1, 8, 8, true);
    _ships[idx++].Create(*this, 1, 0, 8, true);
    _ships[idx++].Create(*this, 1, 6, 0, true);
}
void GameBoard::Print()
{
    for (int i = 0; i < _size; i++)
    {
        cout << "|";
        for (int j = 0; j < _size; j++)
        {
            _cells[i][j].Print();
            cout << "|";
        }
        cout << endl;
    }
}

void GameBoard::Shoot(int x, int y)
{
    for (int i = 0; i < _shipsCount; i++)    // просмотр всех кораблей
        if (_ships[i].TryHit(x, y))        // проверка попадания
        {
            _ships[i].Shoot(*this, x, y);            // если попадаем - стреляем по кораблю
            break;
        }
        else
        {
            _cells[y][x].SetState(Miss);            // иначе промах
        }
}
bool GameBoard::AllShipsDestroyed()
{
    for (int i = 0; i < _shipsCount; i++)    // обход всех кораблей
        if (_ships[i].GetState() != Destroyed)        // если хотя бы один не уничтожен, возвращаем false
            return false;
    return true;    // иначе true
}