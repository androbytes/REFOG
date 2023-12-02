newoption {
    trigger = "git",
    description = "Use this if you are using git instead of vcpkg"
}

project "Core"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.cpp", "Include/**.h" }

   includedirs
   {
      "Include",

      "../Vendor",
      "../Vendor/vulkan-extra"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   postbuildcommands
   {
       "rd /S /Q \"../Vendor/Shaders\"",
       "mkdir ../Vendor/Shaders",
       "xcopy /Y /I /E \"Include/Shaders\" \"../Vendor/Shaders\""
   }

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG", "VK_EXT_debug_utils" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"

   filter "options:git"
        includedirs
        {
            "Vendor/glfw/include",
            "Vendor/glad/include",
            "Vendor/glm",
            "Vendor/vma/Include"
        }