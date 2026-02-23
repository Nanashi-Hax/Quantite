#include "ConnectionWidget.hpp"
#include "ui_ConnectionWidget.h"

ConnectionWidget::ConnectionWidget(QWidget *parent)
:
QWidget(parent), ui(new Ui::ConnectionWidget)
{
    ui->setupUi(this);

    connect(ui->pushButtonConnect, &QPushButton::clicked, this, &ConnectionWidget::onConnect);
    connect(ui->pushButtonDisconnect, &QPushButton::clicked, this, &ConnectionWidget::onDisconnect);
}

ConnectionWidget::~ConnectionWidget()
{
    delete ui;
}

void ConnectionWidget::onConnect()
{
    emit requestConnectServer(ui->lineEditIPAddress->text());
}

void ConnectionWidget::onDisconnect()
{
    emit requestDisconnectServer();
}
