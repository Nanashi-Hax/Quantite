#pragma once

#include <QWidget>
#include "Quantite.hpp"
#include "BreakInfoModel.hpp"

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
    void onDataBreakReceived(RegisterInfo info);
    void onSetDataPrimaryKey();

    void onSetInstructionBreakpoint();
    void onInstructionBreakpointClear();
    void onInstructionBreakReceived(RegisterInfo info);
    void onSetInstructionPrimaryKey();

signals:
    void requestSetDataBreakpoint(uint32_t address, bool read, bool write, Quantite::BreakpointSize size);
    void requestUnsetDataBreakpoint();

    void requestSetInstructionBreakpoint(uint32_t address);
    void requestUnsetInstructionBreakpoint();

private:
    Ui::BreakpointWidget *ui;
    BreakInfoModel dataBreakInfoModel;
    BreakInfoModel instructionBreakInfoModel;
};
