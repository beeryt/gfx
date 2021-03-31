#include "mesh.h"
#include <spdlog/spdlog.h>
using std::vector;

struct Mesh::Vertex { int i; };
struct Mesh::Texture { int i; };

struct Mesh::Internal {
  unsigned int VAO, VBO, EBO;
  vector<Vertex> vertices;
  vector<unsigned> indices;
  vector<Texture> textures;
};

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned> indices, vector<Texture> textures) {
  spdlog::info("{}", __PRETTY_FUNCTION__);
  internal = std::make_unique<Internal>();
  internal->vertices = vertices;
  internal->indices = indices;
  internal->textures = textures;
}

Mesh::~Mesh() {
  spdlog::info("{}", __PRETTY_FUNCTION__);
}

