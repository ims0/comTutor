#include "../inc/internal.h"
#include "print_lib/inc/inc.h"
#include <iostream>


int main() {
  std::cout << "[" << __FUNCTION__ << "]:" << __LINE__ << std::endl;

  std::cout<< GET_HELLO <<std::endl;
  printFun();
  return 0;
}
