#include "Rand.h"
#include <map>
#include <string>
#include <vector>
#include "Investments.h"
#include "Bonds.h"
#include "Loan.h"

using namespace std;

#ifndef ECONOMICSMODEL_INSTITUTIONS_H
#define ECONOMICSMODEL_INSTITUTIONS_H

class MarketInfo {
public:
    MarketInfo() = default;

    double reserve_ratio = (double) random() / 5;

    map<int, double> bond_returns = {{4,   normal_random(0.02, 0.003)},
                                    {12,  normal_random(0.018, 0.002)},
                                    {24,  normal_random(0.016, 0.002)},
                                    {60,  normal_random(0.0145, 0.0015)},
                                    {84,  normal_random(0.013, 0.01)},
                                    {120, normal_random(0.011, 0.01)},
                                    {240, normal_random(0.0099, 0.008)},
                                    {360, normal_random(0.009, 0.008)}};


    Bonds bonds = Bonds();
    InvestmentGen investments = InvestmentGen();
    LoanGen loans = LoanGen();

    string get_bond_data() {
        string ret_str;
        auto it = bond_returns.begin();
        while (it != bond_returns.end()) {
            ret_str += to_string(it->first) + " months: " + to_string(it->second * 100) + "% annually\n";
            ++it;
        }
        return ret_str;
    }
};

class BankInfo {
public:
    explicit BankInfo(double reserve_ratio) {
        demand_deposits = (double) normal_random(50000, 10000);
        required_reserves = reserve_ratio * demand_deposits;
        excess_reserves = demand_deposits - required_reserves;
        marketing_funding = 0;
        demand_deposits_growth_rate = 0;
    }

    string list_investments() {
        std::string ret_str = "Current Investments: \n";
        int i = 0;
        for (auto &item: taken_investments) {
            ret_str += std::to_string(i) + "> " + item.investment_info();
            i++;
        }
        return ret_str;
    }

    double excess_reserves;
    double required_reserves;
    double marketing_funding;
    double demand_deposits_growth_rate;
    double demand_deposits;
    vector<Investment> taken_investments;
    vector<Loan> taken_loans;

    string list_loans() {
        string ret_str;
        int i = 0;
        for (auto &item: taken_loans) {
            if(item.monthly_payment != 0)
                ret_str += to_string(i)+"> "+item.bought_info()+"\n";
            i++;
        }
        return ret_str;
    }
};

#endif //ECONOMICSMODEL_INSTITUTIONS_H
