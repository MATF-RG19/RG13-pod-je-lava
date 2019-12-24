#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include "image.h"
#include "models.h"
#include "logic.h"

#define NUM 20

static struct platform arr1[NUM];               //prvi niz platformi
static struct platform arr2[NUM];               //drugi niz platformi
static float x_curr, y_curr, z_curr;            //koordinate loptice
static float v;                                 //brzina navise
static float time1;                             //vreme proteklo od dodira platforme
static int time2 = 0;                           //ukupno proteklo vreme
static int movement[4] = {0, 0, 0, 0};          //da li je pritisnuto neko dugme
static int animation;                           //da li je animacija u toku
static int colision = 1;                        //da li se desila kolizija
static int pos = 0;                             //koliko nizova je generisano
static float size = 0.5;                        //poluprecnik loptice
static int score = 0;                           //broj poena
static GLuint names[2];                         //teksture

static void on_keyboard(unsigned char key, int x, int y);
static void on_keyboardUp(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);
static void init();
static void lights();
static void texture();

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
    lights();
    init();
    texture();
    
    glutMainLoop();
    
    return 0;
}

//inicijalizacija svetla
static void lights(){
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat light_ambient[] =  {1, 1, 1, 1};
    GLfloat light_diffuse[] =  {0.9, 0.9, 0.9, 1};
    GLfloat light_specular[] = {0.1, 0.1, 0.1, 1};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

//inicijalizacija koordinata loptice i niza platformi
static void init(){
    
    x_curr = 0;
    y_curr = 25;
    z_curr = 10;
    
    v = 500;
    time1 = 50;
    
    pos = 0;
    score = 0;
    
    generate_platforms(arr1, NUM, -1);
}

static void texture(){
    
    Image * image;
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    image = image_init(0, 0);

    image_read(image, "lava.bmp");
    
    glGenTextures(2, names);
    glBindTexture(GL_TEXTURE_2D, names[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    image_read(image, "rock.bmp");

    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    image_done(image);
}

static void on_keyboard(unsigned char key, int x, int y){
    
    switch (key) {
    case 27:
        exit(0);
        break;
    //pokrece igru
    case 'g':
        if (!animation) {
            glutTimerFunc(20, on_timer, 0);
            animation = 1;
        }
        break;
    //pauzira igru
    case 'p':
        animation = 0;
        break;
    //restartuje igru
    case 'r':
        init();
        colision = 1;
        glutPostRedisplay();
        break;
    //da li je pritisnuto neko dugme
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

//da li je pusteno neko dugme
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
    time2++;
    
    //y koordinatu racunamo po formuli hica navise
    y_curr = (v*time1 - time1*time1*5)/500;
    //y_curr = 100;
    
    move_platforms(arr1, arr2, NUM, time2);
    
    //ako je pritisnuto neko dugme pomeri lopticu
    if(movement[0])
        z_curr -= 0.2;
    if(movement[1] && x_curr > -40+2*size)
        x_curr -= 0.2;
    if(movement[2])
        z_curr += 0.2;
    if(movement[3] && x_curr < 40-2*size)
        x_curr += 0.2;
    
    int z10 = (int)-z_curr%(NUM*10);          //z koordinata od pocetka niza do igraca
    int z100 = (int)-z_curr/(NUM*10);         //broj trenutnog niza platformi
    int ind = z10/10;                         //indeks platforme u trenutnom nizu
    
    if(z100*NUM + ind + 1 > score)
        score = z100*NUM + ind + 1;
    
    //ako je loptica u nivou platformi
    if(y_curr <= size+0.4 && colision)
        detect_collision(x_curr, z_curr, &colision, z100, z10, ind, &v, &time1, arr1, arr2);
    
    //zaustavi igru ako je loptica u lavi
    if(y_curr < -3)
        animation = 0;
    
    //ako se loptica nalazi na petoj platformi u nekom nizu azuriraj drugi niz
    //ovo sluzi da se pomocu dva niza dobije beskonacno platformi (endless runner)
    if(z10 > 45 && z100 == pos){
        //azuriramo 2. niz
        if(z100 % 2 == 0)
            generate_platforms(arr2, NUM, z100);
        //azuriramo 1. niz
        else
            generate_platforms(arr1, NUM, z100);
        
        pos++;
    }
    
    glutPostRedisplay();

    if (animation)
        glutTimerFunc(20, on_timer, 0);
}

static void on_reshape(int width, int height){
    
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, (float) width / height, 1, 200);
}

static void on_display(void){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //kamera je postavljena direktno iznad loptice na uvek istoj udaljenosti od nje
    gluLookAt(x_curr, 10*sin(1.57) + y_curr, 10*cos(1.57) + z_curr,
              x_curr, y_curr, z_curr, 
              0, 1, 0);
    
    GLfloat light_position[] = {0, 50, 0, 0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    draw_player(x_curr, y_curr, z_curr, size);
    draw_platforms(arr1, arr2, NUM);
    show_score(x_curr, y_curr, z_curr, score);
    
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, names[0]);
    lava(-40, 40, -NUM*10*(pos+1), -NUM*10*(pos-1), time2);
    
    glBindTexture(GL_TEXTURE_2D, names[1]);
    wall(-40, -10, 50, -NUM*10*(pos+1), -NUM*10*(pos-1));
    wall(40, -10, 50, -NUM*10*(pos+1), -NUM*10*(pos-1));
    
    glutSwapBuffers();
}
