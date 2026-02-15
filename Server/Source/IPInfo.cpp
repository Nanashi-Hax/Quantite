#include <nn/ac.h>

#include <stdexcept>
#include <string>

#include "IPInfo.hpp"

IPInfo::IPInfo()
{
    auto result = nn::ac::Initialize();
    if(!result.IsSuccess()) throw std::runtime_error("Failed to nn::ac::Initialize()");
    result = nn::ac::Connect();
    if(!result.IsSuccess()) throw std::runtime_error("Failed to nn::ac::Connect()");
    result = nn::ac::GetAssignedAddress(&_address);
    if(!result.IsSuccess()) throw std::runtime_error("Failed to nn::ac::GetAssignedAddress()");
}

IPInfo::~IPInfo()
{
    nn::ac::Finalize();
}

std::string IPInfo::address()
{
    char buffer[INET_ADDRSTRLEN];

    in_addr addr;
    addr.s_addr = _address;

    inet_ntop(AF_INET, &addr, buffer, sizeof(buffer));
    return std::string(buffer);
}