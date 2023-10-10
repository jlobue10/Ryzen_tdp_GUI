#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QSettings>
#include <QString>
#include <sstream>
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
int GPU_Clock_value_int = 2200;
int slow_boost_int;
int fast_boost_int;
int tdp_value_int;
int Update_Num;
int VERSION = 100;
ostringstream user_home_path;
QString gpu_clock_value;
QString settings_path;
QString settings_path_suffix{"/.local/Ryzen_tdp_GUI/Ryzen_tdp_GUI.ini"};
QString fast_boost_value = "53000";  // mW
QString fast_boost_value_display = "53";
QString slow_boost_value = "44000";  // mW
QString slow_boost_value_display = "44";
QString fast_min = "5";
QString fast_max = "53";
QString slow_min = "5";
QString slow_max = "45";
QString tdp_value;
QString user_home_path_q;
QString Ryzen_tdp_debug_out;
string gpu_clock_value_str;
string slow_boost_str;
string fast_boost_str;
string Ryzen_gpu_command_str;
string Ryzen_tdp_command_str;
string tdp_info_str;
string tdp_USER = getlogin();
string tdp_value_str;
string Update_Num_str;
string user_home_path_str;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    Ryzen_tdp_command_str.clear();
    Ryzen_tdp_command_str.append("sudo ryzenadj");
    switch(tdp_value_int){
        case 5:
            Ryzen_tdp_command_str.append(" --stapm-limit=5000 --tctl-temp=85");
            if(!Boost_bool){
                slow_boost_value = "6000";
                fast_boost_value = "7000";
                }
            break;
        case 6:
            Ryzen_tdp_command_str.append(" --stapm-limit=6000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "7500";
                fast_boost_value = "9000";
                }
            break;
        case 7:
            Ryzen_tdp_command_str.append(" --stapm-limit=7000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "9000";
                fast_boost_value = "11000";
                }
            break;
        case 8:
            Ryzen_tdp_command_str.append(" --stapm-limit=8000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "9000";
                fast_boost_value = "10538";
                }
            break;
        case 9:
            Ryzen_tdp_command_str.append(" --stapm-limit=9000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "10000";
                fast_boost_value = "11717";
                }
            break;
        case 10:
            Ryzen_tdp_command_str.append(" --stapm-limit=10000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "11000";
                fast_boost_value = "12897";
                }
            break;
        case 11:
            Ryzen_tdp_command_str.append(" --stapm-limit=11000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "12000";
                fast_boost_value = "14076";
                }
            break;
        case 12:
            Ryzen_tdp_command_str.append(" --stapm-limit=12000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "13000";
                fast_boost_value = "15256";
                }
            break;
        case 13:
            Ryzen_tdp_command_str.append(" --stapm-limit=13000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "14000";
                fast_boost_value = "16435";
                }
            break;
        case 14:
            Ryzen_tdp_command_str.append(" --stapm-limit=14000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "15000";
                fast_boost_value = "17615";
                }
            break;
        case 15:
            Ryzen_tdp_command_str.append(" --stapm-limit=15000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "16000";
                fast_boost_value = "18794";
                }
            break;
        case 16:
            Ryzen_tdp_command_str.append(" --stapm-limit=16000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "17000";
                fast_boost_value = "19974";
                }
            break;
        case 17:
            Ryzen_tdp_command_str.append(" --stapm-limit=17000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "18000";
                fast_boost_value = "21153";
                }
            break;
        case 18:
            Ryzen_tdp_command_str.append(" --stapm-limit=18000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "19000";
                fast_boost_value = "22333";
                }
            break;
        case 19:
            Ryzen_tdp_command_str.append(" --stapm-limit=19000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "20000";
                fast_boost_value = "23512";
                }
            break;
        case 20:
            Ryzen_tdp_command_str.append(" --stapm-limit=20000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "21000";
                fast_boost_value = "24692";
                }
            break;
        case 21:
            Ryzen_tdp_command_str.append(" --stapm-limit=21000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "22000";
                fast_boost_value = "25871";
                }
            break;
        case 22:
            Ryzen_tdp_command_str.append(" --stapm-limit=22000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "23000";
                fast_boost_value = "27051";
                }
            break;
        case 23:
            Ryzen_tdp_command_str.append(" --stapm-limit=23000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "24000";
                fast_boost_value = "28230";
                }
            break;
        case 24:
            Ryzen_tdp_command_str.append(" --stapm-limit=24000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "25000";
                fast_boost_value = "29410";
                }
            break;
        case 25:
            Ryzen_tdp_command_str.append(" --stapm-limit=25000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "26000";
                fast_boost_value = "30589";
                }
            break;
        case 26:
            Ryzen_tdp_command_str.append(" --stapm-limit=26000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "27000";
                fast_boost_value = "31769";
                }
            break;
        case 27:
            Ryzen_tdp_command_str.append(" --stapm-limit=27000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "28000";
                fast_boost_value = "32948";
                }
            break;
        case 28:
            Ryzen_tdp_command_str.append(" --stapm-limit=28000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "29000";
                fast_boost_value = "34128";
                }
            break;
        case 29:
            Ryzen_tdp_command_str.append(" --stapm-limit=29000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "30000";
                fast_boost_value = "35307";
                }
            break;
        case 30:
            Ryzen_tdp_command_str.append(" --stapm-limit=30000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "26000";
                fast_boost_value = "30589";
            }
            break;
        case 31:
            Ryzen_tdp_command_str.append(" --stapm-limit=31000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "32000";
                fast_boost_value = "37666";
                }
            break;
        case 32:
            Ryzen_tdp_command_str.append(" --stapm-limit=32000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "33000";
                fast_boost_value = "38846";
                }
            break;
        case 33:
            Ryzen_tdp_command_str.append(" --stapm-limit=33000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "34000";
                fast_boost_value = "40025";
                }
            break;
        case 34:
            Ryzen_tdp_command_str.append(" --stapm-limit=34000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "35000";
                fast_boost_value = "41205";
                }
            break;
        case 35:
            Ryzen_tdp_command_str.append(" --stapm-limit=35000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "36000";
                fast_boost_value = "42384";
                }
            break;
        case 36:
            Ryzen_tdp_command_str.append(" --stapm-limit=36000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "37000";
                fast_boost_value = "43564";
                }
            break;
        case 37:
            Ryzen_tdp_command_str.append(" --stapm-limit=37000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "38000";
                fast_boost_value = "44743";
                }
            break;
        case 38:
            Ryzen_tdp_command_str.append(" --stapm-limit=38000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "39000";
                fast_boost_value = "45923";
                }
            break;
        case 39:
            Ryzen_tdp_command_str.append(" --stapm-limit=39000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "40000";
                fast_boost_value = "47102";
                }
            break;
        case 40:
            Ryzen_tdp_command_str.append(" --stapm-limit=40000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "41000";
                fast_boost_value = "48282";
                }
            break;
        case 41:
            Ryzen_tdp_command_str.append(" --stapm-limit=41000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "42000";
                fast_boost_value = "49461";
                }
            break;
        case 42:
            Ryzen_tdp_command_str.append(" --stapm-limit=42000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "43000";
                fast_boost_value = "50641";
                }
            break;
        case 43:
            Ryzen_tdp_command_str.append(" --stapm-limit=43000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "44000";
                fast_boost_value = "51820";
                }
            break;
        case 44:
            Ryzen_tdp_command_str.append(" --stapm-limit=44000 --tctl-temp=95");
            if(!Boost_bool){
                slow_boost_value = "45000";
                fast_boost_value = "53000";
            }
            break;
    }
        if(Boost_bool){
            slow_boost_value = ui->slow_tdp_lineEdit->text();
            fast_boost_value = ui->fast_tdp_lineEdit->text();
            slow_boost_int = slow_boost_value.toInt(&ok);
            slow_boost_int = slow_boost_int * 1000;
            slow_boost_str = to_string(slow_boost_int);
            fast_boost_int = fast_boost_value.toInt(&ok);
            fast_boost_int = fast_boost_int * 1000;
            fast_boost_str = to_string(fast_boost_int);
        }else {
            slow_boost_float = slow_boost_value.toFloat(&ok);
            slow_boost_float = slow_boost_float / 1000.0;
            slow_boost_int = static_cast<int>(round(slow_boost_float));
            slow_boost_value_display = QString::number(slow_boost_int);
            fast_boost_float = fast_boost_value.toFloat(&ok);
            fast_boost_float = fast_boost_float / 1000.0;
            fast_boost_int = static_cast<int>(round(fast_boost_float));
            fast_boost_value_display = QString::number(fast_boost_int);
            ui->slow_tdp_lineEdit->setText(slow_boost_value_display);
            ui->fast_tdp_lineEdit->setText(fast_boost_value_display);
            slow_boost_str = slow_boost_value.toStdString();
            fast_boost_str = fast_boost_value.toStdString();
        }
        Ryzen_tdp_command_str.append(" --slow-limit=");
        Ryzen_tdp_command_str.append(slow_boost_str);
        Ryzen_tdp_command_str.append(" --fast-limit=");
        Ryzen_tdp_command_str.append(fast_boost_str);
        Ryzen_tdp_command(Ryzen_tdp_command_str);
}

void MainWindow::on_GPU_Apply_pushButton_clicked()
{
    Ryzen_gpu_command_str.clear();
    GPU_Clock_bool = ui->GPU_Clock_checkBox->isChecked();
    if (GPU_Clock_bool){
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
        Ryzen_gpu_command_str = "sudo ryzenadj --gfx-clk=";
        Ryzen_gpu_command_str.append(gpu_clock_value_str);
        Ryzen_tdp_command(Ryzen_gpu_command_str);
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
    string tdp_info_disp = Get_tdp_Info();
    QString tdp_info_QString = QString::fromStdString(tdp_info_disp);
    tdp_info_Box.setTextFormat(Qt::RichText);
    tdp_info_Box.setText(tdp_info_QString);
    tdp_info_Box.setStandardButtons(QMessageBox::Ok);
    tdp_info_Box.exec();
}

void MainWindow::on_exit_pushButton_clicked()
{
    MainWindow::~MainWindow();
}

string MainWindow::Get_tdp_Info() {
    tdp_info_str.clear();
    FILE *process;
    char buff[1024];
    process = popen("sudo ryzenadj --info | grep -e 'STAPM' -e 'PPT .* FAST' -e 'PPT .* SLOW'", "r");
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
