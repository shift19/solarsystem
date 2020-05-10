#include "draw.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "camera.h"

void draw_triangles(const struct Model *model) {

    int i, k;
    int vertex_index, normal_index;
    double x, y, z, normal_x, normal_y, normal_z;

    glBegin(GL_TRIANGLES);

    for (i = 0; i < model->n_triangles; ++i) {

        for (k = 0; k < 3; ++k) {

            normal_index = model->triangles[i].points[k].normal_index;
            normal_x = model->normals[normal_index].x;
            normal_y = model->normals[normal_index].y;
            normal_z = model->normals[normal_index].z;
            glNormal3d(normal_x, normal_y, normal_z);
            vertex_index = model->triangles[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            switch (k) {

                case 0:
                    glTexCoord2f(0, 0);
                    break;
                case 1:
                    glTexCoord2f(0.1 * z, 0);
                    break;
                case 2:
                    glTexCoord2f(0, 0.01);
                    break;

            }

            glVertex3d(x, y, z);


        }


    }


    glEnd();

}


void draw_quads(const struct Model *model) {

    int i, k;
    int vertex_index, texture_index;
    double x, y, z, u, v;

    glBegin(GL_QUADS);

    for (i = 0; i < model->n_quads; ++i) {

        for (k = 0; k < 4; ++k) {


            texture_index = model->quads[i].points[k].texture_index;
            u = model->texture_vertices[texture_index].u;
            v = model->texture_vertices[texture_index].v;
            glTexCoord2f(u, 1 - v);

            vertex_index = model->quads[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            glVertex3d(x, y, z);

        }


    }


    glEnd();

}


void draw_normals(const struct Model *model, double length) {

    int i;
    double x1, y1, z1, x2, y2, z2;

    glColor3f(0, 0, 1);

    glBegin(GL_LINES);

    for (i = 0; i < model->n_vertices; ++i) {

        x1 = model->vertices[i].x;
        y1 = model->vertices[i].y;
        z1 = model->vertices[i].z;
        x2 = x1 + model->normals[i].x * length;
        y2 = y1 + model->normals[i].y * length;
        z2 = z1 + model->normals[i].z * length;
        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y2, z2);

    }

    glEnd();

}


void draw_model(const struct Model *model) {

    draw_triangles(model);
    draw_quads(model);

}


void draw_skybox_bottom(Entity skybox) {

    double theta, phi1, phi2;
    double x1, y1, z1;
    double x2, y2, z2;
    double u, v1, v2;

    int n_slices, n_stacks;
    double radius;
    int i, k;

    n_slices = 12;
    n_stacks = 6;
    radius = SKYBOX_SIZE;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, skybox.texture);
    glScaled(radius, radius, radius);

    glColor3f(1, 1, 1);

    glBegin(GL_TRIANGLE_STRIP);

    for (i = 0; i < n_stacks; ++i) {

        v1 = (double) i / n_stacks;
        v2 = (double) (i + 1) / n_stacks;
        phi1 = v1 * M_PI / 2.0;
        phi2 = v2 * M_PI / 2.0;
        for (k = 0; k <= n_slices; ++k) {

            u = (double) k / n_slices;
            theta = u * 2.0 * M_PI;
            x1 = cos(theta) * cos(phi1);
            y1 = sin(theta) * cos(phi1);
            z1 = sin(phi1);
            x2 = cos(theta) * cos(phi2);
            y2 = sin(theta) * cos(phi2);
            z2 = sin(phi2);
            glTexCoord2d(u, 1.0 - v1);
            glVertex3d(x1, y1, -z1);
            glTexCoord2d(u, 1.0 - v2);
            glVertex3d(x2, y2, -z2);

        }


    }


    glEnd();
    glPopMatrix();

}


void draw_skybox_top(Entity skybox) {

    double theta, phi1, phi2;
    double x1, y1, z1;
    double x2, y2, z2;
    double u, v1, v2;

    int n_slices, n_stacks;
    double radius;
    int i, k;

    n_slices = 12;
    n_stacks = 6;
    radius = SKYBOX_SIZE;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, skybox.texture);
    glScaled(radius, radius, radius);

    glColor3f(1, 1, 1);

    glBegin(GL_TRIANGLE_STRIP);

    for (i = 0; i < n_stacks; ++i) {

        v1 = (double) i / n_stacks;
        v2 = (double) (i + 1) / n_stacks;
        phi1 = v1 * M_PI / 2.0;
        phi2 = v2 * M_PI / 2.0;
        for (k = 0; k <= n_slices; ++k) {

            u = (double) k / n_slices;
            theta = u * 2.0 * M_PI;
            x1 = cos(theta) * cos(phi1);
            y1 = sin(theta) * cos(phi1);
            z1 = sin(phi1);
            x2 = cos(theta) * cos(phi2);
            y2 = sin(theta) * cos(phi2);
            z2 = sin(phi2);
            glTexCoord2d(u, 1.0 - v1);
            glVertex3d(x1, y1, z1);
            glTexCoord2d(u, 1.0 - v2);
            glVertex3d(x2, y2, z2);

        }


    }


    glEnd();
    glPopMatrix();

}


void draw_environment(World world, Rotate *rotate, Move move) {

    glEnable(GL_TEXTURE_2D);


    //Draw the bottom skybox.
    draw_skybox_bottom(world.skybox);


    //Draw the top skybox.
    draw_skybox_top(world.skybox);


    //Draw the sun.
    glPushMatrix();

    glTranslatef(0, 0, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.sun.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.sun.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->sun_rotation, 1, 1, 1);
    draw_model(&world.sun.model);

    glPopMatrix();


    //Draw the mercury.
    glPushMatrix();

    glTranslatef(move.mercury.x, move.mercury.y, move.mercury.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.mercury.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.mercury.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->mercury_rotation, 0, 0, 1);
    draw_model(&world.mercury.model);

    glPopMatrix();

    //Draw the venus.
    glPushMatrix();

    glTranslatef(move.venus.x, move.venus.y, move.venus.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.venus.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.venus.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->venus_rotation, 0, 0, 1);
    draw_model(&world.venus.model);

    glPopMatrix();

    //Draw the earth.
    glPushMatrix();

    glTranslatef(move.earth.x, move.earth.y, move.earth.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.earth.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.earth.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->earth_rotation, 0, 0, 1);
    draw_model(&world.earth.model);

    glPopMatrix();

    //Draw the mars.
    glPushMatrix();

    glTranslatef(move.mars.x, move.mars.y, move.mars.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.mars.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.mars.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->mars_rotation, 0, 0, 1);
    draw_model(&world.mars.model);

    glPopMatrix();

    //Draw the jupiter.
    glPushMatrix();

    glTranslatef(move.jupiter.x, move.jupiter.y, move.jupiter.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.jupiter.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.jupiter.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->jupiter_rotation, 0, 0, 1);
    draw_model(&world.jupiter.model);

    glPopMatrix();

    //Draw the saturn.
    glPushMatrix();

    glTranslatef(move.saturn.x, move.saturn.y, move.saturn.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.saturn.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.saturn.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->saturn_rotation, 0, 0, 1);
    draw_model(&world.saturn.model);

    glPopMatrix();

    //Draw the uranus.
    glPushMatrix();

    glTranslatef(move.uranus.x, move.uranus.y, move.uranus.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.uranus.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.uranus.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->uranus_rotation, 0, 0, 1);
    draw_model(&world.uranus.model);

    glPopMatrix();

    //Draw the neptune.
    glPushMatrix();

    glTranslatef(move.neptune.x, move.neptune.y, move.neptune.z);
    glMaterialfv(GL_FRONT, GL_AMBIENT, world.neptune.material_ambient);
    glBindTexture(GL_TEXTURE_2D, world.neptune.texture);
    glScalef(1.0f, 1.0f, 1.0f);
    glRotatef(rotate->neptune_rotation, 0, 0, 1);
    draw_model(&world.neptune.model);

    glPopMatrix();


}