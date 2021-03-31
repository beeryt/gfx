#pragma once
#include "shader.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class Mesh {
  public:
    struct Vertex;
    struct Texture;

    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned> indices,
         std::vector<Texture> textures);
    ~Mesh();

    void Draw(Shader& shader);

  protected:
    void setupMesh();

  private:
    struct Internal;
    std::unique_ptr<Internal> internal;
};
