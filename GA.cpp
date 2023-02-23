#include "GA.hpp"
#include "Individuo2.hpp"
#include "newGASS-base/newGASS.hpp"
#include <iostream>

namespace optimizeGASS{

GA::GA(int numeroDeBits, int tamanhoDaPopulacao, double limiteInferior, 
        double limiteSuperior, int precisao, int torneioTam, double mut,
        double cruzamento){
    rd = new std::random_device;
    gen = std::mt19937((*rd)());
    numBits=numeroDeBits;
    this->taxaMutacao=mut;
    this->taxaCruzamento=cruzamento;
    this->precisao=precisao;
    this->tamanhoPopulacao=tamanhoDaPopulacao;
    this->tamanhoTorneio=torneioTam;
    limites[0]=limiteInferior;
    limites[1]=limiteSuperior;
    geraPopulacao();
    mostraPopulacao();
    calculaFitPopulacao();
    mostraPopulacao();

}
void GA::mostraPopulacao(){
    int x=0;
    for(Individuo2 ind:populacao){
        std::cout<<"\nIndividuo2 "<<x++<<": ";
        for(int i=0;i<numBits;i++)
            std::cout<<" "<<ind.getCromossomo()[i];
        std::cout<<";\n\t populationSize="<<ind.getParameters().AG_POPULATION_SIZE
            <<";\n\t generations="<<ind.getParameters().AG_NUMBER_OF_GENERATIONS
            <<";\n\t elite="<<ind.getParameters().AG_NUMBER_OF_ELITE_INDIVIDUALS
            <<";\n\t torneio="<<ind.getParameters().AG_TOURNAMENT_SIZE
            <<";\n\t mut="<<ind.getParameters().AG_MUTATION_RATE
            <<";\n\t cruza="<<ind.getParameters().AG_CROSSOVER_RATE
            <<";\n\t y="<<ind.getFitness();
    }
}
Individuo2 GA::geraIndividuo2Valido(){
    std::uniform_int_distribution<int> distribution(0,1);
    std::vector<bool> cromossomo(numBits);
    for(int i=0;i<numBits;i++)
        cromossomo[i]=distribution(gen);
    Individuo2 novo(cromossomo);
    return novo;
}
void GA::run(int geracoes){
    for(int i=0;i<geracoes;i++){
        auto elite = melhores(populacao, quantidadeElitismo);
        cruzaPopulacao();
        mutaPopulacao();
        calculaFitPopulacao();
        for(auto ind:elite)
            populacao.push_back(ind);
        melhorPorGeracao.push_back(melhor(populacao));
    }
}
 void GA::calculaFitPopulacao(){
    std::vector<std::thread> vThreads;
    std::mutex mut;
    Individuo2* vec = populacao.data();
    int tam = populacao.size();
    int* controle=new int;
    *controle=0;
    for(int i=0;i<2;i++){
        vThreads.push_back(std::thread([](Individuo2* vec, int* controle, int tam, std::mutex* mut,GA* g){
            int i;
            while(*controle<tam){
                mut->lock();
                i=*controle;
                *controle=(*controle) +1;
                mut->unlock();
                if(i<tam)
                    vec[i].setFitness(g->calculaFit(vec[i]));
            }
        }, vec, controle,tam, &mut,this));
        

    }
    for(std::thread& t:vThreads)
        t.join();
}
void GA::geraPopulacao(){
    for(int i=0;i<this->tamanhoPopulacao;i++)
        populacao.push_back(geraIndividuo2Valido());
}
double GA::calculaFit(Individuo2 ind){
    //Roda o newGASS 5 vezes em teste de sanidade one-to-one, 
    //captura a média de tempo de execução e a quantidad de vezes que o melhor indivíduo foi encontrado
    float fitness=0;
    if(ind.getParameters().AG_POPULATION_SIZE<=ind.getParameters().AG_NUMBER_OF_ELITE_INDIVIDUALS
        || ind.getParameters().AG_POPULATION_SIZE<=ind.getParameters().AG_TOURNAMENT_SIZE)
        return 1000;
    for(int i=0;i<10;i++){
        std::set<Individuo> resultados;
        auto start = std::chrono::high_resolution_clock::now();
        runSanityTest(ind.getParameters(),&resultados);
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        bool achou=false;
        for(auto ind:resultados)
            if(ind.getFitness()==0){
                achou=true;
                break;
            }
        fitness+=(!achou) + elapsed.count();
    }
    return fitness;
    
}
void GA::cruzaPopulacao(){
    std::vector<Individuo2> novapop;
    std::vector<Individuo2> filhos;
    std::uniform_real_distribution<double> distribution(0,1);
    while (novapop.size()<(tamanhoPopulacao-quantidadeElitismo)){
        Individuo2 pai1 = torneio();
        Individuo2 pai2 = torneio();

            filhos = cruza(pai1, pai2);
            novapop.push_back(filhos[0]);
            novapop.push_back(filhos[1]);
    }
    populacao=novapop;
}
std::vector<Individuo2> GA::melhores( std::vector<Individuo2> vec, int n){
    std::vector<Individuo2> v;
    for(int i=0;i<n;i++){
        v.push_back(melhor(vec));
        vec.erase(std::find(vec.begin(),vec.end(),v.at(i)));
    }
    return v;
}
std::vector<Individuo2> GA::cruza( Individuo2 pai1,  Individuo2 pai2){
    
    std::uniform_real_distribution<double> dist(0,1);
    if(dist(gen)<taxaCruzamento){
        std::uniform_int_distribution<int> distribution(0,numBits-1);
        std::vector<std::vector<bool>> genFilhos(2);
        std::vector<bool> genesFilho1;
        std::vector<bool> genesFilho2;

        genesFilho1.clear();
        genesFilho2.clear();

        int corte = distribution(gen);

        for(int i=0;i<corte;i++){
            genesFilho1.push_back(pai1.getCromossomo()[i]);
            genesFilho2.push_back(pai2.getCromossomo()[i]);
        }
        for(int i=corte;i<numBits;i++){
            genesFilho1.push_back(pai2.getCromossomo()[i]);
            genesFilho2.push_back(pai1.getCromossomo()[i]);
        }
        Individuo2 f1(genesFilho1);
        Individuo2 f2(genesFilho2);


        std::vector<Individuo2>res;
        res.emplace_back(genesFilho1);
        res.emplace_back(genesFilho2);
        return res;
    }
    std::vector<Individuo2>res;
    res.push_back(pai1);
    res.push_back(pai2);
    return res;
    
}
void GA::mutaPopulacao(){
    std::uniform_real_distribution<double> distribution(0,1);
    for(Individuo2& ind:populacao)
        //if(distribution(gen)<taxaMutacao)
        muta(ind);
}
void GA::muta(Individuo2& ind){
    std::uniform_real_distribution<double> distribution(0,1);
    auto vec = ind.getCromossomo();
    Individuo2 mutado(vec);

    vec = ind.getCromossomo();
    int tam = vec.size();
    for(int i =0;i<tam;i++)
        if(distribution(gen)<taxaMutacao)
            vec[i] = !vec[i];
    mutado.setCromossomo(vec);

    mutado.setFitness(calculaFit(mutado));
    ind = mutado;
}
Individuo2 GA::torneio(){
    std::vector<Individuo2> aux;
    std::uniform_int_distribution<int> distribution(0,tamanhoPopulacao-1);
    for(int i=0;i<tamanhoTorneio;i++){
        int pos = distribution(gen);
        aux.push_back(populacao.at(pos));
    }
    return melhor(aux);
}
Individuo2 GA::melhor(std::vector<Individuo2>vec){
    return *(std::max_element(vec.begin(), vec.end()));
}
Individuo2 GA::pior (std::vector<Individuo2>vec){
    return *(std::min_element(vec.begin(), vec.end()));;
}
Individuo2 GA::piordaPopulacao (){
    return pior(populacao);
}
Individuo2 GA::melhordaPopulacao(){
    return melhor(populacao);
}
double GA::converte_bin_dec(std::vector<bool>bin)
{

    double dec =0;
    for(int i =0, j=(numBits-1); j>=0; i++,j--)
    {
        dec += bin[j]*pow(2,i);
    }
    
    dec = dec/pow(10, precisao);
    dec+=limites[0];
        return dec;

}
std::vector<bool> GA::converte_dec_bin(double x)
{
    x=-x;
    x+=limites[0];
    float decf = x*pow(10, precisao);
    int dec = decf;
    std::vector<bool>bin(numBits);

    for(int i =(numBits-1); i>=0; i--)
    {
        bin[i] = dec%2;
        dec = dec/2;
    }
    return bin;
}
double GA::fitnessMedia(){
    double media = 0;
    for(auto ind:populacao)
        media+=ind.getFitness()/populacao.size();
    return media;
}
void GA::setPopulation(std::vector<Individuo2> nova){
    this->populacao=nova;
    calculaFitPopulacao();
}
std::vector<Individuo2> GA::getmelhorPorGeracao(){
    return this->melhorPorGeracao;
}
std::vector<Individuo2> GA::getpiorPorGeracao(){
    return this->piorPorGeracao;
}
std::vector<double> GA::getMediaPorGeracao(){
    return this->mediaPorGeracao;
}
std::vector<double> GA::getDesvioPadraoPorGeracao(){
    return this->desvioPadraoPorGeracao;
}
double GA::desvioPadraoDaPopulacao(){
    double media = fitnessMedia();
    double dp=0;
    for(auto ind:populacao){
        dp+=pow((ind.getFitness() - media),2)/populacao.size();
    }
    return dp;
}
double GA::converte_gray_dec(std::vector<bool>gray)
{
    std::vector<bool>bin(numBits);
    int i= 1;
    bin[0]=gray[0];
    while(i<numBits)
    {
 
        bin[i]=gray[i]^bin[i-1];
        i++;
    }
    return converte_bin_dec(bin);
    

}

std::vector<bool> GA::converte_dec_gray(double x)
{

    std::vector<bool>bin = converte_dec_bin(x);
    std::vector<bool>gray(numBits);
    int i= 1;
    gray[0]=bin[0];
    while(i<numBits)
    {
        {
            gray[i]=bin[i]^bin[i-1];
        }
        i++;
    }
    return gray;
}
double GA::getFenotipo(std::vector<bool>genotipo){
    //return converte_bin_dec(genotipo);
    return converte_gray_dec(genotipo);
}

}