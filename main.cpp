#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <png.h>

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
#include "galaxy.h"
#include "fleet.h"
#include "rand.h"
#include "player.h"
#include "loader/glm.h"
#include "texturizer.h"

#include <boost/format.hpp>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

//A donde apuntan las naves, Hacer que un vector apunte a otro
//Agregar Anillos o algo de ese tipo

Texturizer *hud;

Player *player;
//Material
GLfloat matSpecular[] = { 0.3, 0.3, 0.3, 1.0 }; //Color Charolazo(Blanco)
GLfloat matAmbience[] = { 0.3, 0.3, 0.3, 1.0 }; //Color del planeta
GLfloat matShininess[] = { 70.0 };              //Charolazo
//Luces
GLfloat lightAmbient[] = { 0.1, 0.1, 0.1, 0.5 }; //Ambiente
GLfloat lightDiffuse[] = { 0.1, 0.1, 0.1, 1.0 }; //Difusa
GLfloat lightSpecular[] = { 1, 1, 1, 1.0 }; //Specular blanca

GLfloat sunAmbience[] = {1.0,1.0,0.0, 1.0 };

int screenWidth=1200,screenHeight=700;
bool showOrbits = false;
unsigned seed;
GLenum *lights;

float lookingAt[3];             //Aqui guardaremos a donde estamos viendo
int current_ship = 0;

Camera camera;
SolarSystem * temp = nullptr;

RandomGalaxy *galaxy;
RandomFleet *fleet;

std::vector<char *> huds;
char filename[] = "assets/imgs/cockpit.png";
char obj_filename[] = "assets/models/menu_arwing/menu_arwing.obj";

//Valores para controlar desplazamientos
double _time = 2.552f;
double timeSpeed = 0.1f;

//struct con banderas de movimiento, mientras esten true es que se mantiene apretado el boton
//Y por ende se debe mantener el movimiento hata que exista un ButtonUp que es soltar el boton
struct ControlStates{bool forward, backward, left, right, yawLeft, yawRight, pitchUp,pitchDown, rollLeft, rollRight;}
controls;

int linestart = 10;		/* start point on y axis for text lines */
int linespace = 20;		/* spac betwwen text lines */
void *Help_Font = GLUT_BITMAP_8_BY_13;

//Mandamos a llamar una funcion de tiempo en lugar de manejar el idle, dado que de esta manera
//podemos controlar los fps
void timer(int)
{
    glutPostRedisplay(); //invocamos func display
    glutTimerFunc(10, timer, 0); //100fps
}

void getDataFromDB( std::string query , int columns ) //Region = seed
{
    try
    {
        std::cout << query << "\n";
        
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        
        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
        /* Connect to the MySQL test database */
        con->setSchema("dbii");
        
        stmt = con->createStatement();
        res = stmt->executeQuery(query);
        
        while (res->next())
        {
            
            std::cout << res->getInt("f") << " "<< res->getString("s")<< "\n";
            
            /* Access column data by alias or column name */
            //std::cout << res->getInt("rid") <<","<<res->getInt("gid")<<","<<res->getString("name")<<"\n";
            /* Access column data by numeric offset, 1 is the first column */
            //std::cout << "\t... MySQL says it again: ";
        }
        delete res;
        delete stmt;
        delete con;
        
    } catch (sql::SQLException &e)
    {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << "\n";
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << "\n";
    }
}

void getRegionByCoords( float x,float y,float z)
{
    //Osea el de obtener la region dadas unas coordenadas
    //inputs: coordenadas {x, y, z} dadas por el usuario
    std::string query = boost::str(boost::format(
                                                 "SELECT r_id as f, name as s FROM region WHERE %1% <= xmax AND %1% >= xmin AND %2% <= ymax AND %2% >= ymin AND %3% <= zmax AND %3% >= zmin;"
                                                 ) % x % y % z);
    getDataFromDB(query, 2);
}

void getPlanetsByRegion(std::string region)
{
    //Todos los planetas dentro de la Región 7
    //inputs: región {r} dada por el usuario
    std::string query = boost::str(boost::format(
                                                 "SELECT planet.p_id as f, planet.name as s FROM planet JOIN solar_system ON planet.ss_id = solar_system.ss_id JOIN region ON solar_system.r_id = region.r_id WHERE region.name = '%1%';"
                                                 ) % region);
    getDataFromDB(query, 2);
}

void getPlanetsInRadius(int planetId, float distance_radius)
{
    //Todos los planetas a menos de 1,000,000 km de mi planeta
    //inputs: p_id {p} de un planeta
    std::string query = boost::str(boost::format(
                                                 "SELECT p_id as f, name as s FROM planet WHERE p_id in (SELECT p_id FROM planet WHERE sqrt(power((xmax - xmin) - (SELECT (xmax - xmin) FROM planet WHERE p_id = %1%), 2) + power((ymax - ymin) - (SELECT (ymax - ymin) FROM planet WHERE p_id = %1%), 2) + power((zmax - zmin) - (SELECT (zmax - zmin) FROM planet WHERE p_id = %1%), 2)) < %2%);"
                                                 ) % planetId % distance_radius);
    getDataFromDB(query, 2);
}

void initLights()
{
    //Materiales
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbience);
    //Luz
    lights = new GLenum[8];
    lights[0] = GL_LIGHT0;
    lights[1] = GL_LIGHT1;
    lights[2] = GL_LIGHT2;
    lights[3] = GL_LIGHT3;
    lights[4] = GL_LIGHT4;
    lights[5] = GL_LIGHT5;
    lights[6] = GL_LIGHT6;
    lights[7] = GL_LIGHT7;
}

void resetControls()
{
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
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0); //Negro
    //Inits
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glShadeModel (GL_SMOOTH);
    //PNG TRANSPARENCY
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    initLights();
    resetControls();
    
    //SetPlayer
    player = Player::getInstance();
    player->playNext();
    
    fleet = new RandomFleet();
    galaxy = new RandomGalaxy();
    
    for (int i = 0; i < galaxy->getTotalSystems(); ++i)
    {
        glLightfv( lights[i], GL_AMBIENT, lightAmbient);
        glLightfv( lights[i], GL_DIFFUSE, lightDiffuse);
        glLightfv( lights[i], GL_SPECULAR, lightSpecular);
        glEnable ( lights[i]);
    }
    //glEnable ( lights[0]);
    
    
    if (!Spaceship::pmodel)
    {		/* load up the model */
        Spaceship::setModel( obj_filename );
    }
    
    hud = new Texturizer(filename , true);
    
    glutFullScreen();
    
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

void HelpRenderBitmapString(float x, float y, void *font, std::string s)
{
    char *c;
    /*  set position to start drawing fonts */
    glRasterPos2f(x, y);
    /*  loop all the characters in the string */
    for (int i = 0; i < s.size(); i++)
    {
        glutBitmapCharacter(font, s[i]);
    }
    
}

void HelpDisplay(GLint ww, GLint wh)
{
    glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 1.0f);
        /*  switch to projection mode */
        glMatrixMode(GL_PROJECTION);
        /*  save previous matrix which contains the  */
        /* settings for the perspective projection */
        glPushMatrix();
            /*  reset matrix */
            glLoadIdentity();
            /*  set a 2D orthographic projection */
            gluOrtho2D(0, ww, 0, wh);
            /*  invert the y axis, down is positive */
            glScalef(1, -1, 1);
            /*  mover the origin from the bottom left corner */
            /*  to the upper left corner */
            glTranslatef(0, -wh, 0);
            glMatrixMode(GL_MODELVIEW);
            
            
            glPushMatrix();
                glLoadIdentity();
    
                linestart = 10; //Regresar al inicio
    
                HelpRenderBitmapString(30, linestart +=
                                       linespace, Help_Font, "Region: "+std::to_string(seed) );
                HelpRenderBitmapString(30, linestart +=
                                       linespace, Help_Font, "Systems: "+std::to_string(galaxy->getTotalSystems()) );
                HelpRenderBitmapString(30, linestart +=
                                       linespace, Help_Font, "---------");
                HelpRenderBitmapString(30, linestart +=
                                       linespace, Help_Font, "Speed: "+std::to_string(camera.getSpeed()) );
                HelpRenderBitmapString(30, linestart +=
                                       linespace, Help_Font, "Time speed: "+std::to_string(timeSpeed) );
                HelpRenderBitmapString(30, linestart +=
                                       linespace, Help_Font, showOrbits?"Orbits Activated":"Orbits Deactivated");

                HelpRenderBitmapString(30, screenHeight - linespace*2, Help_Font, player->getCurrentPlaying() );
    
                HelpRenderBitmapString(30, screenHeight - linespace, Help_Font, "("+std::to_string(camera.getPositionX())+","
                                       +std::to_string(camera.getPositionY())+","
                                       +std::to_string(camera.getPositionZ())+")");
                
    
            glPopMatrix();
        /*  set the current matrix to GL_PROJECTION */
        glMatrixMode(GL_PROJECTION);
        /*  restore previous settings */
        glPopMatrix();
    /*  get back to GL_MODELVIEW matrix */
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
    
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Buffers
    
    _time += timeSpeed;                         //Simular que el tiempo pasa
    galaxy->calculatePositions(_time);
    
    fleet->calculatePositions(_time);
    
    setUpPerspective();
    moveCamera();
    
    glEnable(GL_DEPTH_TEST);  //Que pasen detras de otros, permitir profundidades
        glPushMatrix();
    
            for (int i = 0; i < galaxy->getTotalSystems(); ++i)
            {
                glLightfv( lights[i] , GL_POSITION, galaxy->getSystem(i)->getPosition() ); //Posicionar luz en el Sol
            }
    
            glEnable(GL_LIGHTING);    //Nos sirve para iluminar orbitas
                galaxy->render();      //Pintar Sistema Solar
                fleet->render();
            glDisable(GL_LIGHTING);
    
            if (showOrbits)
            {
                
                fleet->renderTrajectories();
                galaxy->renderOrbits();
            }
        glPopMatrix();
    glDisable(GL_DEPTH_TEST);
    
    
    // HUD
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble) screenWidth, (GLdouble) screenHeight, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnable(GL_TEXTURE_2D);
    
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);	glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f);	glVertex2f(screenWidth, 0.0f);
            glTexCoord2f(1.0f, 1.0f);	glVertex2f(screenWidth, screenHeight);
            glTexCoord2f(0.0f, 1.0f);	glVertex2f(0.0f, screenHeight);
        glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    HelpDisplay(screenWidth,screenHeight);
    
    glutSwapBuffers();        //End
}
void setLight(GLenum l)
{
    for (int i = 0; i<galaxy->getTotalSystems(); ++i) glDisable(lights[i]);
    glEnable(l);
}

int pid;
std::string in_region;
float in_rad,_x,_y,_z;

void keyDown(unsigned char key, int x, int y)
{
    // check for numerical keys
    if ('0' <= key&&key <= '9')
    {
        if (key-'0' < galaxy->getTotalSystems() )
        {
            galaxy->getSystemPosition(key-'0', lookingAt);//Obtener su posicion
            //printf("%f %f %f\n",vec[0],vec[1],vec[2]);
            camera.pointAt(lookingAt);
            //setLight( lights[key-'0'] );
        }
        return;
    }
    
    switch (key)
    {
//        case 'p':
//            std::cout<<"Buscar planetas a X distancia de Y planeta\n";
//            std::cout<<"Ingresa el planeta origen: ";
//            std::cin>>pid;
//            std::cout<<"Ingresa el radio de busqueda: ";
//            std::cin>>in_rad;
//            getPlanetsInRadius( pid, in_rad);
//            break;
//        case '[':
//            std::cout<<"Ingresa la region a buscar: ";
//            std::cin>>in_region;
//            getPlanetsByRegion(in_region); //"SantaFe"
//            break;
//        case ']':
//            std::cout<<"Obtener region por coordenadas: \n";
//            std::cout<<"Coordenada x: ";
//            std::cin>>_x;
//            std::cout<<"Coordenada y: ";
//            std::cin>>_y;
//            std::cout<<"Coordenada z: ";
//            std::cin>>_z;
//            getRegionByCoords(_x,_y,_z);
//            break;
        case 'z': //Nave Anterior
            current_ship = current_ship-1 > -1? current_ship-1: fleet->getTotalShips()-1 ;
            fleet->getShipPosition(current_ship, lookingAt);
            camera.pointAt(lookingAt);
            break;
        case 'x': //Nave actual
            fleet->getShipPosition(current_ship, lookingAt);
            camera.pointAt(lookingAt);
            break;
        case 'c': //Nave siguiente o reinicia
            current_ship = current_ship+1 < fleet->getTotalShips()? current_ship+1:0;
            fleet->getShipPosition(current_ship, lookingAt);
            camera.pointAt(lookingAt);
            break;
        case 27:
            delete player;
            exit(0);
            break;
        case '-':
            if (timeSpeed>0.00001)
                timeSpeed /= 2.0f; // Disminuir velocidad
            break;
        case '=':
            if (timeSpeed < 200)
                timeSpeed *= 2.0f; // Aumentar velocidad
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
        case 'b':
            player->playPrev();
            break;
        case 'n':
            player->togglePlay();
            break;
        case 'm':
            player->playNext();
            break;
        default:
            player->playEffect(0);
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
void instructions()
{
    std::cout<<"Recuerda que al comenzar el programa puedes pasar la region que desees (re)explorar\n";
    
    std::cout<<"La simulacion requiere que tu teclado este en minúsculas y de preferencia en ingles.\n";
    std::cout<<"Te puedes mover usando WASD \nGirar lateralmente usando QE\nRotar la camara con IJKL\n";
    
    std::cout<<"- \t Reducir velocidad del tiempo\n";
    std::cout<<"= \t Aumentar velocidad del tiempo\n";
    
    std::cout<<", \t Reducir velocidad de nave\n";
    std::cout<<". \t Aumentar velocidad de nave\n";
    
    std::cout<<"o \t Mostrar/Esconder Orbitas y trayectorias\n";
    
    std::cout<<"z \t Apuntar a una nave anterior\n";
    std::cout<<"x \t Apuntar a una nave actual\n";
    std::cout<<"c \t Apuntar a una nave siguiente\n";
    
    std::cout<<"0 - 7 \t Apuntar a sistemas solares\n";
    
    std::cout<<"ESC te permite acabar la simulacion\n";
}
void idle(){/*glutPostRedisplay(); //Mejor usamos timer para que sea tiempo mas real*/}

int main(int argc, char** argv)
{
    seed = argc<2 ? time(NULL) : atoi(argv[1]); //Random o lo que nos pasaron
    std::cout<<"Explorando región "<<seed<<"\n";
    srand(seed);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(screenWidth,screenHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(argv[0]);
    glutSetWindowTitle("A01019102");
    instructions();
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutMainLoop();
    return 0;
}

