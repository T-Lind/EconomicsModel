#include <iostream>
#include <map>
#include "Rand.h"
#include "Bonds.h"
#include <cmath>

#define growth_rate_func(x) std::sqrt(x)/10000

using namespace std;

class FedInfo {
public:
    FedInfo() = default;

    float reserve_ratio = random() / 5;
};

class BankInfo {
public:
    BankInfo(float reserve_ratio) {
        demand_deposits = normal_random(50000, 10000);
        required_reserves = reserve_ratio * demand_deposits;
        excess_reserves = demand_deposits - required_reserves;
        marketing_funding = 0;
        demand_deposits_growth_rate = 0;
    }

    float excess_reserves;
    float required_reserves;
    float marketing_funding;
    float demand_deposits_growth_rate;
    float demand_deposits;
};

auto fed_data = FedInfo();
auto bank_data = BankInfo(fed_data.reserve_ratio);
Bonds bond_data = Bonds();

int month = 0;

map<int, float> bond_returns = {{4,   normal_random(0.02, 0.003)},
                                {12,  normal_random(0.018, 0.002)},
                                {24,  normal_random(0.016, 0.002)},
                                {60,  normal_random(0.0145, 0.0015)},
                                {84,  normal_random(0.013, 0.01)},
                                {120, normal_random(0.011, 0.01)},
                                {240, normal_random(0.0099, 0.008)},
                                {360, normal_random(0.009, 0.008)}};

string get_bond_returns() {
    string ret_str;
    auto it = bond_returns.begin();
    while (it != bond_returns.end()) {
        ret_str += to_string(it->first) + " months: " + to_string(it->second * 100) + "% annually\n";
        ++it;
    }
    return ret_str;
}

bool able_to_buy(float amount) {
    return bank_data.excess_reserves >= amount;
}

bool buy_treasuries() {
    cout << "Buying Treasuries: " << endl
         << "Excess Reserves: " << bank_data.excess_reserves << endl
         << bond_data.list_bonds() << endl
         << "Accepted duration (months): " << bond_data.accepted_durations() << endl
         << "Amount to purchase: $";

    float amountToBuy;
    cin >> amountToBuy;
    cout << endl;

    cout << "Duration to buy: ";
    int durationToBuy;
    cin >> durationToBuy;
    cout << endl;


    if (!bond_data.is_accepted_duration(durationToBuy)) {
        cout << "Unavailable duration." << endl;
        return false;
    }

    if (!able_to_buy(amountToBuy) && amountToBuy >= 0) {
        cout << "Insufficient funds to purchase!" << endl;
        return false;
    }

    bank_data.excess_reserves -= amountToBuy;

    bond_data.new_bond(durationToBuy, amountToBuy, bond_returns[durationToBuy]);

    cout << bond_data.list_bonds();

    return true;
}

bool sell_treasuries() {
    cout << "Selling Treasuries:\n" << bond_data.list_bonds() << "Sell treasury #: ";

    int bondToSell;
    cin >> bondToSell;
    cout << endl;

    if (!bond_data.is_active_bond(bondToSell)) {
        cout << "Unable to sell treasury." << endl;
        return false;
    }

    bank_data.excess_reserves += bond_data.sell_bond(bondToSell,
                                                     bond_returns[(int) bond_data.get_bond(bondToSell)[0]]);
    return true;
}

bool analyze_investments() {

}

bool marketing() {
    cout << "Marketing / Advertising Initiatives: " << endl;
    cout << "Current money spent on marketing for this month: $" << bank_data.marketing_funding << endl;
    cout << "\n Spending money on marketing helps to increase demand deposits." << endl
         << "The more spent, the greater that impact is." << endl
         << "That being said, there is some risk in it - occasionally, marketing flops! More spending helps reduce the risk of a flop."
         << endl;

    cout << "Excess reserves: $" << bank_data.excess_reserves << endl;

    cout << "Additional spending: $";
    float amountToSpend;
    cin >> amountToSpend;
    cout << endl;

    if (amountToSpend <= 0 || !able_to_buy(amountToSpend)) {
        cout << "Cannot spend an invalid amount on marketing." << endl;
        return false;
    }

    bank_data.marketing_funding += amountToSpend;
    bank_data.excess_reserves -= amountToSpend;

    return true;
}

int main() {
    bool done = false;
    while (!done) {
        cout << "----------------------\n   BANK ACTIONS\n----------------------" << endl;
        cout << "1. Increment Month" << endl;
        cout << "2. Buy Treasuries" << endl;
        cout << "3. Sell Treasuries" << endl;
        cout << "4. Analyze Investments" << endl;
        cout << "5. Marketing/Advertising" << endl;
        cout << "6. " << endl;
        cout << "7. Bank Statement" << endl;
        cout << "8. Exit\n\nSelection: ";

        int selection;
        cin >> selection;

        cout << endl;

        switch (selection) {
            case 1: {
                month++;

                cout << "A month has passed. Current month: " << month << endl;


                // Increase / Decrease demand deposits due to marketing
                bank_data.demand_deposits_growth_rate += growth_rate_func(bank_data.marketing_funding);
                float before_dd = bank_data.demand_deposits;
                bank_data.demand_deposits *= (1 + bank_data.demand_deposits_growth_rate);
                bank_data.demand_deposits_growth_rate = min(bank_data.demand_deposits_growth_rate, 0.8f);
                float after_dd = bank_data.demand_deposits;
                cout << "Demand deposits grew from $" << before_dd << " to $" << after_dd << " with a change of $"
                     << after_dd - before_dd << endl;

                float before_req = bank_data.required_reserves;
                bank_data.required_reserves =  fed_data.reserve_ratio * bank_data.demand_deposits;
                float after_req = bank_data.required_reserves;
                float before_ex = bank_data.excess_reserves;
                // Update bond coupons and principals
                bank_data.excess_reserves += bond_data.get_earnings();
                bank_data.excess_reserves += (after_dd - before_dd) - (after_req - before_req);
                float after_ex = bank_data.excess_reserves;

                cout << "Excess reserves changed from $" << before_ex << " to $" << after_ex << " with a change of $"
                     << after_ex - before_ex << endl;
                cout << "Required reserves changed from $" << before_req << " to $" << after_req << " with a change of $"
                     << after_req - before_req << endl << endl;

                bank_data.marketing_funding = 0;
                continue;
            }
            case 2:
                buy_treasuries();
                break;
            case 3:
                sell_treasuries();
                break;
            case 4:
                analyze_investments();
                break;
            case 5:
                marketing();
                break;
            case 6:
                break;
            case 7:
                cout << "Bank Financial Statement: " << endl
                     << "Excess Reserves: $" << bank_data.excess_reserves << endl
                     << "Required Reserves: $" << bank_data.required_reserves << endl
                     << "Demand Deposits: $" << bank_data.demand_deposits << endl
                     << bond_data.list_bonds();


                cout << "Market Statement: " << endl
                     << "Reserve ratio: " << fed_data.reserve_ratio * 100 << "%" << endl
                     << "Bond returns, yearly:\n" << get_bond_returns() << endl;
                break;
            case 8:
                done = true;
                break;
            default:
                cout << "Invalid option specified. Continuing..." << endl;
                continue;
        }
    }
}