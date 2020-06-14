#include "../interfaces/WeightBalanceCalculator.h"
#include "Parser.h"

WeightBalanceCalculator::BalanceStatus WeightBalanceCalculator::tryOperation(char loadUnload, int kg, int X, int Y) {
    std::tuple<bool, bool> x_y_imbalance;
    switch(loadUnload){
        case 'L':
            if(kg > 0 && X >= 0 && Y >= 0){
                return APPROVED;
            }
            break;
        case 'U':
            return APPROVED;
    }

    if(std::get<0>(x_y_imbalance) && std::get<1>(x_y_imbalance)){
        return X_Y_IMBALANCED;
    }
    else if(std::get<0>(x_y_imbalance) && !std::get<1>(x_y_imbalance)){
        return X_IMBALANCED;
    } else if(!std::get<0>(x_y_imbalance) && std::get<1>(x_y_imbalance)){
        return Y_IMBALANCED;
    } else {
        return APPROVED;
    }
}

int WeightBalanceCalculator::readShipPlan(const std::string &full_path_and_file_name) {
    if(full_path_and_file_name.empty()){
        return -1;
    }
    return 0;
}
