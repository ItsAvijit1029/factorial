#include<bits/stdc++.h>
#include <windows.h>
using namespace std;

// Forward declaration of the Board class
class Board;

// Base class for game objects
class GameObject {
protected:
    char symbol;
    int x, y;

public:
    GameObject(char sym = ' ')
    {
         symbol=sym;
         x=0;
         y=0;
    }

    virtual ~GameObject() {}

    virtual void setPosition(int posX, int posY) {
        x = posX;
        y = posY;
    }

    virtual char getSymbol() const {
        return symbol;
    }

    virtual int getX() const {
        return x;
    }

    virtual int getY() const {
        return y;
    }
};

// Food class derived from GameObject
class Food : public GameObject {
public:

Food(): GameObject('X') {}

    // Method to reset food position
    void resetPosition(const Board& board);
};

// Snake class derived from GameObject
class Snake : public GameObject {
private:
    char headSymbol;
    vector<pair<int, int>> body;
    int dirX, dirY;
    int score; // Score to track
    bool alive; // Game status

public:
    Snake(char headSym = '@'): GameObject(headSym), headSymbol(headSym), dirX(1), dirY(0), score(0), alive(true) {
        body.push_back({3,3}); // Initial position of snake
    }

    void move() {
        int newX = body[0].first + dirX;
        int newY = body[0].second + dirY;

        // Check if snake hits wall
        if (newX <= 0 || newY <= 0 || newX >= 51 || newY >= 21) {
            alive = false; // Set game status to false (snake died)
            return;
        }

        // Update the snake's body
        for (int i = body.size() - 1; i > 0; i--)
            body[i] = body[i - 1];

        body[0] = {newX, newY};
    }

    // Method to change snake's direction
    void setDirection(int dx, int dy) {
        dirX = dx;
        dirY = dy;
    }

    // Method to check if snake eats food
    bool eatsFood(const Food& food) {
        if (body[0].first == food.getX() && body[0].second == food.getY()) {
            score++; // Increment score
            // Add a new segment to the snake's body at the tail position
            body.push_back(body.back());
            return true;
        }
        return false;
    }

    // Getter for score
    int getScore() const {
        return score;
    }

    // Getter for game status
    bool isAlive() const {
        return alive;
    }

    // Method to get snake's body
    const vector<pair<int, int>>& getBody() const {
        return body;
    }
};

// Board class
class Board {
private:
    int height, width;
    char boundarySymbol;

public:
    Board(int h = 20, int w = 50, char sym = '*')
    {
         height=h;
         width=w;
         boundarySymbol=sym;
    }

    void showBoard(const Snake& snake, const Food& food) const {
        for (int i = 0; i <= height + 1; i++) {
            for (int j = 0; j <= width + 1; j++) {
                bool objectFound = false;
                for (const auto& part : snake.getBody()) {
                    if (part.first == j && part.second == i) {
                        cout << snake.getSymbol();
                        objectFound = true;
                        break;
                    }
                }
                if (!objectFound) {
                    if (i == 0 || i == height + 1 || j == 0 || j == width + 1)
                        cout << boundarySymbol;
                    else if (i == food.getY() && j == food.getX())
                        cout << food.getSymbol();
                    else
                        cout << ' ';
                }
            }
            cout << endl;
        }
        cout << "Score: " << snake.getScore() << endl; // Print score
    }

    // Getter for board width
    int getBoardWidth() const {
        return width;
    }

    // Getter for board height
    int getBoardHeight() const {
        return height;
    }
};

// Method to reset food position
void Food::resetPosition(const Board& board) {
    srand(time(0));
    x = 1 + rand() % board.getBoardWidth();
    y = 1 + rand() % board.getBoardHeight();
}

// Main function
int main() {
    srand(time(0)); // Seed for random number generation

    Board board; // Create the game board

    // Create objects
    Snake snake;
    Food food;

    // Set initial positions
    snake.setPosition(3, 3);
    food.resetPosition(board); // Pass the Board object

    // Game mode selection
    int mode;
    cout << "Select game mode: " << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    cin >> mode;

    // Adjust game speed based on mode
    int speed;
    switch (mode) {
        case 1:
            speed = 200; // Easy mode
            break;
        case 2:
            speed = 100; // Medium mode
            break;
        case 3:
            speed = 50; // Hard mode
            break;
        default:
            cout << "Invalid mode. Using medium mode by default." << endl;
            speed = 100;
            break;
    }

    // Loop until the game ends
    while (snake.isAlive()) {
        // Clear screen
        system("cls");

        // Move snake (modify as needed)
        snake.move();

        // Check if snake eats food
        if (snake.eatsFood(food)) {
            food.resetPosition(board); // Reset food position
        }

        // Display the game board
        board.showBoard(snake, food);

        // Optional: Add a delay for controlling game speed
        Sleep(speed); // Adjust as needed

        // Optional: Get user input to change snake direction
        if (GetAsyncKeyState(VK_UP))
            snake.setDirection(0, -1); // Move up
        else if (GetAsyncKeyState(VK_DOWN))
            snake.setDirection(0, 1); // Move down
        else if (GetAsyncKeyState(VK_LEFT))
            snake.setDirection(-1, 0); // Move left
        else if (GetAsyncKeyState(VK_RIGHT))
            snake.setDirection(1, 0); // Move right
    }

    // Game over
    cout << "Score: " << snake.getScore() << endl; // Print score
    cout << "SNAKE DEAD.....!!!!" << endl;
    system("pause"); // Pause to display the message
    return 0;
}

