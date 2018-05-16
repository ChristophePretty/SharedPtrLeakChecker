//
//  test.hpp
//  SharedPtrLeakChecker
//
//  Created by Christophe Genova on 09/05/2018.
//  Copyright © 2018 Pretty Simple. All rights reserved.
//

#pragma once
#include <memory>
#include <iostream>

class testB;

class testA {
public:
    testA()
    {
        std::cout << "testA()" << std::endl;
    }
    ~testA()
    {
        std::cout << "~testA()" << std::endl;
    }
    std::shared_ptr<testB> refB;
    
    void test();
};

class testB {
public:
    testB()
    {
        std::cout << "testB()" << std::endl;
    }
    ~testB()
    {
        std::cout << "~testB()" << std::endl;
    }
    std::shared_ptr<testA> refA;
};
