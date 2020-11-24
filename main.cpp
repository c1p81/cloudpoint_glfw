#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES // M_PI constant
#include <math.h>
#include <iostream>

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/io/ply_io.h>




using namespace std;


// Window size
const int WINDOWS_WIDTH = 1280;
const int WINDOWS_HEIGHT = 720;
GLfloat g_alpha = 210.0f, g_beta = -70.0f, g_zoom = 2.0f;
GLboolean g_locked = GL_FALSE;
bool g_freeze = false;

int g_cursorX = 0;
int g_cursorY = 0;

//basic structure for a Vertex to simplify data passing
typedef struct
{
    GLfloat x, y, z;
    GLfloat r, g, b, a;
} Vertex;

//structure for data stream
typedef struct
{
    GLfloat x, y, z;
} Data;

// Camera params depend on window size
// This is the callback that gives us updates to window size
void FrameBufferSizeCallback(GLFWwindow* a_window, int a_width, int a_height)
{
    const double DEG2RAD = 3.14159265 / 180;
    // Preset the camera parameters:
    // the vertical field of view angle (fovY)
    const float l_fovY = 45.0f;
    // the distance to the Near clipping plane (front)
    const float l_front = 0.1f;
    // the distance to Far clipping plane (back)
    const float l_back = 128.0f;
    // the screen aspect ratio (width/height)
    float l_ratio = 1.0f;
    if (a_height > 0)
        l_ratio = (float) a_width / (float) a_height;

    // Setup the viewport of the virtual camera (using the window size)
    glViewport(0, 0, a_width, a_height);

    // Specify that matrix operations should be applied to the projection matrix stack
    glMatrixMode(GL_PROJECTION);

    // Reset the matrix to it's default state (identity)
    glLoadIdentity();

    double l_tangent = tan(l_fovY/2 * DEG2RAD);   // tangent of half fovY
    double l_height_f = l_front * l_tangent;          // half height of near plane
    double l_width_f = l_height_f * l_ratio;      // half width of near plane

    //Create the projection matrix based on the front
    //clipping plane and the location of the corners
    glFrustum(-l_width_f, l_width_f, -l_height_f, l_height_f, l_front, l_back);
}


void linea(pcl::PointCloud<pcl::PointXYZRGB>& nuvola)
{
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    //glColor4f(1.0, 1.0, 1.0, 0.5);


   for(unsigned long w=0;w < nuvola.size();w++)
        {
            //cout << nuvola.points[w]._PointXYZRGB::x  <<endl;
            glColor4f(nuvola.points[w]._PointXYZRGB::r, nuvola.points[w]._PointXYZRGB::g, nuvola.points[w]._PointXYZRGB::b, 1.0);
            glVertex3f(nuvola.points[w]._PointXYZRGB::x, nuvola.points[w]._PointXYZRGB::y, nuvola.points[w]._PointXYZRGB::z);
        }
    glEnd();
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_SPACE:
            g_freeze=!g_freeze;
            break;
        case GLFW_KEY_LEFT:
            g_alpha += 5.0f;
            break;
        case GLFW_KEY_RIGHT:
            g_alpha -= 5.0f;
            break;
        case GLFW_KEY_UP:
            g_beta -= 5.0f;
            break;
        case GLFW_KEY_DOWN:
            g_beta += 5.0f;
            break;
        case GLFW_KEY_PAGE_UP:
            g_zoom -= 0.25f;
            if (g_zoom < 0.0f)
                g_zoom = 0.0f;
            break;
        case GLFW_KEY_PAGE_DOWN:
            g_zoom += 0.25f;
            break;
        default:
            break;
    }
}

void MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        g_locked = GL_TRUE;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "Coordinate Schermo (" << xpos << " : " << ypos << std::endl;
    }
    else
    {
        g_locked = GL_FALSE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void CursorPositionCallback(GLFWwindow* window, double x, double y)
{
    //if the mouse button is pressed
    if (g_locked)
    {
        g_alpha += (GLfloat) (x - g_cursorX) / 10.0f;
        g_beta += (GLfloat) (y - g_cursorY) / 10.0f;
    }
    //update the cursor position
    g_cursorX = (int) x;
    g_cursorY = (int) y;
}

void ScrollCallback(GLFWwindow* window, double x, double y)
{
    g_zoom += (float) y / 4.0f;
    if (g_zoom < 0.0f)
        g_zoom = 0.0f;
}

void DrawOrigin(){
    float transparency = 0.5f;
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    //draw a red line for the x-axis
    glColor4f(1.0f, 0.0f, 0.0f, transparency);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor4f(1.0f, 0.0f, 0.0f, transparency);
    glVertex3f(0.3f, 0.0f, 0.0f);

    //draw a green line for the y-axis
    glColor4f(0.0f, 1.0f, 0.0f, transparency);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor4f(0.0f, 1.0f, 0.0f, transparency);
    glVertex3f(0.0f, 0.0f, 0.3f);

    //draw a blue line for the z-axis
    glColor4f(0.0f, 0.0f, 1.0f, transparency);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor4f(0.0f, 0.0f, 1.0f, transparency);
    glVertex3f(0.0f, 0.3f, 0.0f);
    glEnd();
}

//===================================================================================


int main(int argc, char const *argv[])
{
    GLFWwindow* l_window;
    int l_width, l_height;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);

    //pcl::PointCloud<pcl::PointXYZRGB>::Ptr *cloud;

    //cloud= new pcl::PointCloud<pcl::PointXYZRGB>::Ptr (new pcl::PointCloud<pcl::PointXYZRGB>);

    //pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
    if (pcl::io::loadPLYFile<pcl::PointXYZRGB> (argv[1], *cloud) == -1)
       {
           PCL_ERROR ("Couldn't read file PLY \n");
           return (-1);
       }



     // controlla se il sistema di gestione GLFWe' inizializzato
    // in caso negativo abbandona
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    // crea una finestra di visualizzazione
    l_window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, argv[1], NULL, NULL);
    //se fallisce termina GLFW ed esce
    if (!l_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    //gestione dei callback ovvero l'interazione della finestra
    //con tastiera mouse, ridimenzionamento della finesta
    //callback da tastiera agganciato alla funzione KeyCallBack
    glfwSetKeyCallback(l_window, KeyCallback);
    //callback del framebuffer agganciato alla funzione FrameBufferSizeCallBack
    //questo viene attivato dal ridimensionamento da parete dell'utente
    //della finestra
    glfwSetFramebufferSizeCallback(l_window, FrameBufferSizeCallback);
    //mouse callback
    glfwSetMouseButtonCallback(l_window, MouseCallback);
    //mouse movement callback
    glfwSetCursorPosCallback(l_window, CursorPositionCallback);
    //mouse scroll callback
    glfwSetScrollCallback(l_window, ScrollCallback);

    // aggancia la finestra al thread attuale
    glfwMakeContextCurrent(l_window);
    //viene definito l'intervallo con cui vengono effettuati gli aggiornamenti
    //della finesta di visualizzazione passando i dati dal buffer alla finestra
    glfwSwapInterval(1);

    //inizializza il framebuffer con le dimesioni della viewport
    glfwGetFramebufferSize(l_window, &l_width, &l_height);
    // inizializza la camera
    FrameBufferSizeCallback(l_window, l_width, l_height);

    // enable anti-aliasing
    glEnable(GL_BLEND);
    // smooth the points
    glEnable(GL_LINE_SMOOTH);
    // smooth the lines
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    // needed for alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);

    //initialize all parameters
    float sigma = 0.1f;
    float sign = 1.0f;
    float step_size = 0.01f;

    while (!glfwWindowShouldClose(l_window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //colore di sfondo RGB+trasparenza
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //draw the scene
        //switch to modelview so the tranformation applies to entire model
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // imposta il valore dello zoom sull'asse Z
        // la variabile viene modificata dal Callback
        glTranslatef(0.0, 0.0, -g_zoom);
        // ruota lungo l'asse X
        // la variabile g_beta viene modificata dal Callback
        glRotatef(g_beta, 1.0, 0.0, 0.0);
        //  ruota lungo l'asse Z
        // la variabile g_alpha viene modificata dal Callback
        glRotatef(g_alpha, 0.0, 0.0, 1.0);

        // ****************** Genera il disegno *******************
        linea(*cloud);


        // scambia il buffer e gestisce gli eventi
        glfwSwapBuffers(l_window);
        glfwPollEvents();
    }

    // pulisce in uscita
    glfwDestroyWindow(l_window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
