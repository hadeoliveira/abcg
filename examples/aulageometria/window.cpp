#include "window.hpp"

void Window::onCreate() {
  auto const *vertexShader{R"gl(#version 300 es

    layout(location = 0) in vec2 inPosition;
    layout(location = 1) in vec4 inColor;

    uniform vec2 translation;
    uniform float scale;

    out vec4 fragColor;

    void main() {
      vec2 newPosition = inPosition * scale + translation;
      gl_Position = vec4(newPosition, 0, 1);
      fragColor = inColor;
    }
  )gl"};

  auto const *fragmentShader{R"gl(#version 300 es

    precision mediump float;  

    in vec4 fragColor;

    out vec4 outColor;

    void main() { outColor = fragColor; }
  )gl"};

  m_program = abcg::createOpenGLProgram(
      {{.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
       {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  auto const filename{abcg::Application::getAssetsPath() +
                      "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 72.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError{
        "Nao e' possivel carregar a fonte no diretório assets"};
  }

  m_figuraGeometrica = FiguraGeometrica::Vazio;
}

void Window::onPaintUI() {
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // Tela da Aula de Geometria
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("Aula de Geometria", nullptr, flags);

    static int selected = -1;
    const char *items[] = {"", "Triangulo", "Retangulo", "Quadrado",
                           "Pentagono"};
    char buf[32];
    for (int item = 0; item < IM_ARRAYSIZE(items); item++) {
      sprintf(buf, "%s", items[item]);

      // Verificar qual é a figura selecionada e trocar o estado do enum de
      // figura geométrica
      if (ImGui::Selectable(buf, selected == item)) {
        selected = item;
        if (strcmp(items[item], "") == 0) {
          m_figuraGeometrica = FiguraGeometrica::Vazio;
        } else if (strcmp(items[item], "Triangulo") == 0) {
          m_figuraGeometrica = FiguraGeometrica::Triangulo;
        } else if (strcmp(items[item], "Retangulo") == 0) {
          m_figuraGeometrica = FiguraGeometrica::Retangulo;
        } else if (items[item] == "Quadrado") {
          m_figuraGeometrica = FiguraGeometrica::Quadrado;
        } else if (items[item] == "Pentagono") {
          m_figuraGeometrica = FiguraGeometrica::Pentagono;
        }
      }
    }
    // ImGui::TreePop();

    ImGui::End();
  }

  void Window::reiniciar() {}

  void Window::onPaint() {
    setUp();

    abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

    abcg::glUseProgram(m_program);
    abcg::glBindVertexArray(m_VAO);

    abcg::glDrawArrays(GL_TRIANGLES, 0, 3);

    abcg::glBindVertexArray(0);
    abcg::glUseProgram(0);
  }

  void Window::setUp() {
    // Liberação dos Buffers caso tenham sido criados anteriormente
    abcg::glDeleteBuffers(1, &m_VBOPositions);
    abcg::glDeleteBuffers(1, &m_VBOColors);
    abcg::glDeleteVertexArrays(1, &m_VAO);

    // Criação dos buffers para cada uma das figuras geométricas
    if (m_figuraGeometrica == FiguraGeometrica::Vazio) {
      // std::array<glm::vec2, 1> const positions{{0, 0}};
    } else if (m_figuraGeometrica == FiguraGeometrica::Triangulo) {

      std::array<glm::vec2, 3> const positions{
          {{0, 0.8}, {-0.8, 0}, {0.8f, 0}}};

      abcg::glGenBuffers(1, &m_VBOPositions);
      abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
      abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                         GL_STATIC_DRAW);
      abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

      // Generate VBO of colors

      abcg::glGenBuffers(1, &m_VBOColors);
      abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
      abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors.data(),
                         GL_STATIC_DRAW);
      abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

      // Get location of attributes in the program
      auto const positionAttribute{
          abcg::glGetAttribLocation(m_program, "inPosition")};
      auto const colorAttribute{
          abcg::glGetAttribLocation(m_program, "inColor")};

      // Create VAO
      abcg::glGenVertexArrays(1, &m_VAO);

      // Bind vertex attributes to current VAO
      abcg::glBindVertexArray(m_VAO);

      abcg::glEnableVertexAttribArray(positionAttribute);
      abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
      abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                                  nullptr);
      abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

      abcg::glEnableVertexAttribArray(colorAttribute);
      abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
      abcg::glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0,
                                  nullptr);
      abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

      // End of binding to current VAO
      abcg::glBindVertexArray(0);
    } else if (m_figuraGeometrica == FiguraGeometrica::Retangulo) {
    } else if (m_figuraGeometrica == FiguraGeometrica::Quadrado) {
    } else if (m_figuraGeometrica == FiguraGeometrica::Pentagono) {
    }
  }
