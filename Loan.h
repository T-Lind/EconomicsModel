#ifndef ECONOMICSMODEL_LOAN_H
#define ECONOMICSMODEL_LOAN_H

class Loan {
public:
    Loan(int amount, float yearly_rate){
        this->amount = amount;
        this->yearly_rate = yearly_rate;
        monthly_return = amount * yearly_rate / 12;
    }
    int amount;
    float yearly_rate;
    float monthly_return;
};


#endif //ECONOMICSMODEL_LOAN_H
