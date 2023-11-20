workspace "REFOG"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Test-App"

   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "REFOG-Core/Build-Core.lua"
group ""

include "Test-App/Build-App.lua"