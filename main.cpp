#include <cmath>
#include <cstdlib>
#include <iostream>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include "solarsystem.h"
#include "camera.h"
#include "constants.h"

//Material
GLfloat matSpecular[] = { 0.3, 0.3, 0.3, 1.0 }; //Color Charolazo(Blanco)
GLfloat matAmbience[] = { 0.3, 0.3, 0.3, 1.0 }; //Color del planeta
GLfloat matShininess[] = { 70.0 };              //Charolazo
//Luces
GLfloat lightAmbient[] = { 1.0, 1.0, 1.0, 0.5 }; //Ambiente
GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 }; //Difusa
GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 }; //Specular blanca
GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 }; //SOL

GLfloat sunAmbience[] = {1.0,1.0,0.0, 1.0 };

int screenWidth=1200,screenHeight=700;
bool showOrbits = false;
int planetSelected = 1; //A que planeta estaremos viendo (0 es el Sol)
SolarSystem solarSystem; //Vector de Planetas (Cada Planeta tiene x lunas)
Camera camera;

//Valores para controlar desplazamientos
double _time = 2.552f;
double timeSpeed = 0.1f;

//struct con banderas de movimiento, mientras esten true es que se mantiene apretado el boton
//Y por ende se debe mantener el movimiento hata que exista un ButtonUp que es soltar el boton
struct ControlStates{bool forward, backward, left, right, yawLeft, yawRight, pitchUp,pitchDown, rollLeft, rollRight;}
controls;


//Mandamos a llamar una funcion de tiempo en lugar de manejar el idle, dado que de esta manera
//podemos controlar los fps
void timer(int)
{
    glutPostRedisplay(); //invocamos func display
    glutTimerFunc(10, timer, 0); //100fps
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0); //Negro
    //Inits
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glShadeModel (GL_SMOOTH);
    //Materiales
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbience);
    //Luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glEnable (GL_LIGHT0);
    
    //Distancia desde el centro del Sol (km)
    //Tiempo de traslacion              (Dias terrestres)
    //Tiempo de rotacion                (Dias terrestres)
    //Radio                             (km)
    solarSystem.addPlanet(0, 1, 500, 695500,1.0,1.0,0.0);                            //Sun
    solarSystem.addPlanet(57910000, 88, 58.6, 2440,0.5529,0.1803,0.5921);            //Mercury
    solarSystem.addPlanet(108200000, 224.65, 243, 6052,0.6,0.5215,0.3882);           //Venus
    solarSystem.addPlanet(149600000, 365, 1, 6371,0.1294,0.1843,0.2863);             //Earth
    solarSystem.addPlanet(227939100, 686, 1.03f, 3389,0.9725,0.5843,0.3882);         //Mars
    solarSystem.addPlanet(778500000, 4332, 0.4139, 69911,0.5882,0.2941,0);           //Jupiter
    solarSystem.addPlanet(1433000000, 10759, 0.44375, 58232,0.8118,0.6902,0.5686);   //Saturn
    solarSystem.addPlanet(2877000000, 30685, 0.718056, 25362,0.8039,0.9529,0.9569);  //Uranus
    solarSystem.addPlanet(4503000000, 60188, 0.6713, 24622,0.2784,0.4392,0.9661);    //Neptune
    solarSystem.addPlanet(5906380000, 90616, 6.39, 1137,0.8510,0.6784,0.5255);       //Pluto
    
    //Numero de planeta
    //Distancia desde el centro de su planeta (km)
    //Dias de traslacion (orbita)
    //Dias de rotacion
    //radio
    
    solarSystem.addMoon(3, 7000000, 27.3, 27.3, 1738);         //Luna de la Tierra
    solarSystem.addMoon(6,39090000, 79,79,5150);               //Titan - Saturn
    solarSystem.addMoon(5,47090000, 3.51 , 3.51, 1568);        //Europa - Jupiter
    solarSystem.addMoon(5,87090000, 7.15 ,7.15, 5264);         //Ganymede - Jupiter
    solarSystem.addMoon(5,127090000, 17,17,4120);              //Callisto - Jupiter
    solarSystem.addMoon(8,16090000, -5.8,-5.8,1353);           //Triton- Neptune
    solarSystem.addMoon(7,41090000, 8.7,8.7,1578);             //Titania - Uranus
    
    //Numero de planeta
    //Distancia interna
    //Tiempo de rotacion
    //Distancia externa
    //Angulo sobre X
    
    solarSystem.addRing(6,20500,100,92000,15); //Saturno - Medio inclinado
    solarSystem.addRing(7,3000,100,46000,90);  //Uranus - Vertical
    solarSystem.addRing(8,4000,100,46000,6);   //Neptuno
    
    // reset controls
    controls.forward = false;
    controls.backward = false;
    controls.left = false;
    controls.right = false;
    controls.rollRight = false;
    controls.rollLeft = false;
    controls.pitchDown = false;
    controls.pitchUp = false;
    controls.yawLeft = false;
    controls.yawRight = false;
    
    timer(0);
}

void moveCamera() //Ajustamos camara
{
    
    if (controls.forward) camera.forward();		if (controls.backward) camera.backward();
    if (controls.left) camera.left();			if (controls.right) camera.right();
    if (controls.yawLeft) camera.yawLeft();		if (controls.yawRight) camera.yawRight();
    if (controls.rollLeft) camera.rollLeft();	if (controls.rollRight) camera.rollRight();
    if (controls.pitchUp) camera.pitchUp();		if (controls.pitchDown) camera.pitchDown();
    
    camera.transformOrientation();
    camera.transformTranslation();
}
void setUpPerspective() //Inicializar perspectiva
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //70 veces en x por cada y (Lo que se ve)
    //aspecto -> tamaño de pantalla ; zNear y far son los planos
    gluPerspective(70.0f, (float)screenWidth / (float)screenHeight, 0.001f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Buffers
    
    _time += timeSpeed;                         //Simular que el tiempo pasa
    solarSystem.calculatePositions(_time);      //Generar coordenadas basadas en tiempo
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); //Posicionar luz
    
    setUpPerspective();
    moveCamera();
    
    glEnable(GL_DEPTH_TEST);  //Que pasen detras de otros, permitir profundidades
        glEnable(GL_LIGHTING);    //Nos sirve para iluminar orbitas
            solarSystem.render(); // Pintar Sistema Solar
        glDisable(GL_LIGHTING);
        if (showOrbits) solarSystem.renderOrbits();
    glDisable(GL_DEPTH_TEST);
    
    glutSwapBuffers();        //End
}
void keyDown(unsigned char key, int x, int y)
{
    // check for numerical keys
    if ('0' <= key&&key <= '9')
    {
        if (key-'0' < solarSystem.getTotalPlanets())
        {
            float vec[3];             //Aqui guardaremos sus actuales coordenadas
            planetSelected = key-'0'; //Restar 0 es como convertirlo a numero
            solarSystem.getPlanetPosition(planetSelected, vec); //Obtener su posicion
            camera.pointAt(vec);
        }
    }
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case '-':
            timeSpeed /= 2.0f; // Disminuir velocidad
            break;
        case '=':
            timeSpeed *= 2.0f; // Aumentar velocidad
            break;
        case '[':
            planetSizeScale /= 1.2; // reducir planetas
            break;
        case ']':
            planetSizeScale *= 1.2; // Aumentar size planeta
            break;
        case 'o':
            showOrbits = !showOrbits; //Mostrar/ocultar orbitas
            break;
        case 'r':
            planetSizeScale = distanceScale;
            break;
        // Controles de la camara
        case ',':
            camera.slowDown();
            break;
        case '.':
            camera.speedUp();
            break;
        case 'w':
            controls.forward = true;
            break;
        case 's':
            controls.backward = true;
            break;
        case 'a':
            controls.left = true;
            break;
        case 'd':
            controls.right = true;
            break;
        case 'l':
            controls.rollRight = true;
            break;
        case 'j':
            controls.rollLeft = true;
            break;
        case 'i':
            controls.pitchDown = true;
            break;
        case 'k':
            controls.pitchUp = true;
            break;
        case 'q':
            controls.yawLeft = true;
            break;
        case 'e':
            controls.yawRight = true;
            break;
    }
}

void keyUp(unsigned char key, int x, int y) //Dejar de presionar una tecla
{
    switch (key)
    {
        case 'w':
            controls.forward = false;
            break;
        case 's':
            controls.backward = false;
            break;
        case 'a':
            controls.left = false;
            break;
        case 'd':
            controls.right = false;
            break;
        case 'l':
            controls.rollRight = false;
            break;
        case 'j':
            controls.rollLeft = false;
            break;
        case 'i':
            controls.pitchDown = false;
            break;
        case 'k':
            controls.pitchUp = false;
            break;
        case 'q':
            controls.yawLeft = false;
            break;
        case 'e':
            controls.yawRight = false;
            break;
    }
}

void reshape(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

void idle(){/*glutPostRedisplay(); //Mejor usamos timer para que sea tiempo mas real*/}
void printCommands()
{
    std::cout<<"Bienvenid@ a la nave 9102, es recomendable usar un teclado en formato US\nSus controles son:\n\t wasd -> Movimiento \n\t qe   -> Voltear vista costados\n\t ijkl -> Rolar/Maniobrar\n\t ,    -> Aumentar velocidad\n\t .    -> Disminuir Velocidad\n\n";
    std::cout<<"Otros comandos:\n\t o -> Muestras/Escondes orbita\n\t - -> El tiempo pasara mas lento\n\t = -> El tiempo pasara mas rapido\n\t r -> Tamaño de planetas real(No recomendable, no se ven)\n\t [ -> Disminuir tamaño de planetas \n\t ] -> Aumentar tamaño de planetas\n";
    std::cout<<"Podras apuntar la camara a algun planeta usando los numeros\nEj. Ver al sol '0',Tierra '3'...";
    
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(screenWidth,screenHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    glutSetWindowTitle("A01019102");
    printCommands();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutMainLoop();
    return 0;
}
