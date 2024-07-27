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
    void Ryzen_tdp_command(string Ryzen_command);
    string Get_tdp_Info(const char* tdp_type);

   void set_thermal_policy(int thermal_policy_int);
   void set_scaling_governor();
   void set_energy_performance_pref();

private slots:
    void on_tdp_Slider_valueChanged(int value);

    void on_GPU_Clock_Slider_valueChanged(int value);

    void on_tdp_Apply_pushButton_clicked();

    void on_GPU_Apply_pushButton_clicked();

    void on_Smoke_checkBox_toggled(bool checked);

    void on_Boost_checkBox_toggled(bool checked);

    void on_CPU_Boost_checkBox_toggled(bool checked);

    void on_tdp_lineEdit_editingFinished();

    void on_GPU_Clock_lineEdit_editingFinished();

    void readSettings();

    void writeSettings();

    void on_tdp_info_pushButton_clicked();

    void on_About_pushButton_clicked();

    void on_updateButton_Clicked();

    void on_GPU_Clock_checkBox_stateChanged(int arg1);

    void update_MCU_Mode_lineEdit();

    void on_Refresh_pushButton_clicked();

    void on_Mode_Change_Apply_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
