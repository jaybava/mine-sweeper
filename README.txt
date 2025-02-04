A fully-featured Minesweeper game built with C++ and Qt, offering a clean, interactive UI and multiple difficulty levels. Navigate the minefield, reveal safe cells, and strategically place flags to avoid hidden mines!

🚀 Features
🖥 Built with C++ and Qt for a modern and responsive GUI.
🎛 Main Menu with 3 Difficulty Levels:
	Beginner (4 lives)
	Intermediate (2 lives)
	Expert (no lives)
🔍 Smart Reveal System using BFS (Breadth-First Search) to uncover empty spaces efficiently.
🏴 Right-Click Flagging: Mark suspected mines with flags or question marks.
🔔 Win/Loss Detection: Game alerts when you win or hit a mine, with the option to restart.
🎨 Polished UI with a dynamic layout and real-time updates.
📂 Modular Codebase: Separated into Minesweeper.h, Minesweeper.cpp, and main.cpp for clean organization.
🛠 Installation & Build Instructions
Using qmake (Linux/macOS/Windows):
	git clone https://github.com/jay-bava/mine-sweeper.git
	cd mine-sweeper
	qmake Minesweeper.pro
	make (or mingw32-make on Windows)
	./Minesweeper
Using Qt Creator:
	Open Minesweeper.pro in Qt Creator.
	Click Build (🔨) and then Run (▶).
