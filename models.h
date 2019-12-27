#ifndef __MODELS__
#define __MODELS__

struct platform;

void draw_player(float x_curr, float y_curr, float z_curr, float size, float t, float rotx, float rotz);
void draw_platforms(struct platform *arr1, struct platform *arr2, int n);
void lava(int u1, int u2, int v1, int v2, float t);
void wall(int x, int u1, int u2, int v1, int v2);
void show_text(int x, int y, char *s);

#endif
