#include "mesh.h"
#include <spdlog/spdlog.h>
#include <glad/gl.h>
using std::vector;

using VertexList = vector<Mesh::Vertex>;
using IndexList = vector<unsigned int>;
using TextureList = vector<std::shared_ptr<Texture>>;

struct Mesh::Internal {
  unsigned int VAO, VBO, EBO;
  VertexList vertices;
  IndexList indices;
  TextureList textures;
};

Mesh::Mesh(const Mesh& o) : Mesh(o.internal->vertices, o.internal->indices, o.internal->textures) {}

Mesh::Mesh(VertexList vertices, IndexList indices, TextureList textures) {
  spdlog::info("{}", __FUNCTION__);
  internal = std::make_unique<Internal>();
  internal->vertices = vertices;
  internal->indices = indices;
  internal->textures = textures;
  setupMesh();
}

Mesh::~Mesh() {
  spdlog::info("{}", __FUNCTION__);
}

void Mesh::setupMesh() {
  spdlog::info("{}", __FUNCTION__);
  glGenVertexArrays(1, &internal->VAO);
  glGenBuffers(1, &internal->VBO);
  glGenBuffers(1, &internal->EBO);

  glBindVertexArray(internal->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, internal->VBO);
  glBufferData(GL_ARRAY_BUFFER, internal->vertices.size() * sizeof(Vertex), &internal->vertices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, internal->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, internal->indices.size() * sizeof(unsigned int), &internal->indices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
}

void Mesh::Draw(Shader& shader) {
  shader.use();
  glBindVertexArray(internal->VAO);

  GLenum t = GL_TRIANGLES;
  switch (type) {
    case TRIANGLES: t = GL_TRIANGLES; break;
    case TRIANGLE_STRIP: t = GL_TRIANGLE_STRIP; break;
    case TRIANGLE_FAN: t = GL_TRIANGLE_FAN; break;
  }
  glDrawElements(t, internal->indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::DrawInstanced(Shader& shader, unsigned count)
{
  shader.use();
  glBindVertexArray(internal->VAO);
  GLenum t = GL_TRIANGLES;
  switch (type) {
    case TRIANGLES: t = GL_TRIANGLES; break;
    case TRIANGLE_STRIP: t = GL_TRIANGLE_STRIP; break;
    case TRIANGLE_FAN: t = GL_TRIANGLE_FAN; break;
  }
  glDrawElementsInstanced(t, internal->indices.size(), GL_UNSIGNED_INT, 0, count);
}
