#!/usr/local/bin/fish
#brew install png
#brew install cppconn ?
#brew install sdl2
#brew install sdl2_mixer
#brew install boost

g++ *.cpp -framework OpenGL -framework GLUT -I/usr/local/include -L/usr/local/lib -lpng -lmysqlcppconn -lSDL2_mixer -lSDL2 -lboost_filesystem -lboost_system
./a.out