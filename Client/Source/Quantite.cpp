#include "Quantite.hpp"
#include <IO.hpp>
#include <Network.hpp>
#include <QDebug>

using namespace Library::IO;
using namespace Library::Network;

enum class Quantite::Command : uint32_t
{
    MemoryWrite32 = 0x0,
    SetDataBreakpoint = 0x1
};

enum class Quantite::Data : uint32_t
{
    DataBreakInfo = 0x0,
};

Quantite::Quantite()
{
    Library::Network::Initialize();
}

Quantite::~Quantite()
{
    Library::Network::Shutdown();
}

void Quantite::start()
{
    processThread = std::make_unique<std::jthread>([this](std::stop_token token){ processLoop(token); });
}

void Quantite::stop()
{
    processThread->request_stop();
    processThread.reset();
}

void Quantite::connectServer(const std::string& ipAddress)
{
    std::lock_guard<std::mutex> lock(socketMutex);
    if(!socket)
    {
        socket = std::make_unique<TcpSocket>();
        socket->connect(ipAddress, Quantite_PORT);
        transporter = std::make_unique<Transporter>(*socket);
        qDebug() << "Connected";
    }
}

void Quantite::disconnectServer()
{
    std::lock_guard<std::mutex> lock(socketMutex);
    if(socket)
    {
        transporter.reset();
        socket->shutdown();
        socket.reset();
        qDebug() << "Disconnected";
    }
}

void Quantite::setDataBreakpoint(uint32_t address, bool read, bool write, BreakpointSize size)
{
    if(!transporter) return;
    try
    {
        BufferStream stream;
        uint32_t id = static_cast<uint32_t>(Command::SetDataBreakpoint);
        uint32_t enable = static_cast<uint32_t>(true);
        uint32_t r = static_cast<uint32_t>(read);
        uint32_t w = static_cast<uint32_t>(write);
        uint32_t s = static_cast<uint32_t>(size);
        stream << id << enable << address << r << w << s;
        transporter->write(BufferStream::toPacket(stream));
    }
    catch(std::exception& e)
    {
        qDebug() << e.what();
        disconnectServer();
    }
}

void Quantite::unsetDataBreakpoint()
{
    if(!transporter) return;
    try
    {
        BufferStream stream;
        uint32_t id = static_cast<uint32_t>(Command::SetDataBreakpoint);
        uint32_t enable = 0;
        uint32_t address = 0;
        uint32_t r = 0;
        uint32_t w = 0;
        uint32_t s = static_cast<uint32_t>(BreakpointSize::Invalid);
        stream << id << enable << address << r << w << s;
        transporter->write(BufferStream::toPacket(stream));
    }
    catch(std::exception& e)
    {
        qDebug() << e.what();
        disconnectServer();
    }
}

void Quantite::setDataBreakInfoCallback(DataBreakInfoCallbackFunction function)
{
    dataBreakInfoCallback = function;
}

void Quantite::processLoop(std::stop_token token)
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
                    processData(stream);
                }
            }
            catch(std::exception& e)
            {
                qDebug() << e.what();
                disconnectServer();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Quantite::processData(Stream& stream)
{
    Data data;
    {
        uint32_t id;
        stream >> id;
        data = static_cast<Data>(id);
    }

    switch(data)
    {
        case Data::DataBreakInfo:
        {
            uint32_t count = 0;
            stream >> count;
            for(uint32_t i = 0; i < count; i++)
            {
                uint32_t dataAddress = 0;
                uint32_t instructionAddress = 0;
                stream >> dataAddress >> instructionAddress;
                if(dataBreakInfoCallback) dataBreakInfoCallback(dataAddress, instructionAddress);
            }
            break;
        }
    }
}
