#!/usr/local/bin/fish
#Tener Frameworks de OpenGL (Vienen con XCode)
#brew install png
#brew install mysql-connector-c++
#brew install sdl2
#brew install sdl2_mixer
#brew install boost
#brew install glm

cd loader
gcc -DHAVE_CONFIG_H -g -O2 -c *.c
#gcc -DHAVE_CONFIG_H -g -O2 -I/usr/local/include -c *.c
ar cru libglm.a *.o
mv libglm.a ..
cd ..
g++ -DHAVE_CONFIG_H -g -O2 -I/usr/local/include -c -std=c++11 *.cpp
g++ *.o libglm.a -L/usr/local/lib -framework OpenGL -framework GLUT -ljpeg -lpng -lIL -lm -lmysqlcppconn  -lSDL2_mixer -lSDL2 -lboost_filesystem -lboost_system
#g++ -DHAVE_CONFIG_H -g -O2 -c -std=c++11 *.cpp
#g++ *.o libglm.a -L/usr/local/lib -lGL -lGLU -lglut -ljpeg -lpng -lIL -lm -lmysqlcppconn  -lSDL2_mixer -lSDL2 -lboost_filesystem -lboost_system
./a.out
rm loader/*.o