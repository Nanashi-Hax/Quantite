#pragma once

#include <QWidget>
#include "Quantite.hpp"
#include "DataBreakInfoModel.hpp"
#include "InstructionBreakInfoModel.hpp"

namespace Ui
{
    class BreakpointWidget;
}

class BreakpointWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BreakpointWidget(QWidget *parent = nullptr);
    ~BreakpointWidget();

public slots:
    void onSetDataBreakpoint();
    void onDataBreakpointClear();
    void onDataBreakReceived(uint32_t dAddress, uint32_t iAddress);

    void onSetInstructionBreakpoint();
    void onInstructionBreakpointClear();
    void onInstructionBreakReceived(uint32_t iAddress);

signals:
    void requestSetDataBreakpoint(uint32_t address, bool read, bool write, Quantite::BreakpointSize size);
    void requestUnsetDataBreakpoint();

    void requestSetInstructionBreakpoint(uint32_t address);
    void requestUnsetInstructionBreakpoint();

private:
    Ui::BreakpointWidget *ui;
    DataBreakInfoModel dataBreakInfoModel;
    InstructionBreakInfoModel instructionBreakInfoModel;
};
