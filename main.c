#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/freeglut.h>

int window_res[] = {1280,720};
float fps = 60.0;

int pd,pl,bs,brx,bry,brw,brl,score,inputs[2],plv,sm;
float bsp,bdx,bdy,bx,by,px,py,pw,ps;

float brick_colors[4][3] = {{1.0,0.0,0.0},{1.0,0.5,0.0},{0.0,1.0,0.0},{1.0,1.0,0.0}};

int brick_collide_allow = 1;
int num_bricks = 100;
int brick_vals[200];

void reset();
void reset_ball();

void draw_paddle(){
    glColor3f(0,0,1);
    glLineWidth(pw);
    glBegin(GL_LINES);
    glVertex2i(px,py);
    glVertex2i(px,py+pl);
    glEnd();
}

void draw_ball(){
    glColor3f(1,1,1);
    glPointSize(bs);
    glBegin(GL_POINTS);
    glVertex2i(bx,by);
    glEnd();
}

void display_text(){
    char score_string[10];
    char lives_string[10];
    sprintf(score_string, "%d", score);
    sprintf(lives_string, "%d", plv);
    char string[25] = "SCORE: ";
    strcat(string, score_string);
    strcat(string, "\nLIVES: ");
    strcat(string, lives_string);
    glColor3f(1, 1, 1);
    glRasterPos2f(10, 30);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, string);

}

void bricks(int first){
    int drawn_bricks = 0;
    int current_color = 0;
    for (int i = 0; i<num_bricks; i++){
        if (by+(bs/2) >= bry-(brl/2) && by-(bs/2) <= bry+(brl/2) && bx+(bs/2) >= brx-(brw/2) && bx-(bs/2) <= brx+(brw/2) && brick_vals[i] == 1 && brick_collide_allow == 1){
            brick_collide_allow = 0;
            brick_vals[i] = 0;
            bdy = -bdy;
        }
        if ((first == 0 && brick_vals[i] == 1) || first == 1){
            glColor3f(brick_colors[current_color][0], brick_colors[current_color][1], brick_colors[current_color][2]);
            drawn_bricks += 1;
        } else {
            glColor3f(0,0,0);
        }
        glLineWidth(brw);
        glBegin(GL_LINES);
        glVertex2i(brx,bry);
        glVertex2i(brx,bry+brl);
        glEnd();
        if (first == 1){
            brick_vals[i] = 1;
        }
        brx += brw+10;
        if (brx >= window_res[0]-30){
            brx = 40;
            bry += brl+10;
            current_color++;
            if (current_color >= 4){
                current_color = 0;
            }
        }
    }
    brx = 40;
    bry = 100;
    score = (num_bricks-drawn_bricks)+(sm*num_bricks);
    if (drawn_bricks == 0){
        sm += 1;
        reset_ball();
        bricks(1);
    }
}

void reset_ball() {
    bx = 680;
    by = 350;
    bsp = 4;
    bdx = 0.5;
    bdy = 1;
}

void reset(){
    px = window_res[0]/2;
    py = 650;
    pw = 80;
    pl = 20;
    ps = 15;
    brw = 40;
    brl = 20;
    brx = 40;
    bry = 100;
    bs = 15;
    plv = 3;
    sm = 0;
    reset_ball();
    bricks(1);
}

void paddle_movement(){
    if (inputs[0] == 1 && px > (pw/2)){px -= ps;}
    if (inputs[1] == 1 && px < window_res[0]-(pw/2)){px += ps;}
}

void ball_movement(){
    bx += bsp * bdx;
    by += bsp * bdy;
    int pre_val = brick_collide_allow;
    brick_collide_allow = 1;
    if (plv == 0){reset();}
    if (by > window_res[1]){
        reset_ball();
        plv--;
    }
    else if (by < 0){bdy = -bdy;}
    else if (bx < 0){bdx = -bdx;}
    else if (bx > window_res[0]){bdx = -bdx;}
    else if (by >= py-(pl/2) && by <= py+(pl/2) && bx >= px-(pw/1.8) && bx <= px+(pw/1.8)){
        bdx = (bx-px)/(pw/3);
        if (bdx >= 1){bdy = bdx - 2;}
        else if (bdx <= -1){bdy = -(bdx + 2);}
        else {bdy = -1;}
        bsp += 0.12;
    } else {brick_collide_allow = pre_val;}
}

void button_down(unsigned char key, int x, int y){
    if (key == 'a'){inputs[0] = 1;}
    if (key == 'd'){inputs[1] = 1;}
}

void button_up(unsigned char key, int x, int y){
    if (key == 'a'){inputs[0] = 0;}
    if (key == 'd'){inputs[1] = 0;}
}

void cap_fps(int){
    glutTimerFunc(1000/fps, cap_fps, 0);
    glutPostRedisplay();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutReshapeWindow(window_res[0], window_res[1]);
    bricks(0);
    draw_paddle();
    draw_ball();
    paddle_movement();
    ball_movement();
    display_text();
    glutSwapBuffers();
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(window_res[0], window_res[1]);
    glutInitWindowPosition(200,200);
    glutCreateWindow("Breakout! (C + OpenGL)");
    glClearColor(0,0,0,0);
    gluOrtho2D(0,window_res[0],window_res[1],0);
    reset();
    glutTimerFunc(0, cap_fps, 0);
    glutDisplayFunc(display);
    glutKeyboardFunc(button_down);
    glutKeyboardUpFunc(button_up);
    glutMainLoop();
}