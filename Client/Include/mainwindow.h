#pragma once

#include <QMainWindow>
#include "QuantiteViewModel.hpp"
#include "DataBreakInfoModel.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onConnect();
    void onDisconnect();
    void onSetDataBreakpoint();
    void onDataBreakpointClear();
    void onDataBreakReceived(uint32_t dAddress, uint32_t iAddress);

signals:
    void requestStart();
    void requestStop();

    void requestConnectServer(const QString& ipAddress);
    void requestDisconnectServer();

    void requestSetDataBreakpoint(uint32_t address, bool read, bool write, Quantite::BreakpointSize size);
    void requestUnsetDataBreakpoint();

private:
    Ui::MainWindow *ui;
    QuantiteViewModel Quantite;
    DataBreakInfoModel dataBreakInfoModel;
};

