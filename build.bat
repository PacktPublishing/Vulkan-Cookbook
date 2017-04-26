@ECHO OFF

rem Permission is hereby granted, free of charge, to any person obtaining a
rem copy of this software and associated documentation files (the "Software"),
rem to deal in the Software without restriction, including without limitation
rem the rights to use, copy, modify, merge, publish, distribute, sublicense,
rem and / or sell copies of the Software, and to permit persons to whom the
rem Software is furnished to do so, subject to the following conditions:
rem
rem The below copyright notice and this permission notice shall be included
rem in all copies or substantial portions of the Software.
rem
rem THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
rem IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
rem FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
rem THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
rem LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
rem FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
rem DEALINGS IN THE SOFTWARE.
rem
rem Vulkan Cookbook
rem ISBN: 9781786468154
rem © Packt Publishing Limited
rem
rem Author:   Pawel Lapinski
rem LinkedIn: https://www.linkedin.com/in/pawel-lapinski-84522329
rem
rem build.bat

echo Creating a solution for the 'Vulkan Cookbook'...
echo:

mkdir build
cd build

cmake.exe .. -DVK_USE_PLATFORM=WIN32 -G "Visual Studio 14 2015 Win64"

if exist VulkanCookbook.sln (
	start "" "VulkanCookbook.sln"
) else (
	echo "Error occurred during solution creation!"
)

cd ..