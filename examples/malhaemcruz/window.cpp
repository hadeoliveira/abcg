#include "window.hpp"

void Window::onCreate() {
  auto const *vertexShader{R"gl(#version 300 es

    layout(location = 0) in vec2 inPosition;
    layout(location = 1) in vec4 inColor;

    uniform float translation;
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

  scale = 1.0f;
  translation = 0.0f;
  for (int i = 0; i < 13; i++) {
    posicoesAtualizadas[i][0] = posicoesIniciais[i][0];
    posicoesAtualizadas[i][1] = posicoesIniciais[i][1];
    coresAtualizadas[i][0] = coresIniciais[i][0];
    coresAtualizadas[i][1] = coresIniciais[i][1];
    coresAtualizadas[i][2] = coresIniciais[i][2];
  }
}

void Window::onPaint() {

  configurarCruz(sides);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Definição da translação no m_program
  auto const translationLocation{
      abcg::glGetUniformLocation(m_program, "translation")};
  abcg::__glewUniform1f(translationLocation, translation);

  // Definição de escala no m_program
  auto const scaleLocation{abcg::glGetUniformLocation(m_program, "scale")};
  abcg::glUniform1f(scaleLocation, scale);

  // Etapa de renderização
  abcg::glBindVertexArray(m_VAO);
  abcg::glDrawArrays(GL_LINE_STRIP, 0, sides);
  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  // Widget das posições e reset
  {
    auto const widgetSize{ImVec2(250, 150)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x,
                                   m_viewportSize.y - widgetSize.y - 400));
    ImGui::SetNextWindowSize(widgetSize);
    auto const windowFlags{ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoCollapse |
                           ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin("Movimentação", nullptr, windowFlags);

    // Slider para alterar lados da cruz
    ImGui::SliderInt("Lados", &sides, 3, 12);

    // Alterar a cruz para cima, baixo, esquerda, direita e todas variações de
    // diagonais
    if (ImGui::Button("Esquerda")) {
      for (int i = 0; i < 13; i++) {
        posicoesAtualizadas[i][0] = posicoesAtualizadas[i][0] - 0.1f;
      }
    }
    if (ImGui::Button("Direita")) {
      for (int i = 0; i < 13; i++) {
        posicoesAtualizadas[i][0] = posicoesAtualizadas[i][0] + 0.1f;
      }
    }
    if (ImGui::Button("Cima")) {
      for (int i = 0; i < 13; i++) {
        posicoesAtualizadas[i][1] = posicoesAtualizadas[i][1] + 0.1f;
      }
    }
    if (ImGui::Button("Baixo")) {
      for (int i = 0; i < 13; i++) {
        posicoesAtualizadas[i][1] = posicoesAtualizadas[i][1] - 0.1f;
      }
    }

    if (ImGui::Button("Baixo Direita")) {
      for (int i = 0; i < 13; i++) {
        posicoesAtualizadas[i][0] = posicoesAtualizadas[i][0] + 0.1f;
        posicoesAtualizadas[i][1] = posicoesAtualizadas[i][1] - 0.1f;
      }
    }

    if (ImGui::Button("Baixo Esquerda")) {
      for (int i = 0; i < 13; i++) {
        posicoesAtualizadas[i][0] = posicoesAtualizadas[i][0] - 0.1f;
        posicoesAtualizadas[i][1] = posicoesAtualizadas[i][1] - 0.1f;
      }
    }

    if (ImGui::Button("Cima Direita")) {
      for (int i = 0; i < 13; i++) {
        posicoesAtualizadas[i][0] = posicoesAtualizadas[i][0] + 0.1f;
        posicoesAtualizadas[i][1] = posicoesAtualizadas[i][1] + 0.1f;
      }
    }

    if (ImGui::Button("Cima Esquerda")) {
      for (int i = 0; i < 13; i++) {
        posicoesAtualizadas[i][0] = posicoesAtualizadas[i][0] - 0.1f;
        posicoesAtualizadas[i][1] = posicoesAtualizadas[i][1] + 0.1f;
      }
    }

    if (ImGui::Button("Reset", ImVec2(-1, 30))) {
      cruzInicial();
    }
    ImGui::End();
  }

  // Widget das cores
  {
    auto const widgetSize{ImVec2(225, 150)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x,
                                   m_viewportSize.y - widgetSize.y - 100));
    ImGui::SetNextWindowSize(widgetSize);
    auto const windowFlags{ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoCollapse |
                           ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin("Cores", nullptr, windowFlags);

    // Botões para configurar as cores dos 13 vértices
    ImGui::ColorEdit3(
        "Vértice 1", coresAtualizadas[0],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 2", coresAtualizadas[1],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 3", coresAtualizadas[2],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 4", coresAtualizadas[3],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 5", coresAtualizadas[4],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 6", coresAtualizadas[5],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 7", coresAtualizadas[6],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 8", coresAtualizadas[7],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 9", coresAtualizadas[8],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 10", coresAtualizadas[9],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 11", coresAtualizadas[10],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 12", coresAtualizadas[11],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));
    ImGui::ColorEdit3(
        "Vértice 13", coresAtualizadas[12],
        (ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker));

    ImGui::End();
  }

  // Widget da translação e escala
  {
    auto const widgetSize{ImVec2(200, 70)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 400,
                                   m_viewportSize.y - widgetSize.y - 400));
    ImGui::SetNextWindowSize(widgetSize);
    auto const windowFlags{ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoCollapse |
                           ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin("Escala", nullptr, windowFlags);
    ImGui::SliderFloat("Escala", &scale, 0.1f, 1.0f, "%.1f");

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

void Window::configurarCruz(int sides) {
  /* Liberação de buffers caso o VBOPositions, VBOColors ou VAO tenham sido
   criados anteriormente */
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  /* Etapa responsável por alocar os vértices e cores com os valores de posições
   e cores atualizadas. Visto que são 13 vértices */
  std::vector<glm::vec2> posicoes;
  std::vector<glm::vec3> cores;

  for (int i = 0; i < sides; i++) {
    posicoes.emplace_back(
        glm::vec2{posicoesAtualizadas[i][0], posicoesAtualizadas[i][1]});
    cores.push_back(glm::vec3{coresAtualizadas[i][0], coresAtualizadas[i][1],
                              coresAtualizadas[i][2]});
  }

  // Gerando os VBOs das posições atualizadas
  abcg::glGenBuffers(1, &m_VBOPositions);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glBufferData(GL_ARRAY_BUFFER, posicoes.size() * sizeof(glm::vec2),
                     posicoes.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Gerando os VBOs das cores atualizadas
  abcg::glGenBuffers(1, &m_VBOColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, cores.size() * sizeof(glm::vec3),
                     cores.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  auto const colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

  // Criação do VAO e link com os VBOs criados anteriormente
  abcg::glGenVertexArrays(1, &m_VAO);

  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindVertexArray(0);
}

void Window::cruzInicial() {
  scale = 1.0f;
  translation = 0.0f;
  for (int i = 0; i < 13; i++) {
    posicoesAtualizadas[i][0] = posicoesIniciais[i][0];
    posicoesAtualizadas[i][1] = posicoesIniciais[i][1];
    coresAtualizadas[i][0] = coresIniciais[i][0];
    coresAtualizadas[i][1] = coresIniciais[i][1];
    coresAtualizadas[i][2] = coresIniciais[i][2];
  }
}