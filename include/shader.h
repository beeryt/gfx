#pragma once
#include <glm/glm.hpp>

class Shader {
  public:
    Shader(const char* vPath, const char* fPath = nullptr, const char* gPath = nullptr);
    ~Shader();
    void use();

    void setBool(const char* name, bool value);
    void setInt(const char* name, int value);
    void setFloat(const char* name, float value);
    void setVec2(const char* name, const glm::vec2& value);
    void setVec3(const char* name, const glm::vec3& value);
    void setVec4(const char* name, const glm::vec4& value);
    void setMat2(const char* name, const glm::mat2& value);
    void setMat3(const char* name, const glm::mat3& value);
    void setMat4(const char* name, const glm::mat4& value);

  protected:
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

  private:
    unsigned int ID;
};
