workspace "NARC"
    architecture "x64"
    startproject "NARC"
    flags {
    	"MultiProcessorCompile"
    }
    configurations
	{
		"Debug",
		"Release"
	}
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glfw"] = "dep/glfw/include"
IncludeDir["glad"] = "dep/glad/include"
IncludeDir["ImGui"] = "dep/imgui"

group "Dependencies"
	include "NARC/dep/glfw"
	include "NARC/dep/glad"
	include "NARC/dep/imgui"
group ""

include "NARC"
