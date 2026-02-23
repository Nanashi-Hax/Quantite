#include "MainWindow.hpp"
#include "ConnectionWidget.hpp"
#include "BreakpointWidget.hpp"
#include "ui_MainWindow.h"

#include <QThread>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), quantite()
{
    ui->setupUi(this);

    // Quantiteイベント登録
    connect(this, &MainWindow::requestStart, &quantite, &QuantiteViewModel::onStart);
    connect(this, &MainWindow::requestStop, &quantite, &QuantiteViewModel::onStop);
    connect(ui->widgetConnection, &ConnectionWidget::requestConnectServer, &quantite, &QuantiteViewModel::onConnectServer);
    connect(ui->widgetConnection, &ConnectionWidget::requestDisconnectServer, &quantite, &QuantiteViewModel::onDisconnectServer);
    connect(ui->widgetBreakpoint, &BreakpointWidget::requestSetDataBreakpoint, &quantite, &QuantiteViewModel::onSetDataBreakpoint);
    connect(ui->widgetBreakpoint, &BreakpointWidget::requestUnsetDataBreakpoint, &quantite, &QuantiteViewModel::onUnsetDataBreakpoint);
    connect(ui->widgetBreakpoint, &BreakpointWidget::requestSetInstructionBreakpoint, &quantite, &QuantiteViewModel::onSetInstructionBreakpoint);
    connect(ui->widgetBreakpoint, &BreakpointWidget::requestUnsetInstructionBreakpoint, &quantite, &QuantiteViewModel::onUnsetInstructionBreakpoint);

    // Quantiteコールバック登録
    connect(&quantite, &QuantiteViewModel::dataBreakReceived, ui->widgetBreakpoint, &BreakpointWidget::onDataBreakReceived);
    connect(&quantite, &QuantiteViewModel::instructionBreakReceived, ui->widgetBreakpoint, &BreakpointWidget::onInstructionBreakReceived);

    emit requestStart();
}

MainWindow::~MainWindow()
{
    emit requestStop();
    delete ui;
}

