#ifndef SHIP1_WEIGHTBALANCECALCULATOR_H
#define SHIP1_WEIGHTBALANCECALCULATOR_H

/**
 * The calculator shall be initiated by the simulation with the ship plan file:
  readShipPlan(const std::string& full_path_and_file_name)
It shall preserve the state of all loaded cargo (thus, requiring a calculator instance per ship).
It shall have the following methods:
  BalanceStatus  tryOperation(char loadUnload, int kg, int X, int Y);
  The method gets:
      char: L or U for Load or Unload
      kg weight
      X and Y position for the load / unload - the calculator knows the floor to load to (lowest possible) or to unload from (upper available container)
  The method returns an enum value that can be one of:
      APPROVED - operation approved (and registered as done by the balance calculator!)
      X_IMBALANCED - operation declined - need a better balance on the X coordinate
      Y_IMBALANCED - operation declined - need a better balance on the Y coordinate
      X_Y_IMBALANCED - operation declined - need a better balance on both X and Y
  Note:
      The balance calculator doesn’t have a “move” operation,
      as for balance purposes “move” is actually an “unload” followed by “load”.
      First the “unload” has to be approved, then the “load”.
      */
class Ship;

#include "Ship.h"
#include <tuple>
#include "Container.h"


enum BalanceStatus {APPROVED, X_IMBALANCED, Y_IMBALANCED, X_Y_IMBALANCED, FAILED};

class WeightBalanceCalculator {
    std::vector<std::vector<std::vector<Container>>> shipMap;
public:
    int readShipPlan(const std::string& full_path_and_file_name);
    BalanceStatus  tryOperation(char loadUnload, int kg, int X, int Y);
    void checkLoad(std::tuple<bool, bool> &x_y_imbalance, int kg, std::vector<Container> &column);
    void checkUnload(std::tuple<bool, bool> &x_y_imbalance, int kg, std::vector<Container> &column);
};

#endif
