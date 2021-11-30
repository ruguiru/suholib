#pragma once

#include <string>

#include "Singleton.hpp"
#include "Log.h"

using namespace suho::log;

// ���ο� �α� Ÿ���� �̱��� ���·� �� �� �ִ�.
#define BUILD_LOGTYPE(Type, Path, Cycle)                                          \
class Type : public Logger, public suho::pattern::singleton::Singleton<Type>      \
{                                                                                 \
public:                                                                           \
    Type(const std::string& division=#Type, const std::string& path=Path, cycle::FileCreateCycle cycle=Cycle) : Logger(division, path, cycle)   \
    {                                                                                                                       \
    }                                                                                                                       \
    ~Type() override                                                                                                        \
    {\
    }\
};