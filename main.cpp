#include <QApplication>
#include <QDebug>


#include "Minesweeper.h"
/*
// Create an object called Minesweeper that returns a QWidget
class  Minesweeper : public QWidget{
public:
    // Pointer to all the buttons
    QVector<QPushButton*> buttons;
    // List to tell whether a cell is a bomb
    QVector<bool> bombs;
    // List with bomb counts for each cell
    QVector<int> bombCounts;
    Minesweeper(QWidget *parent = nullptr) : QWidget(parent){
        // set constraints for the game board
        const int width = 30;
        const int height = 16;
        const int bombCount = 99;
        const int totalCells = width*height;


        // Create a gridlayout pointer
        QGridLayout *gridLayout = new QGridLayout(this);
        // Set spacing between buttons
        gridLayout->setSpacing(1);

        // Set button's size to totalCell count
        buttons.resize(totalCells);
        // Set bombs size to totalCells
        bombs.resize(totalCells);
        // Fill bombs list with false
        bombs.fill(false);
        // Size the bombCount with totalCells
        bombCounts.resize(totalCells);
        // fill bomb counts with 0
        bombCounts.fill(0);

        // counter for placed bombs
        int placedBombs = 0;
        while (placedBombs < bombCount){
            // generate a random number upper bounded by total cells
            int index = QRandomGenerator::global()->bounded(totalCells);
            // if the index isn't already a bomb
            if(!bombs[index]){
                // set the bomb to true
                bombs[index] = true;
                // increase counter
                placedBombs++;
            }
        }

        // Calculate the bomb counts for each cell
        for (int row = 0; row < height; ++row) {
            // loop through each cell
            for (int col = 0; col < width; ++col) {
                // calculate the index
                int index = row * width + col;
                // if cell at index is a bomb move to next cell
                if (bombs[index]) {
                    continue;
                }

                // set the bombCount to 0
                int bombCount = 0;
                // look into all 8 neighbouring cells
                for(int dr = -1; dr <= 1; ++dr){
                    for (int dc = -1; dc <= 1; ++dc){
                        // calculate the neighbour cell's coordinate
                        int newRow = row + dr;
                        int newCol = col + dc;
                        // check if this coordinate is in the boundary of the grid
                        if (newRow >= 0 && newRow < height && newCol >= 0 && newCol < width){
                            // calculate the index of this coordinate
                            int neighborIndex = newRow * width+ newCol;
                            // if bomb exists
                            if (bombs[neighborIndex]){
                                // increase bombCount
                                bombCount++;
                            }
                        }
                    }
                }
                // put bombCount in bombCounts
                bombCounts[index] = bombCount;
            }
        }

        // Iterate through the grid
        for (int row = 0; row < height; ++row){
            for(int col = 0; col < width; ++col){
                // Calculate index
                int index = row * width + col;
                // Create a new button
                QPushButton *button = new QPushButton(this);
                // set the size of the button
                button->setFixedSize(40, 40);
                // style the button
                button->setStyleSheet("QPushButton { background-color: #c0c0c0; border: 1px solid #808080; }"
                     "QPushButton:hover { background-color: #d0d0d0; }");
                // add to buttons list
                buttons[index] = button;

                // set properties for the button
                button->setProperty("isBomb", bombs[index]);
                button->setProperty("bombCount", bombCounts[index]);
                button->setProperty("index", index);
                button->setProperty("revealed", false);
                button->setProperty("markState", 0);  // 0: unmarked, 1: flagged, 2: question mark

                // Left click
                // Connect the revealCell function
                connect(button, &QPushButton::clicked, this, [this, row, col, width, height](){
                    revealCell(row, col, width, height);
                });

                // Right click
                // allow for custom handling of the right click
                button->setContextMenuPolicy(Qt::CustomContextMenu);
                // Connect the handleRightClick function
                connect(button, &QPushButton::customContextMenuRequested, this, [this, row, col, width](){
                    handleRightClick(row, col, width);
                });

                // Add button to gridLayout
                gridLayout->addWidget(button, row, col);
            }
        }
        // Assign the layout to the Minesweeper widget
        setLayout(gridLayout);
    }
private:
    // Function to check if the game is won
    void checkWin() {
        // Check if all non-bomb cells are revealed
        for (int i = 0; i < buttons.size(); ++i) {
            if (!bombs[i] && !buttons[i]->property("revealed").toBool()) {
                return; // Found an unrevealed safe cell, game not won yet
            }
        }
        
        // Create message box with restart option
        QMessageBox msgBox(this);  // Set parent to this widget
        msgBox.setWindowTitle("Congratulations!");
        msgBox.setText("You've won the game!");
        msgBox.setStandardButtons(QMessageBox::Reset | QMessageBox::Close);
        msgBox.setDefaultButton(QMessageBox::Reset);

        // Center the message box relative to the game window
        msgBox.setWindowModality(Qt::WindowModal);
        
        int ret = msgBox.exec();
        
        if (ret == QMessageBox::Reset) {
            // Reset the game
            resetGame();
        }
    }

    // Function to reset Game
    void resetGame() {
        // Set game constraints
        const int width = 30;
        const int height = 16;
        const int bombCount = 99;
        const int totalCells = width * height;

        // Clear existing state
        bombs.fill(false);
        bombCounts.fill(0);

        // Reset all buttons
        for (auto* button : buttons) {
            button->setText("");
            button->setIcon(QIcon());  // Clear any icons (flags, mines, question marks)
            button->setProperty("revealed", false);
            button->setStyleSheet("QPushButton { background-color: #c0c0c0; border: 1px solid #808080; }"
                                "QPushButton:hover { background-color: #d0d0d0; }");
            button->setProperty("markState", 0);
        }

        // Place new bombs
        int placedBombs = 0;
        while (placedBombs < bombCount) {
            int index = QRandomGenerator::global()->bounded(totalCells);
            if (!bombs[index]) {
                bombs[index] = true;
                placedBombs++;
            }
        }

        // Recalculate bomb counts
        for (int row = 0; row < height; ++row) {
            for (int col = 0; col < width; ++col) {
                int index = row * width + col;
                if (bombs[index]) {
                    continue;
                }

                int bombCount = 0;
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        int newRow = row + dr;
                        int newCol = col + dc;
                        if (newRow >= 0 && newRow < height && newCol >= 0 && newCol < width) {
                            int neighborIndex = newRow * width + newCol;
                            if (bombs[neighborIndex]) {
                                bombCount++;
                            }
                        }
                    }
                }
                bombCounts[index] = bombCount;
            }
        }
    }

    // Function to reveal Cells
    void revealCell(int row, int col, int width, int height) {
        // Calculate index
        int index = row * width + col;
        QPushButton *button = buttons[index];


        // Skip already revealed cells
        if (button->property("revealed").toBool()) {
            return;
        }

        // Handle bomb case
        if (bombs[index]) {
            button->setText("");  // Clear any text
            QIcon mineIcon("/home/jbava/projects/mine-sweeper/images/Mine.png");
            button->setIcon(mineIcon);
            button->setIconSize(QSize(30, 30));
            button->setStyleSheet("QPushButton { background-color: red; border: 1px solid #808080; padding: 0; }");
            button->setProperty("revealed", true);
            
            // Create message box with restart option
            QMessageBox msgBox(this);  // Set parent to this widget
            msgBox.setWindowTitle("Game Over!");
            msgBox.setText("You hit a mine!");
            msgBox.setStandardButtons(QMessageBox::Reset | QMessageBox::Close);
            msgBox.setDefaultButton(QMessageBox::Reset);

            // Center the message box relative to the game window
            msgBox.setWindowModality(Qt::WindowModal);
            
            int ret = msgBox.exec();
            
            if (ret == QMessageBox::Reset) {
                resetGame();
            } else if (ret == QMessageBox::Close) {
                QApplication::quit();  // Exit the application
            }
            
            return;
        }

        // Use a queue for BFS
        QQueue<QPair<int, int>> queue;
        queue.enqueue(qMakePair(row, col));

        while (!queue.isEmpty()) {
            QPair<int, int> current = queue.dequeue();
            int curRow = current.first;
            int curCol = current.second;
            int curIndex = curRow * width + curCol;
            QPushButton *curButton = buttons[curIndex];

            // Skip already revealed cells
            if (curButton->property("revealed").toBool()) {
                continue;
            }

            // Mark the cell as revealed and set its style
            curButton->setProperty("revealed", true);
            curButton->setStyleSheet("QPushButton { background-color: #00FF00; border: 1px solid #808080; }");

            // Show the bomb count if the cell has adjacent bombs
            int bombCount = bombCounts[curIndex];
            if (bombCount > 0) {
                curButton->setText(QString::number(bombCount));
                curButton->setStyleSheet("QPushButton { background-color: #00FF00; border: 1px solid #808080; color: black; }");
                continue;
            }

            // Enqueue all valid neighbors
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int newRow = curRow + dr;
                    int newCol = curCol + dc;

                    if (newRow >= 0 && newRow < height && newCol >= 0 && newCol < width) {
                        int neighborIndex = newRow * width + newCol;
                        QPushButton *neighborButton = buttons[neighborIndex];

                        if (!neighborButton->property("revealed").toBool() && !bombs[neighborIndex]) {
                            queue.enqueue(qMakePair(newRow, newCol));
                        }
                    }
                }
            }
        }
        checkWin();
    }

    // Function to handle right clicks
    void handleRightClick(int row, int col, int width) {
        int index = row * width + col;
        QPushButton *button = buttons[index];
        
        // Skip if already revealed
        if (button->property("revealed").toBool()) {
            return;
        }

        // Get current state of the button
        int currentState = button->property("markState").toInt();
        // Cycle through states: 0 -> 1 -> 2 -> 0
        currentState = (currentState + 1) % 3;
        // Set the new state
        button->setProperty("markState", currentState);

        switch (currentState) {
            case 0: // Unmarked
            {
                button->setText("");
                button->setIcon(QIcon());  // Clear the icon
                button->setStyleSheet("QPushButton { background-color: #c0c0c0; border: 1px solid #808080; }"
                                    "QPushButton:hover { background-color: #d0d0d0; }");
                break;
            }
            case 1: // Flagged
            {
                button->setText("");  // Clear any text
                QIcon flagIcon("/home/jbava/projects/mine-sweeper/images/Flag.png");
                button->setIcon(flagIcon);
                button->setIconSize(QSize(30, 30));  // Made icon size match button size
                button->setStyleSheet("QPushButton { background-color: #c0c0c0; border: 1px solid #808080; padding: 0; }"
                                    "QPushButton:hover { background-color: #d0d0d0; }");
                break;
            }
            case 2: // Question mark
            {
                button->setText("");  // Clear any text
                QIcon questionIcon("/home/jbava/projects/mine-sweeper/images/Mark.png");
                button->setIcon(questionIcon);
                button->setIconSize(QSize(30, 30));
                button->setStyleSheet("QPushButton { background-color: #c0c0c0; border: 1px solid #808080; padding: 0; }"
                                      "QPushButton:hover { background-color: #d0d0d0; }");
                break;
            }
        }
    }
};
 */

int main(int argc, char *argv[]) {
    // Create instance of QApplication
    QApplication app(argc, argv);

    // Create instance of Minesweeper clas
    Minesweeper game;
    // Set title of the window
    game.setWindowTitle("Minesweeper");
    // show the window
    game.show();

    // start event loop
    return app.exec();
}
