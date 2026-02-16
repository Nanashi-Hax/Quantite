#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Quantite.hpp"

#include <QThread>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), Quantite()
{
    ui->setupUi(this);

    // ボタンコールバック登録
    connect(ui->pushButtonConnect, &QPushButton::clicked, this, &MainWindow::onConnect);
    connect(ui->pushButtonDisconnect, &QPushButton::clicked, this, &MainWindow::onDisconnect);
    connect(ui->pushButtonSetDataBreakpoint, &QPushButton::clicked, this, &MainWindow::onSetDataBreakpoint);
    connect(ui->pushButtonDataBreakpointClear, &QPushButton::clicked, this, &MainWindow::onDataBreakpointClear);
    connect(ui->pushButtonSetInstructionBreakpoint, &QPushButton::clicked, this, &MainWindow::onSetInstructionBreakpoint);
    connect(ui->pushButtonInstructionBreakpointClear, &QPushButton::clicked, this, &MainWindow::onInstructionBreakpointClear);

    // Quantiteイベント登録
    connect(this, &MainWindow::requestStart, &Quantite, &QuantiteViewModel::onStart);
    connect(this, &MainWindow::requestStop, &Quantite, &QuantiteViewModel::onStop);
    connect(this, &MainWindow::requestConnectServer, &Quantite, &QuantiteViewModel::onConnectServer);
    connect(this, &MainWindow::requestDisconnectServer, &Quantite, &QuantiteViewModel::onDisconnectServer);
    connect(this, &MainWindow::requestSetDataBreakpoint, &Quantite, &QuantiteViewModel::onSetDataBreakpoint);
    connect(this, &MainWindow::requestUnsetDataBreakpoint, &Quantite, &QuantiteViewModel::onUnsetDataBreakpoint);
    connect(this, &MainWindow::requestSetInstructionBreakpoint, &Quantite, &QuantiteViewModel::onSetInstructionBreakpoint);
    connect(this, &MainWindow::requestUnsetInstructionBreakpoint, &Quantite, &QuantiteViewModel::onUnsetInstructionBreakpoint);

    // Quantiteコールバック登録
    connect(&Quantite, &QuantiteViewModel::dataBreakReceived, this, &MainWindow::onDataBreakReceived);
    connect(&Quantite, &QuantiteViewModel::instructionBreakReceived, this, &MainWindow::onInstructionBreakReceived);

    // コンボボックス登録
    ui->comboBoxDataBreakpointSize->addItem("8-bit",  static_cast<uint32_t>(Quantite::BreakpointSize::Bit8));
    ui->comboBoxDataBreakpointSize->addItem("16-bit", static_cast<uint32_t>(Quantite::BreakpointSize::Bit16));
    ui->comboBoxDataBreakpointSize->addItem("32-bit", static_cast<uint32_t>(Quantite::BreakpointSize::Bit32));
    ui->comboBoxDataBreakpointSize->addItem("64-bit", static_cast<uint32_t>(Quantite::BreakpointSize::Bit64));

    // ブレークポイントビュー登録
    ui->tableViewDataBreakpoint->setModel(&dataBreakInfoModel);
    ui->tableViewInstructionBreakpoint->setModel(&instructionBreakInfoModel);

    // ヘッダーのレイアウト変更
    QHeaderView* header;
    header = ui->tableViewDataBreakpoint->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header = ui->tableViewInstructionBreakpoint->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    emit requestStart();
}

MainWindow::~MainWindow()
{
    emit requestStop();
    delete ui;
}

void MainWindow::onConnect()
{
    emit requestConnectServer(ui->lineEditIPAddress->text());
}

void MainWindow::onDisconnect()
{
    emit requestDisconnectServer();
}

void MainWindow::onSetDataBreakpoint()
{
    bool ok;
    uint32_t address = ui->lineEditDataBreakpointAddress->text().toUInt(&ok, 16);
    if(!ok) return;
    bool enable = ui->checkBoxDataBreakpointEnable->isChecked();
    bool read = ui->checkBoxDataBreakpointRead->isChecked();
    bool write = ui->checkBoxDataBreakpointWrite->isChecked();
    auto size = static_cast<Quantite::BreakpointSize>(ui->comboBoxDataBreakpointSize->currentData().toUInt());

    if(enable) emit requestSetDataBreakpoint(address, read, write, size);
    else emit requestUnsetDataBreakpoint();
}

void MainWindow::onDataBreakpointClear()
{
    dataBreakInfoModel.clear();
}

void MainWindow::onDataBreakReceived(uint32_t dAddress, uint32_t iAddress)
{
    DataBreakInfoEntry entry(dAddress, iAddress);
    dataBreakInfoModel.add(entry);
}

void MainWindow::onSetInstructionBreakpoint()
{
    bool ok;
    uint32_t address = ui->lineEditInstructionBreakpointAddress->text().toUInt(&ok, 16);
    if(!ok) return;
    bool enable = ui->checkBoxInstructionBreakpointEnable->isChecked();

    if(enable) emit requestSetInstructionBreakpoint(address);
    else emit requestUnsetInstructionBreakpoint();
}

void MainWindow::onInstructionBreakpointClear()
{
    instructionBreakInfoModel.clear();
}

void MainWindow::onInstructionBreakReceived(uint32_t iAddress)
{
    InstructionBreakInfoEntry entry(iAddress);
    instructionBreakInfoModel.add(entry);
}
