#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <glad/gl.h>
#include <spdlog/spdlog.h>

using GetInfoLogFn = std::function<void(GLuint,GLsizei,GLsizei*,GLchar*)>;
using GetIntegerFn = std::function<void(GLuint, GLenum, GLint*)>;
struct InfoLog {
  GetInfoLogFn GetInfoLog;
  GetIntegerFn GetInteger;
  GLenum type;
};

void CheckCompileErrors(unsigned int id, bool compiler) {
  auto& GetInteger = compiler ? glGetShaderiv : glGetProgramiv;
  auto& GetInfoLog = compiler ? glGetShaderInfoLog : glGetProgramInfoLog;
  GLenum type = compiler ? GL_COMPILE_STATUS : GL_LINK_STATUS;
  const char* name = compiler ? "COMPILE" : "LINK";

  GLint success;
  GLchar infoLog[1024];
  GetInteger(id, type, &success);
  if (!success) {
    GetInfoLog(id, sizeof(infoLog), NULL, infoLog);
    fprintf(stderr, "ERROR:SHADER:%s: %s\n", name, infoLog);
    exit(1);
  }
}

unsigned int CompileShader(const char* path, GLenum type) {
  if (!path) return 0;
  std::string string;
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open file
    file.open(path);
    // read file buffer into stream
    std::stringstream stream;
    stream << file.rdbuf();
    // close file
    file.close();
    // convert stream into string
    string = stream.str();
  } catch (std::ifstream::failure& e) {
    fprintf(stderr, "ERROR:SHADER:FILE: (%s) %s\n", path, e.what());
  }

  const char* code = string.c_str();
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &code, NULL);
  glCompileShader(shader);
  CheckCompileErrors(shader, true);

  return shader;
}

Shader::Shader(const char* vPath, const char* fPath, const char* gPath) {
  spdlog::trace("{}", __PRETTY_FUNCTION__);
  auto vertex = CompileShader(vPath, GL_VERTEX_SHADER);
  auto fragment = CompileShader(fPath, GL_FRAGMENT_SHADER);
  auto geometry = CompileShader(gPath, GL_GEOMETRY_SHADER);

  ID = glCreateProgram();
  if (vPath) glAttachShader(ID, vertex);
  if (fPath) glAttachShader(ID, fragment);
  if (gPath) glAttachShader(ID, geometry);
  glLinkProgram(ID);
  CheckCompileErrors(ID, false);

  if (vPath) glDeleteShader(vertex);
  if (fPath) glDeleteShader(fragment);
  if (gPath) glDeleteShader(geometry);

}

void Shader::Info() {
  GLint param;
  glGetProgramiv(ID, GL_ATTACHED_SHADERS, &param);
  spdlog::debug("GL_ATTACHED_SHADERS: {}", param);

  auto shaders = new GLuint[param];
  glGetAttachedShaders(ID, param, NULL, shaders);
  for (int i = 0; i < param; ++i) {
    GLint param2;
    glGetShaderiv(shaders[i], GL_SHADER_TYPE, &param2);
    spdlog::debug("  {}: {} ({})", i, shaders[i],
        param2 == GL_VERTEX_SHADER ? "VERTEX" :
        param2 == GL_FRAGMENT_SHADER ? "FRAGMENT" :
        param2 == GL_GEOMETRY_SHADER ? "GEOMETRY" : "UNKNOWN");
    if (param2 == GL_GEOMETRY_SHADER) {
      glGetProgramiv(ID, GL_GEOMETRY_INPUT_TYPE, &param2);
      spdlog::debug("GL_GEOMETRY_INPUT_TYPE: {}", param2);
      glGetProgramiv(ID, GL_GEOMETRY_OUTPUT_TYPE, &param2);
      spdlog::debug("GL_GEOMETRY_OUTPUT_TYPE: {}", param2);
    }
  }
  delete shaders;
}

Shader::~Shader() {
  spdlog::trace("{}", __PRETTY_FUNCTION__);
  glDeleteProgram(ID);
}

void Shader::use() {
  glUseProgram(ID);
}

void Shader::setVec2(const char* name, const glm::vec2& value) {
  auto loc = glGetUniformLocation(ID, name);
  glUniform2fv(loc, 1, &value[0]);
}

void Shader::setVec3(const char* name, const glm::vec3& value) {
  auto loc = glGetUniformLocation(ID, name);
  glUniform3fv(loc, 1, &value[0]);
}

void Shader::setMat4(const char* name, const glm::mat4& value) {
  auto loc = glGetUniformLocation(ID, name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}
