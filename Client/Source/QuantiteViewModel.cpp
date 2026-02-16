#include "QuantiteViewModel.hpp"
#include <QDebug>

void QuantiteViewModel::onStart()
{
    Quantite.start();
    Quantite.setDataBreakInfoCallback([this](uint32_t d, uint32_t i){ onDataBreakReceived(d, i); });
    Quantite.setInstructionBreakInfoCallback([this](uint32_t i){ onInstructionBreakReceived(i); });
}

void QuantiteViewModel::onStop()
{
    Quantite.stop();
}

void QuantiteViewModel::onConnectServer(const QString& ipAddress)
{
    Quantite.connectServer(ipAddress.toStdString());
}

void QuantiteViewModel::onDisconnectServer()
{
    Quantite.disconnectServer();
}

void QuantiteViewModel::onSetDataBreakpoint(uint32_t address, bool read, bool write, Quantite::BreakpointSize size)
{
    Quantite.setDataBreakpoint(address, read, write, size);
}

void QuantiteViewModel::onUnsetDataBreakpoint()
{
    Quantite.unsetDataBreakpoint();
}

void QuantiteViewModel::onSetInstructionBreakpoint(uint32_t address)
{
    Quantite.setInstructionBreakpoint(address);
}

void QuantiteViewModel::onUnsetInstructionBreakpoint()
{
    Quantite.unsetInstructionBreakpoint();
}

void QuantiteViewModel::onDataBreakReceived(uint32_t dAddress, uint32_t iAddress)
{
    emit dataBreakReceived(dAddress, iAddress);
}

void QuantiteViewModel::onInstructionBreakReceived(uint32_t iAddress)
{
    emit instructionBreakReceived(iAddress);
}
