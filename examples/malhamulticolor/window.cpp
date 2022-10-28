#include "window.hpp"

void Window::onCreate() {
  auto const *vertexShader{R"gl(#version 300 es
                                   
    layout(location = 0) in vec2 inPosition;
    layout(location = 1) in vec4 inColor;

    out vec4 fragColor;

    void main() { 
      gl_Position = vec4(inPosition, 0, 1);
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

  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);
}

void Window::onPaint() {
  if (m_timer.elapsed() < delay / 90)
    return;
  m_timer.restart();

  setupModel();

  abcg::glViewport(0, 0, m_viewportSize.x - 5, m_viewportSize.y - 5);

  abcg::glUseProgram(m_program);
  abcg::glBindVertexArray(m_VAO);

  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const widgetSize{ImVec2(250, 90)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x,
                                   m_viewportSize.y - widgetSize.y));
    ImGui::SetNextWindowSize(widgetSize);
    auto windowFlags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin(" ", nullptr, windowFlags);

    // Slider do delay
    ImGui::SliderFloat("Delay", &delay, 0.0f, 100.0f, "%.3f");

    // Sliders das cores r, g, b que vão se somar a cor gerada pelo random
    ImGui::SliderFloat("Color 1", &color1, 0.0f, 10.00f, "%.2f");
    ImGui::SliderFloat("Color 2", &color2, 0.0f, 10.00f, "%.2f");
    ImGui::SliderFloat("Color 3", &color3, 0.0f, 10.00f, "%.2f");

    ImGui::PushItemWidth(215);
    ImGui::PopItemWidth();

    if (ImGui::Button("Apagar", ImVec2(0, 0))) {
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    ImGui::End();
  }
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

void Window::setup() {
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Create array of random vertex positions
  std::uniform_real_distribution random_position(-0.5f, 1.5f);
  std::array<glm::vec2, 4> const positions{
      {{random_position(m_randomEngine), random_position(m_randomEngine)},
       {random_position(m_randomEngine), random_position(m_randomEngine)},
       {random_position(m_randomEngine), random_position(m_randomEngine)},
       {random_position(m_randomEngine), random_position(m_randomEngine)}}};

  std::uniform_real_distribution random_color(0.0f, 1.0f);
  std::array<glm::vec4, 4> colors{{{random_color(m_randomEngine) + color1,
                                    random_color(m_randomEngine) + color2,
                                    random_color(m_randomEngine) + color3, 1},
                                   {random_color(m_randomEngine) + color1,
                                    random_color(m_randomEngine) + color2,
                                    random_color(m_randomEngine) + color3, 1},
                                   {random_color(m_randomEngine) + color1,
                                    random_color(m_randomEngine) + color2,
                                    random_color(m_randomEngine) + color3, 1},
                                   {random_color(m_randomEngine) + color1,
                                    random_color(m_randomEngine) + color2,
                                    random_color(m_randomEngine) + color3, 1}}};

  // Generate VBO of positions
  abcg::glGenBuffers(1, &m_VBOPositions);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate VBO of colors
  abcg::glGenBuffers(1, &m_VBOColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  auto const colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

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
  abcg::glBindVertexArray(0);
}