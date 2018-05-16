//
//  CallStack.h
//  NewYork
//
//  Created by Christophe Genova on 30/01/2018.
//  Copyright © 2018 Pretty Simple. All rights reserved.
//

#pragma once

#include <functional>
#include <map>
#include <vector>
#include <set>
#include <string>

namespace prettysimple {
    namespace callstack {
        std::vector<std::string> currentCallStack();
        std::string demangle(std::string call);
        
        std::set<std::map<std::string,int>> test();
    }
}
