#include "window.hpp"
#include <fmt/core.h>

void Window::onCreate() {
  auto const filename{abcg::Application::getAssetsPath() +
                      "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 72.0f);

  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }

  auto const *vertexShader{R"gl(#version 300 es
    layout(location = 0) in vec2 inPosition;

    void main() { 
      gl_Position = vec4(inPosition, 0, 1); 
    }
  )gl"};

  auto const *fragmentShader{R"gl(#version 300 es
    precision mediump float;

    out vec4 outColor;

    void main() { outColor = vec4(1.0, 1.0, 1.0, 1.0); }
  )gl"};

  // Create shader program
  m_program = abcg::createOpenGLProgram(
      {{.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
       {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment}});

  // Clear window
  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onPaintUI() {
  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // "Geometry Class" window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("", nullptr, flags);

    const char *figuras[] = {"", "Circulo", "Triangulo", "Quadrado",
                             "Retangulo"};
    static int item_index = 0; // Indice que indicará opção selecionada

    ImGui::BeginListBox("");

    for (int i = 0; i < IM_ARRAYSIZE(figuras); i++) {

      const bool is_selected = (item_index == i);
      if (ImGui::Selectable(figuras[i], is_selected)) {
        item_index = i;
      }

      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }

      if (strcmp(figuras[item_index], "") == 0) {
        m_geometry = Geometria::Inicio;
      } else if (strcmp(figuras[item_index], "Triangulo") == 0) {
        m_geometry = Geometria::Triangulo;
      } else if (strcmp(figuras[item_index], "Quadrado") == 0) {
        m_geometry = Geometria::Quadrado;
      } else if (strcmp(figuras[item_index], "Retangulo") == 0) {
        m_geometry = Geometria::Retangulo;
      }
    }

    ImGui::End();
  }
}

void Window::onPaint() {
  setupModel();

  // Set the viewport
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // Start using the shader program
  abcg::glUseProgram(m_program);
  // Start using VAO
  abcg::glBindVertexArray(m_VAO);

  // Draw a single point
  abcg::glDrawArrays(GL_LINE_STRIP, 0, 3);

  // End using VAO
  abcg::glBindVertexArray(0);
  // End using the shader program
  abcg::glUseProgram(0);
}

void Window::setupModel() {
  std::array<glm::vec2, 3> const positions{
      {{0.0f, 0.8f}, {-0.8f, -0.8f}, {0.8f, -0.8f}}};

  // Release previous VBO and VAO
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Generate a new VBO and get the associated ID
  abcg::glGenBuffers(1, &m_VBOPositions);
  // Bind VBO in order to use it
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  // Upload data to VBO

  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions,
                     GL_STATIC_DRAW);

  // Unbinding the VBO is allowed (data can be released now)
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}