#pragma once
#include "shader.h"
#include "texture.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class Mesh {
  public:
    struct Vertex {
      glm::vec3 position;
      glm::vec3 normal;
      glm::vec2 uv;
      Vertex(glm::vec3 p = {}, glm::vec3 n = {}, glm::vec2 u = {});
    };

    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned> indices,
         std::vector<std::shared_ptr<Texture>> textures);
    ~Mesh();

    Mesh(const Mesh&);
    Mesh& operator=(const Mesh&);

    void Draw(Shader& shader);

  protected:
    void setupMesh();

  private:
    struct Internal;
    std::unique_ptr<Internal> internal;
};
