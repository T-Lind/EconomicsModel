
#ifndef ECONOMICSMODEL_LOAN_H
#define ECONOMICSMODEL_LOAN_H
#include "Institutions.h"
#include "Rand.h"
#define loan_rate_func(a, b, c) (std::max((std::min(1-a/1000000, 1.f)-std::min(1-a/1000000000, 1.f)), 0.f)/c)

class Loan {
public:
    Loan(int amount, int duration_months, float yearly_interest){
        this->amount = amount;
        this->yearly_interest = yearly_interest;
        monthly_payment = amount / duration_months + amount * yearly_interest / 12;
        duration_months_left = duration_months;
    }

    float get_payment(){
        duration_months_left--;
        if(duration_months_left == 0){
            monthly_payment = 0;
        }
        return monthly_payment;
    }

    int amount;
    int duration_months_left;
    float yearly_interest;
    float monthly_payment;
};

class LoanGen {
public:
    LoanGen(BankInfo bank){
        this->bank = &bank;
    }
//    std::string
private:
    BankInfo* bank;
    std::vector<Loan> available_loans;
};

#endif //ECONOMICSMODEL_LOAN_H
