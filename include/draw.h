#ifndef DRAW_H
#define DRAW_H

#include "model.h"

#define SKYBOX_SIZE 10000

typedef struct Position {
    double x;
    double y;
    double z;
} Position;

typedef struct {
    double sun_rotation;
    double mercury_rotation;
    double venus_rotation;
    double mars_rotation;
    double earth_rotation;
    double jupiter_rotation;
    double saturn_rotation;
    double uranus_rotation;
    double neptune_rotation;
} Rotate;

typedef struct {
    Position sun;
    Position mercury;
    Position venus;
    Position mars;
    Position earth;
    Position jupiter;
    Position saturn;
    Position uranus;
    Position neptune;
} Move;

//Draw the model.
void draw_model(const struct Model *model);

//Draw the triangles of the model.
void draw_triangles(const struct Model *model);

//Draw the quads of the model.
void draw_quads(const struct Model *model);

//Draw the normal vectors of the model.
void draw_normals(const struct Model *model, double length);

//Draw the top skybox.
void draw_skybox_top(Entity skybox);

//Draw the bottom skybox.
void draw_skybox_bottom(Entity skybox);

//Draw the entitys to the world.
void draw_environment(World world, Rotate *rotate, Move move);

#endif