mkdir Libs
mkdir Libs\Library
mkdir Libs\lib
mkdir Libs\dll
mkdir Libs\dll\Debug
mkdir Libs\dll\Release
cd Libs\Library\
git clone https://github.com/butibuti/ButiBulletWrap
git clone https://github.com/butibuti/ButiScript.git
git clone https://github.com/butibuti/ButiMemorySystem.git
git clone https://github.com/butibuti/ButiUtil.git
git clone https://github.com/butibuti/ButiMath
git clone https://github.com/butibuti/ButiEventSystem
git clone https://github.com/butibuti/ButiFont
git clone https://github.com/butibuti/ButiRendering_Dx12
git clone https://github.com/butibuti/ButiEngineHeader
git clone https://github.com/butibuti/ButiCereal
git clone https://github.com/butibuti/ButiSound
cd ..\..\
cd Libs\Library\ButiBulletWrap\
git pull
cd ..\ButiScript\
git pull
git checkout escape_from_u22
cd ..\ButiMemorySystem\
git pull
cd ..\ButiUtil\
git pull
git checkout escape_from_u22
cd ..\ButiMath\
git pull
cd ..\ButiEventSystem\
git pull
cd ..\ButiFont\
git pull
cd ..\ButiRendering_Dx12\
git pull
cd ..\ButiEngineHeader\
git pull
git checkout escape_from_u22
cd ..\ButiCereal\
git pull
cd ..\ButiSound\
git pull

cd ..\..\