#include "mesh.h"

vec3_t mesh_vertices[N_MESH_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 0
    {.x = 1, .y = -1, .z = -1},  // 1
    {.x = 1, .y = 1, .z = -1},   // 2
    {.x = -1, .y = 1, .z = -1},  // 3
    {.x = -1, .y = -1, .z = 1},  // 4
    {.x = 1, .y = -1, .z = 1},   // 5
    {.x = 1, .y = 1, .z = 1},    // 6
    {.x = -1, .y = 1, .z = 1},   // 7
};

// 12 triangles (2 per face of cube)
face_t mesh_faces[N_MESH_FACES] = {
    // Front face (z = +1)
    {0, 1, 2},
    {0, 2, 3},
    // Back face (z = -1)
    {5, 4, 7},
    {5, 7, 6},
    // Left face (x = -1)
    {4, 0, 3},
    {4, 3, 7},
    // Right face (x = +1)
    {1, 5, 6},
    {1, 6, 2},
    // Top face (y = +1)
    {3, 2, 6},
    {3, 6, 7},
    // Bottom face (y = -1)
    {4, 5, 1},
    {4, 1, 0}};
