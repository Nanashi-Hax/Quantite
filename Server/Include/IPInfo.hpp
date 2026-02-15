#pragma once

#include <cstdint>
#include <string>

class IPInfo
{
public:
    IPInfo();
    ~IPInfo();

    std::string address();

private:
    uint32_t _address;
};