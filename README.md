README.md

fish run.fish

Crear proyecto de xcode
Ir a la carpeta donde se encuentra el main.cpp
Ahi clonar o crear un git y jalar este repo
Configurar el proyecto de xcode para que use GLUT y OpenGL .frameworks
Asi mismo modificar el headers/libs search paths para que incluyan /usr/local/include y /usr/local/lib
Agregar a 'other linker flags' : '-lpng' , el cual debemos tener instalado, usar homebrew para que coincida con los paths de arriba (brew install libpng)
Hacer drag and drop desde el finder al proyecto de xcode para que el codigo pueda ver los files, no se por que no le aparecen ya por estar en el folder...
Configurar para que xcode use como source el folder que nos encontramos, 'comand' + '<' y de ahi options> working directory y le damos click al folder actual

Checar run.fish para ejecutar
