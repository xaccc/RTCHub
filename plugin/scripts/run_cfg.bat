@ECHO OFF
REM.-- Prepare the Command Processor
SETLOCAL ENABLEEXTENSIONS
SETLOCAL ENABLEDELAYEDEXPANSION

REM ** set the version of LIBJINGLE to fetch
SET LIBJINGLE_REV=303
SET BUILD_TYPE=Release
SET VSDIR="C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin"
SET PKGDIR=C:\webrtc_plugin_pkg

for %%* in (.) do set InstDirName=%cd%\..\
echo WebrtcEngine Root Directory: %InstDirName%

:menuLOOP
echo.
echo.= Menu =================================================
echo.
for /f "tokens=1,2,* delims=_ " %%A in ('"findstr /b /c:":menu_" "%~f0""') do echo.  %%B  %%C
set choice=
echo.&set /p choice=Make a choice or hit ENTER to quit: ||GOTO:EOF
echo.&call:menu_%choice%
GOTO:menuLOOP

::-----------------------------------------------------------
:: menu functions follow below here
::-----------------------------------------------------------

:menu_1   prepare
	IF EXIST %InstDirName%\third_party (goto tpdirexist) else goto tpdircreate  
	:tpdircreate
		echo Creating %InstDirName%\third_party directory...
		md %InstDirName%\third_party
	:tpdirexist
		cd third_party

		IF EXIST %InstDirName%\third_party\libjingle (goto ljdirexist) else goto ljdircreate  
		:ljdircreate
			echo Creating %InstDirName%\third_party\libjingle directory...
			md %InstDirName%\third_party\libjingle
		:ljdirexist  
			cd libjingle

			echo Running gclient config with target url [http://libjingle.googlecode.com/svn/trunk/]...
			call gclient config http://libjingle.googlecode.com/svn/trunk/

			echo Running [gclient sync -r %LIBJINGLE_REV% --force] to obtain webrtc source...
			call gclient sync -r %LIBJINGLE_REV% --force

	REM #############
	REM firebreath #
	REM #############
	cd %InstDirName%\third_party
	IF EXIST %InstDirName%\third_party\FireBreath (goto fbdirexist) else goto fbdircreate  
	:fbdircreate
		echo Cloning firebreath branch from [http://github.com/PeterXu/FireBreath.git]...
		call git clone http://github.com/PeterXu/FireBreath.git
		cd %InstDirName%\third_party\FireBreath
		echo Initializing FireBreath submodules....
		call git submodule update --init --recursive		
	:fbdirexist
		echo FireBreath software already current.

echo Prepare step succesfully completed.
GOTO:EOF

:menu_2   jingle
	IF EXIST %InstDirName%\third_party\libjingle (goto ljdirexist_2) else goto ljdirmissing  
	:ljdirmissing
		echo Missing libjingle directory. Please execute Prepare step (Menu 1) first.
		GOTO:EOF
	:ljdirexist_2
		echo Backing up selected ligjingle files and applying patches....
		cd %InstDirName%\third_party
		
		copy %InstDirName%\third_party\libjingle\trunk\build\common.gypi %InstDirName%\third_party\libjingle\trunk\build\common.gypi.bak
		copy %InstDirName%\third_party\libjingle\trunk\third_party\winsdk_samples\src\Samples\multimedia\directshow\baseclasses\dllentry.cpp ^
			%InstDirName%\third_party\libjingle\trunk\third_party\winsdk_samples\src\Samples\multimedia\directshow\baseclasses\dllentry.cpp.bak

		call python ..\bin\patch.py -p0 --verbose ..\third_party_mods\common\libjingle_build.diff
		call python ..\bin\patch.py -p0 --verbose ..\third_party_mods\Win\dllentry.diff

		echo Generating native build files for Windows environment.
		cd %InstDirName%\third_party\libjingle
		call python trunk/build/gyp_chromium --depth=trunk trunk/talk/libjingle_all.gyp
		
		echo Compiling libjingle...	
		echo Setting Visual Studio Environment for command line compilation...
		call %VSDIR%\vcvars32.bat
		msbuild.exe %InstDirName%\third_party\libjingle\trunk\talk\libjingle_all.sln /p:Configuration=%BUILD_TYPE% /p:Platform=Win32 /m	
		
echo Jingle step succesfully completed.
GOTO:EOF

:menu_3   webrtc
	echo Skipping creation of the monolithic static library for now....
	echo We will implement creating the monolithi static library in future using lib.exe...
	echo This will not impact the ability to create the plugin...

echo Webrtc step succesfully completed.
GOTO:EOF

:menu_4   engine
	IF EXIST %InstDirName%\third_party\FireBreath (goto fbdirexist_2) else goto fbdirmissing  
	:fbdirmissing
		echo Missing FireBreath directory. Please execute Prepare step (Menu 1) first.
		GOTO:EOF
	:fbdirexist_2
		cd %InstDirName%
		echo Generating FireBreath project for WebrtcEngine plugin...
		call %InstDirName%\third_party\FireBreath\prep2010.cmd src\js_api\projects src\js_api\build
		
		echo Compiling WebrtcEngine plugin...	
		echo Setting Visual Studio Environment for command line compilation...
		call %VSDIR%\vcvars32.bat
		msbuild.exe %InstDirName%\src\js_api\build\FireBreath.sln /p:Configuration=%BUILD_TYPE% /p:Platform=Win32 /m	
		
echo Engine step succesfully completed.
GOTO:EOF

:menu_5   package

	IF EXIST %PKGDIR% (goto pkgdirexist) else goto pkgdircreate
	:pkgdirexist
		echo Removing existing %PKGDIR% directory...
		rmdir /s /q %PKGDIR%
	:pkgdircreate
		echo Creating new %PKGDIR% directory...
		md %PKGDIR%
	
		cd %PKGDIR%

    rem copy tests and docs
    copy %InstDirName%\tests\* %PKGDIR%

    rem copy plugin
    copy %InstDirName%\src\js_api\build\bin\WebrtcEngine\Release\npWebrtcEngine.dll %PKGDIR%

    rem test programs
	copy %InstDirName%\third_party\libjingle\trunk\build\%BUILD_TYPE%\peerconnection_server.exe %PKGDIR%

	rem test programs
	echo Package created successfully...
	echo Skipping creating zip file because of software restrictions on Windows platform
	echo Next Steps....
	echo Step 1:  cd %PKGDIR%
	echo Step 2:  Unregister existing plugin using "regsvr32 /u npWebrtcEngine.dll"
	echo Step 2:  Register WebrtcEngine using "regsvr32 npWebrtcEngine.dll"
	echo Step 3:  Start peerconnection_server.exe
	echo Step 4:  Open the webrtc_ex.html file in IE/Chrome/Firefox...
	echo Step 5:  Happy testing...
	
GOTO:EOF

