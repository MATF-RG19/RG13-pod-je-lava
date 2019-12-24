#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include "models.h"
#include "logic.h"

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
            diffuse_coeffs[0] = 0.2;
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

void draw_player(float x_curr, float y_curr, float z_curr, float size){
    
    glPushMatrix();
        glTranslatef(x_curr, y_curr, z_curr);
        set_material(1);
        glutSolidSphere(size, 50, 50);
    glPopMatrix();
}

void draw_platforms(struct platform *arr1, struct platform *arr2, int n){
    
    set_material(2);
    
    //pocetna platforma
    glPushMatrix();
        glTranslatef(0, -3, 10);
        glutSolidCube(6);
    glPopMatrix();
    
    //ostale platforme
    for(int i = 0; i<n; i++){
        
        glPushMatrix();
            glTranslatef(arr1[i].x, -arr1[i].size/2, arr1[i].z - arr1[i].size/2);
            glutSolidCube(arr1[i].size);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(arr2[i].x, -arr2[i].size/2, arr2[i].z - arr2[i].size/2);
            glutSolidCube(arr2[i].size);
        glPopMatrix();
    }
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

void wall(int x, int u1, int u2, int v1, int v2){
    
    int u, v;
    
    glPushMatrix();
    
    for (u = u1; u < u2; u+=10) {
        glBegin(GL_QUADS);
        for (v = v1; v <= v2; v+=10) {
            
            glTexCoord2f(0, 0);     glVertex3f(x, u, v);
            glTexCoord2f(1, 0);     glVertex3f(x, u+10, v);
            glTexCoord2f(1, 1);     glVertex3f(x, u+10, v+10);
            glTexCoord2f(0, 1);     glVertex3f(x, u, v+10);
        }
        glEnd();
    }
    
    glPopMatrix();
}

void show_score(float x_curr, float y_curr, float z_curr, int score){
        
    char s[12];
    sprintf(s,"%d", score);

    glColor3f(0.96,0.96,0.96);
    glRasterPos3f(x_curr - 5, y_curr, z_curr - 5);
    
    for(int i=0; i<strlen(s); i++) {
        glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
}
