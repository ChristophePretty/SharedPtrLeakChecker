//
//  main.cpp
//  SharedPtrLeakChecker
//
//  Created by Christophe Genova on 04/05/2018.
//  Copyright © 2018 Pretty Simple. All rights reserved.
//

#include <memory>
#include <iostream>
#include "CallStack.h"
#include "test.hpp"

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
    shared_ptr_leak_checker::getInstance().disable<B>();
    shared_ptr_leak_checker::getInstance().disable<testA>();
    shared_ptr_leak_checker::getInstance().disable<testB>();
    
    // test 1 no leak
    auto leaks = shared_ptr_leak_checker::getInstance().getLeaks();
   {
        auto a = make_shared<A>();
       leaks = shared_ptr_leak_checker::getInstance().getLeaks();
       auto b = make_shared<B>();
       leaks = shared_ptr_leak_checker::getInstance().getLeaks();
   }
   leaks = shared_ptr_leak_checker::getInstance().getLeaks();
   // test 2 leak!
    {
        auto a = make_shared<A>();
       leaks = shared_ptr_leak_checker::getInstance().getLeaks();
       auto b  = make_shared<B>();
        a->refB = b;
        b->refA = a;
       leaks = shared_ptr_leak_checker::getInstance().getLeaks();
       weak_ptr<A> wa = a;
       leaks = shared_ptr_leak_checker::getInstance().getLeaks();
       auto sa = wa.lock();
       leaks = shared_ptr_leak_checker::getInstance().getLeaks();
       sa.reset();
       leaks = shared_ptr_leak_checker::getInstance().getLeaks();
   }
    auto testa = make_unique<testA>();
    testa->test();
   leaks = shared_ptr_leak_checker::getInstance().getLeaks();
   testa.reset();

   leaks = shared_ptr_leak_checker::getInstance().getLeaks();

    cout << "Leaks " << endl;
    for (const auto &leak : leaks) {
        cout << leak.first << " " << leak.second.size() << endl;
        for(const auto &leakInfo : leak.second) {
            cout << "    Use count: " << leakInfo.count << endl;
            if (leakInfo.sharedIncr.size() > 0) {
                cout << "    Stack add: ";
                auto i = 1;
                for(const auto &stacks : leakInfo.sharedIncr) {
                    cout << endl << "    "  << i++ << endl;
                    for(const auto &stack : stacks)
                        cout << "      " << stack << endl;
                }
                cout << endl;
            }
            if (leakInfo.sharedRelease.size() > 0) {
                cout << "    Stack release: ";
                auto i = 1;
                for(const auto &stacks : leakInfo.sharedRelease) {
                    cout << endl << "    "  << i++ << endl;;
                    for(const auto &stack : stacks)
                        cout << "      " << stack << endl;
                }
                cout << endl;
            }
        }
    }
   return 0;
}
