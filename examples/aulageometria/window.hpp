#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;

private:
  std::array<glm::vec4, 3> m_colors{{{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}}};
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBOPositions{};
  GLuint m_VBOColors{};
  GLuint m_program{};

  enum class FiguraGeometrica {
    Vazio,
    Triangulo,
    Retangulo,
    Quadrado,
    Pentagono
  };
  FiguraGeometrica m_figuraGeometrica;

  ImFont *m_font{};

  void reiniciar();

  void setUp();
};

#endif