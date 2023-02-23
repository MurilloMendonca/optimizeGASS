#pragma once
#include <vector>
#include <random>
#include "Individuo2.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>
namespace optimizeGASS{
class GA {
    protected:

    //Parametros

    /**
     * @brief Vetor com as limitacoes inferior e superior da regiao de operacao
     * 
     */
    double limites[2];

    /**
     * @brief A quantidade de cadas decimais apos a virgula a serem consideradas para a parametrizacao
     * 
     */
    int precisao = 2;

    /**
     * @brief A probabilidade de mutacao sendo qualquer valor de 0.0(0%) a 1.0(100%)
     * 
     */
    double taxaMutacao = 0.01;

    /**
     * @brief A probabilidade de cruzamento sendo qualquer valor de 0.0(0%) a 1.0(100%)
     * 
     */
    double taxaCruzamento = 0.8;

    /**
     * @brief A quantidade de Individuo2s que compoem a populacao
     * 
     */
    int tamanhoPopulacao = 100;

    /**
     * @brief A quanteidade de melhores Individuo2s a terem passagem garantida para a proxima geracao
     * 
     */
    int quantidadeElitismo=2;

    /**
     * @brief A quantidade de Individuo2s a serem comparados em um torneio
     * 
     */
    int tamanhoTorneio=3;

    /**
     * @brief A quantidade de bits do genotipo
     * 
     */
    int numBits=15;

    //Principal

    /**
     * @brief Vetor que armazena todos os Individuo2s da populacao atual
     * 
     */
    std::vector<Individuo2> populacao;

    //Estatisticas

    /**
     * @brief Vetor que armazena uma copia do melhor Individuo2 a cada geracao
     * 
     */
    std::vector<Individuo2> melhorPorGeracao;

    /**
     * @brief Vetor que armazena uma copia do pior Individuo2 a cada geracao
     * 
     */
    std::vector<Individuo2> piorPorGeracao;

    /**
     * @brief Vetor que armazena o desvio padrao entre a populacao a cada geracao
     * 
     */
    std::vector<double>desvioPadraoPorGeracao;

    /**
     * @brief Vetor que armazena o valor medio da fitness de toda a populacao a cada geracao
     * 
     */
    std::vector<double> mediaPorGeracao;

    //Aleatorio
    std::random_device* rd;
    std::mt19937 gen;
    
    
    public:
    //Principais
    GA(){}
    /**
     * @brief Constroi um novo objeto da classe GA
     * 
     * @param numeroDeBits A quantidade de bits do genotipo
     * @param tamanhoDaPopulacao A quantidade de Individuo2s que compoem a populacao
     * @param limiteInferior O minimo valor da regiao de operacao
     * @param limiteSuperior O maximo valor da regiao de operacao
     * @param precisao A quantidade de cadas decimais apos a virgula a serem consideradas para a parametrizacao
     * @param torneioTam A quantidade de Individuo2s a serem comparados em um torneio
     * @param mut A probabilidade de mutacao sendo qualquer valor de 0.0(0%) a 1.0(100%)
     * @param cruzamento A probabilidade de cruzamento sendo qualquer valor de 0.0(0%) a 1.0(100%)
     */
    GA( int numeroDeBits, 
        int tamanhoDaPopulacao, 
        double limiteInferior, 
        double limiteSuperior, 
        int precisao,
        int torneioTam, 
        double mut, 
        double cruzamento);
    
    virtual ~GA(){}

    /**
     * @brief Executa todas as operacoes do GA por g geracoes
     * 
     * @param g Quantidade de geracoes a serem executadas em sequencia
     */
    virtual void run(int g);

    //helpers

    /**
     * @brief Avalia se um Individuo2 esta dentro da regiao de operacao
     * 
     * @param i Individuo2 a ser analisado
     * @return true Caso o fenotipo deste Individuo2 esteja dentro dos limites de busca
     * @return false Caso o fenotipo deste Individuo2 esteja fora dos limites de busca
     */
    bool valido(Individuo2 i);

    /**
     * @brief Popula o vetor populacao com tamanhoDaPopulacao novos Individuo2s
     * distribuidos aleatoriamente dentro dos limites de busca
     */
    virtual void geraPopulacao();

    /**
     * @brief Printa no console o genotipo, fenotipo e aptidao de cada Individuo2 da populacao
     * 
     */
    void mostraPopulacao();

    void setPopulation(std::vector<Individuo2>);

    /**
     * @brief Cria um novo Individuo2 com genotipo aleatorio dentro da regiao de operacao
     * 
     * @return Individuo2 
     */
    Individuo2 geraIndividuo2Valido();

    //Operadores evolutivos

    /**
     * @brief Calcula a aptidao de cada Individuo2 da populacao
     */
    virtual void calculaFitPopulacao();

    /**
     * @brief Calcula a aptidao de um Individuo2
     * 
     * @param i Individuo2 a ser analisado
     * @return double aptidao calculada do Individuo2 i
     */
    double calculaFit(Individuo2 i);

    /**
     * @brief Aplica o operador de cruzamento em toda a populacao gerando uma nova populacao
     * 
     */
    virtual void cruzaPopulacao();

    /**
     * @brief Realiza o cruzamento entre dois inviduos utilizando single point crossover
     * 
     * @param pai1 Primeiro Individuo2 para o cruzamento
     * @param pai2 Segundo Individuo2 para o cruzamento
     * @return std::vector<Individuo2> Dois filhos complementares gerados a partir do cruzamento
     */
    std::vector<Individuo2> cruza( Individuo2 pai1,  Individuo2 pai2);

    /**
     * @brief Aplica o operador de mutação em cada Individuo2 da populacao
     * 
     */
    virtual void mutaPopulacao();

    /**
     * @brief Aplica a mutacao em um Individuo2, onde a probabilidade de mutacao eh avalidada
     * a cada bit do genotipo, invertendo este bit quando dentro da probabilidade
     * 
     * @param ind Individio a ser mutado
     */
    void muta(Individuo2& ind);

    /**
     * @brief Realiza um operador de torneio em tamanhoTorneio Individuo2s aleatoriamente 
     * sorteados da populacao
     * 
     * @return Individuo2 o melhor Individuo2 dentre os avaliados
     */
    Individuo2 torneio();

    //Estatisticas

    /**
     * @brief Calcula a aptidao media da populaca
     * 
     * @return double a aptidao media dentro todos os Individuo2s
     */
    double fitnessMedia();

    /**
     * @brief Obtem o melhor Individuo2 dentre os presentes no vetor recebido
     * 
     * @return Individuo2 Individuo2 com a maior aptidao dentre os recebidos
     */
    Individuo2 melhor(std::vector<Individuo2>);

    /**
     * @brief Obtem o melhor individio dentre todos da populacao
     * 
     * @return Individuo2 
     */
    Individuo2 melhordaPopulacao();

    /**
     * @brief Obtem o melhor Individuo2 dentre os presentes no vetor recebido
     * 
     * @return Individuo2 Individuo2 com a menor aptidao dentre os recebidos
     */
    Individuo2 pior (std::vector<Individuo2>);

    /**
     * @brief Obtem o pior individio dentre todos da populacao
     * 
     * @return Individuo2 
     */
    Individuo2 piordaPopulacao ();

    /**
     * @brief Obtem o vetor dos melhores Individuo2s de cada geracao
     * 
     * @return std::vector<Individuo2> vetor com o melhor Individuo2 de cada populacao i em cada posicao i
     */
    std::vector<Individuo2> getmelhorPorGeracao();

    /**
     * @brief Obtem o vetor dos piores Individuo2s de cada geracao
     * 
     * @return std::vector<Individuo2> vetor com o pior Individuo2 de cada populacao i em cada posicao i
     */
    std::vector<Individuo2> getpiorPorGeracao();

    /**
     * @brief Obtem o vetor das fitness medias de cada geracao
     * 
     * @return std::vector<double> vetor com a fitness de cada populacao i em cada posicao i
     */
    std::vector<double> getMediaPorGeracao();
    std::vector<double> getDesvioPadraoPorGeracao();

    /**
     * @brief Obtem os n melhores Individuo2s dentre o vetor vec
     * 
     * @param vec Vetor de Individuo2s para a fonte da busca
     * @param n Quantidade de melhores a serem retornados
     * @return std::vector<Individuo2> Vetor contendo os n melhores Individuo2s de vec
     */
    std::vector<Individuo2> melhores( std::vector<Individuo2> vec, int n);
    double desvioPadraoDaPopulacao();
    
    //Conversoes
    double converte_bin_dec(std::vector<bool> gray);
    std::vector<bool> converte_dec_bin(double x);
    double converte_gray_dec(std::vector<bool>gray);
    std::vector<bool> converte_dec_gray(double x);
    double getFenotipo(std::vector<bool>genotipo);
    std::vector<bool> getGenotipo(double fenotipo);
};
}