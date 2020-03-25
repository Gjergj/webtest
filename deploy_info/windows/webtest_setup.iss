; -- Example3.iss --
; Same as Example1.iss, but creates some registry entries too.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=WebTest
AppVersion=0.017
DefaultDirName={pf}\WebTest
DefaultGroupName=WebTest
UninstallDisplayIcon={app}\WebTest.exe
;OutputDir=userdocs:Inno Setup Examples Output
OutputDir=C:\Users\1\Desktop\webtestDeploy
ArchitecturesInstallIn64BitMode=x64


[Files]
Source: "C:\Users\1\Desktop\webtestDeploy\npcap-0.86.exe"; DestDir: "{app}"; AfterInstall: RunOtherInstaller
Source: "C:\Users\1\Desktop\webtestDeploy\webtest.exe"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\msvcdis140.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\msvcp140.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5Core.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5Gui.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5Network.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5Widgets.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\ssleay32.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\vcruntime140.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\libeay32.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\D3Dcompiler_47.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5WebEngineWidgets.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5WebEngineCore.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5WebChannel.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5SerialPort.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5PrintSupport.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5Positioning.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5PrintSupport.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\QtWebEngineProcess.exe"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\libEGL.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\libGLESV2.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\opengl32sw.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5Qml.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5Quick.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5QuickWidgets.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5Svg.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5QuickControls2.dll"; DestDir: "{app}"
Source: "C:\Users\1\Desktop\webtestDeploy\Qt5QuickTemplates2.dll"; DestDir: "{app}"

Source: "C:\Users\1\Desktop\webtestDeploy\cert\ca.pem"; DestDir: "{app}\cert"
Source: "C:\Users\1\Desktop\webtestDeploy\cert\dh2048.pem"; DestDir: "{app}\cert"
Source: "C:\Users\1\Desktop\webtestDeploy\cert\server.pem"; DestDir: "{app}\cert"

Source: "C:\Users\1\Desktop\webtestDeploy\Universal\*"; DestDir: "{app}\Universal"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\bearer\*"; DestDir: "{app}\bearer"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\Material\*"; DestDir: "{app}\Material"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\platforminputcontexts\*"; DestDir: "{app}\platforminputcontexts"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\private\*"; DestDir: "{app}\private"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\qmltooling\*"; DestDir: "{app}\qmltooling"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\QtGraphicalEffects\*"; DestDir: "{app}\QtGraphicalEffects"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\QtQml\*"; DestDir: "{app}\QtQml"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\QtQuick\*"; DestDir: "{app}\QtQuick"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\QtQuick.2\*"; DestDir: "{app}\QtQuick.2"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\scenegraph\*"; DestDir: "{app}\scenegraph"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\resources\*"; DestDir: "{app}\resources"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\printsupport\*"; DestDir: "{app}\printsupport"; Flags: ignoreversion recursesubdirs
Source: "C:\Users\1\Desktop\webtestDeploy\position\*"; DestDir: "{app}\position"; Flags: ignoreversion recursesubdirs
;Source: "Readme.txt"; DestDir: "{app}"; Flags: isreadme

[Dirs]
Name: "{commonappdata}\WebTest"

[Code]
procedure RunOtherInstaller;
var
  ResultCode: Integer;
begin
  if not Exec(ExpandConstant('{app}\npcap-0.86.exe'), '', '', SW_SHOWNORMAL,
    ewWaitUntilTerminated, ResultCode)
  then
    MsgBox('Other installer failed to run!' + #13#10 +
      SysErrorMessage(ResultCode), mbError, MB_OK);
end;

[Icons]
Name: "{group}\WebTest"; Filename: "{app}\WebTest.exe"

; NOTE: Most apps do not need registry entries to be pre-created. If you
; don't know what the registry is or if you need to use it, then chances are
; you don't need a [Registry] section.

[Registry]
; Start "Software\My Company\My Program" keys under HKEY_CURRENT_USER
; and HKEY_LOCAL_MACHINE. The flags tell it to always delete the
; "My Program" keys upon uninstall, and delete the "My Company" keys
; if there is nothing left in them.
;Root: HKCU; Subkey: "Software\My Company"; Flags: uninsdeletekeyifempty
;Root: HKCU; Subkey: "Software\My Company\My Program"; Flags: uninsdeletekey
;Root: HKLM; Subkey: "Software\My Company"; Flags: uninsdeletekeyifempty
;Root: HKLM; Subkey: "Software\My Company\My Program"; Flags: uninsdeletekey
;Root: HKLM; Subkey: "Software\My Company\My Program\Settings"; ValueType: string; ValueName: "Path"; ValueData: "{app}"
