#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <iostream>
#include <libudev.h>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QSettings>
#include <QString>
#include <QTimer>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <unistd.h>

using std::ostringstream;
using std::round;
using std::stoi;
using std::string;
using std::to_string;

bool ok;
bool Smoke_bool;
bool Boost_bool;
bool GPU_Clock_bool;
float fast_boost_float;
float slow_boost_float;
int Find_MCU_Mode_test_res;
int GPU_Clock_value_int = 2200;
int slow_boost_int;
int fast_boost_int;
int tdp_value_int;
int Update_Num;
int VERSION = 100;
const char* tdp_limit_value_search = "sudo ryzenadj --info | grep -e 'STAPM LIMIT' | grep -Eo '[0-9]+([.][0-9]+)?'";
const char* tdp_value_search = "sudo ryzenadj --info | grep -e 'STAPM VALUE' | grep -Eo '[0-9]+([.][0-9]+)?'";
const char* tdp_fast_lim_value_search = "sudo ryzenadj --info | grep -e 'PPT LIMIT FAST' | grep -Eo '[0-9]+([.][0-9]+)?'";
const char* tdp_fast_value_search = "sudo ryzenadj --info | grep -e 'PPT VALUE FAST' | grep -Eo '[0-9]+([.][0-9]+)?'";
const char* tdp_slow_lim_value_search = "sudo ryzenadj --info | grep -e 'PPT LIMIT SLOW' | grep -Eo '[0-9]+([.][0-9]+)?'";
const char* tdp_slow_value_search = "sudo ryzenadj --info | grep -e 'PPT VALUE SLOW' | grep -Eo '[0-9]+([.][0-9]+)?'";
const char* gpu_0_value = "cat /sys/class/drm/card*/device/pp_od_clk_voltage | grep -a '0:' | cut -d: -f2 | grep -Eo '[0-9]{1,4}'";
const char* gpu_1_value = "cat /sys/class/drm/card*/device/pp_od_clk_voltage | grep -a '0:' | cut -d: -f2 | grep -Eo '[0-9]{1,4}'";
const char* Secure_Boot_status = "mokutil --sb-state | grep 'SecureBoot enabled'";
const char* GPU_SYSPATH = "SYSPATH=`find /sys/devices -name pp_od_clk_voltage 2>/dev/null | sed 's|/pp_od_clk_voltage||g' |head -n1` && echo $SYSPATH";
const char* amd_pstate_status = "cat /sys/devices/system/cpu/amd_pstate/status";
const char* scaling_governor = "cat /sys/devices/system/cpu/cpufreq/policy0/scaling_governor";
const char* scaling_driver = "cat /sys/devices/system/cpu/cpufreq/policy0/scaling_driver";
const char* energy_performance_preference = "cat /sys/devices/system/cpu/cpufreq/policy0/energy_performance_preference";
const char* throttle_thermal_policy = "cat /sys/devices/platform/asus-nb-wmi/throttle_thermal_policy";
ostringstream user_home_path;
QString gpu_clock_value;
QString settings_path;
QString settings_path_suffix{"/.local/Ryzen_tdp_GUI/Ryzen_tdp_GUI.ini"};
QString fast_boost_value = "53";
QString fast_boost_value_display = "53";
QString slow_boost_value = "43";
QString slow_boost_value_display = "43";
QString fast_min = "5";
QString fast_max = "53";
QString slow_min = "5";
QString slow_max = "45";
QString tdp_value;
QString MCU_Mode;
QString user_home_path_q;
QString Ryzen_tdp_debug_out;
QString qdeb;
string dev_name;
string GPU_auto_mode = "echo \"auto\" > \"";
string GPU_manual_mode = "echo \"manual\" > \"";
string GPU_mode_select_str;
string GPU_SYSPATH_str;
string gpu_clock_value_str;
string slow_boost_str;
string slow_boost_str_sb;
string fast_boost_str;
string fast_boost_str_sb;
string MCU_Mode_path_str;
string MCU_Mode_str;
string MCU_Mode_test_str;
string Ryzen_gpu_command_str;
string Ryzen_tdp_command_str;
string Secure_Boot_status_str;
string tdp_display_info;
string tdp_info_disp_temp;
string tdp_info_str;
string tdp_USER = getlogin();
string tdp_value_str;
string tdp_value_str_sb;
string thermal_str;
string Update_Num_str;
string user_home_path_str;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *MCU_Mode_check_timer = new QTimer(this);
    // Set up the timer to trigger every 10 seconds (10000 milliseconds)
    MCU_Mode_check_timer->start(10000);
    QRegExp slow_int_regex("^(?:[5-9]|[1-3][0-9]|4[0-5])$");
    QRegExpValidator *slow_int_validator = new QRegExpValidator(slow_int_regex, this);
    ui->slow_tdp_lineEdit->setValidator(slow_int_validator);
    QRegExp fast_int_regex("^([5-9]|[1-4][0-9]|5[0-3])$");
    QRegExpValidator *fast_int_validator = new QRegExpValidator(fast_int_regex, this);
    ui->fast_tdp_lineEdit->setValidator(fast_int_validator);
    QRegExp gpu_int_regex("^(8[0-9]{2}|9[0-9]{2}|[12][0-6][0-9]{2}|2700)$");
    QRegExpValidator *gpu_int_validator = new QRegExpValidator(gpu_int_regex, this);
    ui->GPU_Clock_lineEdit->setValidator(gpu_int_validator);
    QRegExp tdp_int_regex("^(?:[5-9]|1[0-9]|2[0-9]|30)$");
    QRegExpValidator *tdp_int_validator = new QRegExpValidator(tdp_int_regex, this);
    ui->tdp_lineEdit->setValidator(tdp_int_validator);
    connect(ui->GPU_Clock_checkBox, &QCheckBox::stateChanged, this, &MainWindow::on_GPU_Clock_checkBox_stateChanged);
    connect(MCU_Mode_check_timer, &QTimer::timeout, this, &MainWindow::update_MCU_Mode_lineEdit);
    ui->MCU_Mode_lineEdit->setReadOnly(true);
    update_MCU_Mode_lineEdit();
    user_home_path << "/home/" << tdp_USER;
    user_home_path_str = user_home_path.str();
    user_home_path_q = QString::fromStdString(user_home_path_str);
    settings_path.reserve(user_home_path_q.length() + settings_path_suffix.length());
    settings_path.append(user_home_path_q);
    settings_path.append(settings_path_suffix);
    readSettings();
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}

void MainWindow::on_tdp_Slider_valueChanged(int value)
{
    tdp_value_str = to_string(value);
    tdp_value = QString::fromStdString(tdp_value_str);
    ui->tdp_lineEdit->setText(tdp_value);
}

void MainWindow::on_GPU_Clock_Slider_valueChanged(int value)
{
    gpu_clock_value_str = to_string(value);
    gpu_clock_value = QString::fromStdString(gpu_clock_value_str);
    ui->GPU_Clock_lineEdit->setText(gpu_clock_value);
}

void MainWindow::on_tdp_Apply_pushButton_clicked()
{
    tdp_value_int = ui->tdp_Slider->value();
    Boost_bool = ui->Boost_checkBox->isChecked();
    switch(tdp_value_int){
        case 5:
            if(!Boost_bool){
                slow_boost_value = "6";
                fast_boost_value = "7";
                }
            break;
        case 6:
            if(!Boost_bool){
                slow_boost_value = "7";
                fast_boost_value = "9";
                }
            break;
        case 7:
            if(!Boost_bool){
                slow_boost_value = "9";
                fast_boost_value = "11";
                }
            break;
        case 8:
            if(!Boost_bool){
                slow_boost_value = "9";
                fast_boost_value = "10";
                }
            break;
        case 9:
            if(!Boost_bool){
                slow_boost_value = "10";
                fast_boost_value = "12";
                }
            break;
        case 10:
            if(!Boost_bool){
                slow_boost_value = "11";
                fast_boost_value = "13";
                }
            break;
        case 11:
            if(!Boost_bool){
                slow_boost_value = "12";
                fast_boost_value = "14";
                }
            break;
        case 12:
            if(!Boost_bool){
                slow_boost_value = "13";
                fast_boost_value = "15";
                }
            break;
        case 13:
            if(!Boost_bool){
                slow_boost_value = "14";
                fast_boost_value = "16";
                }
            break;
        case 14:
            if(!Boost_bool){
                slow_boost_value = "15";
                fast_boost_value = "18";
                }
            break;
        case 15:
            if(!Boost_bool){
                slow_boost_value = "16";
                fast_boost_value = "19";
                }
            break;
        case 16:
            if(!Boost_bool){
                slow_boost_value = "17";
                fast_boost_value = "20";
                }
            break;
        case 17:
            if(!Boost_bool){
                slow_boost_value = "18";
                fast_boost_value = "21";
                }
            break;
        case 18:
            if(!Boost_bool){
                slow_boost_value = "19";
                fast_boost_value = "22";
                }
            break;
        case 19:
            if(!Boost_bool){
                slow_boost_value = "20000";
                fast_boost_value = "23";
                }
            break;
        case 20:
            if(!Boost_bool){
                slow_boost_value = "21";
                fast_boost_value = "25";
                }
            break;
        case 21:
            if(!Boost_bool){
                slow_boost_value = "22";
                fast_boost_value = "26";
                }
            break;
        case 22:
            if(!Boost_bool){
                slow_boost_value = "23";
                fast_boost_value = "27";
                }
            break;
        case 23:
            if(!Boost_bool){
                slow_boost_value = "24";
                fast_boost_value = "28";
                }
            break;
        case 24:
            if(!Boost_bool){
                slow_boost_value = "25";
                fast_boost_value = "29";
                }
            break;
        case 25:
            if(!Boost_bool){
                slow_boost_value = "26";
                fast_boost_value = "30";
                }
            break;
        case 26:
            if(!Boost_bool){
                slow_boost_value = "27";
                fast_boost_value = "32";
                }
            break;
        case 27:
            if(!Boost_bool){
                slow_boost_value = "28";
                fast_boost_value = "33";
                }
            break;
        case 28:
            if(!Boost_bool){
                slow_boost_value = "29";
                fast_boost_value = "34";
                }
            break;
        case 29:
            if(!Boost_bool){
                slow_boost_value = "30";
                fast_boost_value = "35";
                }
            break;
        case 30:
            if(!Boost_bool){
                slow_boost_value = "26";
                fast_boost_value = "30";
            }
            break;
        case 31:
            if(!Boost_bool){
                slow_boost_value = "32";
                fast_boost_value = "38";
                }
            break;
        case 32:
            if(!Boost_bool){
                slow_boost_value = "33";
                fast_boost_value = "39";
                }
            break;
        case 33:
            if(!Boost_bool){
                slow_boost_value = "34";
                fast_boost_value = "40";
                }
            break;
        case 34:
            if(!Boost_bool){
                slow_boost_value = "35";
                fast_boost_value = "41";
                }
            break;
        case 35:
            if(!Boost_bool){
                slow_boost_value = "36";
                fast_boost_value = "42";
                }
            break;
        case 36:
            if(!Boost_bool){
                slow_boost_value = "37";
                fast_boost_value = "43";
                }
            break;
        case 37:
            if(!Boost_bool){
                slow_boost_value = "38";
                fast_boost_value = "44";
                }
            break;
        case 38:
            if(!Boost_bool){
                slow_boost_value = "39";
                fast_boost_value = "45";
                }
            break;
        case 39:
            if(!Boost_bool){
                slow_boost_value = "40";
                fast_boost_value = "47";
                }
            break;
        case 40:
            if(!Boost_bool){
                slow_boost_value = "41";
                fast_boost_value = "48";
                }
            break;
        case 41:
            if(!Boost_bool){
                slow_boost_value = "42";
                fast_boost_value = "49";
                }
            break;
        case 42:
            if(!Boost_bool){
                slow_boost_value = "43";
                fast_boost_value = "50";
                }
            break;
        case 43:
            if(!Boost_bool){
                slow_boost_value = "44";
                fast_boost_value = "51";
                }
            break;
        case 44:
            if(!Boost_bool){
                slow_boost_value = "45";
                fast_boost_value = "53";
            }
            break;
    }
        if(Boost_bool){
            slow_boost_value = ui->slow_tdp_lineEdit->text();
            fast_boost_value = ui->fast_tdp_lineEdit->text();
            slow_boost_int = slow_boost_value.toInt(&ok);
            slow_boost_str = to_string(slow_boost_int);
            fast_boost_int = fast_boost_value.toInt(&ok);
            fast_boost_str = to_string(fast_boost_int);
        }else {
            slow_boost_float = slow_boost_value.toFloat(&ok);
            slow_boost_int = static_cast<int>(round(slow_boost_float));
            slow_boost_value_display = QString::number(slow_boost_int);
            fast_boost_float = fast_boost_value.toFloat(&ok);
            fast_boost_int = static_cast<int>(round(fast_boost_float));
            fast_boost_value_display = QString::number(fast_boost_int);
            ui->slow_tdp_lineEdit->setText(slow_boost_value_display);
            ui->fast_tdp_lineEdit->setText(fast_boost_value_display);
            slow_boost_str = slow_boost_value.toStdString();
            fast_boost_str = fast_boost_value.toStdString();
        }
        tdp_value_str = to_string(tdp_value_int);
        Ryzen_tdp_command_str.clear();
        Ryzen_tdp_command_str.append("echo ");
        Ryzen_tdp_command_str.append(fast_boost_str);
        Ryzen_tdp_command_str.append(" | tee /sys/devices/platform/asus-nb-wmi/ppt_fppt");
        Ryzen_tdp_command(Ryzen_tdp_command_str);
        Ryzen_tdp_command_str.clear();
        Ryzen_tdp_command_str.append("echo ");
        Ryzen_tdp_command_str.append(slow_boost_str);
        Ryzen_tdp_command_str.append(" | tee /sys/devices/platform/asus-nb-wmi/ppt_pl2_sppt");
        Ryzen_tdp_command(Ryzen_tdp_command_str);
        Ryzen_tdp_command_str.clear();
        Ryzen_tdp_command_str.append("echo ");
        Ryzen_tdp_command_str.append(tdp_value_str);
        Ryzen_tdp_command_str.append(" | tee /sys/devices/platform/asus-nb-wmi/ppt_pl1_spl");
        Ryzen_tdp_command(Ryzen_tdp_command_str);
        tdp_value_str_sb = tdp_value_str;
        fast_boost_str_sb = fast_boost_str;
        slow_boost_str_sb = slow_boost_str;
}

void MainWindow::on_GPU_Apply_pushButton_clicked()
{
    Ryzen_gpu_command_str.clear();
    GPU_Clock_bool = ui->GPU_Clock_checkBox->isChecked();
    GPU_mode_select_str.clear();
    GPU_SYSPATH_str.clear();
    GPU_SYSPATH_str = Get_tdp_Info(GPU_SYSPATH);
    if (!GPU_SYSPATH_str.empty() && GPU_SYSPATH_str.back() == '\n') {
        GPU_SYSPATH_str.pop_back(); // Remove the last character
    }
    if (GPU_Clock_bool){
        GPU_mode_select_str.append(GPU_manual_mode);
        GPU_mode_select_str.append(GPU_SYSPATH_str);
        GPU_mode_select_str.append("/power_dpm_force_performance_level\"");
        Ryzen_tdp_command(GPU_mode_select_str);
        gpu_clock_value = ui->GPU_Clock_lineEdit->text();
        if(gpu_clock_value == ""){
            GPU_Clock_value_int = ui->GPU_Clock_Slider->value();
        }else {
            GPU_Clock_value_int = gpu_clock_value.toInt(&ok);
        }
        if(GPU_Clock_value_int < 800){
            GPU_Clock_value_int = 800;
        }
        if(GPU_Clock_value_int > 2700){
            GPU_Clock_value_int = 2700;
        }
        gpu_clock_value_str = to_string(GPU_Clock_value_int);
        Ryzen_gpu_command_str.append("echo -e \"s 0 ");
        Ryzen_gpu_command_str.append(gpu_clock_value_str);
        Ryzen_gpu_command_str.append("\ns 1 ");
        Ryzen_gpu_command_str.append(gpu_clock_value_str);
        Ryzen_gpu_command_str.append("\nc\" > \"");
        Ryzen_gpu_command_str.append(GPU_SYSPATH_str);
        Ryzen_gpu_command_str.append("/pp_od_clk_voltage\"");
        Ryzen_tdp_command(Ryzen_gpu_command_str);
    } else {
        GPU_mode_select_str.append(GPU_auto_mode);
        GPU_mode_select_str.append(GPU_SYSPATH_str);
        GPU_mode_select_str.append("/power_dpm_force_performance_level\"");
        Ryzen_tdp_command(GPU_mode_select_str);
    }
}

void MainWindow::on_Smoke_checkBox_toggled(bool checked)
{
    Smoke_bool = ui->Smoke_checkBox->isChecked();
    if (Smoke_bool){
        ui->tdp_Slider->setMaximum(44);
        QRegExp smoke_int_regex("^(?:[5-9]|[1-3][0-9]|4[0-4])$");
        QRegExpValidator *smoke_int_validator = new QRegExpValidator(smoke_int_regex, this);
        ui->tdp_lineEdit->setValidator(smoke_int_validator);
    } else {
        ui->tdp_Slider->setMaximum(30);
        QRegExp tdp_modded_int_regex("^(?:[5-9]|1[0-9]|2[0-9]|30)$");
        QRegExpValidator *tdp_modded_int_validator = new QRegExpValidator(tdp_modded_int_regex, this);
        ui->tdp_lineEdit->setValidator(tdp_modded_int_validator);
    }

}

void MainWindow::on_Boost_checkBox_toggled(bool checked)
{
    Boost_bool = ui->Boost_checkBox->isChecked();
    if(Boost_bool){
        ui->slow_tdp_lineEdit->setReadOnly(false);
        ui->fast_tdp_lineEdit->setReadOnly(false);
    }else {
        ui->slow_tdp_lineEdit->setReadOnly(true);
        ui->fast_tdp_lineEdit->setReadOnly(true);
    }
}

void MainWindow::on_tdp_lineEdit_editingFinished()
{
    if (ui->tdp_lineEdit->hasAcceptableInput()) {
        tdp_value = ui->tdp_lineEdit->text();
        ui->tdp_Slider->setValue(tdp_value.toInt(&ok));
    }
}

void MainWindow::on_GPU_Clock_lineEdit_editingFinished()
{
    if (ui->GPU_Clock_lineEdit->hasAcceptableInput()) {
        gpu_clock_value = ui->GPU_Clock_lineEdit->text();
        ui->GPU_Clock_Slider->setValue(gpu_clock_value.toInt(&ok));
    }
}

void MainWindow::on_tdp_info_pushButton_clicked()
{
    QMessageBox tdp_info_Box;
    tdp_display_info.clear();
    Secure_Boot_status_str.clear();
    Secure_Boot_status_str = Get_tdp_Info(Secure_Boot_status);
    if (!Secure_Boot_status_str.empty() && Secure_Boot_status_str.back() == '\n') {
        Secure_Boot_status_str.pop_back(); // Remove the last character
    }
    // show tdp settings info a different way if Secure Boot is enabled...
    if(Secure_Boot_status_str == "SecureBoot enabled"){
        tdp_display_info.append("Sustained TDP setting: ");
        tdp_display_info.append(tdp_value_str_sb);
        tdp_display_info.append("\nFast TDP setting: ");
        tdp_display_info.append(fast_boost_str_sb);
        tdp_display_info.append("\nSlow TDP setting: ");
        tdp_display_info.append(slow_boost_str_sb);
    }else {
        tdp_display_info.append("Sustained TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_limit_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("Sustained TDP value: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("Fast TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_fast_lim_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("Fast TDP value: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_fast_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("Slow TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_slow_lim_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("Slow TDP value: ");
        tdp_info_disp_temp = (tdp_slow_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
    }
    tdp_display_info.append("\nAMD Pstate status: ");
    tdp_info_disp_temp = Get_tdp_Info(amd_pstate_status);
    tdp_display_info.append(tdp_info_disp_temp);
    tdp_display_info.append("Scaling Driver: ");
    tdp_info_disp_temp = Get_tdp_Info(scaling_driver);
    tdp_display_info.append(tdp_info_disp_temp);
    tdp_display_info.append("Scaling Governor: ");
    tdp_info_disp_temp = Get_tdp_Info(scaling_governor);
    tdp_display_info.append(tdp_info_disp_temp);
    tdp_display_info.append("EPP setting in use: ");
    tdp_info_disp_temp = Get_tdp_Info(energy_performance_preference);
    tdp_display_info.append(tdp_info_disp_temp);
    tdp_display_info.append("Thermal policy: ");
    thermal_str = Get_tdp_Info(throttle_thermal_policy);
    if (!thermal_str.empty() && thermal_str.back() == '\n') {
        thermal_str.pop_back(); // Remove the last character
    }
    if(thermal_str == "0"){
        tdp_info_disp_temp = "Balanced";
    }
    if(thermal_str == "1"){
        tdp_info_disp_temp = "Performance";
    }
    if(thermal_str == "2"){
        tdp_info_disp_temp = "Quiet";
    }
    if((thermal_str != "0") && (thermal_str != "1") && (thermal_str != "2")){
        tdp_info_disp_temp = "Unknown";
    }
    tdp_display_info.append(tdp_info_disp_temp);
    QString tdp_info_QString = QString::fromStdString(tdp_display_info);
    tdp_info_Box.setText(tdp_info_QString);
    tdp_info_Box.setStandardButtons(QMessageBox::Ok);
    tdp_info_Box.exec();
}

void MainWindow::on_exit_pushButton_clicked()
{
    MainWindow::~MainWindow();
}

string MainWindow::Get_tdp_Info(const char* tdp_type) {
    tdp_info_str.clear();
    FILE *process;
    char buff[1024];
    process = popen(tdp_type, "r");
    if (process != NULL) {
        while (fgets(buff, sizeof(buff), process)) {
            printf("%s", buff);
            tdp_info_str += buff;
        }
        pclose(process);
    }
    return tdp_info_str;
}

void MainWindow::Ryzen_tdp_command(string Ryzen_command)
{
    system(Ryzen_command.c_str());
}

void MainWindow::readSettings()
{
    QSettings settings(settings_path, QSettings::NativeFormat);
    settings.beginGroup("CheckBoxes");
    bool temp_Smoke_bool = settings.value("Smoke_checkBox").toBool();
    bool temp_Boost_bool = settings.value("Boost_checkBox").toBool();
    bool temp_GPU_Clock_bool = settings.value("GPU_Clock_checkBox").toBool();
    settings.endGroup();
    settings.beginGroup("Values");
    QString tempSlow = settings.value("slow_tdp").toString();
    QString tempFast = settings.value("fast_tdp").toString();
    QString tempGPU = settings.value("gpu_clock").toString();
    QString tempTDP_Slider = settings.value("apu_tdp_slider").toString();
    QString tempGPU_Slider = settings.value("gpu_clock_slider").toString();
    QString tempTDP = settings.value("tdp_stored").toString();
    QString tempFastBoost = settings.value("fast_boost_stored").toString();
    QString tempSlowBoost = settings.value("slow_boost_stored").toString();
    settings.endGroup();
    ui->Smoke_checkBox->setChecked(temp_Smoke_bool);
    ui->Boost_checkBox->setChecked(temp_Boost_bool);
    ui->GPU_Clock_checkBox->setChecked(temp_GPU_Clock_bool);
    ui->slow_tdp_lineEdit->setText(tempSlow);
    ui->fast_tdp_lineEdit->setText(tempFast);
    ui->tdp_lineEdit->setText(tempTDP_Slider);
    ui->tdp_Slider->setValue(tempTDP_Slider.toInt(&ok));
    ui->GPU_Clock_Slider->setValue(tempGPU_Slider.toInt(&ok));
    ui->GPU_Clock_lineEdit->setText(tempGPU_Slider);
    tdp_value_str_sb = tempTDP.toStdString();
    fast_boost_str_sb = tempFastBoost.toStdString();
    slow_boost_str_sb = tempSlowBoost.toStdString();
}

void MainWindow::writeSettings()
{
    QSettings settings(settings_path, QSettings::NativeFormat);
    settings.beginGroup("CheckBoxes");
    settings.setValue("Smoke_checkBox", ui->Smoke_checkBox->isChecked());
    settings.setValue("Boost_checkBox", ui->Boost_checkBox->isChecked());
    settings.setValue("GPU_Clock_checkBox", ui->GPU_Clock_checkBox->isChecked());
    settings.endGroup();
    settings.beginGroup("Values");
    settings.setValue("slow_tdp", ui->slow_tdp_lineEdit->text());
    settings.setValue("fast_tdp", ui->fast_tdp_lineEdit->text());
    settings.setValue("gpu_clock", ui->GPU_Clock_Slider->value());
    settings.setValue("apu_tdp_slider", ui->tdp_Slider->value());
    settings.setValue("gpu_clock_slider", ui->GPU_Clock_Slider->value());
    settings.setValue("tdp_stored", QString::fromStdString(tdp_value_str_sb));
    settings.setValue("fast_boost_stored", QString::fromStdString(fast_boost_str_sb));
    settings.setValue("slow_boost_stored", QString::fromStdString(slow_boost_str_sb));
    settings.endGroup();
}

void MainWindow::on_About_pushButton_clicked()
{
    QMessageBox AboutBox;
    QPushButton* updateButton = new QPushButton("Check For Update");
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_Clicked);
    AboutBox.setTextFormat(Qt::RichText);
    AboutBox.setText("<p align='center'>Ryzen tdp GUI v1.0.0<br><br>"
                     "Original GUI Creator: "
                     "<a href='https://github.com/jlobue10'>jlobue10</a><br><br>"
                     "Special Thanks to Nobara Discord for testing and QA");
    AboutBox.setStandardButtons(QMessageBox::Ok);
    AboutBox.addButton(updateButton, QMessageBox::ActionRole);
    AboutBox.exec();
}

void MainWindow::on_updateButton_Clicked()
{
    QMessageBox UpdateBox;
    UpdateBox.setTextFormat(Qt::RichText);
    FILE *Update_process;
    char Update_buff[1024];
    Update_Num_str.clear();
    Update_process = popen("echo $(curl https://raw.githubusercontent.com/jlobue10/Ryzen_tdp_GUI/main/VERSION) | sed 's/\\./ /g' | sed 's/\\s\\+//g'", "r");
    if (Update_process != NULL) {
        while (fgets(Update_buff, sizeof(Update_buff), Update_process)) {
            printf("%s", Update_buff);
            Update_Num_str += Update_buff;
        }
        pclose(Update_process);
    }
    Update_Num = stoi(Update_Num_str);
    if(Update_Num > VERSION) {
        UpdateBox.setText("<p align='center'>An update is available "
                          "<a href='https://github.com/jlobue10/Ryzen_tdp/releases'>here</a><br><br></p>");
    } else {
        UpdateBox.setText("<p align='center'>No update found. You are using the latest version.<br><br></p>");
    }
    UpdateBox.setStandardButtons(QMessageBox::Ok);
    UpdateBox.exec();
}

void MainWindow::on_GPU_Clock_checkBox_stateChanged(int arg1)
{
    Ryzen_gpu_command_str.clear();
    GPU_mode_select_str.clear();
    GPU_SYSPATH_str.clear();
    GPU_SYSPATH_str = Get_tdp_Info(GPU_SYSPATH);
    if (!GPU_SYSPATH_str.empty() && GPU_SYSPATH_str.back() == '\n') {
        GPU_SYSPATH_str.pop_back(); // Remove the last character
    }
    if (arg1 == Qt::Checked){
        GPU_mode_select_str.append(GPU_manual_mode);
        GPU_mode_select_str.append(GPU_SYSPATH_str);
        GPU_mode_select_str.append("/power_dpm_force_performance_level\"");
        Ryzen_tdp_command(GPU_mode_select_str);
        gpu_clock_value = ui->GPU_Clock_lineEdit->text();
        if(gpu_clock_value == ""){
            GPU_Clock_value_int = ui->GPU_Clock_Slider->value();
        }else {
            GPU_Clock_value_int = gpu_clock_value.toInt(&ok);
        }
        if(GPU_Clock_value_int < 800){
            GPU_Clock_value_int = 800;
        }
        if(GPU_Clock_value_int > 2700){
            GPU_Clock_value_int = 2700;
        }
        gpu_clock_value_str = to_string(GPU_Clock_value_int);
        Ryzen_gpu_command_str.append("echo -e \"s 0 ");
        Ryzen_gpu_command_str.append(gpu_clock_value_str);
        Ryzen_gpu_command_str.append("\ns 1 ");
        Ryzen_gpu_command_str.append(gpu_clock_value_str);
        Ryzen_gpu_command_str.append("\nc\" > \"");
        Ryzen_gpu_command_str.append(GPU_SYSPATH_str);
        Ryzen_gpu_command_str.append("/pp_od_clk_voltage\"");
        Ryzen_tdp_command(Ryzen_gpu_command_str);
    } else {
        GPU_mode_select_str.append(GPU_auto_mode);
        GPU_mode_select_str.append(GPU_SYSPATH_str);
        GPU_mode_select_str.append("/power_dpm_force_performance_level\"");
        Ryzen_tdp_command(GPU_mode_select_str);
    }
}

string MainWindow::find_device(struct udev *udev) {
    struct udev_enumerate *const enumerate = udev_enumerate_new(udev);
    if (enumerate == NULL) {
        fprintf(stderr, "Error in udev_enumerate_new: mode switch will not be available.\n");
        return "\0";
    }

    const int add_match_subsystem_res = udev_enumerate_add_match_subsystem(enumerate, "hid");
    if (add_match_subsystem_res != 0) {
        fprintf(stderr, "Error in udev_enumerate_add_match_subsystem: %d\n", add_match_subsystem_res);

        udev_enumerate_unref(enumerate);

        return "\0";
    }

    const int add_match_sysattr_res = udev_enumerate_add_match_sysattr(enumerate, "gamepad_mode", NULL);
    if (add_match_sysattr_res != 0) {
        fprintf(stderr, "Error in udev_enumerate_add_match_sysattr: %d\n", add_match_sysattr_res);

        udev_enumerate_unref(enumerate);

        return "\0";
    }

    const int enumerate_scan_devices_res = udev_enumerate_scan_devices(enumerate);
    if (enumerate_scan_devices_res != 0) {
        fprintf(stderr, "Error in udev_enumerate_scan_devices: %d\n", enumerate_scan_devices_res);

        udev_enumerate_unref(enumerate);

        return "\0";
    }

    struct udev_list_entry *const udev_lst_frst = udev_enumerate_get_list_entry(enumerate);

    struct udev_list_entry *list_entry = NULL;
    udev_list_entry_foreach(list_entry, udev_lst_frst) {
        const char* name = udev_list_entry_get_name(list_entry);

        udev_enumerate_unref(enumerate);
        MCU_Mode_path_str = string(name);
        MCU_Mode_path_str.append("/gamepad_mode");
        return MCU_Mode_path_str;
    }

    udev_enumerate_unref(enumerate);
    return "\0";
}

void MainWindow::update_MCU_Mode_lineEdit() {
    // Update the value of MCU_Mode_lineEdit
    MCU_Mode_path_str.clear();
    MCU_Mode_test_str.clear();
    MCU_Mode_str.clear();
    /* create udev object */
    Find_MCU_Mode_test_res = init_platform(&Find_MCU_Mode_test.platform);
    printf("The MCU Mode test result is: %d\n", Find_MCU_Mode_test_res);
    //printf("MCU Mode path if it was found is here %s\n", MCU_Mode_test_str.c_str());
    if (Find_MCU_Mode_test_res == 0) {
        printf("MCU finding platform correctly initialized\n");
    }
    if(!MCU_Mode_path_str.empty()){
        MCU_Mode_test_str.append("cat ");
        MCU_Mode_test_str.append(MCU_Mode_path_str);
    }
    MCU_Mode_str = Get_tdp_Info(MCU_Mode_test_str.c_str());
    if (!MCU_Mode_str.empty() && MCU_Mode_str.back() == '\n') {
        MCU_Mode_str.pop_back(); // Remove the last character
    }
    if(MCU_Mode_str == "1"){
        MCU_Mode = "GamePad Mode";
    }
    if(MCU_Mode_str == "2"){
        MCU_Mode = "WASD Mode";
    }
    if(MCU_Mode_str == "3"){
        MCU_Mode = "Mouse Mode";
    }
    if((MCU_Mode_str != "1") && (MCU_Mode_str != "2") && (MCU_Mode_str != "3")){
        MCU_Mode = "Unknown Mode";
    }
    ui->MCU_Mode_lineEdit->setText(MCU_Mode);
}

int MainWindow::init_platform(rc71l_platform_t *const platform) {
    platform->udev = {};
    dev_name.clear();

        /* create udev object */
        platform->udev = udev_new();
        if (platform->udev == NULL) {
            fprintf(stderr, "Cannot create udev context: mode switch will not be available.\n");
            platform->mode = -1;
            return 1;
        }

        dev_name = find_device(platform->udev);
        if (dev_name.empty()) {
            fprintf(stderr, "Cannot locate asus-mcu device: mode switch will not be available.\n");
            platform->mode = -1;
            return 1;
        } else {
            printf("Asus MCU over hidraw has been found\n");
            //MCU_Mode_path_str = dev_name;
            return 0;
        }

        return 1;
}

void MainWindow::on_Refresh_pushButton_clicked()
{
    update_MCU_Mode_lineEdit();
}
