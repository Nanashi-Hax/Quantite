#pragma once

#include <QObject>
#include "Quantite.hpp"
#include "BreakInfoModel.hpp"

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
    void onDataBreakReceived(RegisterInfo info);
    void onInstructionBreakReceived(RegisterInfo info);

signals:
    void dataBreakReceived(RegisterInfo info);
    void instructionBreakReceived(RegisterInfo info);

private:
    Quantite Quantite;
};
