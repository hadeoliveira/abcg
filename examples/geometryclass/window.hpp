#ifndef WINDOW_HPP_
#define WINDOW_HPP_
#define PI 3.1415926535898

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;
  void onPaint() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  enum class Geometria { Inicio, Circulo, Triangulo, Quadrado, Retangulo };
  Geometria m_geometry;

  ImFont *m_font{};

  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBOPositions{};
  GLuint m_VBOColors{};
  GLuint m_program{};

  void setupModel();
  void onPaintTriangulo();
  void onPaintQuadrado();
  void onPaintRetangulo();

  std::array<glm::vec4, 3> m_colors{{{0.36f, 0.83f, 1.00f, 1},
                                     {0.63f, 0.00f, 0.61f, 1},
                                     {1.00f, 0.69f, 0.30f, 1}}};
};

#endif
