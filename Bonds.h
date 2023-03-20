#include <cstdio>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

#ifndef ECONOMICSMODEL_BONDS_H
#define ECONOMICSMODEL_BONDS_H

class Bonds {
public:
    Bonds() = default;

    bool new_bond(int duration_months, float amount, float yearly_rate) {
        if (!is_accepted_duration(duration_months))
            return false;
        std::vector<float> info = {(float) duration_months, amount, yearly_rate, (float) month};
        bond_data.push_back(info);
        return true;
    }

    float sell_bond(int i, float current_rate) {
        if (bond_data[i][1] == 0 && bond_data[i][2] == 0) {
            return 0;
        }
        float value_sold = bond_data[i][2] / current_rate * bond_data[i][1];
        bond_data[i][1] = 0;
        bond_data[i][2] = 0;
        return value_sold;
    }

    std::string list_bonds() {
        std::string ret_str = "\nCurrent bond holdings:\n";
        short i = 0;
        for (auto &item: bond_data) {
            if (item[1] != 0 && item[2] != 0) {
                ret_str +=
                        std::to_string(i) + "> Time to maturity: " + std::to_string((int) (item[3] + item[0] - month)) +
                        " months; Amount: " + std::to_string(item[1]) + "; Yearly Rate: " + std::to_string(item[2]) +
                        "\n";
            }
            i++;
        }
        return ret_str;
    }

    float get_earnings() {
        month++;
        float earnings = 0;
        for (auto &item: bond_data) {
            earnings += item[2] / 12 * item[1];
            if ((float) month == item[3] + item[0]) {
                earnings += item[1];
                item[1] = 0;
                item[2] = 0;
            }
        }
        return earnings;
    }

    std::string accepted_durations() {
        std::string ret_str;
        for (auto &item: acceptedDurations)
            ret_str += std::to_string(item) + " ";
        return ret_str;
    }

    bool is_accepted_duration(int len) {
        return std::count(acceptedDurations.begin(), acceptedDurations.end(), len) != 0;
    }

    bool is_active_bond(int i) {
        if (bond_data[i][1] != 0 && bond_data[i][2] != 0) {
            return true;
        }
        return false;
    }

    std::vector<float> get_bond(int i) {
        return bond_data[i];
    }

    std::vector<int> acceptedDurations = {4, 12, 24, 60, 84, 120, 240, 360};
    std::vector<std::vector<float>> bond_data;
private:
    int month = 0;
};

#endif //ECONOMICSMODEL_BONDS_H
