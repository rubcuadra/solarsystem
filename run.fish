#!/usr/local/bin/fish
g++ *.cpp -framework OpenGL -framework GLUT -I/usr/local/include -L/usr/local/lib -lpng -lmysqlcppconn
./a.out