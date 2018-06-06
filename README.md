# Manipulador com servo motor

### Proposta 
O projeto tem a proposta de utilizar dois servos motores, de forma a ter-se uma garra,
cuja abertura é controlada por um potênciometro e por meio do acionamento de um botão
a garra abre e fecha.
Sua utilidade é pegar objetos e poderia ser parte de um projeto maior, o qual envolveria o
deslocamento da garra. Isso facilitaria pessoas dentro de casa com dificuldade de locomoção
a conseguirem pegar objetos distantes.

### Periféricos 
A elaboração do projeto será feita com uma STM32F407Discovery. Serão utilizados
dois servos motores com comunicação PWM simétrica, um potenciômetro como entrada
analógica e um botão como entrada GPIO, de acordo com o ilustrado na figura 1.

![Screenshot](diagrama.JPG)

Figura 1: Diagrama de blocos da eletrônica

### Pinagem
![Screenshot](pinagem.JPEG)

### Fluxograma do firmware
![Screenshot](fluxograma.JPEG)

### Referências
1. Proposta de projeto dada pelo professor da cadeira de Microcontroladores (2018),
Cap. Renault.
2. [Manual de Referência STM32F4](http://www.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf)
3. [Projeto da Aluna Jéssica Aires, cadeira de Microcontroladores 2017, Instituto Militar de Engenharia.](https://gitlab.luizrenault.com/microcontroladores-2017/jessica/tree/master)
