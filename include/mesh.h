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
      constexpr Vertex(glm::vec3 p = {}, glm::vec3 n = {}, glm::vec2 u = {}) : position(p), normal(n), uv(u) {}
      constexpr ~Vertex() = default;
    };

    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned> indices,
         std::vector<std::shared_ptr<Texture>> textures);
    ~Mesh();

    template <class CV, class CI, class CT>
      Mesh(CV v, CI i, CT t) :
        Mesh(
            std::vector<Vertex>(v.begin(), v.end()),
            std::vector<unsigned>(i.begin(), i.end()),
            std::vector<std::shared_ptr<Texture>>(t.begin(), t.end())
        ) {}

    Mesh(const Mesh&);
    Mesh& operator=(const Mesh&);

    virtual void Draw(Shader& shader);

  protected:
    void setupMesh();
    enum Type { TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN };
    Type type = TRIANGLES;

  private:
    struct Internal;
    std::unique_ptr<Internal> internal;
};
