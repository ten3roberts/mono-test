workspace "mono-test"
	configurations { "Debug", "Release" }

project "mono-test"
	kind "ConsoleApp"
	language "C"

	files { "src/**.c", "src/**.h" }
	files { "scripts/**.cs" }
		
	--prebuildcommands "./compile.sh"

	-- C# compilation
	--filter "files:**.cs"
	--	buildmessage "Compiling %{file.relpath}"
	--	buildcommands {
	--		"mcs %{file.relpath}"
	--	}
	--	buildoutputs {
	--		"%{cfg.objdir}/%{file.name}.exe"
	--	}
	
	filter "configurations:Debug"
		optimize "off"
		symbols "on"

	filter "configurations:Release"
		optimize "on"
		symbols "on"


	filter "toolset:gcc"
		buildoptions { "-Wall" }
		defines { "REENTRANT" }
		links { "mono-2.0", "m", "rt", "dl", "pthread" }
		includedirs { "/usr/include/mono-2.0" }