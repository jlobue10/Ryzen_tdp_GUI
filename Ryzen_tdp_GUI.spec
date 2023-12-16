%global _name   Ryzen_tdp_GUI

Name:           Ryzen_tdp_GUI
Version:        1.1.0
Release:        1%{?dist}
Summary:        Small GUI for Ryzen TDP and GPU clock control

License:        GPL3
URL:            https://github.com/jlobue10/Ryzen_tdp_GUI
Source1:        99-amdgpu-z1e-clock.rules
Source2:        99-ryzen-tdp.rules
Source3:        ryzenadj
Source4:        Ryzen_tdp_GUI.desktop

BuildRequires:  cmake gcc-c++ qt5-qtbase-devel qt5-qttools-devel qt5-linguist
Requires:       mokutil ryzenadj
Provides:       Ryzen_tdp_GUI
Conflicts:      Ryzen_tdp_GUI

%description
Ryzen_tdp_GUI

%prep
rm -rf %{_builddir}/Ryzen_tdp_GUI
cd %{_builddir}
git clone %{url}
mkdir -p %{_builddir}/Ryzen_tdp_GUI/GUI/src/build
cp -f %{_builddir}/Ryzen_tdp_GUI/{99-amdgpu-z1e-clock.rules,99-ryzen-tdp.rules,ryzenadj,Ryzen_tdp_GUI.desktop} $RPM_SOURCE_DIR

%build
cd %{_builddir}/Ryzen_tdp_GUI/GUI/src/build
cmake ..
make

%install
mkdir -p %{buildroot}/usr/bin
cp %{_builddir}/Ryzen_tdp_GUI/GUI/src/build/Ryzen_tdp_GUI %{buildroot}/usr/bin/Ryzen_tdp_GUI

mkdir -p %{buildroot}/usr/lib/udev/rules.d
mkdir -p %{buildroot}/usr/share/applications
mkdir -p %{buildroot}/etc/sudoers.d

install -m 644 %{SOURCE1} %{buildroot}/usr/lib/udev/rules.d
install -m 644 %{SOURCE2} %{buildroot}/usr/lib/udev/rules.d
install -m 644 %{SOURCE3} %{buildroot}/etc/sudoers.d
install -m 777 %{SOURCE4} %{buildroot}/usr/share/applications

%post
sed -i "s@\$USER@${SUDO_USER}@g" /etc/sudoers.d/ryzenadj
sed -i "s@\$HOME@/home/${SUDO_USER}@g" /usr/share/applications/Ryzen_tdp_GUI.desktop
udevadm control --reload-rules
udevadm trigger

%files
/usr/lib/udev/rules.d/99-amdgpu-z1e-clock.rules
/usr/lib/udev/rules.d/99-ryzen-tdp.rules
/usr/bin/Ryzen_tdp_GUI
/usr/share/applications/Ryzen_tdp_GUI.desktop
/etc/sudoers.d/ryzenadj

%changelog
* Wed Dec 6 2023 Jon LoBue <jlobue10@gmail.com> [1.1.0-1]
- Initial package
