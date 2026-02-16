#include "Quantite.hpp"
#include "IPInfo.hpp"
#include "notifications/notifications.h"
#include <Debug.hpp>
#include <IO.hpp>

#include <chrono>
#include <coreinit/exception.h>
#include <coreinit/debug.h>
#include <memory>
#include <mutex>
#include <thread>
#include <whb/log.h>

#include <Logger.hpp>
#include <Debug.hpp>

#include <stop_token>

using namespace Library::IO;

enum class Quantite::Command : uint32_t
{
    SetDataBreakpoint = 0x0,
    SetInstructionBreakpoint = 0x1
};

enum class Quantite::Data : uint32_t
{
    DataBreakInfo = 0x0,
    InstructionBreakInfo = 0x1
};

Quantite::Quantite(IPInfo * info)
{
    acceptor = std::make_unique<Library::Network::TcpSocket>();
    acceptor->listen(Quantite_PORT);

    acceptThread = std::make_unique<std::jthread>([this](std::stop_token token){ AcceptLoop(token); });
    processThread = std::make_unique<std::jthread>([this](std::stop_token token){ ProcessLoop(token); });
    breakThread = std::make_unique<std::jthread>([this](std::stop_token token){ BreakLoop(token); });

    std::string ip = "Unknown";
    if(info) ip = info->address();
    std::string msg = std::format("Quantite server started on ip: {0}, port: {1}", ip, Quantite_PORT).c_str();
    NotificationModule_AddInfoNotification(msg.c_str());
}

Quantite::~Quantite()
{
    acceptThread->request_stop();
    processThread->request_stop();
    breakThread->request_stop();

    if(acceptor)
    {
        acceptor->shutdown();
        acceptor.reset();
    }

    if(client)
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        
        client->shutdown();
        client.reset();
    }

    processThread.reset();
    acceptThread.reset();
    breakThread.reset();
}

void Quantite::AcceptLoop(std::stop_token token)
{
    while(!token.stop_requested())
    {
        try
        {
            AcceptClient();
        }
        catch(std::exception& e)
        {
            NotificationModule_AddInfoNotification(std::format("Quantite::AcceptLoop(): {}", e.what()).c_str());
            DisconnectClient();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Quantite::ProcessLoop(std::stop_token token)
{
    while(!token.stop_requested())
    {
        if(transporter)
        {
            try
            {
                transporter->poll();
                while(true)
                {
                    Packet packet;
                    if(!transporter->read(packet)) break;
                    BufferStream stream = BufferStream::fromPacket(packet);
                    ProcessCommand(stream);
                }
            }
            catch(std::exception& e)
            {
                NotificationModule_AddInfoNotification(std::format("Quantite::ProcessLoop(): {}", e.what()).c_str());
                DisconnectClient();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Quantite::BreakLoop(std::stop_token token)
{
    while(!token.stop_requested())
    {
        try
        {
            auto dInfo = Library::Debug::ConsumeDataBreakInfo();
            auto iInfo = Library::Debug::ConsumeInstructionBreakInfo();
            if(transporter)
            {
                {
                    uint32_t id = static_cast<uint32_t>(Data::DataBreakInfo);
                    uint32_t count = dInfo.size();
                    if(count > 0)
                    {
                        BufferStream stream;
                        stream << id << count;
                        for(auto& it : dInfo)
                        {
                            uint32_t dAddress = it.dar;
                            uint32_t iAddress = it.pc;
                            stream << dAddress << iAddress;
                        }
                        transporter->write(BufferStream::toPacket(stream));
                    }
                }
                {
                    uint32_t id = static_cast<uint32_t>(Data::InstructionBreakInfo);
                    uint32_t count = iInfo.size();
                    if(count > 0)
                    {
                        BufferStream stream;
                        stream << id << count;
                        for(auto& it : iInfo)
                        {
                            uint32_t iAddress = it.pc;
                            stream << iAddress;
                        }
                        transporter->write(BufferStream::toPacket(stream));
                    }
                }
            }
        }
        catch(std::exception& e)
        {
            NotificationModule_AddInfoNotification(std::format("Quantite::BreakLoop(): {}", e.what()).c_str());
            DisconnectClient();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Quantite::AcceptClient()
{
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        if(client) return;
    }

    auto s = acceptor->accept();
    if(!s)
    {
        NotificationModule_AddInfoNotification("Failed to accept client");
        return;
    }

    {
        std::lock_guard<std::mutex> lock(clientMutex);
        client = std::make_unique<Library::Network::TcpSocket>(std::move(*s));
        transporter = std::make_unique<Library::IO::Transporter>(*client);
    }
    NotificationModule_AddInfoNotification("Client connected");
}

void Quantite::DisconnectClient()
{
    if(client)
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        transporter.reset();
        client->shutdown();
        client.reset();
        NotificationModule_AddInfoNotification("Disconnected");
    }
}

void Quantite::ProcessCommand(Stream& stream)
{
    Command command;
    {
        uint32_t id;
        stream >> id;
        command = static_cast<Command>(id);
        std::string str = std::format("Command: 0x{:08X}", id);
        NotificationModule_AddInfoNotification(str.c_str());
    }
    
    switch (command)
    {
        case Command::SetDataBreakpoint:
        {
            using BreakpointSize = Library::Debug::BreakpointSize;
            uint32_t enable;
            uint32_t address;
            uint32_t read;
            uint32_t write;
            uint32_t size;
            stream >> enable >> address >> read >> write >> size;
        
            BreakpointSize s = static_cast<BreakpointSize>(size);
            switch(s)
            {
                case BreakpointSize::Bit8:  break;
                case BreakpointSize::Bit16: break;
                case BreakpointSize::Bit32: break;
                case BreakpointSize::Bit64: break;
                default: s = BreakpointSize::Invalid; break;
            }
        
            if(enable)
            {
                if(s == BreakpointSize::Invalid)
                {
                    NotificationModule_AddInfoNotification("Invalid breakpoint size");
                    break;
                }
            
                NotificationModule_AddInfoNotification(std::format("Set data breakpoint at 0x{:08X}, Read: {}, Write: {}", address, read ? "True" : "False", write ? "True" : "False").c_str());
                Library::Debug::SetDataBreakpoint(address, read, write, s);
            }
            else
            {
                NotificationModule_AddInfoNotification("Unset data breakpoint");
                Library::Debug::UnsetDataBreakpoint();
            }
            break;
        }
        case Command::SetInstructionBreakpoint:
        {
            uint32_t enable;
            uint32_t address;
            stream >> enable >> address;
        
            if(enable)
            {        
                NotificationModule_AddInfoNotification(std::format("Set instruction breakpoint at 0x{:08X}", address).c_str());
                Library::Debug::SetInstructionBreakpoint(address);
            }
            else
            {
                NotificationModule_AddInfoNotification("Unset instruction breakpoint");
                Library::Debug::UnsetInstructionBreakpoint();
            }
            break;
        }
        default:
        {
            break;
        }
    }
}