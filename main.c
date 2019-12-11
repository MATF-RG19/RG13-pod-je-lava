#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

#define NUM 10

struct platform {
    
    float x;
    float z;
    float size;
};

static struct platform arr1[NUM];               //niz platformi
static struct platform arr2[NUM];               //niz platformi
static float x_curr, y_curr, z_curr;            //koordinate loptice
static float v;                                 //brzina navise
static float time1;                             //vreme proteklo od dodira platforme
static int movement[4] = {0, 0, 0, 0};          //da li je pritisnuto neko dugme
static int animation;                           //da li je animacija u toku
static int colision = 1;                        //da li se desila kolizija
static float size = 0.6;                        //precnik loptice

static void on_keyboard(unsigned char key, int x, int y);
static void on_keyboardUp(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);

int main(int argc, char **argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    
    glutKeyboardFunc(on_keyboard);
    glutKeyboardUpFunc(on_keyboardUp);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat light_ambient[] =  {0.5, 0.5, 0.5, 1};
    GLfloat light_diffuse[] =  {0.5, 0.5, 0.5, 1};
    GLfloat light_specular[] = {0.5, 0.5, 0.5, 1};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    x_curr = 0;
    y_curr = 25;
    z_curr = 10;
    
    v = 500;
    time1 = 50;
    
    srand(time(NULL));
    
    for(int i = 0; i<NUM; i++){
        
        arr1[i].x = 40*(rand()/(float)RAND_MAX) - 20;
        arr1[i].z = -i*10;
        arr1[i].size = ceil(4*(rand()/(float)RAND_MAX) + 1);
    }
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y){
    
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'g':
        if (!animation) {
            glutTimerFunc(20, on_timer, 0);
            animation = 1;
        }
        break;
    case 'h':
        animation = 0;
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

    time1++;

    int z10 = (int)-z_curr%100;
    int z100 = (int)-z_curr/100;
    int ind = z10/10;
    
    y_curr = (v*time1 - time1*time1*5)/500;
    
    if(movement[0])
        z_curr -= 0.2;
    if(movement[1])
        x_curr -= 0.2;
    if(movement[2])
        z_curr += 0.2;
    if(movement[3])
        x_curr += 0.2;
    
    if(y_curr <= size && colision){
        
        if(z100 % 2 == 0){
            if(!(z10 >= (10*ind) && 
                z10 <= (10*ind + arr1[ind].size - 1) &&
                x_curr <= arr1[ind].x + arr1[ind].size/2 && 
                x_curr >= arr1[ind].x - arr1[ind].size/2)){
                
                colision = 0;
            }
        }
        else{
            if(!(z10 >= (10*ind) && 
                z10 <= (10*ind + arr2[ind].size - 1) &&
                x_curr <= arr2[ind].x + arr2[ind].size/2 && 
                x_curr >= arr2[ind].x - arr2[ind].size/2)){
            
                colision = 0;
            }
        }
        
        if(z_curr > 7)
            colision = 1;
        
        if(colision){
            
            time1 = 0;
            v = 500;
        }
    }
    
    if(y_curr < -3)
        animation = 0;
    
    if(z10 == 50){
        
        if(z100 % 2 == 0){
         
            for(int i = 0; i<NUM; i++){
        
                arr2[i].x = 40*(rand()/(float)RAND_MAX) - 20;
                arr2[i].z = -i*10 - 100 * (z100 + 1);
                arr2[i].size = ceil(4*(rand()/(float)RAND_MAX) + 1);
            }
        }
        else{
            
            for(int i = 0; i<NUM; i++){
        
                arr1[i].x = 40*(rand()/(float)RAND_MAX) - 20;
                arr1[i].z = -i*10 - 100 * (z100 + 1);
                arr1[i].size = ceil(4*(rand()/(float)RAND_MAX) + 1);
            }
        }
    }
    
    glutPostRedisplay();

    if (animation)
        glutTimerFunc(20, on_timer, 0);
}

static void on_reshape(int width, int height){
    
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float) width / height, 1, 100);
}

static void set_material(int id){
    
    GLfloat ambient_coeffs[] =  {0.1, 0.1, 0.1, 1};
    GLfloat diffuse_coeffs[] =  {0.1, 0.1, 0.1, 1};
    GLfloat specular_coeffs[] = {1, 1, 1, 1};
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
    
    gluLookAt(0 + x_curr, 10*sin(1.57) + y_curr, 10*cos(1.57) + z_curr,
              x_curr, y_curr, z_curr, 
              0, 1, 0);
    
    GLfloat light_position[] = {0, 1, 0, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glPushMatrix();
        glTranslatef(0, -3, 10);
        set_material(2);
        glutSolidCube(6);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(x_curr, y_curr, z_curr);
        glRotatef(time1, -1, 0, 0);
        set_material(1);
        glutSolidSphere(size, 20, 20);
    glPopMatrix();
    
    for(int i = 0; i<NUM; i++){
        
        glPushMatrix();
            glTranslatef(arr1[i].x, -arr1[i].size/2, arr1[i].z - arr1[i].size/2);
            set_material(2);
            glutSolidCube(arr1[i].size);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(arr2[i].x, -arr2[i].size/2, arr2[i].z - arr2[i].size/2);
            set_material(2);
            glutSolidCube(arr2[i].size);
        glPopMatrix();
    }
    
    glutSwapBuffers();
}
