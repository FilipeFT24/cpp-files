project "Graphs"
	kind          "ConsoleApp"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "Off"
	warnings      "Extra"

	objdir    "%{wks.location}/bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	targetdir     "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	files{
		"graphs.cpp"
	}
	systemversion "latest"

	filter   "configurations:Debug"
	defines  "_DEBUG"
	runtime  "Debug"
	optimize "Off"
	symbols  "On"

	filter   "configurations:Dist"
	defines  "_DIST"
	runtime  "Release"
	optimize "On"

   	filter   "configurations:Release"
   	defines  "_RELEASE"
   	runtime  "Release"
   	optimize "On"
   	symbols  "On"