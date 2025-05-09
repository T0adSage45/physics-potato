#include "mesh.h"
#include "array.h"
#include "triangle.h"
#include "vector.h"
#include <stdio.h>
#include <string.h>

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0, 0, 0},
};
vec3_t cube_vertices[N_CUBE_VERTICES] = {

    {.x = -1, .y = -1, .z = -1}, // 0
    {.x = -1, .y = 1, .z = -1},  // 1
    {.x = 1, .y = 1, .z = -1},   // 2
    {.x = 1, .y = -1, .z = -1},  // 3
    {.x = 1, .y = 1, .z = 1},    // 4
    {.x = 1, .y = -1, .z = 1},   // 5
    {.x = -1, .y = 1, .z = 1},   // 6
    {.x = -1, .y = -1, .z = 1},  // 7
};

face_t cube_faces[N_CUBE_FACES] = {
    {.a = 1, .b = 2, .c = 3}, {.a = 1, .b = 3, .c = 4},
    {.a = 4, .b = 3, .c = 5}, {.a = 4, .b = 5, .c = 6},
    {.a = 6, .b = 5, .c = 7}, {.a = 6, .b = 7, .c = 8},
    {.a = 8, .b = 7, .c = 2}, {.a = 8, .b = 2, .c = 1},
    {.a = 2, .b = 7, .c = 5}, {.a = 2, .b = 5, .c = 3},
    {.a = 6, .b = 8, .c = 1}, {.a = 6, .b = 1, .c = 4},

};

void load_cube_mesh_data(void) {
  for (int i = 0; i < N_CUBE_VERTICES; i++) {
    vec3_t cube_vertex = cube_vertices[i];
    array_push(mesh.vertices, cube_vertex);
  }
  for (int i = 0; i < N_CUBE_FACES; i++) {
    face_t cube_face = cube_faces[i];
    array_push(mesh.faces, cube_face);
  }
};

void load_obj_mesh_data(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Unable to open file!");
    return;
  }

  char line[1024];
  while (fgets(line, sizeof(line), file) != NULL) {
    if (strncmp(line, "v ", 2) == 0) {
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      array_push(mesh.vertices, vertex);
    }

    else if (strncmp(line, "f ", 2) == 0) {
      int vertex_indices[3];

      int matched =
          sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &vertex_indices[0],
                 &vertex_indices[1], &vertex_indices[2]);

      if (matched == 3) {
        face_t face = {.a = vertex_indices[0],
                       .b = vertex_indices[1],
                       .c = vertex_indices[2]};
        array_push(mesh.faces, face);
      }
    }
  }

  fclose(file);
}
