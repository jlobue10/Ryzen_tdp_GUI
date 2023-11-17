#!/bin/bash
# A simple script to install the rEFInd customization GUI

cd $HOME
rm -rf ./Ryzen_tdp_GUI/
git clone https://github.com/jlobue10/Ryzen_tdp_GUI
cd Ryzen_tdp_GUI
CURRENT_WD=$(pwd) 2>/dev/null
mkdir -p $HOME/.local/Ryzen_tdp_GUI
cp Ryzen_tdp_GUI.ini $HOME/.local/Ryzen_tdp_GUI
#Create file for passwordless sudo for config file, background and icon installation

yes | cp -rf $CURRENT_WD/GUI/ $HOME/.local/Ryzen_tdp_GUI 2>/dev/null

#Create .desktop icon entry. Needs cat with generic username
cat > $HOME/.local/Ryzen_tdp_GUI/GUI/Ryzen_tdp_GUI.desktop <<EOF
[Desktop Entry]
Categories=System;
Comment=Ryzen tdp GUI
Exec=/usr/bin/Ryzen_tdp_GUI
GenericName=Ryzen tdp GUI
Name=Ryzent tdp GUI
Path=/usr/bin
Icon=$HOME/.local/Ryzen_tdp_GUI/GUI/Ryzen_tdp_GUI_icon.png
StartupNotify=true
Terminal=false
Type=Application
X-KDE-SubstituteUID=false
EOF

chmod +x $HOME/.local/Ryzen_tdp_GUI/GUI/Ryzen_tdp_GUI.desktop 2>/dev/null

which dnf 2>/dev/null
FEDORA_BASE=$?

cat /etc/nobara-release
NOBARA=$?

if [ $FEDORA_BASE == 0 ]; then
	echo -e '\nFedora based installation starting.\n'
	sudo dnf install -y cmake gcc-c++ mokutil qt5-qtbase-devel qt5-qttools-devel qt5-linguist ryzenadj
  	mkdir -p $HOME/rpmbuild/{SPECS,SOURCES}
	cp Ryzen_tdp_GUI.spec $HOME/rpmbuild/SPECS
	rpmbuild -bb --target=x86_64 $HOME/rpmbuild/SPECS/Ryzen_tdp_GUI.spec
 	sudo dnf list --installed | grep Ryzen_tdp_GUI
  	STATUS=$?
   	if [ $STATUS == 0 ]; then
    		sudo dnf remove -y Ryzen_tdp_GUI
	fi
 	sudo dnf install -y $HOME/rpmbuild/RPMS/x86_64/Ryzen_tdp_GUI*.rpm
fi

which apt 2>/dev/null
UBUNTU_BASE=$?

if [ $UBUNTU_BASE == 0 ]; then
	echo -e '\nUbuntu based installation starting.\n'
	#sudo apt-get update && sudo apt-get install build deps
fi

which pacman 2>/dev/null
ARCH_BASE=$?

cat /etc/os-release | grep ChimeraOS
CHIMERA_BASE=$?

if [ $ARCH_BASE == 0 ]; then
	echo -e '\nArch based installation starting.\n'
	if [ $CHIMERA_BASE == 0 ]; then
        	sudo frzr-unlock
	fi
 	sudo pacman -Sy --noconfirm base-devel cmake make clang gc jsoncpp cppdap libisl libmpc rhash guile gcc
	sudo pacman -Sy --noconfirm lib32-glibc glibc linux-api-headers qt5-base qt5-translations qt5-tools mokutil
 
 	cat > $HOME/.local/Ryzen_tdp_GUI/ryzenadj <<EOF
	$USER ALL = NOPASSWD: /usr/bin/ryzenadj, /usr/sbin/ryzenadj
EOF
	sudo cp -f $HOME/.local/Ryzen_tdp_GUI/ryzenadj /etc/sudoers.d 2>/dev/null
	sudo cp $CURRENT_WD/{99-amdgpu-z1e-clock.rules,99-ryzen-tdp.rules} /usr/lib/udev/rules.d
	sudo udevadm control --reload-rules && sudo udevadm trigger
 	cd $HOME/.local/Ryzen_tdp_GUI/GUI/src 2>/dev/null
	mkdir -p build 2>/dev/null
	cd build 2>/dev/null
	cmake ..
	make
 	# Move compiled Ryzen_tdp_GUI binary into GUI folder
	cp Ryzen_tdp_GUI /usr/bin/Ryzen_tdp_GUI 2>/dev/null
 	sudo cp $HOME/.local/Ryzen_tdp_GUI/GUI/Ryzen_tdp_GUI.desktop /usr/share/applications 2>/dev/null
fi

SYSPATH=`find /sys/devices -name pp_od_clk_voltage 2>/dev/null | sed 's|/pp_od_clk_voltage||g' |head -n1` && echo $SYSPATH
sudo chmod 666 $SYSPATH/{power_dpm_force_performance_level,pp_od_clk_voltage}
sudo chmod 222 /sys/devices/platform/asus-nb-wmi/{ppt_fppt,ppt_pl1_spl,ppt_pl2_sppt}

if [ ! -f /usr/bin/Ryzen_tdp_GUI ]; then
	echo -e "\nGUI installation failed...\n"
	exit 1
fi

cp /usr/share/applications/Ryzen_tdp_GUI.desktop $HOME/Desktop/Ryzen_tdp_GUI.desktop
chmod +x $HOME/Desktop/Ryzen_tdp_GUI.desktop
rm -rf $HOME/.local/Ryzen_tdp_GUI/GUI/src
