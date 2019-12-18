#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include "image.h"

#define NUM 20

struct platform {
    
    float x;
    float z;
    float size;
    float speed;
    int turn;
};

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
    
    srand(time(NULL));
    
    for(int i = 0; i<NUM; i++){
        
        arr1[i].x = 40*(rand()/(float)RAND_MAX) - 20;
        arr1[i].z = -i*10;
        arr1[i].size = ceil(4*(rand()/(float)RAND_MAX) + 1);
        arr1[i].turn = ceil(60*(rand()/(float)RAND_MAX)) + 70;
        if(i%2 == 0)
            arr1[i].speed = 0.05;
        else
            arr1[i].speed = -0.05;
    }
}

static void texture(){
    
    Image * image;
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    image = image_init(0, 0);

    image_read(image, "lava.bmp");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
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
    //y_curr = 2;
    
    for(int i = 0; i<NUM; i++){
        
        if(time2 % arr1[i].turn == 0)
            arr1[i].speed = -arr1[i].speed;
        if(arr2[i].turn != 0 && time2 % arr2[i].turn == 0)
            arr2[i].speed = -arr2[i].speed;
        
        arr1[i].x += arr1[i].speed;
        arr2[i].x += arr2[i].speed;
    }
    
    //ako je pritisnuto neko dugme pomeri lopticu
    if(movement[0])
        z_curr -= 0.2;
    if(movement[1])
        x_curr -= 0.2;
    if(movement[2])
        z_curr += 0.2;
    if(movement[3])
        x_curr += 0.2;
    
    int z10 = (int)-z_curr%(NUM*10);          //z koordinata od pocetka niza
    int z100 = (int)-z_curr/(NUM*10);         //broj niza platformi
    int ind = z10/10;                         //indeks platforme u nizu
    
    //ako je loptica u nivou platformi
    if(y_curr <= size+0.4 && colision){
        
        //da li loptica dodiruje platformu
        //prvi niz
        if(z100 % 2 == 0){                                      
            if(!(z10 >= (10*ind) && 
                z10 <= (10*ind + arr1[ind].size - 1) &&
                x_curr <= arr1[ind].x + arr1[ind].size/2 && 
                x_curr >= arr1[ind].x - arr1[ind].size/2)){
                
                colision = 0;
            }
        }
        //drugi niz
        else{                                                   
            if(!(z10 >= (10*ind) && 
                z10 <= (10*ind + arr2[ind].size - 1) &&
                x_curr <= arr2[ind].x + arr2[ind].size/2 && 
                x_curr >= arr2[ind].x - arr2[ind].size/2)){
            
                colision = 0;
            }
        }
        
        //kolizija za pocetnu platformu
        if(z_curr > 7)
            colision = 1;
        
        //azuriraj vreme i ubrzanje ako je loptica dodirnula platformu
        if(colision){
            
            time1 = 0;
            v = 750 + 500/3;
            
            if(z100 % 2 == 0)
                v -= arr1[ind].size*250 / 3;
            else
                v -= arr2[ind].size*250 / 3;
            
            if(z_curr > 0)
                v = 500;
        }
    }
    
    //zaustavi igru ako je loptica u lavi
    if(y_curr < -3)
        animation = 0;
    
    //ako se loptica nalazi na petoj platformi u nekom nizu azuriraj drugi niz
    //ovo sluzi da se pomocu dva niza dobije beskonacno platformi (endless runner)
    if(z10 > 10*NUM/2 - 5 && z100 == pos){
        //azuriramo 2. niz
        if(z100 % 2 == 0){              
         
            for(int i = 0; i<NUM; i++){
        
                arr2[i].x = 40*(rand()/(float)RAND_MAX) - 20;
                arr2[i].z = -i*10 - (NUM*10) * (z100 + 1);
                arr2[i].size = ceil(4*(rand()/(float)RAND_MAX) + 1);
                arr2[i].turn = ceil(60*(rand()/(float)RAND_MAX)) + 70;
                if(i%2 == 0)
                    arr2[i].speed = ((float)z100+2)/20;
                else
                    arr2[i].speed = -((float)z100+2)/20;
            }
        }
        //azuriramo 1. niz
        else{
            
            for(int i = 0; i<NUM; i++){
        
                arr1[i].x = 40*(rand()/(float)RAND_MAX) - 20;
                arr1[i].z = -i*10 - (NUM*10) * (z100 + 1);
                arr1[i].size = ceil(4*(rand()/(float)RAND_MAX) + 1);
                arr1[i].turn = ceil(60*(rand()/(float)RAND_MAX)) + 70;
                if(i%2 == 0)
                    arr1[i].speed = ((float)z100+2)/20;
                else
                    arr1[i].speed = -((float)z100+2)/20;
            }
        }
        
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

//oboji objekte
static void set_material(int id){
    
    GLfloat ambient_coeffs[] =  {0, 0, 0, 1};
    GLfloat diffuse_coeffs[] =  {0.1, 0.1, 0.1, 1};
    GLfloat specular_coeffs[] = {0.1, 0.1, 0.1, 1};
    GLfloat shininess = 50;
    
    switch (id) {
        case 1:
            diffuse_coeffs[0] = 1.0;
            break;
        case 2:
            diffuse_coeffs[1] = 1.0;
            break;
        case 3:
            diffuse_coeffs[0] = 1.0;
            diffuse_coeffs[1] = 0.6;
            break;
    }
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

float f(float t){
    
    return sin(t/50)/2 - 1.5;
}

void lava(int u1, int u2, int v1, int v2, float t){
    
    int u, v;
    
    glPushMatrix();
    
    for (u = u1; u < u2; u+=5) {
        glBegin(GL_QUADS);
        for (v = v1; v <= v2; v+=5) {
            
            if((u+v) % 10 == 0){
                
                glTexCoord2f(0, 0);     glVertex3f(u, f(t), v);
                glTexCoord2f(1, 0);     glVertex3f(u+5, f(t)/2, v);
                glTexCoord2f(1, 1);     glVertex3f(u+5, f(t), v+5);
                glTexCoord2f(0, 1);     glVertex3f(u, f(t)/2, v+5);
            }
            else{
                
                glTexCoord2f(0, 0);     glVertex3f(u, f(t)/2, v);
                glTexCoord2f(1, 0);     glVertex3f(u+5, f(t), v);
                glTexCoord2f(1, 1);     glVertex3f(u+5, f(t)/2, v+5);
                glTexCoord2f(0, 1);     glVertex3f(u, f(t), v+5);
            }
        }
        glEnd();
    }

    glPopMatrix();
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
    
    //pocetna platforma
    glPushMatrix();
        glTranslatef(0, -3, 10);
        set_material(2);
        glutSolidCube(6);
    glPopMatrix();
    
    //loptica
    glPushMatrix();
        glTranslatef(x_curr, y_curr, z_curr);
        set_material(1);
        glutSolidSphere(size, 50, 50);
    glPopMatrix();
    
    //ostale platforme
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
    
    glEnable(GL_TEXTURE_2D);
    lava(-40, 40, -NUM*10*(pos+1), -NUM*10*(pos-1), time2);
    
    glutSwapBuffers();
}
