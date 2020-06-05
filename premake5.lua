workspace "mono-test"
	configurations { "Debug", "Release" }

project "mono-test"
	kind "ConsoleApp"
	language "C"

	files { "src/**.c", "src/**.h" }
		
	prebuildcommands "./compile.sh"

	filter "configurations:Debug"
		optimize "off"
		symbols "on"

	filter "configurations:Release"
		optimize "on"
		symbols "on"


	filter "toolset:gcc"
		buildoptions { "-Wall", "-Werror" }
		defines { "REENTRANT" }
		links { "mono-2.0", "m", "rt", "dl", "pthread" }
		includedirs { "/usr/include/mono-2.0" }