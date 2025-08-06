#pragma once

#include <iostream>
#include <any>

#include "util.h"



struct Callbacks
{
    static void ButtonPressed(std::string sButtonName)
    {
        util::Log("Default callback for " + sButtonName);
    }


    static void RealButton(std::string sButtonName)
    {
        util::Log("Custom callback for " + sButtonName);
    }
};


struct IInvoker
{
    virtual ~IInvoker() = default;
};


template <typename Func>
struct Invoker;


template <typename R, typename... Args>
struct Invoker<R(Args...)> : IInvoker
{
    std::function<R(Args...)> func;

    Invoker(std::function<R(Args...)> f) : func(f) {}

    template <typename... BoundArgs>
    std::function<void()> bind(BoundArgs&&... boundArgs)
    {
        return [f = func, args = std::make_tuple(std::forward<BoundArgs>(boundArgs)...)]()
        {
            std::apply(f, args);
        };
    }
};
