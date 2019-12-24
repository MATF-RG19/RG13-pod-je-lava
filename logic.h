#ifndef __LOGIC__
#define __LOGIC__

struct platform {
    
    float x;
    float z;
    float size;
    float speed;
    int turn;
};

void generate_platforms(struct platform *arr, int n, int z100);
void move_platforms(struct platform *arr1, struct platform *arr2, int n, int time2);
void detect_collision(float x_curr, float z_curr, int *colision,
                      int z100, int z10, int ind, float *v, float *time1,
                      struct platform *arr1, struct platform *arr2);

#endif
