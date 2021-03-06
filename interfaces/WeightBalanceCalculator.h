#pragma once

#include <string>

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

class WeightBalanceCalculator {
public:

    enum BalanceStatus {
        APPROVED, X_IMBALANCED, Y_IMBALANCED, X_Y_IMBALANCED
    };

// EDIT, 05-05-2020: the method below is required in ex2
// but you may implement it just as: return 0;
    int readShipPlan(const std::string& full_path_and_file_name);

    BalanceStatus tryOperation(char loadUnload, int kg, int x, int y);
};