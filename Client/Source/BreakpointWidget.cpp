#include "BreakpointWidget.hpp"
#include "ui_BreakpointWidget.h"

BreakpointWidget::BreakpointWidget(QWidget *parent)
:
QWidget(parent), ui(new Ui::BreakpointWidget)
{
    ui->setupUi(this);

    connect(ui->pushButtonSetData, &QPushButton::clicked, this, &BreakpointWidget::onSetDataBreakpoint);
    connect(ui->pushButtonDataClear, &QPushButton::clicked, this, &BreakpointWidget::onDataBreakpointClear);
    connect(ui->pushButtonSetInstruction, &QPushButton::clicked, this, &BreakpointWidget::onSetInstructionBreakpoint);
    connect(ui->pushButtonInstructionClear, &QPushButton::clicked, this, &BreakpointWidget::onInstructionBreakpointClear);

    // コンボボックス登録
    ui->comboBoxDataSize->addItem("8-bit",  static_cast<uint32_t>(Quantite::BreakpointSize::Bit8));
    ui->comboBoxDataSize->addItem("16-bit", static_cast<uint32_t>(Quantite::BreakpointSize::Bit16));
    ui->comboBoxDataSize->addItem("32-bit", static_cast<uint32_t>(Quantite::BreakpointSize::Bit32));
    ui->comboBoxDataSize->addItem("64-bit", static_cast<uint32_t>(Quantite::BreakpointSize::Bit64));

    // ブレークポイントビュー登録
    ui->tableViewData->setModel(&dataBreakInfoModel);
    ui->tableViewInstruction->setModel(&instructionBreakInfoModel);

    // ヘッダーのレイアウト変更
    QHeaderView* header;
    header = ui->tableViewData->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header = ui->tableViewInstruction->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

BreakpointWidget::~BreakpointWidget()
{
    delete ui;
}

void BreakpointWidget::onSetDataBreakpoint()
{
    bool ok;
    uint32_t address = ui->lineEditDataAddress->text().toUInt(&ok, 16);
    if(!ok) return;
    bool enable = ui->checkBoxDataEnable->isChecked();
    bool read = ui->checkBoxDataRead->isChecked();
    bool write = ui->checkBoxDataWrite->isChecked();
    auto size = static_cast<Quantite::BreakpointSize>(ui->comboBoxDataSize->currentData().toUInt());

    if(enable) emit requestSetDataBreakpoint(address, read, write, size);
    else emit requestUnsetDataBreakpoint();
}

void BreakpointWidget::onDataBreakpointClear()
{
    dataBreakInfoModel.clear();
}

void BreakpointWidget::onDataBreakReceived(uint32_t dAddress, uint32_t iAddress)
{
    DataBreakInfoEntry entry(dAddress, iAddress);
    dataBreakInfoModel.add(entry);
}

void BreakpointWidget::onSetInstructionBreakpoint()
{
    bool ok;
    uint32_t address = ui->lineEditInstructionAddress->text().toUInt(&ok, 16);
    if(!ok) return;
    bool enable = ui->checkBoxInstructionEnable->isChecked();

    if(enable) emit requestSetInstructionBreakpoint(address);
    else emit requestUnsetInstructionBreakpoint();
}

void BreakpointWidget::onInstructionBreakpointClear()
{
    instructionBreakInfoModel.clear();
}

void BreakpointWidget::onInstructionBreakReceived(uint32_t iAddress)
{
    InstructionBreakInfoEntry entry(iAddress);
    instructionBreakInfoModel.add(entry);
}
