#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include "models.h"
#include "utility.h"

void draw_cylinder(float size, int id){
 
    GLUquadric *q = gluNewQuadric();
    if(id == 1)
        glColor3f(0.7, 0.7, 0.7);
    if(id == 2)
        glColor3f(1, 0.5, 0);
    gluCylinder(q, size, size, 0.1, 20, 20);
    
    if(id == 1)
        glColor3f(0.07, 0.07, 0.07);
    gluDisk(q, 0, size, 20, 20);
    
    if(id == 1)
        glColor3f(0.7, 0.7, 0.7);
    glTranslatef(0, 0, 0.1);
    gluDisk(q, 0, size, 20, 20);
}

void draw_player(float x_curr, float y_curr, float z_curr, float size, float t, float rotx, float rotz){
    
    double plane[] = {0, 1, 0, -(y_curr - size - 0.31)};
    glClipPlane(GL_CLIP_PLANE0, plane);
    glEnable(GL_CLIP_PLANE0);
    
    glPushMatrix();
    
    glTranslatef(x_curr, y_curr, z_curr);
    glRotatef(-rotx, 1, 0, 0);
    glRotatef(-rotz, 0, 0, 1);

    //telo
    glPushMatrix();
        glScalef(1, 2.5, 1);
        glColor3f(0.1, 0.1, 0.1);
        glutSolidSphere(size, 50, 50);
    glPopMatrix();
    
    glDisable(GL_CLIP_PLANE0);
    
    //glava
    glPushMatrix();
        glTranslatef(0, size + 0.2, -0.2);
        glScalef(0.7, 0.7, 0.7);
        glColor3f(0.7, 0.7, 0.7);
        glutSolidSphere(size, 50, 50);
    glPopMatrix();
    
    //oci
    glPushMatrix();
        glTranslatef(0.13, size + 0.4, -0.45);
        glColor3f(0, 0, 0);
        glutSolidSphere(0.07, 50, 50);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(-0.13, size + 0.4, -0.45);
        glColor3f(0, 0, 0);
        glutSolidSphere(0.07, 50, 50);
    glPopMatrix();
    
    //kljun
    glPushMatrix();
        glTranslatef(0, size + 0.2, -0.5);
        glRotatef(90, 1, 0, 0);
        glScalef(0.5, 0.8, 0.8);
        draw_cylinder(size, 2);
    glPopMatrix();
    
    //noge
    glPushMatrix();
        glTranslatef(0.3, -size - 0.3, -0.5);
        glRotatef(90, 1, 0, 0);
        glRotatef(10, 0, 0, 1);
        glScalef(0.5, 0.8, 0.8);
        draw_cylinder(size, 2);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(-0.3, -size - 0.3, -0.5);
        glRotatef(90, 1, 0, 0);
        glRotatef(-10, 0, 0, 1);
        glScalef(0.5, 0.8, 0.8);
        draw_cylinder(size, 2);
    glPopMatrix();
    
    //krila
    glPushMatrix();
        glTranslatef(0.5, 0.2, 0);
        glRotatef(90, 1, 0, 0);
        glRotatef(45*sin(t/10), 0, 1, 0);
        glScalef(1.5, 0.6, 1);
        draw_cylinder(size, 1);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(-0.5, 0.2, 0);
        glRotatef(90, 1, 0, 0);
        glRotatef(-45*sin(t/10), 0, 1, 0);
        glScalef(1.5, 0.6, 1);
        draw_cylinder(size, 1);
    glPopMatrix();
    
    glPopMatrix();
}

void draw_platforms(struct platform *arr1, struct platform *arr2, int n){
    //pocetna platforma
    glPushMatrix();
        glColor3f(0.1, 0.1, 0.3);
        glTranslatef(0, -3.3, 10);
        glutSolidCube(6);
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0.1, 0.1, 0.1);
        glTranslatef(0, -3, 10);
        glutSolidCube(5.5);
    glPopMatrix();
    
    //ostale platforme
    for(int i = 0; i<n; i++){
        
        glColor3f(0.1, 0.1, 0.3);
        glPushMatrix();
            glTranslatef(arr1[i].x, -arr1[i].size/2 - 0.3, arr1[i].z - arr1[i].size/2);
            glutSolidCube(arr1[i].size);
        glPopMatrix();
        
        if(arr2[i].size != 0){
        glPushMatrix();
            glTranslatef(arr2[i].x, -arr2[i].size/2 - 0.3, arr2[i].z - arr2[i].size/2);
            glutSolidCube(arr2[i].size);
        glPopMatrix();
        }
        
        glColor3f(0.1, 0.1, 0.1);
        glPushMatrix();
            glTranslatef(arr1[i].x, -arr1[i].size/2, arr1[i].z - arr1[i].size/2);
            glutSolidCube(arr1[i].size - 0.5);
        glPopMatrix();
        
        if(arr2[i].size != 0){
        glPushMatrix();
            glTranslatef(arr2[i].x, -arr2[i].size/2, arr2[i].z - arr2[i].size/2);
            glutSolidCube(arr2[i].size - 0.5);
        glPopMatrix();
        }
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
