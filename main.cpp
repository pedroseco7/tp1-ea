/*

Peças são triângulos (trikit) que têm 3 número em cada canto.
Cada número está entre 0 e 5
Os números estão escritos na ordem clockwise. Nunca decrescendo no sentido horário. Exemplo: 0-1-2, 0-2-3, 1-2-3, 1-3-5, 2-3-4, 3-4-5, etc.
Alguns triangulos podem repetir o mesmo numero mais de uma vez, como 0-0-1, 0-1-1, 0-2-2, 1-1-2, 1-2-2, 2-2-3, etc.
Nunca há 2 peças iguais

Regras:
- O primeiro trikit pode ser colocado livremente no tabuleiro
- Cada trikit subsequente tem de dar match a um já colocado no tabuleiro
- Match: quando 2 trikits partilham um lado do triangulo com os mesmo 2 números (canto-a-canto, lado-a-lado)
- Pontos do match: os matching numbers são adicionados ao score do jogador. Exemplo: se um trikit 0-1-2 é colocado ao lado de um trikit 0-1-3, o jogador ganha 0+1=1 pontos.

n = número de peças
n < 20

Escolher qual o trikit a colocar primeiro
Escolher qual o trikit a dar match e em que ordem
Aceitar que alguns trikits nunca serão colocados
Maximizar o score do jogador

Um trikit só pode ser colocado se for o primeiro ou se der match a um já colocado no tabuleiro. O jogo termina quando não é possível colocar mais nenhum trikit.

INPUT: n linhas, cada uma com 3 números (0-5) representando os cantos do trikit, separados por espaços. Os números estão em ordem crescente (clockwise).
OUTPUT: dar print ao score máximo possível do jogador (para debug pode ser util imprimir o status do tabuleiro a cada jogada)
RESTRIÇÕES: n < 20, números entre 0 e 5, nunca há 2 peças iguais, os números estão em ordem crescente (clockwise)

Example input:
0 3 4
0 0 2
2 4 4
0 1 2
2 3 4
1 4 5
0 2 2
2 2 4

Example output:
20

Gestão do INPUT:
1ª ideia - Foi ler os dados linha a linha e guardá-los num tuplo. Contudo, como os dados são sempre 3 números, pode ser mais fácil criar uma struct Trikit com um array de 3 inteiros para os cantos. Assim, podemos ter métodos para obter os lados do trikit e facilitar o processo de matching.
2ª ideia - Guardar os trikits num vector de Trikit. Assim, podemos iterar sobre eles facilmente e aplicar as regras do jogo para encontrar o score máximo.

*/

#include <iostream>
#include <vector>
#include <tuple>

struct Trikit {
    int corners[3];

    std::pair<int, int> side(int s) const {
        int a = corners[s];
        int b = corners[(s+1) % 3];
        if (a > b){
            std::swap(a, b);
        }
        return {a, b};
    }
};

std::vector<Trikit> getInput() {
    std::vector<Trikit> v;
    Trikit t;
    while (std::cin >> t.corners[0] >> t.corners[1] >> t.corners[2]) {
        v.push_back(t);
    }
    return v;
}

int main() {
    std::vector<Trikit> trikits = getInput();
    return 0;
}