#pragma once

#include "IPInfo.hpp"
#include <IO/Transporter.hpp>
#include <thread>
#include <stop_token>

#include <coreinit/context.h>

#include <Network.hpp>
#include <IO.hpp>

class Quantite
{
public:
    enum class BreakpointSize : uint32_t
    {
        Invalid = 0,
        Bit8 = 1,
        Bit16 = 2,
        Bit32 = 4,
        Bit64 = 8
    };

    Quantite(IPInfo * info);
    ~Quantite();

    void AcceptLoop(std::stop_token token);
    void ProcessLoop(std::stop_token token);
    void BreakLoop(std::stop_token token);

    void AcceptClient();
    void DisconnectClient();

    static void ProcessCommand(Library::IO::Stream& stream);

private:
    std::unique_ptr<std::jthread> acceptThread;
    std::unique_ptr<std::jthread> processThread;
    std::unique_ptr<std::jthread> breakThread;

    std::unique_ptr<Library::Network::TcpSocket> acceptor;
    std::unique_ptr<Library::Network::TcpSocket> client;
    std::mutex clientMutex;

    std::unique_ptr<Library::IO::Transporter> transporter;

    static constexpr const uint16_t Quantite_PORT = 60000;

    enum class Command : uint32_t;
    enum class Data : uint32_t;
};