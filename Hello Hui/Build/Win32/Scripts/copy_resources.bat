cd ..
cd ..
cd ..
cd ..

xcopy "ThirdParty\freetype\win32\freetype.dll" "Hello Hui\Build\Win32\Debug" /Y
xcopy "ThirdParty\GLFW3\win32\glew32.dll" "Hello Hui\Build\Win32\Debug" /Y
xcopy "Test App\Assets" "Hello Hui\Build\Win32\Debug\Assets\" /Y /E /I