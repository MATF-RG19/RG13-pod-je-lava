#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include "logic.h"

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
