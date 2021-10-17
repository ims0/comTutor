#include "add_lib/inc/add.h"
#include "print_lib/inc/inc.h"
#include <iostream>

void printFun() {
  std::cout << __func__ << "add result:" << add(1, 2)
            << std::endl;
}
