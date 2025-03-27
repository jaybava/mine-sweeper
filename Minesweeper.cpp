#include "Minesweeper.h"
#include <QRandomGenerator>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QMouseEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QScreen>

// Add these constants at the class level
const QString BASE_BUTTON_STYLE = "QPushButton { background-color: #c0c0c0; border: 1px solid #808080; }"
                                "QPushButton:hover { background-color: #d0d0d0; }";
const QString REVEALED_BUTTON_STYLE = "QPushButton { background-color: #00FF00; border: 1px solid #808080; }";
const QString REVEALED_NUMBER_STYLE = "QPushButton { background-color: #00FF00; border: 1px solid #808080; color: black; }";

// Constructor: Initializes the main game window with basic styling and size
Minesweeper::Minesweeper(QWidget *parent) : QWidget(parent) {
    setStyleSheet("Minesweeper { background-color: black; }"
                 "QMainWindow::title { background-color: black; color: white; }"
                 "QMainWindow { border: none; background: black; }");
    setFixedSize(800, 600);  // Set initial size for menu
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    
    showMainMenu();
}

// Centers the window on the screen based on screen geometry
void Minesweeper::centerWindow() {
    // Get the screen geometry
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    // Calculate the center position of the screen
    int x = (screenGeometry.width() - this->frameGeometry().width()) / 2;
    int y = (screenGeometry.height() - this->frameGeometry().height()) / 2;

    // Move the window to the calculated center position
    move(x, y);
}

// Creates and displays the main menu with difficulty options
void Minesweeper::showMainMenu() {
    // Clear any existing layout
    if (layout()) {
        QLayout* oldLayout = layout();
        QWidget().setLayout(oldLayout);
    }

    // Reset window size for menu
    setFixedSize(800, 600);
    
    // Center the window
    centerWindow();

    // Create main menu widget
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(20);

    // Create title label
    QLabel* titleLabel = new QLabel("MINESWEEPER", this);
    titleLabel->setStyleSheet("QLabel { color: white; font-size: 36px; font-weight: bold; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Create difficulty buttons
    QPushButton* easyButton = new QPushButton("Easy (4 Lives)", this);
    QPushButton* mediumButton = new QPushButton("Medium (2 Lives)", this);
    QPushButton* extremeButton = new QPushButton("Extreme (No Lives)", this);
    QPushButton* quitButton = new QPushButton("Quit", this);

    // Style the buttons with different colors
    QString baseButtonStyle = "QPushButton { "
                            "    border: 2px solid #808080; "
                            "    padding: 10px; "
                            "    min-width: 200px; "
                            "    font-size: 16px; "
                            "    color: black; "
                            "} ";

    easyButton->setStyleSheet(baseButtonStyle + 
                             "QPushButton { background-color: #90EE90; }" // Light green
                             "QPushButton:hover { background-color: #98FB98; }"); // Lighter green

    mediumButton->setStyleSheet(baseButtonStyle + 
                               "QPushButton { background-color: #FFFF99; }" // Light yellow
                               "QPushButton:hover { background-color: #FFFFC2; }"); // Lighter yellow

    extremeButton->setStyleSheet(baseButtonStyle + 
                                "QPushButton { background-color: #FFB6B6; }" // Light red
                                "QPushButton:hover { background-color: #FFC2C2; }"); // Lighter red

    quitButton->setStyleSheet(baseButtonStyle + 
                             "QPushButton { background-color: #c0c0c0; }" // Gray
                             "QPushButton:hover { background-color: #d0d0d0; }");

    // Connect buttons to their respective slots
    connect(easyButton, &QPushButton::clicked, this, [this]() { startGame(4); });
    connect(mediumButton, &QPushButton::clicked, this, [this]() { startGame(2); });
    connect(extremeButton, &QPushButton::clicked, this, [this]() { startGame(0); });
    connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    // Add buttons to layout
    mainLayout->addWidget(easyButton);
    mainLayout->addWidget(mediumButton);
    mainLayout->addWidget(extremeButton);
    mainLayout->addWidget(quitButton);

    setLayout(mainLayout);
    
    // Clear the buttons vector without manually deleting the buttons
    buttons.clear();
}

// Initializes a new game with the selected number of lives
void Minesweeper::startGame(int lives) {
    maxLives = lives;
    currentLives = lives;
    
    // Clear the menu layout
    if (layout()) {
        QLayout* oldLayout = layout();
        QWidget().setLayout(oldLayout);
    }

    // Initialize the game
    initializeGrid();
    
    // Calculate the window size based on the grid dimensions
    int windowWidth = (width * 40) + 20 + 4;
    int windowHeight = (height * 40) + 20 + 4;
    setFixedSize(windowWidth, windowHeight);
    
    // Center the window
    centerWindow();
}

// Sets up the game grid with buttons and connects signals/slots
void Minesweeper::initializeGrid() {
    QGridLayout *gridLayout = new QGridLayout(this);
    // Increase spacing between buttons to show more of the black background
    gridLayout->setSpacing(2);
    // Set margins around the grid to show black border
    gridLayout->setContentsMargins(10, 10, 10, 10);

    buttons.resize(width * height);
    bombs.resize(width * height);
    bombCounts.resize(width * height);

    placeBombs();
    calculateBombCounts();

    // Pre-calculate neighbor indices
    neighborIndices.resize(width * height);
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int index = row * width + col;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int newRow = row + dr;
                    int newCol = col + dc;
                    if (newRow >= 0 && newRow < height && newCol >= 0 && newCol < width) {
                        neighborIndices[index].append(newRow * width + newCol);
                    }
                }
            }
        }
    }

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int index = row * width + col;
            QPushButton *button = new QPushButton(this);
            button->setFixedSize(40, 40);  // Ensure consistent button size
            button->setContextMenuPolicy(Qt::CustomContextMenu);
            buttons[index] = button;

            // Add initial styling here
            button->setStyleSheet(BASE_BUTTON_STYLE);

            button->setProperty("isBomb", bombs[index]);
            button->setProperty("bombCount", bombCounts[index]);
            button->setProperty("revealed", false);
            button->setProperty("markState", 0);

            connect(button, &QPushButton::clicked, this, [this, row, col]() {
                revealCell(row, col);
            });

            connect(button, &QPushButton::customContextMenuRequested, this, [this, row, col]() {
                handleRightClick(row, col);
            });

            gridLayout->addWidget(button, row, col);
        }
    }
    setLayout(gridLayout);
}

// Randomly places bombs on the grid
void Minesweeper::placeBombs() {
    bombs.fill(false);
    int placedBombs = 0;
    while (placedBombs < bombCount) {
        int index = QRandomGenerator::global()->bounded(width * height);
        if (!bombs[index]) {
            bombs[index] = true;
            ++placedBombs;
        }
    }
}

// Calculates the number of adjacent bombs for each cell
void Minesweeper::calculateBombCounts() {
    bombCounts.fill(0);
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int index = row * width + col;
            if (bombs[index]) continue;

            int count = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    int newRow = row + dr;
                    int newCol = col + dc;
                    if (newRow >= 0 && newRow < height && newCol >= 0 && newCol < width) {
                        int neighborIndex = newRow * width + newCol;
                        if (bombs[neighborIndex]) {
                            ++count;
                        }
                    }
                }
            }
            bombCounts[index] = count;
        }
    }
}

// Handles cell revelation when clicked
// Implements flood-fill algorithm for empty cells
// Manages life system and game over conditions
void Minesweeper::revealCell(int row, int col) {
    int index = row * width + col;
    QPushButton *button = buttons[index];

    if (button->property("revealed").toBool()) {
        return;
    }

    button->setIcon(QIcon());
    button->setProperty("markState", 0);

    if (bombs[index]) {
        currentLives--;  // Decrease lives
        
        button->setText("");
        QIcon mineIcon("/home/jbava/projects/mine-sweeper/images/Mine.png");
        button->setIcon(mineIcon);
        button->setIconSize(QSize(30, 30));
        button->setStyleSheet("QPushButton { background-color: red; border: 1px solid #808080; padding: 0; }");
        button->setProperty("revealed", true);

        if (currentLives <= 0) {
            // Game Over
            showCenteredMessageBox("Game Over!", "You're out of lives!", QMessageBox::Reset | QMessageBox::Close);
        } else {
            // Show remaining lives message
            showCenteredMessageBox("Hit a Mine!", QString("Lives remaining: %1").arg(currentLives), QMessageBox::Ok);
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

        // Clear any flags or question marks for revealed cells
        curButton->setIcon(QIcon());
        curButton->setProperty("markState", 0);

        // Mark the cell as revealed and set its style
        curButton->setProperty("revealed", true);
        curButton->setStyleSheet(REVEALED_BUTTON_STYLE);

        // Show the bomb count if the cell has adjacent bombs
        int bombCount = bombCounts[curIndex];
        if (bombCount > 0) {
            curButton->setText(QString::number(bombCount));
            curButton->setStyleSheet(REVEALED_NUMBER_STYLE);
            continue;
        }

        // Enqueue all valid neighbors
        for (int neighborIndex : neighborIndices[curIndex]) {
            QPushButton *neighborButton = buttons[neighborIndex];

            if (!neighborButton->property("revealed").toBool() && !bombs[neighborIndex]) {
                queue.enqueue(qMakePair(neighborIndex / width, neighborIndex % width));
            }
        }
    }
    checkWin();
}

// Resets the game state while maintaining the current difficulty
void Minesweeper::resetGame() {
    currentLives = maxLives;  // Reset to the current difficulty's lives
    
    // Clear existing state
    bombs.fill(false);
    bombCounts.fill(0);

    // Reset all buttons
    for (auto* button : buttons) {
        resetButton(button);
    }

    placeBombs();
    calculateBombCounts();
}

// Verifies if all non-bomb cells are revealed for win condition
// Displays win message and handles game restart
bool Minesweeper::checkWin() {
    // Check if all non-bomb cells are revealed
    for (int i = 0; i < buttons.size(); ++i) {
        if (!bombs[i] && !buttons[i]->property("revealed").toBool()) {
            return false;
        }
    }

    // Create message box with restart option
    showCenteredMessageBox("Congratulations!", "You've won the game!", QMessageBox::Reset | QMessageBox::Close);

    return true;
}

// Handles right-click cell marking (flag -> question mark -> unmarked)
void Minesweeper::handleRightClick(int row, int col) {
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
            resetButton(button);
            break;
        }
        case 1: // Flagged
        {
            setButtonIcon(button, "/home/jbava/projects/mine-sweeper/images/Flag.png");
            break;
        }
        case 2: // Question mark
        {
            setButtonIcon(button, "/home/jbava/projects/mine-sweeper/images/Mark.png");
            break;
        }
    }
}

// Enables window dragging functionality
void Minesweeper::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

// Handles window movement during drag
void Minesweeper::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void Minesweeper::showCenteredMessageBox(const QString& title, const QString& text, 
                                       QMessageBox::StandardButtons buttons) {
    QMessageBox msgBox;
    msgBox.setParent(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(buttons);
    msgBox.setWindowModality(Qt::ApplicationModal);

    QPoint center = this->mapToGlobal(this->rect().center());
    QSize msgBoxSize = msgBox.sizeHint();
    msgBox.move(center.x() - msgBoxSize.width()/2, center.y() - msgBoxSize.height()/2);

    int ret = msgBox.exec();
    
    if (buttons & QMessageBox::Reset && ret == QMessageBox::Reset) {
        showMainMenu();
    } else if (buttons & QMessageBox::Close && ret == QMessageBox::Close) {
        QApplication::quit();
    }
}

void Minesweeper::resetButton(QPushButton* button) {
    button->setText("");
    button->setIcon(QIcon());
    button->setProperty("revealed", false);
    button->setStyleSheet(BASE_BUTTON_STYLE);
    button->setProperty("markState", 0);
}

void Minesweeper::setButtonIcon(QPushButton* button, const QString& iconPath) {
    button->setText("");
    QIcon icon(iconPath);
    button->setIcon(icon);
    button->setIconSize(QSize(30, 30));
    button->setStyleSheet(BASE_BUTTON_STYLE + "QPushButton { padding: 0; }");
}

