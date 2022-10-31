# Projeto2ABCG
Henrique Augusto de Oliveira, RA: 11065516

## Malha em Cruz - Projeto 2
Semelhante Malha Multicolor apresentado no projeto 1, o projeto 2 é um projeto denominado "Malha em Cruz". O projeto se assemelha com a proposta apresentada no projeto anterior, mas com algumas mudanças. O usuário consegue configurar o tamanho da cruz através da escala e suas respectivas movimentações, sendo elas para cima, baixo, esquerda, direita e todas as suas possibilidades de diagonal.
O intuito do trabalho foi o aprendizado com os VBOs, VAOs e as primitivas do OpenGL, integrando com alguns conhecimentos aprendidos da biblioteca Dear ImGui.

## Execução
Na pasta /abcg, executar o comando abaixo
``` 
./build.sh
```

Após o projeto ser buildado, execute o comando para rodar o executável
```
./build/bin/malhaemcruz/malhaemcruz
```

## Observação
![image](https://user-images.githubusercontent.com/35350662/198903207-37951904-91af-42c2-8c1e-9fb336f9ff97.png)

A ideia do projeto era fazer uma malha em cruz, tanto que o projeto executado localmente, as movimentações foram uma malha. No entanto, executando através do navegador, ocorre apenas a movimentação da cruz, fazendo com que o histórico do percurso não seja gravado na tela
Um outro ponto é que, dependendo do zoom que navegador está, os widgets podem sumir da tela. No navegador, para que os widgets sejam apresentados, deixei um zoom de 80%. Esse foi um ponto que não soube corrigir e explicar para o projeto

![image](https://user-images.githubusercontent.com/35350662/198903324-7135532b-2235-4f92-9a76-5125f34a1a2c.png)


## Histórico
Inicialmente, planejei para fazer um projeto denominado "Aula de Geometria", no qual o usuário selecionasse uma figura geométrica em uma List Box e a mesma figura era desenhada na tela. No entanto, encontrei a mesma dificuldade na primeira tentativa realizada para o projeto 1. Não consegui alterar o estado da figura geométrica e desenhar de acordo com que ela era selecionada. 
Visto essas dificuldades, optei por seguir uma dica discutida em aula, que era fazer um desenho diferente do que foi abordado em aula. Optei por fazer uma cruz utilizando a primitiva GL_LINE_STRIP e botões da biblioteca Dear ImGui para configurar a direção que a malha é gerada, que foi ao contrário do que foi explorado no projeto 1, no qual a malha era gerada aleatoriamente. 
Os projetos bases utilizados foram o de Polígonos Regulares, para desenho das figuras geométricas, e o Asteroids, para configurar a direção da malha.

## Explicação do Projeto
A ideia do projeto consiste em criar uma malha no formato de cruz, no qual o usuário pode configurar a direção que a malha é gerada, o tamanho da cruz e existe a possibilidade de diminuir os lados da cruz e gerar a malha com menos lados da cruz.
Inicialmente, o projeto foi iniciado pela main.cpp e window.hpp. O main.cpp seguiu o mesmo padrão dos projetos ensinados em aula. O window.hpp apresentou algumas alterações. Os pontos iniciais e as cores iniciais foram fixadas, visto que a cruz partirá de um ponto inicial. De forma semelhante, as cores iniciais de cada vértices já são pré-configurados. Uma alteração existente são as variáveis posicoesAtualizadas e coresAtualizadas, que recebem os novos valores configurados pelo usuário. Um problema encontrado, que não foi possível resolver, foi que, ao clicar em uma nova cor para o vértice, o widget de cor trava e não desaparece da tela. Então, para efeito de desenvolvimento, durante os testes, não foram trocadas as cores para não travar a execução do projeto. 

```
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
```

De forma análoga, os sliders de escala e lados também estão presentes pré-configurados no window.hpp também.

Partindo para os widgets, temos as lógicas de movimentação, na qual qualquer movimentação para direita ou esquerda movimenta a cruz no eixo x, pegando o índice 0 da variável de posições; e qualquer movimentação para cima ou baixo movimenta a cruz no eixo y, alterando o índice 1 da variável de posições. Seguem dois exemplos de movimentação:

```
    if (ImGui::Button("Esquerda")) {
      for (int i = 0; i < 13; i++) {
        posicoesAtualizadas[i][0] = posicoesAtualizadas[i][0] - 0.1f;
      }
    }

    if (ImGui::Button("Baixo Esquerda")) {
      for (int i = 0; i < 13; i++) {
        posicoesAtualizadas[i][0] = posicoesAtualizadas[i][0] - 0.1f;
        posicoesAtualizadas[i][1] = posicoesAtualizadas[i][1] - 0.1f;
      }
    }    

```
Seguindo a mesma linha, a alteração de lados e tamanho da cruz também apresentam sliders no onPaintUI().
No onCreate, que é executado durante a iniciação do projeto, a variável de posições atualizadas recebe os valores iniciais. Caso haja uma alteração por um comando do botão na onPaintUI, essa variável de posição atualizada é alterada.
Um ponto que vale ser ressaltado do que foi feito diferente da aula é que os 13 pontos foram incluídos em posições e cores dentro do configurarCruz, que é um método igual ao setUp discutido em aula para configurar os VBOs e os VAOs. Esses vértices foram iterados de acordo com o que é recebido no sides, que é o valor configurado pelo usuário

```
  for (int i = 0; i < sides; i++) {
    posicoes.emplace_back(
        glm::vec2{posicoesAtualizadas[i][0], posicoesAtualizadas[i][1]});
    cores.push_back(glm::vec3{coresAtualizadas[i][0], coresAtualizadas[i][1],
                              coresAtualizadas[i][2]});
  }
```
Esses serão os valores atribuídos ao VBOPositions e VBOColors.

## Notas finais
Por mais que não tenha saído com a ideia inicial de formar uma nova malha, os objetivos iniciais eram de conseguir deslocar uma figura específica e diferente das apresentadas na aula através de input do usuário, que era uma cruz; utilizar uma primitiva diferente da GL_TRIANGLES, que no caso foi utilizado a GL_LINE_STRIP; e, o principal, que é aprender a trabalhar melhor com os VBOs, VAOs e as primitivas do OpenGL.

### Link do site
https://hadeoliveira.github.io/Projeto2ABCG/

### Referência
- https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html
- https://hbatagelo.github.io/cg/regularpolygons.html
- https://hbatagelo.github.io/cg/asteroids.html

