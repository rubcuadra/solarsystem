#!/usr/local/bin/fish
#brew install png
#brew install mysql-connector-c++
#brew install sdl2
#brew install sdl2_mixer
#brew install boost

#g++ *.cpp -framework OpenGL -framework GLUT -I/usr/local/include -L/usr/local/lib -lpng -lmysqlcppconn -lSDL2_mixer -lSDL2 -lboost_filesystem -lboost_system
#./a.out
gcc -DHAVE_CONFIG_H -g -O2 -c loader/*.c
ar cru libglm.a loader/*.o
g++ -DHAVE_CONFIG_H -g -O2 -I/usr/local/include -c -std=c++11 *.cpp
g++ *.o libglm.a -L/usr/local/lib -framework OpenGL -framework GLUT -ljpeg -lpng -lIL -lm -lmysqlcppconn  -lSDL2_mixer -lSDL2 -lboost_filesystem -lboost_system
./a.out
rm *.o