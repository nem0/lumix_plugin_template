project "myplugin"
	libType()
	files { 
		"src/**.c",
		"src/**.cpp",
		"src/**.h",
		"genie.lua"
	}
	defines { "BUILDING_MYPLUGIN" }
	links { "engine" }
	defaultConfigurations()

linkPlugin("myplugin")