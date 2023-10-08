#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

using std::string;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tdp_Slider_valueChanged(int value);

    void on_GPU_Clock_Slider_valueChanged(int value);

    void on_tdp_Apply_pushButton_clicked();

    void on_GPU_Apply_pushButton_clicked();

    void on_Smoke_checkBox_toggled(bool checked);

    void on_Boost_checkBox_toggled(bool checked);

    void on_tdp_lineEdit_editingFinished();

    void on_GPU_Clock_lineEdit_editingFinished();

    void readSettings();

    void writeSettings();

    void Ryzen_tdp_command(string Ryzen_command);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
