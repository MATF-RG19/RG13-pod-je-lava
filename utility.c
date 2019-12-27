#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include "utility.h"
#include "image.h"

void lights(){
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    
    GLfloat light_ambient[] =  {1, 1, 1, 1};
    GLfloat light_diffuse[] =  {0.9, 0.9, 0.9, 1};
    GLfloat light_specular[] = {0.1, 0.1, 0.1, 1};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void texture(GLuint *names){
    
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

void generate_platforms(struct platform *arr, int n, int z100){
    
    srand(time(NULL));
    
    for(int i = 0; i<n; i++){
        
        arr[i].x = 40*(rand()/(float)RAND_MAX) - 20;
        arr[i].z = -i*10 - (n*10) * (z100 + 1);
        arr[i].size = ceil(4*(rand()/(float)RAND_MAX) + 1);
        arr[i].turn = ceil(60*(rand()/(float)RAND_MAX)) + 70;
        if(i%2 == 0)
            arr[i].speed = ((float)z100+2)/20;
        else
            arr[i].speed = -((float)z100+2)/20;
    }
}

void move_platforms(struct platform *arr1, struct platform *arr2, int n, int time2){
    
    for(int i = 0; i<n; i++){
        
        if(time2 % arr1[i].turn == 0)
            arr1[i].speed = -arr1[i].speed;
        if(arr2[i].turn != 0 && time2 % arr2[i].turn == 0)
            arr2[i].speed = -arr2[i].speed;
        
        arr1[i].x += arr1[i].speed;
        arr2[i].x += arr2[i].speed;
    }
}

void detect_collision(float x_curr, float z_curr, int *colision,
                      int z100, int z10, int ind, float *v, float *time1,
                      struct platform *arr1, struct platform *arr2){
    
    
    //da li loptica dodiruje platformu
    //prvi niz
    if(z100 % 2 == 0){                                      
        if(!(z10 >= (10*ind) && 
            z10 <= (10*ind + arr1[ind].size - 1) &&
            x_curr <= arr1[ind].x + arr1[ind].size/2 && 
            x_curr >= arr1[ind].x - arr1[ind].size/2)){
            
            *colision = 0;
        }
    }
    //drugi niz
    else{                                                   
        if(!(z10 >= (10*ind) && 
            z10 <= (10*ind + arr2[ind].size - 1) &&
            x_curr <= arr2[ind].x + arr2[ind].size/2 && 
            x_curr >= arr2[ind].x - arr2[ind].size/2)){
        
            *colision = 0;
        }
    }
    
    //kolizija za pocetnu platformu
    if(z_curr > 7)
        *colision = 1;
    
    //azuriraj vreme i ubrzanje ako je loptica dodirnula platformu
    if(*colision){
        
        *time1 = 0;
        *v = 750 + 500/3;
        
        if(z100 % 2 == 0)
            *v -= arr1[ind].size*250 / 3;
        else
            *v -= arr2[ind].size*250 / 3;
        
        if(z_curr > 0)
            *v = 500;
    }

}

void move_player(float *x_curr, float *z_curr, float *rotx, float *rotz, int *movement, float size){
    
    if(movement[0]){
        
        *z_curr -= 0.2;
        *rotx += 0.3;
        
        if(*rotx >= 30)
            *rotx = 30;
    }
    else{
        *rotx -= 0.5;
        if(*rotx <= 0)
            *rotx = 0;
    }
    
    if(movement[1] && *x_curr > -40+2*size){
        
        *x_curr -= 0.2;
        *rotz -= 0.3;
        
        if(*rotz > 0)
            *rotz -= 0.3;
        
        if(*rotz <= -10)
            *rotz = -10;
    }
    
    if(movement[2])
        *z_curr += 0.2;
    
    if(movement[3] && *x_curr < 40-2*size){
        
        *x_curr += 0.2;
        *rotz += 0.3;
        
        if(*rotz < 0)
            *rotz += 0.3;
        
        if(*rotz >= 10)
            *rotz = 10;
    }
    
    if(!movement[3] && !movement[1]){
        
        if(*rotz < 1)
            *rotz += 0.5;
        else if(*rotz > 1)
            *rotz -= 0.5;
        else
            *rotz = 0;
    }
}
