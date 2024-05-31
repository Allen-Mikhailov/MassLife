#include <stdio.h>
#include <cstdlib> 
#include <glew.h>
#include <glut.h>

#define _USE_MATH_DEFINES
#include <cmath>

#define M_PI 3.14159265358979323846


struct Particle
{
    float x;
    float y;
    float vx;
    float vy;
 
};


int particle_count = 100;
Particle* partciles;

static float rand_float()
{
    return (float)std::rand() / RAND_MAX;
}

static float rand_1_float() {
    return rand_float() * 2.0 - 1.0;
}

void drawCircle(float r, float x, float y, int resolution) {
    float i = 0.0f;

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0, 0, 1);
    glVertex2f(x, y); // Center
    for (i = 0.0f; i <= resolution; i++)
    {
        //glColor3f(0, 0, 1);
        glVertex2f(r * cos(2 * M_PI * i / resolution) + x, r * sin(2 * M_PI * i / resolution) + y);
    }

    glEnd();
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

void display() {  // Display function will draw the image.

    glClearColor(0, 0, 0, 1);  // (In fact, this is the default.)
    glClear(GL_COLOR_BUFFER_BIT);

    // Temp
    float dt = 1 / 60.0;

    Particle* p = partciles;
    for (int i = 0; i < particle_count; i++)
    {
        p->x += dt * p->vx;
        p->y += dt * p->vy;
        drawCircle(.01, p->x, p->y, 3);
        p++;
    }

    glutSwapBuffers(); // Required to copy color buffer onto the screen.

}

int main(int argc, char** argv) {  // Initialize GLUT and 

    int particle_count = 100;

    partciles = (Particle*)malloc(sizeof Particle * particle_count);
    Particle* particle = partciles;
    for (int i = 0; i < particle_count; i++)
    {   
        particle->x = rand_1_float();
        particle->y = rand_1_float();
        particle->vx = rand_1_float();
        particle->vy = rand_1_float();
        particle++;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);    // Use single color buffer and no depth buffer.
    glutInitWindowSize(500, 500);         // Size of display area, in pixels.
    glutInitWindowPosition(100, 100);     // Location of window in screen coordinates.
    glutCreateWindow("MassLife"); // Parameter is window title.
    glutDisplayFunc(display);            // Called when the window needs to be redrawn.

    timer(0);

    glutMainLoop(); // Run the event loop!  This function does not return.
    // Program ends when user closes the window.

    free(partciles);
    return 0;

}
