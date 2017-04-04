#!/usr/local/bin/fish
g++ *.cpp *.h -framework OpenGL -framework GLUT -I/usr/local/include -L/usr/local/lib -lpng
./a.out
rm *.gch