#ifndef ECONOMICSMODEL_LOAN_H
#define ECONOMICSMODEL_LOAN_H

#include "Rand.h"
#include <string>

#define loan_rate_func(a, b, c) (0.2*(std::min(1-a/1000000, 1.0)))

class Loan {
public:
    Loan(double amount, int duration_months, double yearly_interest) {
        this->amount = amount;
        this->yearly_interest = yearly_interest;
        monthly_payment = (double) amount / (double) duration_months + (double) amount * yearly_interest / 12;
        duration_months_left = duration_months;
        total_months = duration_months;
    }

    double get_payment() {
        duration_months_left--;
        if (duration_months_left == 0) {
            monthly_payment = 0;
        }
        return monthly_payment;
    }

    [[nodiscard]] std::string loan_info() const {
        return std::to_string(total_months) + " month loan at " + std::to_string(yearly_interest * 100) + "% interest for $"+std::to_string(monthly_payment)+" per month.";
    }

    [[nodiscard]] std::string bought_info() const {
        return loan_info() + "; " + std::to_string(duration_months_left) + " months left";
    }

    double amount;
    int duration_months_left;
    int total_months;
    double yearly_interest;
    double monthly_payment;
};

class LoanGen {
public:
    void
    update_loans(double amount_wanted, double excess_reserves, double demand_deposits, double demand_deposits_growth_rate) {
        available_loans.clear();
        for (int i = 0; i < 5; i++) {
            available_loans.emplace_back(amount_wanted, (int) random(6, 12 * 10),
                                         loan_rate_func(excess_reserves, demand_deposits, demand_deposits_growth_rate) -
                                         normal_random(0.1, 0.02));
        }
    }

    std::string list_loans() {
        std::string ret_str;
        int i = 0;
        for (auto &item: available_loans) {
            if (item.monthly_payment != 0)
                ret_str += std::to_string(i) + "> " + item.loan_info() + "\n";
            i++;
        }
        return ret_str;
    }

    std::vector<Loan> available_loans;
};

#endif
