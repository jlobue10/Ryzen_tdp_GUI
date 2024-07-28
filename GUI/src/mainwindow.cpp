#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <cstring>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
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
bool CPU_Boost_bool;
float fast_boost_float;
float slow_boost_float;
int Find_MCU_Mode_test_res;
int GPU_Clock_value_int = 2200;
int slow_boost_int;
int fast_boost_int;
int tdp_value_int;
int Update_Num;
int VERSION = 130;
const char* ALLY_type_checker_cmd = "cat /sys/devices/virtual/dmi/id/board_name";
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
const char* asusctl_check = "which asusctl | xargs -n 1 basename";
const char* MCU_Mode_test = "cat /sys/bus/usb/devices/1-3:1.0/0003:0B05:1ABE.0001/gamepad_mode";
const char* tdp_value_sb = "cat /sys/class/firmware-attributes/asus-armoury/attributes/ppt_pl1_spl/current_value";
const char* fast_boost_sb = "cat /sys/class/firmware-attributes/asus-armoury/attributes/ppt_fppt/current_value";
const char* slow_boost_sb = "cat /sys/class/firmware-attributes/asus-armoury/attributes/ppt_pl2_sppt/current_value";
const char* CPU_Boost_status = "cat /sys/devices/system/cpu/cpufreq/policy0/boost";
string throttle_balanced = "echo 0 | tee /sys/devices/platform/asus-nb-wmi/throttle_thermal_policy";
string throttle_performance = "echo 1 | tee /sys/devices/platform/asus-nb-wmi/throttle_thermal_policy";
string throttle_quiet = "echo 2 | tee /sys/devices/platform/asus-nb-wmi/throttle_thermal_policy";
string throttle_balanced_asusctl = "asusctl profile -P Balanced";
string throttle_performance_asusctl = "asusctl profile -P Performance";
string throttle_quiet_asusctl = "asusctl profile -P Quiet";
string power_governor = "echo \"powersave\" | tee /sys/devices/system/cpu/cpufreq/policy*/scaling_governor";
string perform_governor = "echo \"performance\" | tee /sys/devices/system/cpu/cpufreq/policy*/scaling_governor";
string epp_balance_perform = "echo \"balance_performance\" | tee /sys/devices/system/cpu/cpufreq/policy*/energy_performance_preference";
string epp_balance_power = "echo \"balance_power\" | tee /sys/devices/system/cpu/cpufreq/policy*/energy_performance_preference";
string epp_perform = "echo \"performance\" | tee /sys/devices/system/cpu/cpufreq/policy*/energy_performance_preference";
string epp_power = "echo \"power\" | tee /sys/devices/system/cpu/cpufreq/policy*/energy_performance_preference";
string set_gamepad_mode = "echo 1 | tee /sys/bus/usb/devices/1-3:1.0/0003:0B05:1ABE.0001/gamepad_mode";
string set_wasd_mode = "echo 2 | tee /sys/bus/usb/devices/1-3:1.0/0003:0B05:1ABE.0001/gamepad_mode";
string set_mouse_mode = "echo 3 | tee /sys/bus/usb/devices/1-3:1.0/0003:0B05:1ABE.0001/gamepad_mode";
string apply_MCU_settings_change = "echo 1 | tee /sys/bus/usb/devices/1-3:1.0/0003:0B05:1ABE.0001/apply";
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
QString MCU_Mode_Change_check;
QString user_home_path_q;
QString Ryzen_tdp_debug_out;
QString Scaling_Governor;
QString EPP_setting;
string asusctl_check_res;
string CPU_Boost_str;
string GPU_auto_mode = "echo \"auto\" > \"";
string GPU_manual_mode = "echo \"manual\" > \"";
string GPU_mode_select_str;
string GPU_SYSPATH_str;
string gpu_clock_value_str;
string slow_boost_str;
string fast_boost_str;
string MCU_Mode_str;
string Ryzen_gpu_command_str;
string Ryzen_tdp_command_str;
string Secure_Boot_status_str;
string tdp_display_info;
string tdp_info_disp_temp;
string tdp_info_str;
string tdp_USER = getlogin();
string tdp_value_str;
string thermal_policy;
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
    // Connect the button's clicked signal to the MainWindow's close slot
    connect(ui->exit_pushButton, &QPushButton::clicked, this, &MainWindow::close);
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
            slow_boost_value = "31";
            fast_boost_value = "36";
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
    set_thermal_policy(tdp_value_int);
    set_scaling_governor();
    set_energy_performance_pref();
    set_cpu_boost();
    tdp_value_str = to_string(tdp_value_int);
    Ryzen_tdp_command_str.clear();
    Ryzen_tdp_command_str.append("echo ");
    Ryzen_tdp_command_str.append(fast_boost_str);
    Ryzen_tdp_command_str.append(" | tee /sys/class/firmware-attributes/asus-armoury/attributes/ppt_fppt/current_value");
    Ryzen_tdp_command(Ryzen_tdp_command_str);
    Ryzen_tdp_command_str.clear();
    Ryzen_tdp_command_str.append("echo ");
    Ryzen_tdp_command_str.append(slow_boost_str);
    Ryzen_tdp_command_str.append(" | tee /sys/class/firmware-attributes/asus-armoury/attributes/ppt_pl2_sppt/current_value");
    Ryzen_tdp_command(Ryzen_tdp_command_str);
    Ryzen_tdp_command_str.clear();
    Ryzen_tdp_command_str.append("echo ");
    Ryzen_tdp_command_str.append(tdp_value_str);
    Ryzen_tdp_command_str.append(" | tee /sys/class/firmware-attributes/asus-armoury/attributes/ppt_pl1_spl/current_value");
    Ryzen_tdp_command(Ryzen_tdp_command_str);
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

void MainWindow::on_CPU_Boost_checkBox_toggled(bool checked)
{
    set_cpu_boost();
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
    tdp_info_Box.setTextFormat(Qt::RichText);
    Secure_Boot_status_str.clear();
    Secure_Boot_status_str = Get_tdp_Info(Secure_Boot_status);
    if (!Secure_Boot_status_str.empty() && Secure_Boot_status_str.back() == '\n') {
        Secure_Boot_status_str.pop_back(); // Remove the last character
    }
    // show tdp settings info a different way if Secure Boot is enabled...
    if(Secure_Boot_status_str == "SecureBoot enabled"){
        /* Secure boot can work with ryzenadj, even with info
         * if kernel is compiled with CONFIG_SECURITY_LOCKDOWN_LSM_EARLY
         * disabled. Commenting out this portion as unnecessary for now
         * as of Feb 2024.
         */
        // Use same method as secure boot disabled for now
        /*
        tdp_display_info.append("<p style='color:green;'>Secure Boot Enabled</p>");
        tdp_display_info.append("<p align='left'>Sustained TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_value_sb);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Fast TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(fast_boost_sb);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Slow TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(slow_boost_sb);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("</p>");
        */
        tdp_display_info.append("<p style='color:green;'>Secure Boot Enabled</p>");
        tdp_display_info.append("<p align='left'>Sustained TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_limit_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Sustained TDP value: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Fast TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_fast_lim_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Fast TDP value: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_fast_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Slow TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_slow_lim_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Slow TDP value: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_slow_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("</p>");
    }else {
        tdp_display_info.append("<p style='color:red;'>Secure Boot Disabled</p>");
        tdp_display_info.append("<p align='left'>Sustained TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_limit_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Sustained TDP value: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Fast TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_fast_lim_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Fast TDP value: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_fast_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Slow TDP setting: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_slow_lim_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("<br>Slow TDP value: ");
        tdp_info_disp_temp = Get_tdp_Info(tdp_slow_value_search);
        tdp_display_info.append(tdp_info_disp_temp);
        tdp_display_info.append("</p>");
    }
    tdp_display_info.append("<p align='left'>AMD Pstate status: ");
    tdp_info_disp_temp = Get_tdp_Info(amd_pstate_status);
    tdp_display_info.append(tdp_info_disp_temp);
    tdp_display_info.append("<br>Scaling Driver: ");
    tdp_info_disp_temp = Get_tdp_Info(scaling_driver);
    tdp_display_info.append(tdp_info_disp_temp);
    tdp_display_info.append("<br>Scaling Governor: ");
    tdp_info_disp_temp = Get_tdp_Info(scaling_governor);
    tdp_display_info.append(tdp_info_disp_temp);
    tdp_display_info.append("<br>EPP setting in use: ");
    tdp_info_disp_temp = Get_tdp_Info(energy_performance_preference);
    tdp_display_info.append(tdp_info_disp_temp);
    tdp_display_info.append("<br>Thermal policy: ");
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
    CPU_Boost_str = Get_tdp_Info(CPU_Boost_status);
    tdp_display_info.append("<br>CPU Boost ");
    if (!CPU_Boost_str.empty() && CPU_Boost_str.back() == '\n') {
        CPU_Boost_str.pop_back(); // Remove the last character
    }
    if(CPU_Boost_str == "1"){
        tdp_display_info.append("Enabled");
    }
    else
    {
        tdp_display_info.append("Disabled");
    }
    tdp_display_info.append("</p>");
    QString tdp_info_QString = QString::fromStdString(tdp_display_info);
    tdp_info_Box.setText(tdp_info_QString);
    tdp_info_Box.setStandardButtons(QMessageBox::Ok);
    tdp_info_Box.exec();
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
    bool temp_Boost_bool = settings.value("Boost_checkBox").toBool();
    bool temp_CPU_Boost_bool = settings.value("CPU_Boost_checkBox").toBool();
    bool temp_GPU_Clock_bool = settings.value("GPU_Clock_checkBox").toBool();
    bool temp_Smoke_bool = settings.value("Smoke_checkBox").toBool();
    settings.endGroup();
    settings.beginGroup("ComboBoxes");
    int EPP_int = settings.value("EPP_ComboBox").toInt();
    int Governor_int = settings.value("Governor_ComboBox").toInt();
    int Mode_Change_int = settings.value("MCU_Mode_Change").toInt();
    settings.endGroup();
    settings.beginGroup("Values");
    QString tempTDP_Slider = settings.value("apu_tdp_slider").toString();
    QString tempFast = settings.value("fast_tdp").toString();
    QString tempGPU_Slider = settings.value("gpu_clock_slider").toString();
    QString tempSlow = settings.value("slow_tdp").toString();
    settings.endGroup();
    ui->Smoke_checkBox->setChecked(temp_Smoke_bool);
    ui->Boost_checkBox->setChecked(temp_Boost_bool);
    ui->CPU_Boost_checkBox->setChecked(temp_CPU_Boost_bool);
    ui->GPU_Clock_checkBox->setChecked(temp_GPU_Clock_bool);
    ui->slow_tdp_lineEdit->setText(tempSlow);
    ui->fast_tdp_lineEdit->setText(tempFast);
    ui->tdp_lineEdit->setText(tempTDP_Slider);
    ui->tdp_Slider->setValue(tempTDP_Slider.toInt(&ok));
    ui->GPU_Clock_Slider->setValue(tempGPU_Slider.toInt(&ok));
    ui->GPU_Clock_lineEdit->setText(tempGPU_Slider);
    ui->Governor_comboBox->setCurrentIndex(Governor_int);
    ui->EPP_comboBox->setCurrentIndex(EPP_int);
    ui->Mode_Change_comboBox->setCurrentIndex(Mode_Change_int);
}

void MainWindow::writeSettings()
{
    QSettings settings(settings_path, QSettings::NativeFormat);
    settings.beginGroup("CheckBoxes");
    settings.setValue("Boost_checkBox", ui->Boost_checkBox->isChecked());
    settings.setValue("CPU_Boost_checkBox", ui->CPU_Boost_checkBox->isChecked());
    settings.setValue("GPU_Clock_checkBox", ui->GPU_Clock_checkBox->isChecked());
    settings.setValue("Smoke_checkBox", ui->Smoke_checkBox->isChecked());
    settings.endGroup();
    settings.beginGroup("ComboBoxes");
    settings.setValue("EPP_ComboBox", ui->EPP_comboBox->currentIndex());
    settings.setValue("Governor_ComboBox", ui->Governor_comboBox->currentIndex());
    settings.setValue("MCU_Mode_Change", ui->Mode_Change_comboBox->currentIndex());
    settings.endGroup();
    settings.beginGroup("Values");
    settings.setValue("apu_tdp_slider", ui->tdp_Slider->value());
    settings.setValue("fast_tdp", ui->fast_tdp_lineEdit->text());
    settings.setValue("gpu_clock_slider", ui->GPU_Clock_Slider->value());
    settings.setValue("slow_tdp", ui->slow_tdp_lineEdit->text());
    settings.endGroup();
}

void MainWindow::on_About_pushButton_clicked()
{
    QMessageBox AboutBox;
    QPushButton* updateButton = new QPushButton("Check For Update");
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_Clicked);
    AboutBox.setTextFormat(Qt::RichText);
    AboutBox.setText("<p align='center'>Ryzen tdp GUI v1.3.0<br><br>"
                     "Original GUI Creator: "
                     "<a href='https://github.com/jlobue10'>jlobue10</a><br><br>"
                     "Special Thanks to Nobara Discord for testing and QA</p>");
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

void MainWindow::update_MCU_Mode_lineEdit() {
    MCU_Mode_str.clear();
    MCU_Mode_str = Get_tdp_Info(MCU_Mode_test);
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

void MainWindow::on_Refresh_pushButton_clicked()
{
    update_MCU_Mode_lineEdit();
}

void MainWindow::set_thermal_policy(int thermal_policy_int)
{
    asusctl_check_res = Get_tdp_Info(asusctl_check);
    if (!asusctl_check_res.empty() && asusctl_check_res.back() == '\n') {
        asusctl_check_res.pop_back(); // Remove the last character
    }
    if(thermal_policy_int < 9) {
        if(asusctl_check_res == "asusctl") {
            printf("Using Asusctl to set thermal policy\n");
            Ryzen_tdp_command(throttle_quiet_asusctl);
        } else {
            Ryzen_tdp_command(throttle_quiet);
        }
    }
    if((thermal_policy_int >= 9) && (tdp_value_int < 19)) {
        if(asusctl_check_res == "asusctl") {
            printf("Using Asusctl to set thermal policy\n");
            Ryzen_tdp_command(throttle_balanced_asusctl);
        } else {
            Ryzen_tdp_command(throttle_balanced);
        }
    }
    if(thermal_policy_int >= 19) {
        if(asusctl_check_res == "asusctl") {
            printf("Using Asusctl to set thermal policy\n");
            Ryzen_tdp_command(throttle_performance_asusctl);
        } else {
            //printf("$s\n", asusctl_check_res.c_str());
            Ryzen_tdp_command(throttle_performance);
        }
    }
}

void MainWindow::set_cpu_boost()
{
    Ryzen_tdp_command_str.clear();
    CPU_Boost_bool = ui->CPU_Boost_checkBox->isChecked();
    if(CPU_Boost_bool){
        Ryzen_tdp_command_str.append("echo 1 | tee /sys/devices/system/cpu/cpufreq/policy0/boost");
    }else {
        Ryzen_tdp_command_str.append("echo 0 | tee /sys/devices/system/cpu/cpufreq/policy0/boost");
    }
    Ryzen_tdp_command(Ryzen_tdp_command_str);
}

void MainWindow::set_scaling_governor()
{
    Scaling_Governor = ui->Governor_comboBox->currentText();
    if(Scaling_Governor == "powersave")
        Ryzen_tdp_command(power_governor);
    if(Scaling_Governor == "performance")
        Ryzen_tdp_command(perform_governor);
}

void MainWindow::set_energy_performance_pref()
{
    EPP_setting = ui->EPP_comboBox->currentText();
    if(EPP_setting == "balance_performance")
        Ryzen_tdp_command(epp_balance_perform);
    if(EPP_setting == "balance_power")
        Ryzen_tdp_command(epp_balance_power);
    if(EPP_setting == "performance")
        Ryzen_tdp_command(epp_perform);
    if(EPP_setting == "power")
        Ryzen_tdp_command(epp_power);
}

void MainWindow::on_Mode_Change_Apply_pushButton_clicked()
{
    MCU_Mode_Change_check = ui->Mode_Change_comboBox->currentText();
    if(MCU_Mode_Change_check == "GamePad")
        Ryzen_tdp_command(set_gamepad_mode);
    if(MCU_Mode_Change_check == "WASD")
        Ryzen_tdp_command(set_wasd_mode);
    if(MCU_Mode_Change_check == "Mouse")
        Ryzen_tdp_command(set_mouse_mode);
    Ryzen_tdp_command(apply_MCU_settings_change);
}
