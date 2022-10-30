#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  // 13 pontos iniciais da cruz. Os 13 pontos foram idealizados através do plano
  // cartesiano do GeoGebra. Seguem posições e cores
  float posicoesIniciais[13][2] = {
      {-0.10f, 0.5f},   {-0.10f, 0.0f},   {-0.5f, 0.0f},   {-0.5f, -0.18f},
      {-0.10f, -0.18f}, {-0.10f, -0.75f}, {0.10f, -0.75f}, {0.10f, -0.18f},
      {0.5f, -0.18f},   {0.5f, 0.0f},     {0.10f, 0.0f},   {0.10f, 0.5f},
      {-0.05f, 0.5f}};
  float coresIniciais[13][3] = {
      {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f},
      {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f},
      {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 1.0f}};

  // Apos introducao dos valores pelo usuario, os valores dos pontos da cruz
  // serão atualizados assim que as cores dos vértices
  float posicoesAtualizadas[13][2]{};
  float coresAtualizadas[13][3]{};

  float scale{1.0f};
  float translation{0.0f};
  int sides{12};

  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBOPositions{};
  GLuint m_VBOColors{};
  GLuint m_program{};

  std::default_random_engine m_randomEngine;

  abcg::Timer m_timer;

  void configurarCruz(int sides);
  void cruzInicial();
};

#endif