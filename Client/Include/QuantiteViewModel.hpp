#pragma once

#include <QObject>
#include "Quantite.hpp"

class QuantiteViewModel : public QObject
{
    Q_OBJECT

public slots:
    void onStart();
    void onStop();

    void onConnectServer(const QString& ipAddress);
    void onDisconnectServer();

    void onSetDataBreakpoint(uint32_t address, bool read, bool write, Quantite::BreakpointSize size);
    void onUnsetDataBreakpoint();

    void onSetInstructionBreakpoint(uint32_t address);
    void onUnsetInstructionBreakpoint();

public:
    void onDataBreakReceived(uint32_t dAddress, uint32_t iAddress);
    void onInstructionBreakReceived(uint32_t iAddress);

signals:
    void dataBreakReceived(uint32_t dAddress, uint32_t iAddress);
    void instructionBreakReceived(uint32_t iAddress);

private:
    Quantite Quantite;
};
