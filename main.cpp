#include <iostream>
#include <map>
#include "Rand.h"
#include "Bonds.h"

using namespace std;

map<std::string, double> bank_data;
map<std::string, double> fed_data;
Bonds bond_data = Bonds();

int month = 0;

map<int, float> bond_returns = {{4,   normal_random(0.02, 0.003)},
                                {12,  normal_random(0.018, 0.002)},
                                {24,  normal_random(0.016, 0.002)},
                                {60,  normal_random(0.0145, 0.0015)},
                                {84,  normal_random(0.013, 0.01)},
                                {120, normal_random(0.011, 0.01)},
                                {240, normal_random(0.099, 0.008)},
                                {360, normal_random(0.09, 0.008)}};

string get_bond_returns() {
    string ret_str;
    auto it = bond_returns.begin();
    while(it != bond_returns.end()){
        ret_str += to_string(it->first)+" months: "+ to_string(it->second*100)+"% annually\n";
    }
    return ret_str;
}

bool able_to_buy(double amount) {
    return bank_data["excess reserves"] >= amount;
}

bool buy_treasuries() {
    cout << "Buying Treasuries: " << endl
         << "Excess Reserves: " << bank_data["excess reserves"] << endl
         << bond_data.list_bonds() << endl
         << "Accepted duration (months): " << bond_data.accepted_durations() << endl
         << "Amount to buy: ";

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

    bank_data["excess reserves"] -= amountToBuy;

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

    bank_data["excess reserves"] += bond_data.sell_bond(bondToSell,
                                                        bond_returns[(int) bond_data.get_bond(bondToSell)[0]]);
    return true;
}

bool analyze_investments() {

}

bool marketing() {

}

int main() {
    fed_data["reserve ratio"] = random() / 5;

    bank_data["excess reserves"] = normal_random(5000, 3000) + 1000;
    bank_data["required reserves"] = 1000;
    bank_data["demand deposits"] = bank_data["required reserves"] / fed_data["reserve ratio"];


    bool done = false;
    while (!done) {
        cout << "----------------------\n   BANK ACTIONS\n----------------------" << endl;
        cout << "1. Continue" << endl;
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
            case 1:
                month++;
                bank_data["excess reserves"] += bond_data.get_earnings();
                continue;
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
                     << "Excess Reserves: " << bank_data["excess reserves"] << endl
                     << "Required Reserves: " << bank_data["required reserves"] << endl
                     << "Demand Deposits: " << bank_data["demand deposits"] << endl
                     << bond_data.list_bonds();


                cout << "Market Statement: " << endl
                     << "Reserve ratio: " << fed_data["reserve ratio"] << endl;
//                     << "Bond returns:\n" << get_bond_returns() << endl;
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