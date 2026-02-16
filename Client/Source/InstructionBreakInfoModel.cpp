#include "InstructionBreakInfoModel.hpp"

InstructionBreakInfoEntry::InstructionBreakInfoEntry(uint32_t instructionAddress)
:
_instructionAddress(instructionAddress), _times(1) {}

uint32_t InstructionBreakInfoEntry::instructionAddress() const
{
    return _instructionAddress;
}

uint32_t InstructionBreakInfoEntry::times() const
{
    return _times;
}

void InstructionBreakInfoEntry::addTimes()
{
    _times++;
}

InstructionBreakInfoModel::InstructionBreakInfoModel(QObject * parent)
:
QAbstractTableModel(parent), list() {}

int InstructionBreakInfoModel::rowCount(const QModelIndex & parent) const
{
    return list.count();
}

int InstructionBreakInfoModel::columnCount(const QModelIndex & parent) const
{
    return 2;
}

QVariant InstructionBreakInfoModel::data(const QModelIndex & index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
            case 0:
            {
                uint32_t instructionAddress = list.at(index.row()).instructionAddress();
                return QString("0x") + QString("%1").arg(instructionAddress, 8, 16, QChar('0')).toUpper();
            }
            case 1:
            {
                uint32_t times = list.at(index.row()).times();
                return QString("%1").arg(times);
            }
            default:
            {
                return QVariant{};
            }
        }
    }
    return QVariant{};
}

void InstructionBreakInfoModel::add(InstructionBreakInfoEntry entry)
{
    for(int i = 0; i < list.size(); i++)
    {
        auto& it = list[i];
        if(it.instructionAddress() == entry.instructionAddress())
        {
            it.addTimes();

            QModelIndex idx = index(i, 1);
            emit dataChanged(idx, idx);

            return;
        }
    }
    int row = list.size();
    beginInsertRows(QModelIndex(), row, row);
    list.append(entry);
    endInsertRows();
}

void InstructionBreakInfoModel::clear()
{
    beginResetModel();
    list.clear();
    endResetModel();
}
