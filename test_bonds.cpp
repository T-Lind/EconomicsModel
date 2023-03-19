#include <iostream>
#include <map>
#include "Rand.h"
#include "Bonds.h"

using namespace std;


int main() {
    Bonds bond_info = Bonds();
    bond_info.new_bond(12, 1000, 0.10);
    bond_info.new_bond(4, 500, 0.16);

    float total_earnings = 0;
    for (int m = 0; m <= 12; m++) {
        float new_earnings = bond_info.get_earnings();
        if (m == 8)
            new_earnings += bond_info.sell_bond(0, 0.5);

        cout << "Earnings: " << new_earnings << endl;
        cout << "Bonds held: " << bond_info.list_bonds();
        total_earnings += new_earnings;

    }

    cout << "Total earnings: " << total_earnings << endl;
}