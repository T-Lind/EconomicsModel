#include <vector>
#include <map>
#include <string>
#include "Rand.h"
#include <cmath>
#include <iostream>

#ifndef ECONOMICSMODEL_INVESTMENTS_H
#define ECONOMICSMODEL_INVESTMENTS_H

#define risk_func(x) (1/(1+std::pow(M_E, -(30*x-3.5))))

class Investment {
public:
    Investment(double mu_rate, double sigma_rate) {
        annual_return = (double) normal_random(mu_rate, sigma_rate);
        principal = (double) random(10000, 100000);
        risk = (double) std::max(0.0, risk_func(annual_return - principal / 50000000));
        return_per_month = principal * annual_return / 12;
    }

    [[nodiscard]] bool investment_succeeded() const {
        return random() >= risk;
    }

    [[nodiscard]] std::string investment_info() const {
        return
                "Investment: $" + std::to_string(principal) + " down; " + std::to_string(annual_return * 100) +
                "% annual rate; " + std::to_string(risk * 100) + "% fail chance, monthly return: " +
                std::to_string(return_per_month) + "\n";
    }

    double return_per_month;
    double annual_return;
    double principal;
private:
    double risk;
};

class InvestmentGen {
public:
    InvestmentGen() {
        new_options();
    }

    void new_options() {
        current_options.clear();
        current_options.emplace_back(0.05, 0.03);
        current_options.emplace_back(0.05, 0.07);
        current_options.emplace_back(0.1, 0.04);
        current_options.emplace_back(0.1, 0.07);
        current_options.emplace_back(0.15, 0.1);
        current_options.emplace_back(0.20, 0.1);
        current_options.emplace_back(0.25, 0.2);
    }

    std::string get_options() {
        std::string ret_str = "Current Opportunities: \n";
        int i = 0;
        for (auto &item: current_options) {
            ret_str += std::to_string(i) + "> " + item.investment_info();
            i++;
        }
        return ret_str;
    }

    Investment get_option(int option_num) {
        return current_options[option_num];
    }

    std::vector<Investment> current_options;
};

#endif //ECONOMICSMODEL_INVESTMENTS_H
