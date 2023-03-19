#include "Rand.h"
#include <map>
#include <string>
#include <vector>
#include "Investments.h"
#include "Loan.h"
#include "Bonds.h"

using namespace std;

#ifndef ECONOMICSMODEL_INSTITUTIONS_H
#define ECONOMICSMODEL_INSTITUTIONS_H

class MarketInfo {
public:
    MarketInfo() = default;

    float reserve_ratio = (float)random() / 5;

    map<int, float> bond_returns = {{4,   normal_random(0.02, 0.003)},
                                    {12,  normal_random(0.018, 0.002)},
                                    {24,  normal_random(0.016, 0.002)},
                                    {60,  normal_random(0.0145, 0.0015)},
                                    {84,  normal_random(0.013, 0.01)},
                                    {120, normal_random(0.011, 0.01)},
                                    {240, normal_random(0.0099, 0.008)},
                                    {360, normal_random(0.009, 0.008)}};


    Bonds bonds = Bonds();
    InvestmentGen investments = InvestmentGen();


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
    explicit BankInfo(float reserve_ratio) {
        demand_deposits = (float)normal_random(50000, 10000);
        required_reserves = reserve_ratio * demand_deposits;
        excess_reserves = demand_deposits - required_reserves;
        marketing_funding = 0;
        demand_deposits_growth_rate = 0;
    }

    string list_investments(){
        std::string ret_str = "Current Investments: \n";
        int i=0;
        for(auto& item: taken_investments){
            ret_str += std::to_string(i)+"> "+item.investment_info();
            i++;
        }
        return ret_str;
    }

    float excess_reserves;
    float required_reserves;
    float marketing_funding;
    float demand_deposits_growth_rate;
    float demand_deposits;
    vector<Investment> taken_investments;

};

#endif //ECONOMICSMODEL_INSTITUTIONS_H
