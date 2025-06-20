#include "mesh.h"
#include "array.h"
#include "texture.h"
#include "triangle.h"
#include "vector.h"
#include <stdio.h>
#include <string.h>

mesh_t mesh = {.vertices = NULL,
               .faces = NULL,
               .rotation = {0, 0, 0},
               .scale = {1.0, 1.0, 1.0},
               .translation = {0, 0, 0}};
vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = 1, .y = -1, .z = -1},  // 1
    {.x = 1, .y = -1, .z = 1},   // 2
    {.x = -1, .y = -1, .z = -1}, // 3
    {.x = -1, .y = 1, .z = -1},  // 4
    {.x = 1, .y = 1, .z = 1},    // 5
    {.x = 1, .y = -1, .z = 1},   // 6
    {.x = -1, .y = 1, .z = -1},  // 7
    {.x = -1, .y = -1, .z = -1}, // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    {.a = 1,
     .b = 2,
     .c = 3,
     .a_uv = {0, 1},
     .b_uv = {0, 0},
     .c_uv = {1, 0},
     .color = 0x00FFFFFF},
    {.a = 1,
     .b = 3,
     .c = 4,
     .a_uv = {0, 1},
     .b_uv = {1, 0},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF},
    // right
    {.a = 4,
     .b = 3,
     .c = 5,
     .a_uv = {0, 1},
     .b_uv = {0, 0},
     .c_uv = {1, 0},
     .color = 0xFFFFFFFF},
    {.a = 4,
     .b = 5,
     .c = 6,
     .a_uv = {0, 1},
     .b_uv = {1, 0},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF},
    // back
    {.a = 6,
     .b = 5,
     .c = 7,
     .a_uv = {0, 1},
     .b_uv = {0, 0},
     .c_uv = {1, 0},
     .color = 0xFFFFFFFF},
    {.a = 6,
     .b = 7,
     .c = 8,
     .a_uv = {0, 1},
     .b_uv = {1, 0},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF},
    // left
    {.a = 8,
     .b = 7,
     .c = 2,
     .a_uv = {0, 1},
     .b_uv = {0, 0},
     .c_uv = {1, 0},
     .color = 0xFFFFFFFF},
    {.a = 8,
     .b = 2,
     .c = 1,
     .a_uv = {0, 1},
     .b_uv = {1, 0},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF},
    // top
    {.a = 2,
     .b = 7,
     .c = 5,
     .a_uv = {0, 1},
     .b_uv = {0, 0},
     .c_uv = {1, 0},
     .color = 0xFFFFFFFF},
    {.a = 2,
     .b = 5,
     .c = 3,
     .a_uv = {0, 1},
     .b_uv = {1, 0},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF},
    // bottom
    {.a = 6,
     .b = 8,
     .c = 1,
     .a_uv = {0, 1},
     .b_uv = {0, 0},
     .c_uv = {1, 0},
     .color = 0xFFFFFFFF},
    {.a = 6,
     .b = 1,
     .c = 4,
     .a_uv = {0, 1},
     .b_uv = {1, 0},
     .c_uv = {1, 1},
     .color = 0xFFFFFFFF}};
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
  FILE *file;
  file = fopen(filename, "r");
  if (!file)
    perror("file failed to open");
  char line[1024];
  tex2_t *tex_coords = NULL;

  while (fgets(line, 1024, file)) {
    // Vertex information
    if (strncmp(line, "v ", 2) == 0) {
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      array_push(mesh.vertices, vertex);
    }
    if (strncmp(line, "vt ", 3) == 0) {
      tex2_t tex_coord;
      sscanf(line, "vt %f %f", &tex_coord.u, &tex_coord.v);
      array_push(tex_coords, tex_coord)
    }
    // Face information
    if (strncmp(line, "f ", 2) == 0) {
      int vertex_indices[3];
      int texture_indices[3];
      int normal_indices[3];
      sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertex_indices[0],
             &texture_indices[0], &normal_indices[0], &vertex_indices[1],
             &texture_indices[1], &normal_indices[1], &vertex_indices[2],
             &texture_indices[2], &normal_indices[2]);
      face_t face = {.a = vertex_indices[0] - 1,
                     .b = vertex_indices[1] - 1,
                     .c = vertex_indices[2] - 1,
                     .a_uv = tex_coords[texture_indices[0] - 1],
                     .b_uv = tex_coords[texture_indices[1] - 1],
                     .c_uv = tex_coords[texture_indices[2] - 1],
                     .color = 0xFFFFFF45};
      array_push(mesh.faces, face);
    }
  }
  array_free(tex_coords);
  fclose(file);
}
