//
//  CallStack.m
//  NewYork
//
//  Created by Christophe Genova on 30/01/2018.
//  Copyright © 2018 Pretty Simple. All rights reserved.
//

#include "memory"
#include "CallStack.h"
#ifdef __APPLE__
#include <execinfo.h>   // For backtrace on IOS
#endif
#ifdef __ANDROID__
#include <unwind.h>    // For backtrace on Android
#include <dlfcn.h>
#endif
#include <cxxabi.h>     // For demagling

using namespace std;
using namespace prettysimple;

static constexpr size_t MAX_CALLSTACK_SIZE  = 32;
static constexpr size_t MAX_FUNC_SIZE       = 4096;

#ifdef __APPLE__
string __demangle_ios(string call);
#endif

#ifdef __ANDROID__
struct android_backtrace_state
{
    void **current;
    void **end;
};

_Unwind_Reason_Code __android_unwind_callback(struct _Unwind_Context* context, void* arg)
{
    android_backtrace_state* state = (android_backtrace_state *)arg;
    uintptr_t pc = _Unwind_GetIP(context);
    if (pc) {
        if (state->current == state->end)
            return _URC_END_OF_STACK;
        else
            *state->current++ = reinterpret_cast<void*>(pc);
    }
    return _URC_NO_REASON;
}
#endif

vector<string> callstack::currentCallStack()
{
    vector<string> stack;
    void* callstack[MAX_CALLSTACK_SIZE];

#ifdef __APPLE__
    int i, frames = backtrace(callstack, MAX_CALLSTACK_SIZE);
    char** strs = backtrace_symbols(callstack, frames);
    for (i = 0; i < frames; ++i)
        stack.push_back(__demangle_ios(strs[i]));
    free(strs);
#endif
#ifdef __ANDROID__
    android_backtrace_state state;
    state.current = callstack;
    state.end = callstack + MAX_CALLSTACK_SIZE;

    _Unwind_Backtrace(__android_unwind_callback, &state);

    int count = (int)(state.current - callstack);

    for (int idx = 0; idx < count; idx++) {
        const void* addr = callstack[idx];
        const char* symbol = "";

        Dl_info info;
        if (dladdr(addr, &info) && info.dli_sname)
            symbol = info.dli_sname;
        else
            break;

        stack.push_back(to_string(idx) + " " +  demangle(string(symbol)));
    }
#endif
    return stack;
}

#ifdef __APPLE__
string __demangle_ios(string call)
{
    auto mangled    = call;
    auto endName    = mangled.find_last_of(" + ");

    if (endName != string::npos) {
        mangled = call.substr(0, endName - 2);
        auto beginName = mangled.find_last_of(" ");
        if (beginName != string::npos) {
            mangled = mangled.substr(beginName+1, mangled.size()-1);
            call = call.substr(0, beginName + 1) + callstack::demangle(mangled) + call.substr(endName - 2, call.size() -1);
            }
        }
    return call;
}
#endif

string callstack::demangle(string call)
{
    size_t funcnamesize = MAX_FUNC_SIZE;
    char funcname[MAX_FUNC_SIZE];
    int status;
    char* ret = abi::__cxa_demangle(call.c_str(), funcname, &funcnamesize, &status);

    if (status == 0)
        call = string(ret);
    return call;
}
