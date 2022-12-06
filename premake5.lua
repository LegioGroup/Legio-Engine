project "Legio"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	-- pchheader "lgpch.h"
	-- pchsource "src/lgpch.cpp"

	files
	{
        "include/**.h",
        "include/**.cpp",
		"src/**.h",
		"src/**.cpp",
        "include",
		"external/stb_image/**.h",
		"external/stb_image/**.cpp",
		"external/glm/glm/**.hpp",
		"external/glm/glm/**.inl"

		-- "external/ImGuizmo/ImGuizmo.h",
		-- "external/ImGuizmo/ImGuizmo.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"include",
        "src",
		"external/spdlog/include",
        "external/assimp/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.VulkanSDK}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
        "assimp",
		-- "yaml-cpp",
		"opengl32.lib"
	}

	-- filter "files:external/ImGuizmo/**.cpp"
	-- flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "LG_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
            "%{Library.Vulkan}",
            "%{Library.VulkanUtils}",
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "LG_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
            "%{Library.Vulkan}",
            "%{Library.VulkanUtils}",
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "LG_DIST"
		runtime "Release"
		optimize "on"

		links
		{
            "%{Library.Vulkan}",
            "%{Library.VulkanUtils}",
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}