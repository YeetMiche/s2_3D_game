#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include "array_statistics.h"

#define FPS_LIMIT 60
#define STANDARD_TEXT GLUT_BITMAP_TIMES_ROMAN_24
#define ship_graphics_speed 10 //how many seconds should pass in the simulation per real time second (cannot be higher than frame rate)

//variables for space ship
long double ship_acc[100];
long double ship_rot[100];
long double ship_rot_total[101] = {0};
long double ship_temp[100];
long double ship_pos_x[101] = {0};
long double ship_pos_y[101] = {0};
long double ship_speed_x[101] = {0};
long double ship_speed_y[101] = {0};

long double max_speed;
long double max_distance;
long double total_distance;
long double avrg_temp;
long double max_temp;
long double min_temp;
long double variance_temp;

//variables for graphics
int window_x = 800;
int window_y = 800;
int ship_size = 40;
int grid_scale = 2;
int grid_xoffset = -200;
int grid_yoffset = 0;

long double ship_graphics_x = 0;
long double ship_graphics_y = 0;
long double ship_graphics_rot = 0;
long double ship_delta_x;
long double ship_delta_y;
long double ship_delta_rot;
char ship_x_str[10];
char ship_y_str[10];
char ship_rot_str[10];
char simulation_sec_str[4];
char ship_temp_str[10];
float ship_path_x[100];
float ship_path_y[100];
int path_length = 100;
float path_red[100];
float path_blue[100];

//variables for fps counter and fps limit
char fps_str[10];
int fps_limit_start;
int frame_sec = 0;
int frame = 0;
int fps = 0;
int fps_counter_start = 0;

#include "graphics.h"

void read_csv(){
    FILE* spaceship_data = fopen("spaceship_data.csv", "r");
    if (spaceship_data != NULL){
        int i = 0;
        while (feof(spaceship_data) == 0){
            fscanf(spaceship_data, "%Lf,%Lf,%Lf\n", &ship_acc[i], &ship_rot[i], &ship_temp[i]);
            i++;
        }
    }
    else{
        printf("\n\nERROR while trying to open spaceship_data.csv\n\n");
        exit(-1);
    }
}

void write_csv(){
    FILE* data_out = fopen("data_out.csv", "w");
    if (data_out != NULL){
        for (int i=1; i<101; i++){
            fprintf(data_out, "%0.16Lf,%0.16Lf,%0.16Lf\n", ship_pos_x[i], ship_pos_y[i], ship_rot_total[i]);
        }
    }
    else{
        printf("\n\nERROR while trying to open data_out.csv\n\n");
        exit(-1);
    }
}

//converts ship acc and rot to speed and position x,y
void convert_ship_position(){
    for (int i=1; i<101; i++){
        ship_rot_total[i] = ship_rot_total[i-1] + ship_rot[i-1];
        ship_speed_x[i] = ship_speed_x[i-1] + (ship_acc[i-1] * cos(ship_rot_total[i]));
        ship_speed_y[i] = ship_speed_y[i-1] + (ship_acc[i-1] * sin(ship_rot_total[i]));

        ship_pos_x[i] += ship_pos_x[i-1] + ship_speed_x[i];
        ship_pos_y[i] += ship_pos_y[i-1] + ship_speed_y[i];

    }
}

void display(){
    fps_limit_start = clock();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw calls
    draw_grid();
    draw_ship();
    drawShipPath();
    draw_overlay_bg();
    draw_ship_information();
    draw_color_scale();

    //fps limit and displaying
    snprintf(fps_str, 10, "%d", fps);
    draw_text(window_x - window_x*0.1, window_y - window_y*0.95, STANDARD_TEXT, strncat(fps_str, "FPS", 3),1,1,1);

    //wait for FPS limit
    while ((fps_limit_start + 1000/FPS_LIMIT) - clock() > 0){
        ;
    }
    frame_sec += 1;
    if (clock() - fps_counter_start > 1000){
        fps = frame_sec;
        frame_sec = 0;
        fps_counter_start = clock();
    }
    frame += 1;

    glutPostRedisplay();
    glutSwapBuffers();
}

void error_handling(){
    if (ship_graphics_speed > FPS_LIMIT) {printf("\n\nERROR: graphics speed cannot be greater than FPS limit!\n\n"); exit(-1);}
}

void init(){
    error_handling();

    //calculate ship data
    read_csv();
    convert_ship_position();
    write_csv();
    max_speed = get_max_xy(ship_speed_x, ship_speed_y, 101);
    max_distance = get_max_xy(ship_pos_x, ship_pos_y, 101);
    total_distance = get_sum_xy(ship_pos_x, ship_pos_y, 101);
    avrg_temp = get_avrg(ship_temp, 100);
    max_temp = get_max(ship_temp, 100);
    min_temp = get_min(ship_temp, 100);
    variance_temp = get_variance(ship_temp, 100);

    printf("Max speed is: %0.16Lf\n", max_speed);
    printf("Max distance is: %0.16Lf\n", max_distance);
    printf("Total distance is: %0.16Lf\n", total_distance);
    printf("Average temperature is: %0.16Lf\n", avrg_temp);
    printf("Maximum temperature is: %0.16Lf\n", max_temp);
    printf("Minimum temperature is: %0.16Lf\n", min_temp);
    printf("Variance of temperature is: %0.16Lf\n", variance_temp);


    //initialize graphics
    glClearColor(0.12,0.12,0.12,0);
    gluOrtho2D(0, window_x, window_y,0);
    fps_counter_start = clock();
    convert_temp_to_color(path_red, path_blue, 100, ship_temp, min_temp, max_temp);
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_x,window_y);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Spaceship Flight");
    glutDisplayFunc(display);
    init();
    
    glutMainLoop();
}


void buttonsDown(unsigned char key, int x, int y){
     if (key=='a'){Keys.a=1;}
     if (key=='d'){Keys.d=1;}
     if (key=='w'){Keys.w=1;}
     if (key=='s'){Keys.s=1;}
}

void buttonsUp(unsigned char key, int x, int y){
     if (key=='a'){Keys.a=0;}
     if (key=='d'){Keys.d=0;}
     if (key=='w'){Keys.w=0;}
     if (key=='s'){Keys.s=0;}
}

