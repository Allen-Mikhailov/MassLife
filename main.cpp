#include <stdio.h>
#include <cstdlib> 
#include <glew.h>
#include <glut.h>
#include <time.h>

#define _USE_MATH_DEFINES
#include <cmath>

#define M_PI 3.14159265358979323846

#define TIME_STEP (1/60.0)

#define PARTICLE_TYPE_COUNT 5
#define CLOSE_DISTANCE .03
#define FORCE_DISTANCE .05
#define FORCE_MULTI .03
#define PUSH_FORCE .009
#define FRICTION 0.99


struct Particle
{
    float x;
    float y;

    float vx;
    float vy;

    int type;
 
};


int particle_count;
Particle* partciles;

float* type_matrix;
float* type_colors;

static float rand_float()
{
    return (float)std::rand() / RAND_MAX;
}

static float rand_1_float() {
    return rand_float() * 2.0 - 1.0;
}

void drawCircle(float r, float x, float y, int resolution, float rc, float gc, float bc) {
    float i = 0.0f;

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(rc, gc, bc);
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

void update_particles()
{
    float dt = TIME_STEP;
    float dt_2 = dt / 2.0;
    Particle* p = partciles;
    for (int i = 0; i < particle_count; i++)
    {
        float ax = 0;
        float ay = 0;

        Particle* p2 = partciles;

        for (int j = 0; j < particle_count; j++)
        {   
            if (j == i) { 
                p2++;
                continue; 
            }

            float dx = p2->x - p->x;
            float dy = p2->y - p->y;

            float distance = sqrt(dx * dx + dy * dy);

            float matrix_value = type_matrix[p2->type * PARTICLE_TYPE_COUNT + p->type];
            if (distance < CLOSE_DISTANCE)
            {
                ax += -(dx/ distance) * (CLOSE_DISTANCE - distance) * (PUSH_FORCE / CLOSE_DISTANCE);
                ay += -(dy / distance) * (CLOSE_DISTANCE - distance) * (PUSH_FORCE / CLOSE_DISTANCE);
            }
            else if (distance < CLOSE_DISTANCE + FORCE_DISTANCE) {
                ax += (dx / distance) * (distance - CLOSE_DISTANCE) * (FORCE_MULTI / FORCE_DISTANCE) * matrix_value;
                ay += (dy / distance) * (distance - CLOSE_DISTANCE) * (FORCE_MULTI / FORCE_DISTANCE) * matrix_value;
            }
            else if (distance < CLOSE_DISTANCE + FORCE_DISTANCE * 2) {
                ax += (dx / distance) * (FORCE_DISTANCE * 2 + CLOSE_DISTANCE - distance) 
                    * (FORCE_MULTI / FORCE_DISTANCE) * matrix_value;
                ay += (dy / distance) * (FORCE_DISTANCE * 2 + CLOSE_DISTANCE - distance)
                    * (FORCE_MULTI / FORCE_DISTANCE) * matrix_value;
            }


            p2++;
        }

        p->x += p->vx * dt + 0.5 * ax * dt * dt;
        p->y += p->vy * dt + 0.5 * ay * dt * dt;

        p->vx = (p->vx + ax * dt) * FRICTION;
        p->vy = (p->vy + ay * dt) * FRICTION;

        if (p->x > 1)
        {
            p->x = 1;
            p->vx = -p->vx;
        } else if (p->x < -1) {
            p->x = -1;
            p->vx = -p->vx;
        }

        if (p->y > 1)
        {
            p->y = 1;
            p->vy = -p->vy;
        } else if (p->y < -1) {
            p->y = -1;
            p->vy = -p->vy;
        }

        p++;
    }
}

void display() {  // Display function will draw the image.

    glClearColor(0, 0, 0, 1);  // (In fact, this is the default.)
    glClear(GL_COLOR_BUFFER_BIT);

    update_particles();

    Particle* p = partciles;
    for (int i = 0; i < particle_count; i++)
    {
        drawCircle(.01, p->x, p->y, 3, type_colors[p->type*3], type_colors[p->type * 3+1], type_colors[p->type * 3+2]);
        p++;
    }

    glutSwapBuffers(); // Required to copy color buffer onto the screen.

}

int main(int argc, char** argv) {  // Initialize GLUT and 

    particle_count = 1000;

    srand(time(NULL));

    partciles = (Particle*)malloc(sizeof Particle * particle_count);
    Particle* particle = partciles;
    for (int i = 0; i < particle_count; i++)
    {   
        particle->x = rand_1_float();
        particle->y = rand_1_float();
        particle->vx = rand_1_float() / 5.0;
        particle->vy = rand_1_float() / 5.0;
        particle->type = (int) (rand_float() * PARTICLE_TYPE_COUNT);
        particle++;
    }

    // Filling Type Matrix
    type_matrix = (float*)malloc(sizeof(float) * PARTICLE_TYPE_COUNT * PARTICLE_TYPE_COUNT);
    float* matrix_fill = type_matrix;
    for (int i = 0; i < PARTICLE_TYPE_COUNT * PARTICLE_TYPE_COUNT; i++)
    {
        *matrix_fill = rand_1_float();
        matrix_fill++;
    }

    // Type Colors
    type_colors = (float*) malloc(sizeof(float) * 3 * PARTICLE_TYPE_COUNT);
    float* type_color_fill = type_colors;
    for (int i = 0; i < PARTICLE_TYPE_COUNT; i++)
    {
        *type_color_fill = 0.5 + rand_float() * .5; // R
        type_color_fill++;
        *type_color_fill = 0.5 + rand_float() * .5; // G
        type_color_fill++;
        *type_color_fill = 0.5 + rand_float() * .5; // B
        type_color_fill++;
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
