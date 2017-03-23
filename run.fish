#!/usr/local/bin/fish
g++ *.cpp *.h -framework OpenGL -framework GLUT
./a.out
rm *.gch