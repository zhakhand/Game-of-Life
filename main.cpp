#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>


const int MAX_ITERATIONS = 50;

class Grid {
private:
    std::vector<std::vector<int>>grid;
public:
    Grid(int rows, int columns) : grid(rows, std::vector<int>(columns)) {}

    static void drawGrid(const std::vector<std::vector<int>>& currentGrid) {
        std::cout << "┌";
        for (size_t j = 0; j < currentGrid[0].size() * 2; ++j) {
            std::cout << "─";
        }
        std::cout << "┐" << std::endl;

        for (size_t i = 0; i < currentGrid.size(); ++i) {
            std::cout << "│";
            for (size_t j = 0; j < currentGrid[0].size(); ++j) {
                std::cout << currentGrid[i][j] << " ";
            }
            std::cout << "│" << std::endl;
        }

        std::cout << "└";
        for (size_t j = 0; j < currentGrid[0].size() * 2; ++j) {
            std::cout << "─";
        }
        std::cout << "┘" << std::endl;
    }

    void setAlive(int x, int y) {
        if (x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size()) {
            grid[x][y] = 1;
        }
    }

    void generateCells() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int>dist(0, 1);
        for (size_t i = 0; i < grid.size(); ++i) {
            for (size_t j = 0; j < grid[0].size(); ++j) {
                grid[i][j] = dist(gen);
            }
        }
    }

    bool isAlive(const int x, const int y) {
        int alive{ 0 };
        // i found this function on youtube but i adjusted it to prevent overflow and out of range errors
        if (x > 0 && grid[x - 1][y] == 1) alive += 1; //left side
        if (x < grid.size()-1 && grid[x + 1][y] == 1) alive += 1; //right side
        if (y > 0 && grid[x][y - 1] == 1) alive += 1; // top
        if (y < grid[0].size()-1 && grid[x][y + 1] == 1) alive += 1; //bottom

        if (y > 0 && x > 0 && grid[x - 1][y - 1] == 1) alive += 1; //top left
        if (y > 0 && x < grid.size()-1 && grid[x + 1][y - 1] == 1) alive += 1; //top right
        if (y < grid[0].size()-1 && x > 0 && grid[x - 1][y + 1] == 1) alive += 1; //bottom left
        if (y < grid[0].size()-1 && x < grid.size()-1 && grid[x + 1][y + 1] == 1) alive += 1; //bottom right

        // live and fewer than 2 alive neighbours -> dies
        if (grid[x][y] == 1 && alive < 2) return false;
        // live and 2 or 3 neighbours -> lives
        if (grid[x][y] == 1 && (alive == 2 || alive == 3)) return true;
        // more than 3 neighbours -> dies
        if (alive > 3) return false;
        // if dead and 3 neighbours alive -> lives
        if (grid[x][y] == 0 && alive == 3) return true;

        return false;
    }
    std::vector<std::vector<int>> updateGrid() {
        std::vector<std::vector<int>> nextGeneration = grid;

        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[0].size(); ++j) {
                nextGeneration[i][j] = isAlive(i, j) ? 1 : 0;
            }
        }
        grid = nextGeneration;
        return nextGeneration;
    }
};

void clearScreen() { // i don't know which system you are using so i added this function
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

int main() {
    std::cout << "Welcome to the Conways Game Of Life!\n";
    int rows;
    std::cout << "Choose number of rows: ";
    std::cin >> rows;
    int columns;
    std::cout << "Choose number of columns: ";
    std::cin >> columns;
    Grid grid = Grid(rows, columns);
    char option;
    do {
        std::cout << "You want to fill the grid with random numbers? (y / n): ";
        std::cin >> option;
    }while(option != 'y' && option != 'n');

    if(option == 'y'){
        grid.generateCells();
    }
    if(option == 'n'){
        for (int i = 0; i < 5; ++i) {
            int x, y;
            std::cout << "Enter coordinates for living cell " << i + 1 << " (row column): ";
            std::cin >> x >> y;
            grid.setAlive(x, y);
        }
    }
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        grid.drawGrid(grid.updateGrid());

        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //set to 1 frame per second

        clearScreen();
    }
// P.S. I had a lot of fun trying to do it without any external libraries like SFML or raylib
// I will probably try to create more visually appealing version of this with raylib
// I was not sure if everything worked correctly, but I was seeing some patterns of behaviour for cells in GoL


    return 0;
}