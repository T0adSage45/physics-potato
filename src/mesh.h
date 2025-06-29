#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include "vector.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 faces, each split into 2 triangles

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

typedef struct
{
  vec3_t *vertices;
  face_t *faces;
  vec3_t rotation;
  vec3_t scale;
  vec3_t translation;
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);
void load_obj_mesh_data(char *filename);
#endif
