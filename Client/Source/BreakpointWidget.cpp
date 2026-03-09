#include "BreakpointWidget.hpp"
#include "ui_BreakpointWidget.h"

BreakpointWidget::BreakpointWidget(QWidget *parent)
:
QWidget(parent), ui(new Ui::BreakpointWidget), dataBreakInfoModel(Register::PC, 0), instructionBreakInfoModel(Register::DAR, 0)
{
    ui->setupUi(this);

    // レジスタ第1キー
    ui->comboBoxDataPrimaryKey->addItem("PC",  static_cast<uint32_t>(Register::PC));
    ui->comboBoxDataPrimaryKey->addItem("DAR", static_cast<uint32_t>(Register::DAR));
    ui->comboBoxDataPrimaryKey->addItem("GPR", static_cast<uint32_t>(Register::GPR));
    ui->comboBoxDataPrimaryKey->addItem("FPR", static_cast<uint32_t>(Register::FPR));
    ui->comboBoxDataPrimaryKey->addItem("CR", static_cast<uint32_t>(Register::CR));
    ui->comboBoxDataPrimaryKey->addItem("LR", static_cast<uint32_t>(Register::LR));
    ui->comboBoxDataPrimaryKey->addItem("CTR", static_cast<uint32_t>(Register::CTR));
    connect(ui->pushButtonSetDataPrimaryKey, &QPushButton::clicked, this, &BreakpointWidget::onSetDataPrimaryKey);
    ui->comboBoxInstructionPrimaryKey->addItem("PC",  static_cast<uint32_t>(Register::PC));
    ui->comboBoxInstructionPrimaryKey->addItem("DAR", static_cast<uint32_t>(Register::DAR));
    ui->comboBoxInstructionPrimaryKey->addItem("GPR", static_cast<uint32_t>(Register::GPR));
    ui->comboBoxInstructionPrimaryKey->addItem("FPR", static_cast<uint32_t>(Register::FPR));
    ui->comboBoxInstructionPrimaryKey->addItem("CR", static_cast<uint32_t>(Register::CR));
    ui->comboBoxInstructionPrimaryKey->addItem("LR", static_cast<uint32_t>(Register::LR));
    ui->comboBoxInstructionPrimaryKey->addItem("CTR", static_cast<uint32_t>(Register::CTR));
    connect(ui->pushButtonSetInstructionPrimaryKey, &QPushButton::clicked, this, &BreakpointWidget::onSetInstructionPrimaryKey);

    // ボタン
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

void BreakpointWidget::onDataBreakReceived(RegisterInfo info)
{
    BreakInfoEntry entry(info);
    dataBreakInfoModel.add(entry);
}

void BreakpointWidget::onSetDataPrimaryKey()
{
    auto data = ui->comboBoxDataPrimaryKey->currentData();
    Register kind = data.value<Register>();
    int index = ui->spinBoxDataPrimaryKeyIndex->value();
    dataBreakInfoModel.setPrimaryKey(kind, index);
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

void BreakpointWidget::onInstructionBreakReceived(RegisterInfo info)
{
    BreakInfoEntry entry(info);
    instructionBreakInfoModel.add(entry);
}

void BreakpointWidget::onSetInstructionPrimaryKey()
{
    auto data = ui->comboBoxInstructionPrimaryKey->currentData();
    Register kind = data.value<Register>();
    int index = ui->spinBoxInstructionPrimaryKeyIndex->value();
    instructionBreakInfoModel.setPrimaryKey(kind, index);
}
