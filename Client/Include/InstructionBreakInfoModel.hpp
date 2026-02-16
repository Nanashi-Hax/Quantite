#pragma once

#include <QAbstractTableModel>

class InstructionBreakInfoEntry
{
public:
    InstructionBreakInfoEntry(uint32_t instructionAddress);

    uint32_t instructionAddress() const;
    uint32_t times() const;
    void addTimes();

private:
    uint32_t _instructionAddress;
    uint32_t _times;
};

class InstructionBreakInfoModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit InstructionBreakInfoModel(QObject * parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    int columnCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    void add(InstructionBreakInfoEntry entry);
    void clear();

private:
    QList<InstructionBreakInfoEntry> list;
};
