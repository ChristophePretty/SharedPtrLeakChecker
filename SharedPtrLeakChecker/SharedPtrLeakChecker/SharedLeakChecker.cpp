//
//  SharedLeakChecker.cpp
//  SharedPtrLeakChecker
//
//  Created by Christophe Genova on 07/05/2018.
//  Copyright © 2018 Pretty Simple. All rights reserved.
//

#include <memory>

using namespace std;

shared_ptr_leak_checker &shared_ptr_leak_checker::getInstance()
{
    static shared_ptr_leak_checker _instance;
    return _instance;
}

map<string,__shared_ptr_leak_checker_abstract::LeakData> shared_ptr_leak_checker::getNewLeaks(bool filterEmpty) const
{
    map<string,__shared_ptr_leak_checker_abstract::LeakData> ret;
    for(const auto &item : _checkers) {
        auto leaks = item.second.checker->getLeaks();
        if (leaks.nbLeaks > 0 || !filterEmpty)
            ret[item.second.className] = leaks;
    }
    return ret;
}
