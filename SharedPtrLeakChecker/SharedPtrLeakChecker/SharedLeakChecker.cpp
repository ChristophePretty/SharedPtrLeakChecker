//
//  SharedLeakChecker.cpp
//  SharedPtrLeakChecker
//
//  Created by Christophe Genova on 07/05/2018.
//  Copyright © 2018 Pretty Simple. All rights reserved.
//

#include <memory>
#include <functional>

#ifdef SHARED_PTR_LEAK_CHECKER

#define ENABLE_SHARED_PTR_LEAK_CHECKER

#define NB_STACK_TO_DUMP 5

_VSTD::shared_ptr_leak_checker &_VSTD::shared_ptr_leak_checker::getInstance()
{
    // Create a leak on purpose as we cannot insure the order of the delete
    static auto _instance = new shared_ptr_leak_checker(
#ifdef ENABLE_SHARED_PTR_LEAK_CHECKER
                                             true
#else
                                             false
#endif
                                             );
    return *_instance;
}

_VSTD::map<_VSTD::string,_VSTD::vector<_VSTD::__shared_ptr_leak_checker_abstract::LeakInfo>> _VSTD::shared_ptr_leak_checker::getLeaks() const
{
    map<string,vector<__shared_ptr_leak_checker_abstract::LeakInfo>> ret;
    for(const auto &item : _checkers)
        if (item.second.checker->enable())
            ret.emplace(item.second.checker->getClassName(), item.second.checker->getLeaks());
    
    return ret;
}

_VSTD::string _VSTD::__shared_ptr_leak_checker_abstract::__shared_ptr_leak_checker_demangleFunc(_VSTD::string v, _VSTD::__shared_ptr_leak_checker_abstract::__shared_ptr_leak_checker_DemangleFunc func)
{
    static __shared_ptr_leak_checker_DemangleFunc _func = [](_VSTD::string a) { return a; };
    if (func)
        _func = func;
    assert(_func);
    return _func(v);
}

_VSTD::vector<_VSTD::string> _VSTD::__shared_ptr_leak_checker_abstract::__shared_ptr_leak_checker_callStackFunc(_VSTD::__shared_ptr_leak_checker_abstract::__shared_ptr_leak_checker_CallStackFunc func)
{
    static __shared_ptr_leak_checker_CallStackFunc _func = []() { return _VSTD::vector<_VSTD::string>{}; };
    if (func)
        _func = func;
    assert(_func);
    auto ret = _func();
    if (ret.size() > NB_STACK_TO_DUMP)
        ret.erase(ret.begin(), ret.begin() + NB_STACK_TO_DUMP);
    return ret;
}

#endif
