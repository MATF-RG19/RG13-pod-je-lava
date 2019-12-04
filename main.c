#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

static float x_curr, y_curr, z_curr;            //koordinate loptice
static float v;                                 //brzina navise
static int time;                                //vreme proteklo od dodira platforme
static int movement[4] = {0, 0, 0, 0};
 
static int animation_ongoing;                   //da li je animacija u toku
static int animation_parameter = 0; 

static void on_keyboard(unsigned char key, int x, int y);
static void on_keyboardUp(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);

int main(int argc, char **argv){
    
    GLfloat light_ambient[] =  {0.5, 0.5, 0.5, 1};
    GLfloat light_diffuse[] =  {0.5, 0.5, 0.5, 1};
    GLfloat light_specular[] = {0.5, 0.5, 0.5, 1};

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    x_curr = 0;
    y_curr = 50;
    z_curr = 0;
    
    v = 300;
    time = 40;
    
    glutKeyboardFunc(on_keyboard);
    glutKeyboardUpFunc(on_keyboardUp);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y){
    
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'g':
        if (!animation_ongoing) {
            glutTimerFunc(20, on_timer, 0);
            animation_ongoing = 1;
        }
        break;
    case 'h':
        animation_ongoing = 0;
        break;
    case 'w':
        movement[0] = 1;
        break;
    case 'a':
        movement[1] = 1;
        break;
    case 's':
        movement[2] = 1;
        break;
    case 'd':
        movement[3] = 1;
        break;
    }
}

static void on_keyboardUp(unsigned char key, int x, int y){
    
    switch (key) {
    case 'w':
        movement[0] = 0;
        break;
    case 'a':
        movement[1] = 0;
        break;
    case 's':
        movement[2] = 0;
        break;
    case 'd':
        movement[3] = 0;
        break;
    }
}

static void on_timer(int value){
    
    if (value != 0)
        return;

    time++;
    y_curr = (v*time - time*time*5)/400;
    
    if(movement[0])
        z_curr -= 0.1;
    if(movement[1])
        x_curr -= 0.1;
    if(movement[2])
        z_curr += 0.1;
    if(movement[3])
        x_curr += 0.1;
    
    if(y_curr <=  0){
        time = 0;
        v = 500;
    }

    glutPostRedisplay();

    if (animation_ongoing)
        glutTimerFunc(20, on_timer, 0);
}

static void on_reshape(int width, int height){
    
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float) width / height, 1, 100);
}

static void set_material(int id){
    
    GLfloat ambient_coeffs[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat diffuse_coeffs[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat shininess = 50;
    
    switch (id) {
        case 1:
            diffuse_coeffs[0] = 1.0;
            break;
        case 2:
            diffuse_coeffs[1] = 1.0;
            break;
    }
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

static void on_display(void){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(0 + x_curr, 10*sin(1.5707) + y_curr, 10*cos(1.5707) + z_curr,
              x_curr, y_curr, z_curr, 
              0, 1, 0);
    
    GLfloat light_position[] = {0, 1, 0, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glPushMatrix();
        glTranslatef(x_curr, y_curr, z_curr);
        set_material(1);
        glutSolidSphere(1, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
        set_material(2);
        glutSolidCube(5);
    glPopMatrix();
    
    glutSwapBuffers();
}
