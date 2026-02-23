#pragma once

#include <QMainWindow>
#include "QuantiteViewModel.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void requestStart();
    void requestStop();

private:
    Ui::MainWindow *ui;
    QuantiteViewModel quantite;
};

