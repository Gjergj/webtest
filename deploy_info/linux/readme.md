Bazuar tek AppImageKit

--create release directory

mkdir linux_x64_release

--copy linuxdeployqt-continuous-x86_64.AppImage tek linux_x64_release

cp linux_x64_release linux_x64_release

--copy webtest release binary to linux_x64_release

--ekzekuto tek linux_x64_release 

cd linux_x64_release

--path to correct qmake intallation
export PATH=/home/gjergji/Qt5.8.0/5.8/gcc_64/bin/:$PATH

--create AppImage

./linuxdeployqt-continuous-x86_64.AppImage webtest -verbose=2 -appimage -no-strip -bundle-non-qt-libs -qmldir=/home/gjergji/Qt5.8.0/5.8/gcc_64/qml

--kujdes linuxdeployqt nuk kopjon librarite qe nuk jane te instaluara ne sistem si psh. libboost_system.so.1.63.0

--kujdes linuxdeployqt kopjon versionet e librarive qe jane te instaluara ne sistem. Nese ne sistem jane te instaluara versione te librarive te ndryshme nga ato qe jane te linkuara ne file binar atehere dalin probleme gjate ekzekutimit. Duhen kopjuar manualish versionet e librarive Qt dhe te tjera brenda linux_x64_release/lib

--cpld.bash gjen te gjitha varesite e file binar qe jane te instaluara ne sistem the i kopjon lokalisht

--file i gjeneruar Application-x86_64.AppImage eshte programi qe do shperndajme
