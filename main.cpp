#include <iostream>
#include <map>
#include <cmath>
#include "Rand.h"
#include "Bonds.h"
#include "Investments.h"
#include "Loan.h"
#include "Institutions.h"

#define dd_growth_func(x) std::sqrt(x)/10000
#define marketing_fail_func(x) (1/(1+std::pow(M_E, (x/10000))))
#define LINE std::endl << "---------------" << std::endl

using namespace std;

auto market_data = MarketInfo();
auto bank_data = BankInfo(market_data.reserve_ratio);


int month = 0;

bool able_to_buy(float amount) {
    return bank_data.excess_reserves >= amount;
}

bool buy_treasuries() {
    cout << LINE << "Buying Treasuries: " << LINE
         << "Excess Reserves: " << bank_data.excess_reserves << endl
         << market_data.bonds.list_bonds() << endl
         << "Accepted duration (months): " << market_data.bonds.accepted_durations() << endl
         << "Amount to purchase: $";

    float amountToBuy;
    cin >> amountToBuy;
    cout << endl;

    cout << "Duration to buy: ";
    int durationToBuy;
    cin >> durationToBuy;
    cout << endl;


    if (!market_data.bonds.is_accepted_duration(durationToBuy)) {
        cout << "Unavailable duration." << endl;
        return false;
    }

    if (!able_to_buy(amountToBuy) && amountToBuy >= 0) {
        cout << "Insufficient funds to purchase!" << endl;
        return false;
    }

    bank_data.excess_reserves -= amountToBuy;

    market_data.bonds.new_bond(durationToBuy, amountToBuy, market_data.bond_returns[durationToBuy]);

    cout << market_data.bonds.list_bonds();

    return true;
}

bool sell_treasuries() {
    cout << LINE << "Selling Treasuries:" << LINE << market_data.bonds.list_bonds() << "Sell treasury #: ";

    int bondToSell;
    cin >> bondToSell;
    cout << endl;

    if (!market_data.bonds.is_active_bond(bondToSell)) {
        cout << "Unable to sell treasury." << endl;
        return false;
    }

    bank_data.excess_reserves += market_data.bonds.sell_bond(bondToSell,
                                                             market_data.bond_returns[(int) market_data.bonds.get_bond(
                                                                     bondToSell)[0]]);
    return true;
}

bool analyze_investments() {
    cout << LINE << "Investment Initiatives: " << LINE;
    cout << "InvestmentGen generally have higher yields, but are riskier compared to treasuries." << endl
         << "As your bank grows, it will have access to better and better investments." << endl;

    cout << market_data.investments.get_options();

    cout << "Option #";
    int selection;
    cin >> selection;
    cout << endl;

    Investment chosen_investment = market_data.investments.get_option(selection);

    if (chosen_investment.principal > bank_data.excess_reserves) {
        cout << "You have insufficient funds to take this investment." << endl;
        return false;
    }

    if (!chosen_investment.investment_succeeded()) {
        cout << "Investment failed. You have lost your principal." << endl;
        return true;
    }

    bank_data.excess_reserves -= chosen_investment.principal;
    bank_data.taken_investments.push_back(chosen_investment);
    return true;
}

bool marketing() {
    cout << LINE << "Marketing / Advertising Initiatives: " << LINE;
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
    bank_data.excess_reserves -= amountToSpend;

    if (random() < marketing_fail_func(amountToSpend)) {
        cout << "Your marketing initiative has failed, resulting in no increase in demand deposits" << endl;
        cout << " As you spend more, the chance of these failures decreases." << endl;
        return true;
    }

    bank_data.marketing_funding += amountToSpend;

    return true;
}

bool take_loan() {
    cout << LINE << "Take a Loan: " << LINE;

}

int main() {
    bool done = false;
    while (!done) {
        cout << LINE << "BANK DASHBOARD" << LINE;
        cout << "1. Increment Month" << endl;
        cout << "2. Buy Treasuries" << endl;
        cout << "3. Sell Treasuries" << endl;
        cout << "4. Analyze Investments" << endl;
        cout << "5. Marketing/Advertising" << endl;
        cout << "6. Take a Loan" << endl;
        cout << "7. Bank Statement" << endl;
        cout << "8. Exit\n\nSelection: ";

        int selection;
        cin >> selection;

        cout << endl;

        switch (selection) {
            case 1: {
                month++;
                market_data.investments.new_options();

                cout << "A month has passed. Current month: " << month << endl;


                // Increase / Decrease demand deposits due to marketing
                bank_data.demand_deposits_growth_rate += dd_growth_func(bank_data.marketing_funding);
                float before_dd = bank_data.demand_deposits;
                bank_data.demand_deposits *= (1 + bank_data.demand_deposits_growth_rate);
                bank_data.demand_deposits_growth_rate = min(bank_data.demand_deposits_growth_rate, 0.8f);
                bank_data.demand_deposits = min(bank_data.demand_deposits, 1000000000.f);
                float after_dd = bank_data.demand_deposits;
                cout << "Demand deposits grew from $" << before_dd << " to $" << after_dd << " with a change of $"
                     << after_dd - before_dd << endl;

                float before_req = bank_data.required_reserves;
                bank_data.required_reserves = market_data.reserve_ratio * bank_data.demand_deposits;
                float after_req = bank_data.required_reserves;
                float before_ex = bank_data.excess_reserves;
                // Update bond coupons and principals
                bank_data.excess_reserves += market_data.bonds.get_earnings();
                bank_data.excess_reserves += (after_dd - before_dd) - (after_req - before_req);

                // InvestmentGen
                for (auto &item: bank_data.taken_investments)
                    bank_data.excess_reserves += item.return_per_month;
                float after_ex = bank_data.excess_reserves;

                cout << "Excess reserves changed from $" << before_ex << " to $" << after_ex << " with a change of $"
                     << after_ex - before_ex << endl;
                cout << "Required reserves changed from $" << before_req << " to $" << after_req
                     << " with a change of $"
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
                cout << LINE << "Bank Financial Statement: " << LINE
                     << "Excess Reserves: $" << bank_data.excess_reserves << endl
                     << "Required Reserves: $" << bank_data.required_reserves << endl
                     << "Demand Deposits: $" << bank_data.demand_deposits << endl
                     << market_data.bonds.list_bonds() << endl
                     << bank_data.list_investments();


                cout << LINE << "Market Statement: " << LINE
                     << "Reserve ratio: " << market_data.reserve_ratio * 100 << "%" << endl
                     << "Bond returns, yearly:\n" << market_data.get_bond_data() << endl;
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