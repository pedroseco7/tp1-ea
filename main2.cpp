#include <iostream>
#include <vector>
#include <algorithm>

const int MAX_TRIKITS = 20; // A restrição do problema diz n < 20
const int TAM_TABULEIRO = 40;

struct Trikit {
    int id;
    int valores[3];
    bool usada; 
    int max_pontos_possiveis; 
};

struct Celula {
    bool ocupada;
    bool bloqueada; 
    int id_trikit;
    int orientacao; 
};

Trikit pecas[MAX_TRIKITS];
Celula tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO];
int num_pecas_total;
int max_score_global;

//soma das coordenadas=par -> aponta para cima. Caso contrário, aponta para baixo
bool apontaParaCima(int linha, int coluna){
    return (linha + coluna) % 2 == 0;
}

//obter o valor de um canto (0,1,2) com a rotação aplicada
int obter_valor(int id_peca, int orientacao, int canto) {
    return pecas[id_peca].valores[(canto + orientacao) % 3];
}

//testar se podemos colocar a peça, devolvendo a pontuação (-1 se não for possível)
int testar_encaixe(int linha, int coluna, int id_peca, int orientacao, int num_pecas_na_mesa){
    int pontos_ganhos = 0;
    int vizinhos_tocados = 0;
    bool aponta_cima = apontaParaCima(linha, coluna);

    //valores da peça que queremos colocar
    int meu_v0 = obter_valor(id_peca, orientacao, 0);
    int meu_v1 = obter_valor(id_peca, orientacao, 1);
    int meu_v2 = obter_valor(id_peca, orientacao, 2);
    
    //estes dl e dc são as variações de linha e coluna, para aceder aos vizinhos. As variações vão ser diferentes consoante a orientação da peça
    int dl[3], dc[3]; 

    if (aponta_cima) { //então só tem vizinhos à dir, baix e esq
        dl[0] = 0; dc[0] = 1;
        dl[1] = 1; dc[1] = 0;
        dl[2] = 0; dc[2] = -1;
    } else {  //então só tem vizinhos à esq, cima e dir
        dl[0] = 0; dc[0] = -1;
        dl[1] = -1; dc[1] = 0;
        dl[2] = 0; dc[2] = 1;
    }

    for (int i = 0; i < 3; i++){
        //para cada lado da peça, ver os seus vizinhos.
        int viz_l = linha + dl[i];
        int viz_c = coluna + dc[i];

        //fora do tabuleiro
        if (viz_l < 0 || viz_l >= TAM_TABULEIRO || viz_c < 0 || viz_c >= TAM_TABULEIRO){
            continue;
        }

        if(tabuleiro[viz_l][viz_c].ocupada) { //se o vizinho tem uma peça
            vizinhos_tocados++;

            int id_viz = tabuleiro[viz_l][viz_c].id_trikit;
            int ori_viz = tabuleiro[viz_l][viz_c].orientacao;
            
            //obter os valores da peça do vizinho
            int viz_v0 = obter_valor(id_viz, ori_viz, 0);
            int viz_v1 = obter_valor(id_viz, ori_viz, 1);
            int viz_v2 = obter_valor(id_viz, ori_viz, 2);

            //encaixe
            bool encaixa = false;
            if (i==0){
                if (meu_v0 == viz_v1 && meu_v1 == viz_v0) encaixa = true;
            } else if (i==1){
                if (meu_v1 == viz_v2 && meu_v2 == viz_v1) encaixa = true;
            } else if (i==2){
                if (meu_v2 == viz_v0 && meu_v0 == viz_v2) encaixa = true;
            }

            if (!encaixa){
                return -1; // Colisão
            } else {
                if (i == 0) pontos_ganhos += meu_v0 + meu_v1;
                else if (i==1) pontos_ganhos += meu_v1 + meu_v2;
                else if (i==2) pontos_ganhos += meu_v2 + meu_v0;
            }
        }
    }

    //qq peça colocada depois da primeira tem de partilhar um lado com uma já na mesa
    if (num_pecas_na_mesa > 0 && vizinhos_tocados == 0){
        return -1;
    }
    
    return pontos_ganhos;
}

void resolver(int score_atual, int num_pecas_na_mesa, const std::vector<std::pair<int, int>>& fronteira){

    int max_potencial = score_atual;
    for (int i = 0; i < num_pecas_total; i++) {
        if (!pecas[i].usada) {
            max_potencial += pecas[i].max_pontos_possiveis;
        }
    }

    if (max_potencial <= max_score_global) {
        return; 
    }

    if (score_atual > max_score_global){ 
        max_score_global = score_atual;
    }

    if (num_pecas_na_mesa == num_pecas_total) return;

    if (num_pecas_na_mesa == 0){
        int l = TAM_TABULEIRO / 2;
        int c = TAM_TABULEIRO / 2;

        for (int i = 0; i < num_pecas_total; i++){
            for (int ori = 0; ori < 3; ori++){
                pecas[i].usada = true;
                tabuleiro[l][c].ocupada = true;
                tabuleiro[l][c].id_trikit = i;
                tabuleiro[l][c].orientacao = ori;

                std::vector<std::pair<int, int>> fronteira_inicial;
                int dl[3], dc[3];
                if (apontaParaCima(l, c)) { 
                    dl[0]=0; dc[0]=1; dl[1]=1; dc[1]=0; dl[2]=0; dc[2]=-1;
                } else { 
                    dl[0]=0; dc[0]=-1; dl[1]=-1; dc[1]=0; dl[2]=0; dc[2]=1;
                }
                for (int k = 0; k < 3; k++) {
                    fronteira_inicial.push_back({l + dl[k], c + dc[k]});
                }

                resolver(0, 1, fronteira_inicial);

                pecas[i].usada = false;
                tabuleiro[l][c].ocupada = false;
            }
        }
        return;
    }

    if (fronteira.empty()) return;

    std::pair<int, int> alvo = fronteira.back();
    int l = alvo.first;
    int c = alvo.second;

    std::vector<std::pair<int, int>> fronteira_restante = fronteira;
    fronteira_restante.pop_back(); 

    if (tabuleiro[l][c].ocupada || tabuleiro[l][c].bloqueada) {
        resolver(score_atual, num_pecas_na_mesa, fronteira_restante);
        return;
    }

    tabuleiro[l][c].bloqueada = true;
    resolver(score_atual, num_pecas_na_mesa, fronteira_restante);
    tabuleiro[l][c].bloqueada = false; 

    for (int i = 0; i < num_pecas_total; i++) {
        if (pecas[i].usada) continue;

        for (int ori = 0; ori < 3; ori++) {
            int pontos = testar_encaixe(l, c, i, ori, num_pecas_na_mesa);

            if (pontos != -1) { 
                pecas[i].usada = true;
                tabuleiro[l][c].ocupada = true;
                tabuleiro[l][c].id_trikit = i;
                tabuleiro[l][c].orientacao = ori;

                std::vector<std::pair<int, int>> nova_fronteira = fronteira_restante;
                
                int dl[3], dc[3];
                if (apontaParaCima(l, c)) {
                    dl[0]=0; dc[0]=1; dl[1]=1; dc[1]=0; dl[2]=0; dc[2]=-1;
                } else {
                    dl[0]=0; dc[0]=-1; dl[1]=-1; dc[1]=0; dl[2]=0; dc[2]=1;
                }

                for (int k = 0; k < 3; k++) {
                    int nl = l + dl[k];
                    int nc = c + dc[k];
                    if (nl >= 0 && nl < TAM_TABULEIRO && nc >= 0 && nc < TAM_TABULEIRO) {
                        if (!tabuleiro[nl][nc].ocupada && !tabuleiro[nl][nc].bloqueada) {
                            // Impedir duplicados na nova fronteira
                            bool existe = false;
                            for(auto pos : nova_fronteira) {
                                if(pos.first == nl && pos.second == nc) { existe = true; break; }
                            }
                            if(!existe) nova_fronteira.push_back({nl, nc});
                        }
                    }
                }

                resolver(score_atual + pontos, num_pecas_na_mesa + 1, nova_fronteira);

                // Desfazer jogada (Backtracking)
                tabuleiro[l][c].ocupada = false;
                pecas[i].usada = false;
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    num_pecas_total = 0;
    max_score_global = 0;

    for(int i=0; i<TAM_TABULEIRO; i++) {
        for(int j=0; j<TAM_TABULEIRO; j++) {
            tabuleiro[i][j].ocupada = false;
            tabuleiro[i][j].bloqueada = false;
        }
    }

    while (std::cin >> pecas[num_pecas_total].valores[0] 
                    >> pecas[num_pecas_total].valores[1] 
                    >> pecas[num_pecas_total].valores[2]) {
        
        pecas[num_pecas_total].id = num_pecas_total;
        pecas[num_pecas_total].usada = false;
        
        // PRÉ-CÁLCULO do máximo de pontos que esta peça pode contribuir
        int s1 = pecas[num_pecas_total].valores[0] + pecas[num_pecas_total].valores[1];
        int s2 = pecas[num_pecas_total].valores[1] + pecas[num_pecas_total].valores[2];
        int s3 = pecas[num_pecas_total].valores[2] + pecas[num_pecas_total].valores[0];
        pecas[num_pecas_total].max_pontos_possiveis = s1 + s2 + s3;

        num_pecas_total++;
    }

    std::vector<std::pair<int, int>> fronteira_vazia;
    resolver(0, 0, fronteira_vazia);

    std::cout << max_score_global << "\n";

    return 0;
}