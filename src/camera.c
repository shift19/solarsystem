#include "camera.h"
#include "draw.h"
#include <GL/glut.h>
#include <math.h>

#define M_PI 3.14159265358979323846

double degree_to_radian(double degree) {

    return degree * M_PI / 180.0;

}

void init_camera(struct Camera *camera) {

    camera->position.x = 2000;
    camera->position.y = 3000;
    camera->position.z = 0;
    camera->pose.x = 0;
    camera->pose.y = 0;
    camera->pose.z = -90;

}

void set_view_point(const struct Camera *camera) {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-(camera->pose.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->pose.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);

}

void rotate_camera(struct Camera *camera, double horizontal, double vertical) {
    camera->pose.z += horizontal / CAMERA_SPEED;
    camera->pose.x += vertical / CAMERA_SPEED;
    if (camera->pose.z < 0) {

        camera->pose.z += 360.0;

    }

    if (camera->pose.z > 360.0) {

        camera->pose.z -= 360.0;

    }

    if (camera->pose.x < 0) {

        camera->pose.x += 360.0;

    }

    if (camera->pose.x > 360.0) {

        camera->pose.x -= 360.0;

    }


}

void move_camera_up(struct Camera *camera, double distance) {
    if (!(camera->position.z > SKYBOX_SIZE - 1000)) {
        camera->prev_position = camera->position;
        camera->position.z += distance;
    }
}

void move_camera_down(struct Camera *camera, double distance) {
    if (!(camera->position.z < -SKYBOX_SIZE + 1000)) {
        camera->prev_position = camera->position;
        camera->position.z -= distance;
    }
}

void move_camera_backward(struct Camera *camera, double distance) {
    camera->prev_position = camera->position;
    double angle = degree_to_radian(camera->pose.z);
    camera->position.x -= cos(angle) * distance;
    camera->position.y -= sin(angle) * distance;
}

void move_camera_forward(struct Camera *camera, double distance) {
    camera->prev_position = camera->position;
    double angle = degree_to_radian(camera->pose.z);
    camera->position.x += cos(angle) * distance;
    camera->position.y += sin(angle) * distance;
}

void step_camera_right(struct Camera *camera, double distance) {
    camera->prev_position = camera->position;
    double angle = degree_to_radian(camera->pose.z + 90.0);
    camera->position.x -= cos(angle) * distance;
    camera->position.y -= sin(angle) * distance;
}

void step_camera_left(struct Camera *camera, double distance) {
    camera->prev_position = camera->position;
    double angle = degree_to_radian(camera->pose.z - 90.0);
    camera->position.x -= cos(angle) * distance;
    camera->position.y -= sin(angle) * distance;
}