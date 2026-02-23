#pragma once

#include <QWidget>

namespace Ui
{
    class ConnectionWidget;
}

class ConnectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionWidget(QWidget *parent = nullptr);
    ~ConnectionWidget();

public slots:
    void onConnect();
    void onDisconnect();

signals:
    void requestConnectServer(const QString& ipAddress);
    void requestDisconnectServer();

private:
    Ui::ConnectionWidget *ui;
};
