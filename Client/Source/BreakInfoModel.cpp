#include "BreakInfoModel.hpp"

QString RegisterInfo::string(Register kind, uint32_t index)
{
    switch(kind)
    {
        case Register::PC:
        {
            uint32_t value = pc;
            return QString("0x") + QString("%1").arg(value, 8, 16, QChar('0')).toUpper();
        }
        case Register::DAR:
        {
            uint32_t value = dar;
            return QString("0x") + QString("%1").arg(value, 8, 16, QChar('0')).toUpper();
        }
        case Register::GPR:
        {
            uint32_t value = gpr[index];
            return QString("0x") + QString("%1").arg(value, 8, 16, QChar('0')).toUpper();
        }
        case Register::FPR:
        {
            double value = fpr[index];
            return QString("%1").arg(value, 0, 'f', 4);
        }
        case Register::CR:
        {
            uint32_t value = cr;
            return QString("0x") + QString("%1").arg(value, 8, 16, QChar('0')).toUpper();
        }
        case Register::LR:
        {
            uint32_t value = lr;
            return QString("0x") + QString("%1").arg(value, 8, 16, QChar('0')).toUpper();
        }
        case Register::CTR:
        {
            uint32_t value = ctr;
            return QString("0x") + QString("%1").arg(value, 8, 16, QChar('0')).toUpper();
        }
    }
}

BreakInfoEntry::BreakInfoEntry(RegisterInfo info)
:
_info(info), _times(1) {}

RegisterInfo BreakInfoEntry::info() const
{
    return _info;
}

uint32_t BreakInfoEntry::times() const
{
    return _times;
}

void BreakInfoEntry::addTimes()
{
    _times++;
}

BreakInfoModel::BreakInfoModel(Register kind, uint32_t index, QObject * parent)
:
QAbstractTableModel(parent), _primaryKey(kind), _primaryIndex(index), list() {}

int BreakInfoModel::rowCount(const QModelIndex & parent) const
{
    return list.count();
}

int BreakInfoModel::columnCount(const QModelIndex & parent) const
{
    return 2;
}

QVariant BreakInfoModel::data(const QModelIndex & index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
            case 0:
            {
                RegisterInfo info = list.at(index.row()).info();
                return info.string(_primaryKey, _primaryIndex);
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

void BreakInfoModel::setPrimaryKey(Register key, int index)
{
    _primaryKey = key;
    _primaryIndex = index;
}

void BreakInfoModel::add(BreakInfoEntry entry)
{
    for(int i = 0; i < list.size(); i++)
    {
        auto& it = list[i];
        if(it.info().string(_primaryKey, _primaryIndex) == entry.info().string(_primaryKey, _primaryIndex))
        {
            it.addTimes();

            QModelIndex idx = index(i, 2);
            emit dataChanged(idx, idx);

            return;
        }
    }
    int row = list.size();
    beginInsertRows(QModelIndex(), row, row);
    list.append(entry);
    endInsertRows();
}

void BreakInfoModel::clear()
{
    beginResetModel();
    list.clear();
    endResetModel();
}
