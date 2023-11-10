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
cat > $HOME/.local/Ryzen_tdp_GUI/ryzenadj <<EOF
$USER ALL = NOPASSWD: /usr/bin/ryzenadj, /usr/sbin/ryzenadj
EOF

cat > $HOME/.local/Ryzen_tdp_GUI/tee <<EOF
$USER ALL = NOPASSWD: /usr/bin/tee
EOF

sudo cp -f $HOME/.local/Ryzen_tdp_GUI/ryzenadj /etc/sudoers.d 2>/dev/null
sudo cp -f $HOME/.local/Ryzen_tdp_GUI/tee /etc/sudoers.d 2>/dev/null

yes | cp -rf $CURRENT_WD/GUI/ $HOME/.local/Ryzen_tdp_GUI 2>/dev/null

which dnf 2>/dev/null
FEDORA_BASE=$?

cat /etc/nobara-release
NOBARA=$?

if [ $FEDORA_BASE == 0 ]; then
	echo -e '\nFedora based installation starting.\n'
		sudo dnf install cmake gcc-c++ mokutil qt5-qtbase-devel qt5-qttools-devel qt5-linguist ryzenadj
  		#sudo dnf install qt6-qtbase-devel qt6-qttools-devel
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
 	sudo pacman -Sy --noconfirm --needed archlinux-keyring autoconf automake binutils bison debugedit fakeroot file findutils flex gawk gcc gettext\
 grep groff gzip libtool m4 make pacman patch pkgconf sed sudo texinfo which
	sudo pacman -Sy --noconfirm lib32-glibc glibc linux-api-headers qt5-base qt5-translations clang cmake gc guile libisl libmpc qt5-tools
fi

cd $HOME/.local/Ryzen_tdp_GUI/GUI/src 2>/dev/null
mkdir -p build 2>/dev/null
cd build 2>/dev/null
cmake ..
make

if [ ! -f $HOME/.local/Ryzen_tdp_GUI/GUI/src/build/Ryzen_tdp_GUI ]; then
	echo -e "\nGUI compile failed. Please try again after ensuring that your cloned repo is up to date.\n"
	exit 1
fi

# Move compiled Ryzen_tdp_GUI binary into GUI folder
cp Ryzen_tdp_GUI ../../ 2>/dev/null

#Create .desktop icon entry. Needs cat with generic username
cat > $HOME/.local/Ryzen_tdp_GUI/GUI/Ryzen_tdp_GUI.desktop <<EOF
[Desktop Entry]
Categories=System;
Comment=Ryzen tdp GUI
Exec=$HOME/.local/Ryzen_tdp_GUI/GUI/Ryzen_tdp_GUI
GenericName=Ryzen tdp GUI
Name=Ryzent tdp GUI
Path=$HOME/.local/Ryzen_tdp_GUI/GUI
Icon=$HOME/.local/Ryzen_tdp_GUI/GUI/Ryzen_tdp_GUI_icon.png
StartupNotify=true
Terminal=false
Type=Application
X-KDE-SubstituteUID=false
EOF

chmod +x $HOME/.local/Ryzen_tdp_GUI/GUI/Ryzen_tdp_GUI.desktop 2>/dev/null
sudo cp $HOME/.local/Ryzen_tdp_GUI/GUI/Ryzen_tdp_GUI.desktop /usr/share/applications 2>/dev/null
cp $HOME/.local/Ryzen_tdp_GUI/GUI/Ryzen_tdp_GUI.desktop $HOME/Desktop
chmod +x $HOME/Desktop/Ryzen_tdp_GUI.desktop
			
