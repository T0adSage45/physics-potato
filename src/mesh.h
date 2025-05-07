#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include "vector.h"

#define N_MESH_VERTICES 8
#define N_MESH_FACES (6 * 2) // 6 faces, each split into 2 triangles

extern vec3_t mesh_vertices[N_MESH_VERTICES];
extern face_t mesh_faces[N_MESH_FACES];

#endif
