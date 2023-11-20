@echo off

echo Turn on vcpkg in project settings

pushd ..
vcpkg install glm glfw3 glad
popd
pause