//
//  main.cpp
//  SharedPtrLeakChecker
//
//  Created by Christophe Genova on 04/05/2018.
//  Copyright © 2018 Pretty Simple. All rights reserved.
//


//#include <map>
//#include <string>
#include "memory"
#include <iostream>
#include "CallStack.h"

using namespace std;

class B;

class A {
public:
    A()
    {
        cout << "A()" << endl;
    }
    ~A()
    {
        cout << "~A()" << endl;
    }
    shared_ptr<B> refB;
};

class B {
public:
    B()
    {
        cout << "B()" << endl;
    }
    ~B()
    {
        cout << "~B()" << endl;
    }
    shared_ptr<A> refA;
};

int main(int argc, const char *argv[])
{
    shared_ptr_leak_checker::getInstance().setDemagleFunc(prettysimple::callstack::demangle);
    shared_ptr_leak_checker::getInstance().setCallStackFunc(prettysimple::callstack::currentCallStack);
    // test 1 no leak
    auto leaks = shared_ptr_leak_checker::getInstance().getNewLeaks();
    {
        auto a = make_shared<A>();
        leaks = shared_ptr_leak_checker::getInstance().getNewLeaks();
        auto b = make_shared<B>();
        leaks = shared_ptr_leak_checker::getInstance().getNewLeaks();
    }
    leaks = shared_ptr_leak_checker::getInstance().getNewLeaks();
    // test 2 leak!
    {
        auto a = make_shared<A>();
        leaks = shared_ptr_leak_checker::getInstance().getNewLeaks();
        auto b = make_shared<B>();
        a->refB = b;
        b->refA = a;
        leaks = shared_ptr_leak_checker::getInstance().getNewLeaks();
        weak_ptr<A> wa = a;
        leaks = shared_ptr_leak_checker::getInstance().getNewLeaks();
        auto sa = wa.lock();
        leaks = shared_ptr_leak_checker::getInstance().getNewLeaks();
        sa.reset();
        leaks = shared_ptr_leak_checker::getInstance().getNewLeaks();
    }
    leaks = shared_ptr_leak_checker::getInstance().getNewLeaks();
    
    cout << "Leaks " << endl;
    for(const auto &leak : leaks)
        cout << leak.first << " " << leak.second.nbLeaks << endl;
    
    return 0;
}
