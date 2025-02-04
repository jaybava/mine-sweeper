# Define the project type
TEMPLATE = app       # Type of project (application)
CONFIG += qt gui     # Enable Qt and GUI support
CONFIG -= app_bundle # Disable macOS app bundle (optional)

# Define the source files
SOURCES += main.cpp \
           Minesweeper.cpp

# Define the header files
HEADERS += Minesweeper.h

# Include additional Qt modules
QT += widgets         # Include the Qt Widgets module