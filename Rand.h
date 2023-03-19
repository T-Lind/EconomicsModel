#include <iostream>
#include <string>
#include <random>

#ifndef ECONOMICSMODEL_RAND_H
#define ECONOMICSMODEL_RAND_H

double random(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    return dis(gen);
}

double random(double low, double high){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(low, high);
    return dis(gen);
}

double normal_random(double mu, double sigma){
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mu,sigma);
    return distribution(generator);
}

#endif //ECONOMICSMODEL_RAND_H
