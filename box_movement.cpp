#include <raylib.h>
#include <vector>
#include <iostream>


class grid {
protected: 
    int gridWidth=570;
    int gridHeight=38;
public:
    int numRows;
    int numCols;
    int cellSize;

    grid() {
        numRows = 15;
        numCols = 10;
        cellSize = 40;
    }


};

class box : public grid{
public:
    int boxWidth;
    int boxHeight;

    box() {
        boxWidth = 38;
        boxHeight = 38;
    }
    void DrawGrid() {
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                DrawRectangle(j * cellSize, i * cellSize, cellSize - 2, cellSize - 2, WHITE);
            }
        }

    }
};

enum TetrominoType {
    I, O, T, L, J, S, Z
};

class Tetromino: public grid{
public:
    int rotationIndex;
    int shapeX, shapeY;
    TetrominoType type;
    std::vector<std::vector<int>> shape;

    Tetromino(TetrominoType t) {
        type = t;
        rotationIndex = 0;

        switch (type) {
            case I:
                shape = { {1, 1, 1, 1} }; // I-shape
                break;
            case O:
                shape = { {1, 1},
                          {1, 1} }; // O-shape
                break;
            case T:
                shape = { {0, 1, 0},
                          {1, 1, 1} }; // T-shape
                break;
            case L:
                shape = { {1, 0, 0},
                          {1, 1, 1} }; // L-shape
                break;
            case J:
                shape = { {0, 0, 1},
                          {1, 1, 1} }; // J-shape
                break;
            case S:
                shape = { {0, 1, 1},
                          {1, 1, 0} }; // S-shape
                break;
            case Z:
                shape = { {1, 1, 0},
                          {0, 1, 1} }; // Z-shape
                break;
        }
    }
};

class BlockArray: public grid {
public:
    std::vector<std::vector<int>> grid;  // Store settled blocks (1 = block, 0 = empty)

    BlockArray(int rows, int cols) {
        grid.resize(rows, std::vector<int>(cols, 0));  // Initialize empty grid
    }

    void AddToGrid(const Tetromino& tetromino, int shapeX, int shapeY, int cellSize) {
        // Add tetromino to the grid
        for (int row = 0; row < tetromino.shape.size(); row++) {
            for (int col = 0; col < tetromino.shape[row].size(); col++) {
                if (tetromino.shape[row][col] == 1) {
                    int gridX = (shapeX / cellSize) + col;
                    int gridY = (shapeY / cellSize) + row;
                    grid[gridY][gridX] = 1;  // Add block to grid
                }
            }
        }
    }

    bool CheckCollision(const Tetromino& tetromino, int shapeX, int shapeY, int cellSize) {
        // Check if tetromino collides with settled blocks
        for (int row = 0; row < tetromino.shape.size(); row++) {
            for (int col = 0; col < tetromino.shape[row].size(); col++) {
                if (tetromino.shape[row][col] == 1) {
                    int gridX = (shapeX / cellSize) + col;
                    int gridY = (shapeY / cellSize) + row;
                    if (gridY >= grid.size() || gridX >= grid[0].size() || gridX < 0 || grid[gridY][gridX] == 1) {
                         return true;  // Collision detected
                    }
                }
            }
        }
        return false;  // No collision
    }

    void ClearCompleteRows() {
        for (int row = 0; row < grid.size(); row++) {
            bool rowComplete = true;
            for (int col = 0; col < grid[row].size(); col++) {
                if (grid[row][col] == 0) {
                    rowComplete = false;
                    break;
                }
            }
            if (rowComplete) {
                grid.erase(grid.begin() + row);  // Remove the complete row
                grid.insert(grid.begin(), std::vector<int>(grid[0].size(), 0));  // Add new empty row at the top
            }
        }
    }
};

class TetroFunction{
    grid Grid;

public:
    Tetromino generateRandomTetromino() {
        TetrominoType type = static_cast<TetrominoType>(std::rand() % 7);  // Randomly select a type
        return Tetromino(type);
    }

    Tetromino Rotate(Tetromino tet, int rotationValue) {
        switch (tet.type) {
            case I:
                tet.shape = (rotationValue % 2 == 0) ? std::vector<std::vector<int>>{{1, 1, 1, 1}} : std::vector<std::vector<int>>{{1}, {1}, {1}, {1}};
                break;
            case O:
                tet.shape = {{1, 1}, {1, 1}};
                break;
            case T:
                if (rotationValue == 0) tet.shape = {{0, 1, 0}, {1, 1, 1}};
                if (rotationValue == 1) tet.shape = {{1, 0}, {1, 1}, {1, 0}};
                if (rotationValue == 2) tet.shape = {{1, 1, 1}, {0, 1, 0}};
                if (rotationValue == 3) tet.shape = {{0, 1}, {1, 1}, {0, 1}};
                break;
            case L:
                if (rotationValue == 0) tet.shape = {{1, 0, 0}, {1, 1, 1}};
                if (rotationValue == 1) tet.shape = {{1, 1}, {1, 0}, {1, 0}};
                if (rotationValue == 2) tet.shape = {{1, 1, 1}, {0, 0, 1}};
                if (rotationValue == 3) tet.shape = {{0, 1}, {0, 1}, {1, 1}};
                break;
            case J:
                if (rotationValue == 0) tet.shape = {{0, 0, 1}, {1, 1, 1}};
                if (rotationValue == 1) tet.shape = {{1, 0}, {1, 0}, {1, 1}};
                if (rotationValue == 2) tet.shape = {{1, 1, 1}, {1, 0, 0}};
                if (rotationValue == 3) tet.shape = {{1, 1}, {0, 1}, {0, 1}};
                break;
            case S:
                if (rotationValue == 0) tet.shape = {{0, 1, 1}, {1, 1, 0}};
                if (rotationValue == 1) tet.shape = {{1, 0}, {1, 1}, {0, 1}};
                break;
            case Z:
                if (rotationValue == 0) tet.shape = {{1, 1, 0}, {0, 1, 1}};
                if (rotationValue == 1) tet.shape = {{0, 1}, {1, 1}, {1, 0}};
                break;
        }
        return tet;
    }
};

int main() {
    // Initialize the window
    InitWindow(800, 600, "Tetris in Raylib");

    SetTargetFPS(60);

    grid Grid;
    box Box2;
    TetroFunction tf;
    box Box;

    BlockArray fallenBlocks(Grid.numRows, Grid.numCols);
    Tetromino currentBlock = tf.generateRandomTetromino();
    currentBlock.shapeX = (Grid.numCols / 2 - 1) * Grid.cellSize;
    currentBlock.shapeY = 0;

    int rotationIndex = 0;
    float fallSpeed = 0.5f;
    float fallTimer = 0.0f;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_RIGHT)) currentBlock.shapeX += Grid.cellSize;
        if (IsKeyPressed(KEY_LEFT)) currentBlock.shapeX -= Grid.cellSize;
        if (IsKeyPressed(KEY_DOWN)) currentBlock.shapeY += Grid.cellSize;
        if (IsKeyPressed(KEY_UP)) {
            rotationIndex = (rotationIndex + 1) % 4;
            currentBlock = tf.Rotate(currentBlock, rotationIndex);
        }

        if (fallenBlocks.CheckCollision(currentBlock, currentBlock.shapeX, currentBlock.shapeY, Grid.cellSize)) {
            currentBlock.shapeX -= Grid.cellSize;
        }

        fallTimer += GetFrameTime();

        if (fallTimer >= fallSpeed) {
            Tetromino temp = currentBlock;
            temp.shapeY += Grid.cellSize;

            if (!fallenBlocks.CheckCollision(temp, temp.shapeX, temp.shapeY, Grid.cellSize)) {
                currentBlock.shapeY += Grid.cellSize;
            } else {
                fallenBlocks.AddToGrid(currentBlock, currentBlock.shapeX, currentBlock.shapeY, Grid.cellSize);
                fallenBlocks.ClearCompleteRows();
                currentBlock = tf.generateRandomTetromino();
                currentBlock.shapeX = (Grid.numCols / 2 - 1) * Grid.cellSize;
                currentBlock.shapeY = 0;

                if (fallenBlocks.CheckCollision(currentBlock, currentBlock.shapeX, currentBlock.shapeY, Grid.cellSize)) {
                    std::cout << "Game Over!" << std::endl;
                    break;
                }
            }

            fallTimer = 0.0f;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        Box2.DrawGrid();

        // Draw current block
        for (int row = 0; row < currentBlock.shape.size(); row++) {
            for (int col = 0; col < currentBlock.shape[row].size(); col++) {
                if (currentBlock.shape[row][col] == 1) {
                    DrawRectangle(currentBlock.shapeX + col * Grid.cellSize, currentBlock.shapeY + row * Grid.cellSize,
                                  Box.boxWidth, Box.boxHeight, RED);
                }
            }
        }

        // Draw settled blocks
        for (int row = 0; row < Grid.numRows; row++) {
            for (int col = 0; col < Grid.numCols; col++) {
                if (fallenBlocks.grid[row][col] == 1) {
                    DrawRectangle(col * Grid.cellSize, row * Grid.cellSize, Box.boxWidth, Box.boxHeight, BLUE);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
