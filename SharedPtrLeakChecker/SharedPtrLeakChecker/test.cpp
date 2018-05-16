//
//  test.cpp
//  SharedPtrLeakChecker
//
//  Created by Christophe Genova on 09/05/2018.
//  Copyright © 2018 Pretty Simple. All rights reserved.
//

#include "test.hpp"


void testA::test() {
    auto A = std::make_shared<testA>();
    auto A1 = std::make_shared<testA>();
    auto B = std::make_shared<testB>();
    A->refB = B;
    B->refA = A;
    auto C = new testB();
    C->refA = A1;
}
