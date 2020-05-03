#include "WeightBalanceCalculator.h"
#include "Parser.h"

BalanceStatus WeightBalanceCalculator::tryOperation(char loadUnload, int kg, int X, int Y) {
    std::tuple<bool, bool> x_y_imbalance;
    std::vector<Container>& column = (this->shipMap).at(X).at(Y);
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
        switch(loadUnload){
            case 'L': {
                std::unique_ptr<Container> con = std::make_unique<Container>(kg);
                column.emplace_back(*con);
                break;
            }
            case 'U': {
                column.pop_back();
                break;
            }
        }
        return APPROVED;
    }
}

void WeightBalanceCalculator::checkLoad(std::tuple<bool, bool> &x_y_imbalance, int kg, std::vector<Container> &column) {
    if(kg >= 0 && !column.empty()){
        x_y_imbalance = std::pair(false, false);
    }
}

void WeightBalanceCalculator::checkUnload(std::tuple<bool, bool> &x_y_imbalance, int kg, std::vector<Container> &column) {
    if(kg >= 0 && !column.empty()){ //dummy use
        x_y_imbalance = std::pair(false, false);
    }
}

int WeightBalanceCalculator::readShipPlan(const std::string &full_path_and_file_name) {
    std::unique_ptr<Ship> ship;
    int val = extractShipPlan(full_path_and_file_name, ship) + extractArgsForBlocks(ship, full_path_and_file_name);
    this->shipMap = std::move(*ship->getMap());
    return val;
}
