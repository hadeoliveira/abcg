# Projeto1ABCG
Henrique Augusto de Oliveira, RA: 11065516

## Malha Multicolor - Projeto 1
A Malha multicolor no qual o usuário consegue configurar as cores e o delay que uma malha de figuras geométricas é gerada na tela. Aleatoriamente, pontos são gerados na tela em cores específicas. O usuário consegue trocar o R, G, B e trocar o delay que a malha é gerada.
O intuito foi utilizar as funcionalides disponibilizadas pela ImGui. Nesse projeto, foram utilizados o botão de apagar e os sliders para configurar as cores R, G, B para a malha gerada.

## Execução
Na pasta /abcg, executar o comando abaixo
``` 
./build.sh
```

Após o projeto ser buildado, execute o comando para rodar o executável
```
./build/bin/malhamulticolor/malhamulticolor
```

## Histórico
Inicialmente, desejava-se fazer um projeto chamado "Aula de Geometria", no qual o usuário selecionasse uma figura geométrica em uma List Box e a mesma figura era desenhada na tela. No entanto, encontrei diferentes dificuldades técnicas que não consegui resolver dentro do prazo. 
A fim de cumprir o prazo estabelecido para envio do primeiro projeto, buscou-se fazer um projeto semelhante aos apresentados em sala e alterar algumas funcionalidades a fim de estudar e aplicar a biblioteca Dear ImGui. O projeto utilizado como base foi o de Triangulos Coloridos.

## Explicação do Projeto
A ideia do projeto consiste em criar uma malha geométrica, no qual o usuário pudesse alterar o valor dado as componentes R, G e B que são geradas aleatoriamente. Ou seja, os valores de cor são gerados de forma aleatória, semelhante as posições, mas um valor é acrescido nesse valor gerado. Esses parâmetros são dados pelo usuário. De forma análoga funciona com o delay, no qual o usuário pode configurar o delay no qual as figuras são geradas e é formado a malha. O botão de Apagar serve para limpar a tela e as figuras serem geradas novamente
Inicialmente, foram vistas algumas funcionalidades específicas e buscou-se o entendimento de como a biblioteca do Dear ImGui funcionava. Entendido essa etapa, buscou-se algumas funções que cumprem a tarefa desejada.
Para o botão de apagar, é utilizado ImGui::Button, no qual quando esse botão é pressionado, a função glClear é implementada. O código implementado em window.hpp e na função onPaintUI() segue abaixo

```
    if (ImGui::Button("Apagar", ImVec2(0, 0))) {
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

```

Para os valores de cor e delay, existiam duas opções. Utilizar o ImGui::SliderFloat ou ImGui::InputFloat. Por opção, optou-se por utilizar o SliderFloat, como pode ser visto abaixo

```
    // Slider do delay
    ImGui::SliderFloat("Delay", &delay, 0.0f, 100.0f, "%.3f");

    // Sliders das cores r, g, b que vão se somar a cor gerada pelo random
    ImGui::SliderFloat("Color 1", &color1, 0.0f, 10.00f, "%.2f");
    ImGui::SliderFloat("Color 2", &color2, 0.0f, 10.00f, "%.2f");
    ImGui::SliderFloat("Color 3", &color3, 0.0f, 10.00f, "%.2f");

```
As variáveis color1, color2, color3 e delay foram declaradas em window.hpp e as utilizações descritas acima em onPaintUI(), que é executado logo em seguida do onPaint(). 
Após os valores serem definidos, eles serão apenas acrescidos nos que são gerados de forma automática para a cor e, para o delay, é implementado a mesma funcionalidade vista nos triângulos coloridos, mas que o usuário pudesse controlar a forma que as figuras fossem apresentadas. 
```
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
```

```
  if (m_timer.elapsed() < delay / 90)
    return;
  m_timer.restart();
```

Um outro ponto que vale ser ressaltdo é que, no glDrawArray, foram especificados 4 vértices para serem renderizados e apresentados as figuradas geométricas.

### Link do site
https://hadeoliveira.github.io/Projeto1ABCG/

### Referência
- https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html

