#ifndef MINE_SWEEPER_MINESWEEPER_H
#define MINE_SWEEPER_MINESWEEPER_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QVector>
#include <QQueue>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>

/**
 * @brief The Minesweeper class represents the main game window and logic
 * Inherits from QWidget to create a window-based application
 */
class Minesweeper : public QWidget {
Q_OBJECT

public:
    /**
     * @brief Constructor for the Minesweeper game
     * @param parent Parent widget (default is nullptr for main window)
     */
    explicit Minesweeper(QWidget *parent = nullptr);

protected:
    /**
     * @brief Handles mouse press events for window dragging
     * @param event The mouse event containing press information
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse move events for window dragging
     * @param event The mouse event containing movement information
     */
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint dragPosition;        // Stores the position for window dragging
    const int width = 30;       // Width of the game grid
    const int height = 16;      // Height of the game grid
    const int bombCount = 99;   // Total number of bombs in the game
    int maxLives;              // Maximum lives for current difficulty
    int currentLives;          // Current remaining lives
    QVector<QPushButton*> buttons;  // Grid buttons
    QVector<bool> bombs;           // Bomb locations
    QVector<int> bombCounts;       // Number of adjacent bombs for each cell
    QWidget* mainMenu;            // Main menu widget
    QWidget* gameWidget;          // Game board widget

    /**
     * @brief Initializes the game grid with buttons
     */
    void initializeGrid();

    /**
     * @brief Places bombs randomly on the grid
     */
    void placeBombs();

    /**
     * @brief Calculates the number of adjacent bombs for each cell
     */
    void calculateBombCounts();

    /**
     * @brief Reveals a cell when clicked
     * @param row Row of the clicked cell
     * @param col Column of the clicked cell
     */
    void revealCell(int row, int col);

    /**
     * @brief Resets the game to initial state
     */
    void resetGame();

    /**
     * @brief Checks if the player has won
     * @return true if all non-bomb cells are revealed
     */
    bool checkWin();

    /**
     * @brief Handles right-click events for flagging cells
     * @param row Row of the right-clicked cell
     * @param col Column of the right-clicked cell
     */
    void handleRightClick(int row, int col);

    /**
     * @brief Shows the main menu with difficulty options
     */
    void showMainMenu();

    /**
     * @brief Starts a new game with specified number of lives
     * @param lives Number of lives for the game
     */
    void startGame(int lives);

    /**
     * @brief Centers the window on the screen
     */
    void centerWindow();

signals:
    void gameOver(bool win);
};

#endif //MINE_SWEEPER_MINESWEEPER_H
