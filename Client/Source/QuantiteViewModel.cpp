#include "QuantiteViewModel.hpp"
#include <QDebug>

void QuantiteViewModel::onStart()
{
    Quantite.start();
    Quantite.setDataBreakInfoCallback([this](RegisterInfo info){ onDataBreakReceived(info); });
    Quantite.setInstructionBreakInfoCallback([this](RegisterInfo info){ onInstructionBreakReceived(info); });
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

void QuantiteViewModel::onDataBreakReceived(RegisterInfo info)
{
    emit dataBreakReceived(info);
}

void QuantiteViewModel::onInstructionBreakReceived(RegisterInfo info)
{
    emit instructionBreakReceived(info);
}
