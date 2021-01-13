project "NARC"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
    staticruntime "on"

    targetdir ("../build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../build/obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

    defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

    includedirs
	{
		"src",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"dep/spdlog/include"
	}

    links 
	{ 
		"glfw",
		"glad",
		"ImGui",
		"opengl32.lib",
	}

    filter "system:windows"
    systemversion "latest"
		defines
		{
			"GLFW_INCLUDE_NONE"
		}

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"
