#include <GL/glut.h>
#include "SOIL/SOIL.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "model.h"
#include "draw.h"
#include "camera.h"
#include "planet.h"
#include <sys/time.h>

#define resolution 0   // 1: 1920*1080   0: 800*600
#define fullscreen 0   // 1: fullscreen  0: windowed


GLfloat light_position[] = {
        0, 0, 0, 0
};
GLfloat light_ambient[] = {
        0.5, 0.5, 0.5, 0
};
GLfloat light_diffuse[] = {
        0.5, 0.5, 0, 0
};
GLfloat light_specular[] = {
        1, 1, 1, 0
};

typedef GLubyte Pixel;
struct Camera camera;
struct Action action;
struct Action {

    int move_forward;
    int move_backward;
    int step_left;
    int step_right;
    int move_up;
    int move_down;
    int rotate_mercury;
    int move_mercury;
    int rotate_venus;
    int move_venus;
    int rotate_earth;
    int move_earth;
    int rotate_mars;
    int move_mars;
    int rotate_jupiter;
    int move_jupiter;
    int rotate_saturn;
    int move_saturn;
    int rotate_uranus;
    int move_uranus;
    int rotate_neptune;
    int move_neptune;
    int increase_light;
    int decrease_light;

};

World world;
Rotate rotate;
Move move;

int WINDOW_WIDTH;
int WINDOW_HEIGHT;
int mouse_x, mouse_y;
int previous_time;
int help, help_on = 0;
float speed = 80;
float angle = 175;
float degreeMercury, degreeVenus, degreeEarth, degreeMars, degreeJupiter, degreeSaturn, degreeUranus, degreeNeptune;


double calc_elapsed_time() {

    int current_time;
    double elapsed_time;

    current_time = glutGet(GLUT_ELAPSED_TIME);
    elapsed_time = (double) (current_time - previous_time) / 1000.0;
    previous_time = current_time;

    return elapsed_time;

}


void update_camera_position(struct Camera *camera, double elapsed_time) {

    double distance;

    distance = elapsed_time * MOVE_SPEED * speed;

    if (action.move_forward == TRUE) {

        move_camera_forward(camera, distance);

    }


    if (action.move_backward == TRUE) {

        move_camera_backward(camera, distance);

    }


    if (action.step_left == TRUE) {

        step_camera_left(camera, distance);

    }


    if (action.step_right == TRUE) {

        step_camera_right(camera, distance);

    }


    if (action.move_up == TRUE) {

        move_camera_up(camera, distance);

    }


    if (action.move_down == TRUE) {

        move_camera_down(camera, distance);

    }


    if (action.increase_light == TRUE) {

        if (light_ambient[0] < 1)
            light_ambient[0] = light_ambient[1] = light_ambient[2] += 0.01;

    }


    if (action.decrease_light == TRUE) {

        if (light_ambient[0] > -0.51)
            light_ambient[0] = light_ambient[1] = light_ambient[2] -= 0.01;

    }


}


void specialFunc(int key, int x, int y) {

    switch (key) {

        case GLUT_KEY_F1:
            if (help_on) {

                help_on = 0;

                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);

            } else {

                help_on = 1;
                GLfloat ones[] = {
                        1, 1, 1, 1
                };
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ones);
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ones);

            }


    }


}


void reshape(GLsizei width, GLsizei height) {

    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (!help_on)
        gluPerspective(50.0, (GLdouble) width / (GLdouble) height, 0.1, 20000.0);
    else
        gluOrtho2D(0, width, height, 0);

}


void draw_help() {


    GLfloat ones[] = {
            1, 1, 1, 1
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ones);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ones);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, help);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3f(WINDOW_WIDTH, 0, 0);
    glTexCoord2f(1, 1);
    glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    glTexCoord2f(0, 1);
    glVertex3f(0, WINDOW_HEIGHT, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutSwapBuffers();

}


void rotation_of_objects(Rotate *rotate) {


    if (action.rotate_mercury == TRUE)
        rotate->mercury_rotation += 0.5;
    if (action.rotate_venus == TRUE)
        rotate->venus_rotation += 0.5;
    if (action.rotate_earth == TRUE)
        rotate->earth_rotation += 0.5;
    if (action.rotate_mars == TRUE)
        rotate->mars_rotation += 0.5;
    if (action.rotate_jupiter == TRUE)
        rotate->jupiter_rotation += 0.5;
    if (action.rotate_saturn == TRUE)
        rotate->saturn_rotation += 0.5;
    if (action.rotate_uranus == TRUE)
        rotate->uranus_rotation += 0.5;
    if (action.rotate_neptune == TRUE)
        rotate->neptune_rotation += 0.5;

    rotate->sun_rotation += 0.05;


}


void movement_of_objects(Move *move) {

    double divide = 1.4;
    double increment = 10;

    if (move->mercury.x == 0)
        move->mercury.x = defaultX.mercury;
    if (move->venus.x == 0)
        move->venus.x = defaultX.venus;
    if (move->earth.x == 0)
        move->earth.x = defaultX.earth;
    if (move->mars.x == 0)
        move->mars.x = defaultX.mars;
    if (move->jupiter.x == 0)
        move->jupiter.x = defaultX.jupiter;
    if (move->saturn.x == 0)
        move->saturn.x = defaultX.saturn;
    if (move->uranus.x == 0)
        move->uranus.x = defaultX.uranus;
    if (move->neptune.x == 0)
        move->neptune.x = defaultX.neptune;


    if (action.move_mercury == TRUE) {

        degreeMercury += increment;
        double angle = degree_to_radian(degreeMercury);
        move->mercury.x = cos(angle) * defaultX.mercury;
        move->mercury.y = sin(angle) * (defaultX.mercury / divide);
        move->mercury.z = 0;

    }

    if (action.move_venus == TRUE) {

        degreeVenus += increment/2;
        double angle = degree_to_radian(degreeVenus);
        move->venus.x = cos(angle) * defaultX.venus;
        move->venus.y = sin(angle) * (defaultX.venus / divide);
        move->venus.z = 0;

    }

    if (action.move_earth == TRUE) {

        degreeEarth += increment/4;
        double angle = degree_to_radian(degreeEarth);
        move->earth.x = cos(angle) * defaultX.earth;
        move->earth.y = sin(angle) * (defaultX.earth / divide);
        move->earth.z = 0;

    }

    if (action.move_mars == TRUE) {

        degreeMars += increment/6;
        double angle = degree_to_radian(degreeMars);
        move->mars.x = cos(angle) * defaultX.mars;
        move->mars.y = sin(angle) * (defaultX.mars / divide);
        move->mars.z = 0;

    }

    if (action.move_jupiter == TRUE) {

        degreeJupiter += increment/12;
        double angle = degree_to_radian(degreeJupiter);
        move->jupiter.x = cos(angle) * defaultX.jupiter;
        move->jupiter.y = sin(angle) * (defaultX.jupiter / divide);
        move->jupiter.z = 0;

    }

    if (action.move_saturn == TRUE) {

        degreeSaturn += increment/16;
        double angle = degree_to_radian(degreeSaturn);
        move->saturn.x = cos(angle) * defaultX.saturn;
        move->saturn.y = sin(angle) * (defaultX.saturn / divide);
        move->saturn.z = 0;

    }

    if (action.move_uranus == TRUE) {

        degreeUranus += increment/20;
        double angle = degree_to_radian(degreeUranus);
        move->uranus.x = cos(angle) * defaultX.uranus;
        move->uranus.y = sin(angle) * (defaultX.uranus / divide);
        move->uranus.z = 0;

    }

    if (action.move_neptune == TRUE) {

        degreeNeptune += increment/24;
        double angle = degree_to_radian(degreeNeptune);
        move->neptune.x = cos(angle) * (defaultX.neptune);
        move->neptune.y = sin(angle) * (defaultX.neptune / divide);

    }


}


void display() {

    if (!help_on) {

        double elapsed_time;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        elapsed_time = calc_elapsed_time();
        update_camera_position(&camera, elapsed_time);
        set_view_point(&camera);


        glLightfv(GL_LIGHT1, GL_POSITION, light_position);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, light_ambient);
        glEnable(GL_LIGHT1);


        draw_environment(world, &rotate, move);
        movement_of_objects(&move);
        rotation_of_objects(&rotate);
        reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
        glutSwapBuffers();


    } else {

        draw_help();

    }


}


void mouse_handler(int button, int state, int x, int y) {

    mouse_x = x;
    mouse_y = y;

}


void motion_handler(int x, int y) {

    double horizontal, vertical;

    horizontal = mouse_x - x;
    vertical = mouse_y - y;

    rotate_camera(&camera, horizontal, vertical);

    mouse_x = x;
    mouse_y = y;

    glutPostRedisplay();

}


void key_handler(unsigned char key, int x, int y) {

    switch (key) {

        case 'w':
            action.move_forward = TRUE;
            break;
        case 's':
            action.move_backward = TRUE;
            break;
        case 'a':
            action.step_left = TRUE;
            break;
        case 'd':
            action.step_right = TRUE;
            break;
        case 'c':
            action.move_down = TRUE;
            break;
        case 32:
            action.move_up = TRUE;
            break;
        case 'q':
            if (action.rotate_mercury == FALSE)
                action.rotate_mercury = TRUE;
            else action.rotate_mercury = FALSE;
            if (action.rotate_venus == FALSE)
                action.rotate_venus = TRUE;
            else action.rotate_venus = FALSE;
            if (action.rotate_earth == FALSE)
                action.rotate_earth = TRUE;
            else action.rotate_earth = FALSE;
            if (action.rotate_mars == FALSE)
                action.rotate_mars = TRUE;
            else action.rotate_mars = FALSE;
            if (action.rotate_jupiter == FALSE)
                action.rotate_jupiter = TRUE;
            else action.rotate_jupiter = FALSE;
            if (action.rotate_saturn == FALSE)
                action.rotate_saturn = TRUE;
            else action.rotate_saturn = FALSE;
            if (action.rotate_uranus == FALSE)
                action.rotate_uranus = TRUE;
            else action.rotate_uranus = FALSE;
            if (action.rotate_neptune == FALSE)
                action.rotate_neptune = TRUE;
            else action.rotate_neptune = FALSE;
            break;
        case 'e':
            if (action.move_mercury == FALSE)
                action.move_mercury = TRUE;
            else action.move_mercury = FALSE;
            if (action.move_venus == FALSE)
                action.move_venus = TRUE;
            else action.move_venus = FALSE;
            if (action.move_earth == FALSE)
                action.move_earth = TRUE;
            else action.move_earth = FALSE;
            if (action.move_mars == FALSE)
                action.move_mars = TRUE;
            else action.move_mars = FALSE;
            if (action.move_jupiter == FALSE)
                action.move_jupiter = TRUE;
            else action.move_jupiter = FALSE;
            if (action.move_saturn == FALSE)
                action.move_saturn = TRUE;
            else action.move_saturn = FALSE;
            if (action.move_uranus == FALSE)
                action.move_uranus = TRUE;
            else action.move_uranus = FALSE;
            if (action.move_neptune == FALSE)
                action.move_neptune = TRUE;
            else action.move_neptune = FALSE;
            break;
        case '+':
            action.increase_light = TRUE;
            break;
        case '-':
            action.decrease_light = TRUE;
            break;
        case 27:
            exit(0);

    }


    glutPostRedisplay();

}


void key_up_handler(unsigned char key, int x, int y) {

    switch (key) {

        case 'w':
            action.move_forward = FALSE;
            break;
        case 's':
            action.move_backward = FALSE;
            break;
        case 'a':
            action.step_left = FALSE;
            break;
        case 'd':
            action.step_right = FALSE;
            break;
        case 'c':
            action.move_down = FALSE;
            break;
        case 32:
            action.move_up = FALSE;
            break;
        case '+':
            action.increase_light = FALSE;
            break;
        case '-':
            action.decrease_light = FALSE;
            break;
        case 27:
            exit(0);

    }


    glutPostRedisplay();

}


void idle() {

    glutPostRedisplay();

}


GLuint load_texture(const char *filename) {

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint texture_name;
    Pixel *image;
    glGenTextures(1, &texture_name);

    int width;
    int height;

    image = (Pixel *) SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
    glBindTexture(GL_TEXTURE_2D, texture_name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (Pixel *) image);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    return texture_name;

}


void set_callbacks() {

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key_handler);
    glutKeyboardUpFunc(key_up_handler);
    glutMouseFunc(mouse_handler);
    glutMotionFunc(motion_handler);
    glutIdleFunc(idle);
    glutSpecialFunc(specialFunc);
    if (fullscreen == 1) glutFullScreen();

}


void initialize() {

    set_callbacks();
    init_camera(&camera);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearDepth(1.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_ambient);
    glEnable(GL_LIGHTING);
    help = load_texture("textures//help.png");
    init_entities(&world);
    glEnable(GL_TEXTURE_2D);

}


int main(int argc, char **argv) {


    glutInit(&argc, argv);

    if (resolution == 1) {
        glutInitWindowSize(1920, 1080);
    } else {
        glutInitWindowSize(800, 600);
    }

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    int wnd = glutCreateWindow("Solar System");
    glutSetWindow(wnd);
    initialize();

    action.move_forward = FALSE;
    action.move_backward = FALSE;
    action.step_left = FALSE;
    action.step_right = FALSE;

    action.rotate_mercury = TRUE;
    action.rotate_venus = TRUE;
    action.rotate_earth = TRUE;
    action.rotate_mars = TRUE;
    action.rotate_jupiter = TRUE;
    action.rotate_saturn = TRUE;
    action.rotate_uranus = TRUE;
    action.rotate_neptune = TRUE;


    glutMainLoop();
    return 0;

}