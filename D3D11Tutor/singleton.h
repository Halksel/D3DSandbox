#pragma once

/*/////////
https://qiita.com/kikuuuty/items/fcf5f7df2f0493c437dc
/*/////////


#include <cassert>
#include <mutex>
#include <iostream>

class SingletonFinalizer {
public:
    using FinalizerFunc = void(*)();
    static void addFinalizer(FinalizerFunc func);
    static void finalize();
};

template <typename T>
class Singleton final {
public:
    static T& get_instance() {
        std::call_once(initFlag, create);
        assert(instance);
        return *instance;
    }

private:
    static void create() {
        instance = new T;
        SingletonFinalizer::addFinalizer(&Singleton<T>::destroy);
    }

    static void destroy() {
        try {
            if (instance != nullptr) {
                delete instance;
            }
        }
        catch (const std::exception& e) {
             std::cout << e.what();
        }
        instance = nullptr;
    }

    static std::once_flag initFlag;
    static T* instance;
};

template <typename T> std::once_flag Singleton<T>::initFlag;
template <typename T> T* Singleton<T>::instance = nullptr;
