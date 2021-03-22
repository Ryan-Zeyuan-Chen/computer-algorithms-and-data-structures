#include "seamcarving.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double pixel_energy(struct rgb_img *im, int y, int x){
    int height = im->height;
    int width = im->width;
    int left = (x - 1 == -1) ? width - 1 : x - 1;
    int right = (x + 1 == width) ? 0 : x + 1;
    int up = (y - 1 == -1) ? height - 1 : y - 1;
    int down = (y + 1 == height) ? 0 : y + 1;
    double r_x = (double)get_pixel(im, y, right, 0) - (double)get_pixel(im, y, left, 0);
    double g_x = (double)get_pixel(im, y, right, 1) - (double)get_pixel(im, y, left, 1);
    double b_x = (double)get_pixel(im, y, right, 2) - (double)get_pixel(im, y, left, 2);
    double r_y = (double)get_pixel(im, down, x, 0) - (double)get_pixel(im, up, x, 0);
    double g_y = (double)get_pixel(im, down, x, 1) - (double)get_pixel(im, up, x, 1);
    double b_y = (double)get_pixel(im, down, x, 2) - (double)get_pixel(im, up, x, 2);
    return sqrt(pow(r_x,2) + pow(g_x,2) + pow(b_x,2) + pow(r_y,2) + pow(g_y,2) + pow(b_y,2));
}

void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    uint8_t energy;
    create_img(grad, im->height, im->width);
    int height = im->height;
    int width = im->width;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            energy = (uint8_t)(pixel_energy(im, i, j)/10.0);
            set_pixel(*grad, i, j, energy, energy, energy);
        }
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    double energy;
    double min_cost;
    int left;
    int right;
    int height = grad->height;
    int width = grad->width;
    *best_arr = (double *)malloc(height * width * sizeof(double));
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            energy = (double)get_pixel(grad, i, j, 0);
            if(i == 0){
                (*best_arr)[i*width+j] = energy;
            }
            else{
                left = (j - 1 == -1) ? j : j - 1;
                right = (j + 1 == width) ? j : j + 1;
                min_cost = fmin((*best_arr)[(i-1)*width+left], (*best_arr)[(i-1)*width+j]);
                (*best_arr)[i*width+j] = energy + fmin(min_cost, (*best_arr)[(i-1)*width+right]);
            }
        }
    }
}

void recover_path(double *best, int height, int width, int **path){
    double inf = INFINITY;
    double min_cost;
    int min_idx;
    *path = (int *)malloc(height * sizeof(int));
    for(int i = height - 1; i >= 0; i--){
        min_cost = inf;
        min_idx = -1;
        for(int j = 0; j < width; j++){
            if(best[i*width+j] < min_cost){
                if(i == height - 1){
                    min_cost = best[i*width+j];
                    min_idx = j;
                }
                else{
                    if(j - (*path)[i+1] <= 1 && j - (*path)[i+1] >= -1){
                        min_cost = best[i*width+j];
                        min_idx = j;
                  }
                }
            }
        }
        (*path)[i] = min_idx;
    }
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    int r;
    int g;
    int b;
    int flag;
    create_img(dest, src->height, (src->width)-1);
    int height = src->height;
    int width = src->width;
    for(int i = 0; i < height; i++){
        flag = 0;
        for(int j = 0; j < width; j++){
            if(j == path[i]){
                flag = 1;
                continue;
            }
            else{
                r = (int)get_pixel(src, i, j, 0);
                g = (int)get_pixel(src, i, j, 1);
                b = (int)get_pixel(src, i, j, 2);
                if(flag){
                    set_pixel(*dest, i, j-1, r, g, b);
                }
                else{
                    set_pixel(*dest, i, j, r, g, b);
                }
            }
        }
    }
}
