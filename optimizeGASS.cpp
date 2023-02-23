#include "GA.hpp"

int main(){
    optimizeGASS::GA ga(49, 10, 10.0, 15.0, 1, 1, 0.1,0.9);
    ga.run(5);
    ga.setPopulation(ga.getmelhorPorGeracao());
    ga.mostraPopulacao();
    return 0;
}