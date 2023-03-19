#include <iostream>
#include <string>
#include <random>
//
// Created by zenith on 3/17/2023.
//

#ifndef ECONOMICSMODEL_RAND_H
#define ECONOMICSMODEL_RAND_H

double random(){
    using namespace std;
    srand(time(nullptr));
    return static_cast<double>(rand())/RAND_MAX;
}

double normal_random(double mu, double sigma){
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mu,sigma);
    return distribution(generator);
}

#endif //ECONOMICSMODEL_RAND_H
