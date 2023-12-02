@echo off

pushd ..
git clone git@github.com:glfw/glfw.git Vendor/glfw
git clone git@github.com:g-truc/glm.git Vendor/glm
git clone git@github.com:zeux/volk.git Vendor/volk
git clone git@github.com:GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git Vendor/vma
popd ..