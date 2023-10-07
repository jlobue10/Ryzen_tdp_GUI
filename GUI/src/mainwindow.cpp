#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <QIntValidator>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QString>
#include <sstream>
#include <string>
#include <unistd.h>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ostringstream;
using std::stoi;
using std::string;
using std::to_string;

bool Smoke_bool;
bool Boost_bool;
bool GPU_Clock_bool;
int GPU_Clock_value = 2200;
int tdp_value_int;
int Update_Num;
int VERSION = 100;
ostringstream user_home_path;
QIntValidator slow_INT_validator(5, 44);
QIntValidator fast_INT_validator(5, 53);
QIntValidator gpu_INT_validator(800, 2700);
QIntValidator tdp_INT_validator(5, 30);
QString gpu_clock_value;
QString settings_path;
QString settings_path_suffix{"/.local/Ryzen_tdp/Ryzen_tdp.ini"};
QString tdp_value;
QString user_home_path_q;
string gpu_clock_value_str;
string Ryzen_tdp_command_str;
string tdp_USER = getlogin();
string tdp_value_str;
string user_home_path_str;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->slow_tdp_lineEdit->setValidator(&slow_INT_validator);
    ui->fast_tdp_lineEdit->setValidator(&fast_INT_validator);
    ui->GPU_Clock_lineEdit->setValidator(&gpu_INT_validator);
    ui->tdp_lineEdit->setValidator(&tdp_INT_validator);
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
    tdp_value_str = tdp_value.toStdString();
    tdp_value_int = stoi(tdp_value_str);
    Ryzen_tdp_command_str.clear();
    Ryzen_tdp_command_str.append("sudo ryzenadj");
    switch(tdp_value_int){
        case 5:
            Ryzen_tdp_command_str.append(" --stapm-limit=5000 --fast-limit=7000 --slow-limit=6000 --tctl-temp=85 --max-gfxclk=2900");
            break;
        case 6:
            Ryzen_tdp_command_str.append(" --stapm-limit=6000 --fast-limit=9000 --slow-limit=7500 --tctl-temp=95 --max-gfxclk=2900");
            break;
        case 7:
            Ryzen_tdp_command_str.append(" --stapm-limit=7000 --fast-limit=11000 --slow-limit=9000 --tctl-temp=95 --max-gfxclk=2900");
            break;

    }
}

void MainWindow::on_GPU_Apply_pushButton_clicked()
{

}

void MainWindow::on_Smoke_checkBox_toggled(bool checked)
{
    Smoke_bool = ui->Smoke_checkBox->isChecked();
    if (Smoke_bool){
        ui->tdp_Slider->setMaximum(44);
        tdp_INT_validator.setRange(5,44);
        ui->tdp_lineEdit->setValidator(&tdp_INT_validator);
    } else {
        ui->tdp_Slider->setMaximum(30);
        tdp_INT_validator.setRange(5,30);
        ui->tdp_lineEdit->setValidator(&tdp_INT_validator);
    }

}

void MainWindow::on_GPU_Clock_checkBox_toggled(bool checked)
{
    GPU_Clock_bool = ui->GPU_Clock_checkBox->isChecked();
    if (!GPU_Clock_bool){
        /* reset gfxmaxclk to clear manual GPU clock setting */
        on_tdp_Apply_pushButton_clicked();\
    }

}

void MainWindow::Ryzen_tdp_command(string Ryzen_command)
{

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
    QString tempTDP = settings.value("apu_tdp").toString();
    QString tempGPU = settings.value("gpu_clock").toString();
    QString tempTDP_Slider = settings.value("apu_tdp_slider").toString();
    QString tempGPU_Slider = settings.value("gpu_clock_slider").toString();
    settings.endGroup();
    ui->Smoke_checkBox->setChecked(temp_Smoke_bool);
    ui->Boost_checkBox->setChecked(temp_Boost_bool);
    ui->GPU_Clock_checkBox->setChecked(temp_GPU_Clock_bool);
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
    settings.setValue("apu_tdp", ui->tdp_lineEdit->text());
    settings.setValue("gpu_clock", ui->GPU_Clock_lineEdit->text());
    settings.setValue("apu_tdp_slider", ui->tdp_Slider->value());
    settings.setValue("gpu_clock_slider", ui->GPU_Clock_Slider->value());
    settings.endGroup();
}
