#include <iostream>
#include "AlgorithmRegistration.h"

AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()>) {
//    std::cout << "called the alg reg " << typeid(this).name() << std::endl;
}
