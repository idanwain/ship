#include "WeightBalanceCalculator.h"

BalanceStatus WeightBalanceCalculator::tryOperation(char loadUnload, int kg, int X, int Y) {
    std::tuple<bool, bool> x_y_imbalance;
    std::vector<Container> column = (ship->get_map())->at(X).at(Y);
    switch(loadUnload){
        case 'L':
            checkLoad(x_y_imbalance, kg, column);
            break;
        case 'U':
            checkUnload(x_y_imbalance, kg, column);
            break;
        default:
            return FAILED;
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

void WeightBalanceCalculator::checkLoad(std::tuple<bool, bool> &x_y_imbalance, int kg, std::vector<Container> &column) {
    if(kg >= 0 && column.size() >= 0){ //dummy use
        x_y_imbalance = std::pair(false, false);
    }
}

void WeightBalanceCalculator::checkUnload(std::tuple<bool, bool> &x_y_imbalance, int kg, std::vector<Container> &column) {
    if(kg >= 0 && column.size() >= 0){ //dummy use
        x_y_imbalance = std::pair(false, false);
    }
}