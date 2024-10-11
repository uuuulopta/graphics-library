windows:
	g++ -g -o menu.exe menu.cpp libraries/*.cpp examples/*.cpp -lSDL2main -lmingw32 -lSDL2 -lSDL2_ttf -Wall

menu:
	g++ -g -o menu.exe menu.cpp libraries/*.cpp examples/*.cpp -lSDL2main -lSDL2 -lSDL2_ttf -Wall
