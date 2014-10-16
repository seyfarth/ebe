; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "ebe"
#define MyAppVersion "2.4.7"
#define MyAppPublisher "Ray Seyfarth"
#define MyAppURL "http://www.rayseyfarth.com/ebe"
#define MyAppExeName "ebe.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{E4FC70FD-5648-482C-BFE4-C2415EE8DE7F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
OutputBaseFilename={#MyAppName}64-{#MyAppVersion}-setup
Compression=lzma
SolidCompression=yes
ChangesEnvironment=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\cygwin\home\seyfarth\ebe\release\{#MyAppExeName}"; DestDir: "{app}\gcc\bin"; Flags: ignoreversion
Source: "C:\cygwin\home\seyfarth\ebe\*.qm"; DestDir: "{app}\gcc\bin"; Flags: ignoreversion
Source: "C:\cygwin\home\seyfarth\ebe\ebe.ico"; DestDir: "{app}\gcc\bin"; Flags: ignoreversion
Source: "C:\gcc\*"; DestDir: "{app}\gcc"; Flags: ignoreversion recursesubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files   

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\gcc\bin\{#MyAppExeName}"; WorkingDir: "{%USERPROFILE}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\gcc\bin\{#MyAppExeName}"; IconFilename: "{app}\gcc\bin\ebe.ico"; Tasks: desktopicon; WorkingDir: "{%USERPROFILE}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

;[Run]
;Filename: "{app}\gcc\bin\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: shellexec skipifsilent 

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{app}\gcc\bin;{olddata}"; Check: NeedsAddPath('{app}\gcc\bin')
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "EQ_LIBRARY_PATH"; ValueData: "{app}\gcc\lib\gcc\x86_64-w64-mingw32"
;Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "EBE_HOME"; ValueData: "{app}"

[Code]

function NeedsAddPath(Param: string): boolean;
var
  OrigPath: string;
begin
  if not RegQueryStringValue(HKEY_LOCAL_MACHINE,
    'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
    'Path', OrigPath)
  then begin
    Result := True;
    exit;
  end;
  // look for the path with leading and trailing semicolon
  // Pos() returns 0 if not found
  Result := Pos(';' + Param + ';', ';' + OrigPath + ';') = 0;
end;
