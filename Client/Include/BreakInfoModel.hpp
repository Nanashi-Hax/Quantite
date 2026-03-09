#pragma once

#include <QAbstractTableModel>

enum Register
{
    PC,
    DAR,
    GPR,
    FPR,
    CR,
    LR,
    CTR
};

struct RegisterInfo
{
    uint32_t pc;
    uint32_t dar;
    uint32_t gpr[32];
    double fpr[32];
    uint32_t cr;
    uint32_t lr;
    uint32_t ctr;

    QString string(Register kind, uint32_t index);
};

class BreakInfoEntry
{
public:
    BreakInfoEntry(RegisterInfo info);

    RegisterInfo info() const;
    uint32_t times() const;
    void addTimes();

private:
    RegisterInfo _info;
    uint32_t _times;
};

class BreakInfoModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    BreakInfoModel(Register kind, uint32_t index, QObject * parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    int columnCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    void setPrimaryKey(Register key, int index);

    void add(BreakInfoEntry entry);
    void clear();

private:
    Register _primaryKey;
    int _primaryIndex;
    QList<BreakInfoEntry> list;
};
