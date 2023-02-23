#include "Individuo2.hpp"


Individuo2::Individuo2(){

}
Individuo2::Individuo2(std::vector<bool> cromossomo ){
    setCromossomo(cromossomo);
}
std::vector<bool> Individuo2::getCromossomo() const{
    return this->cromossomo;
}

double Individuo2::getFitness(){
    return this->fitness;
}
void Individuo2::setFitness(double x){
    this->fitness=x;
}
void Individuo2::setCromossomo(std::vector<bool> x){
    this->cromossomo=x;
    convertCromossomoToParameters();
}

GASS::Parameters Individuo2::getParameters(){
    return this->parameters;
}

void Individuo2::convertCromossomoToParameters(){
    this->parameters.AG_POPULATION_SIZE=0;
    this->parameters.AG_NUMBER_OF_GENERATIONS=0;
    this->parameters.AG_NUMBER_OF_ELITE_INDIVIDUALS=0;
    this->parameters.AG_TOURNAMENT_SIZE=0;
    this->parameters.AG_MUTATION_RATE=0;
    this->parameters.AG_CROSSOVER_RATE=0;
    int populationBits=9;
    int generationsBits=9;
    int eliteBits=7;
    int tournamentBits=7;
    int mutationBits=9;
    int crossoverBits=9;
    //Soma total de bits: 49
    int i=0;
    for(;i<populationBits;i++)
        this->parameters.AG_POPULATION_SIZE+=this->cromossomo[i]*pow(2,i);
    for(;i<populationBits+generationsBits;i++)
        this->parameters.AG_NUMBER_OF_GENERATIONS+=this->cromossomo[i]*pow(2,i-populationBits);
    for(;i<populationBits+generationsBits+eliteBits;i++)
        this->parameters.AG_NUMBER_OF_ELITE_INDIVIDUALS+=this->cromossomo[i]*pow(2,i-(populationBits+generationsBits));
    for(;i<populationBits+generationsBits+eliteBits+tournamentBits;i++)
        this->parameters.AG_TOURNAMENT_SIZE+=this->cromossomo[i]*pow(2,i-(populationBits+generationsBits+eliteBits));
    // float parameters between 0 and 1
    for(;i<populationBits+generationsBits+eliteBits+tournamentBits+mutationBits;i++)
        this->parameters.AG_MUTATION_RATE+=this->cromossomo[i]*pow(2,i-(populationBits+generationsBits+eliteBits+tournamentBits+mutationBits));
    for(;i<populationBits+generationsBits+eliteBits+tournamentBits+mutationBits+crossoverBits;i++){
        this->parameters.AG_CROSSOVER_RATE+=this->cromossomo[i]*pow(2,i-(populationBits+generationsBits+eliteBits+tournamentBits+mutationBits+crossoverBits));
    }

}


bool operator<( Individuo2 &ind1,  Individuo2 &ind2){
    return ind1.getFitness()>ind2.getFitness();
}

bool operator==( const Individuo2 ind1,  const Individuo2 ind2){
    int s = ind1.getCromossomo().size();
    for(int i=0;i<ind1.getCromossomo().size();i++)
        if(ind1.getCromossomo()[i]!=ind2.getCromossomo()[i]) return false;
    return true;
}
