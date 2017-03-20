--premake4.lua
--SeriousEngine Project Generator

--Solution
solution "SeriousEngineSolution"
	configurations {"Debug","Release"}

--GLFW
project "GLFW"
	--General config
	kind 		"StaticLib"
	language 	"C"
	location 	"projectfiles"
	targetdir 	"lib"
	includedirs {   "depen/GLFW/src",
                  "depen/GLFW/include",
                  "depen/GLFW/deps/**"}
  files "depen/GLFW/src/*.c"

	files "depen/GLFW/src/*.c"
	excludes{ "depen/GLFW/src/egl_context.c",
             "depen/GLFW/src/glx_context.c",
             "depen/GLFW/src/linux_joystick.c",
             "depen/GLFW/src/mach_time.c",
             "depen/GLFW/src/mir_init.c",
             "depen/GLFW/src/mir_monitor.c",
             "depen/GLFW/src/mir_window.c",
             "depen/GLFW/src/posix_time.c",
             "depen/GLFW/src/posix_tls.c",
             "depen/GLFW/src/xkb_unicode.c",
             "depen/GLFW/src/x11_window.c",
             "depen/GLFW/src/x11_monitor.c",
             "depen/GLFW/src/x11_init.c",
             "depen/GLFW/src/wl_init.c",
             "depen/GLFW/src/wl_monitor.c",
             "depen/GLFW/src/wl_window.c"
          }
          defines {	"_GLFW_USE_OPENGL" , 
             			  "_GLFW_WIN32",
             			  "_GLFW_WGL",
             			  "_GLFW_BUILD_DLL"}
            
  configuration "Debug"
    flags "Symbols"

	configuration "Release"
    flags "Optimize"


--GLEW
project "GLEW"
	kind         "StaticLib" 
    language     "C"
    location     "projectfiles"
    targetdir    "lib"
    includedirs "depen/GLEW/include"
    files 		"depen/GLEW/src/*.c"

  configuration "Debug"
  	 defines {"_WIN32"}
  	 flags "Symbols"

  configuration "Release"
     defines {"_WIN32"}
     flags "Optimize"


--SeriousEngine
project "SeriousEngine"
	kind "StaticLib"
	language "C++"
	location "projectfiles"
	targetdir "lib"
	libdirs "lib"
	includedirs {"include",
                 "depen/GLFW/include", 
                 "depen/GLM", 
                 "depen/GLEW/include",
                 "depen/STB/include",
                 "depen/IMGUI",
                 "depen/STB/src",
			     "depen/LUA/src",
			     "depen/LUABRIDGE",
				 "depen/OPENAL/include",
				 "depen/STB/include",
				 "depen/STB/src",
				 "depen/TINYXML"}

	files {"include/Engine/**.h", 
           "src/Engine/**.cc",
           "depen/IMGUI/*.cpp",
           "depen/STB/src/*.c",
		   "depen/LUA/src/*.c",
		   "depen/TINYXML/*"}

  defines {"GLEW_STATIC"}
  --OpenAl should just go here!
  links {	"GLEW",
    	    "GLFW",
    		"opengl32",
    		"OpenAL32",
    		"lua52"}

  configuration "Debug"
    defines "_ITERATOR_DEBUG_LEVEL"
  	flags "Symbols"

  configuration "Release"
    defines "_ITERATOR_DEBUG_LEVEL"
    flags "Optimize"

--SeriousEditor
project "SeriousEditor"
	kind "ConsoleApp"
	language "C++"
	location "projectfiles"
	targetdir "bin"
	libdirs "lib"
 	includedirs {"include",
                 "depen/GLM",
                 "depen/GLEW/include",
                 "depen/GLFW/include",
            	 "depen/IMGUI",
			     "depen/LUA/src",
                 "depen/LUABRIDGE",
				 "depen/OPENAL/include",
				 "depen/STB/include",
				 "depen/STB/src",
				 "depen/TINYXML"}

  links {"SeriousEngine",
			   "opengl32",
			   "OpenAL32"}
				  
	files {"include/App/**.h", 
           "src/App/**.cc"}

  configuration "Debug"
    defines "_ITERATOR_DEBUG_LEVEL"
    flags "Symbols"

  configuration "Release"
    defines "_ITERATOR_DEBUG_LEVEL"
    flags "Optimize"

--SModelImporter
project "SModelImporter"
  kind "ConsoleApp"
  language "C++"
  location "projectfiles"
  targetdir "bin"
  libdirs { "lib"}
  includedirs { "include",
                "depen/GLM",
                "depen/GLEW/include",
                "depen/GLFW/include",
                "depen/STB/include",
                "depen/IMGUI",
                "depen/STB/src",
				"depen/LUA/src",
                "depen/LUABRIDGE",
                "depen/ASSIMP/include",
				"depen/TINYXML"}


  links {"SeriousEngine","opengl32","assimpd"}

  files {"include/SModelImporter/**.h",
         "src/SModelImporter/**.cc"}

  configuration "Debug"
    defines "_ITERATOR_DEBUG_LEVEL"
    flags "Symbols"

  configuration "Release"
    defines "_ITERATOR_DEBUG_LEVEL"
    flags "Optimize"

