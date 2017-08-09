project(ShadyBunny)

cmake_minimum_required (VERSION 2.8.2) 

if(NOT CMAKE_BUILD_TYPE)
  set( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
#MinSizeRel." FORCE)
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${${PROJECT_NAME}_SOURCE_DIR}/build/install" CACHE PATH "default install path" FORCE )
endif()


set (SOURCEFILES src/main.cpp src/BaseApp.cpp src/App.cpp src/Event.cpp src/Mesh.cpp src/Model.cpp src/GLSLProgram.cpp src/Texture.cpp src/TurntableManipulator.cpp src/Line.cpp src/Sphere.cpp src/glad/src/glad.c)

set (HEADERFILES src/BaseApp.h src/App.h src/Event.h src/Mesh.h src/Model.h src/GLSLProgram.h src/Texture.h src/TurntableManipulator.h src/Line.h src/Sphere.h)

source_group("Header Files" FILES ${HEADERFILES})

############################################################
#Start of library detection
#Any libraries outside of your source should have a corresponding
#find module and the 
#find_package method should be used here.
############################################################

#------------------------------------------
# Download dependencies
#------------------------------------------

set(glfw_checkout_Dir ${PROJECT_SOURCE_DIR}/deps/glfw)
make_directory(${glfw_checkout_Dir})
include(ExternalProject)

set(glfw_build_dir ${CMAKE_BINARY_DIR}/glfw)
set(glfw_stamp_dir ${glfw_build_dir}/stamp)
set(glfw_tmp_dir ${glfw_build_dir}/tmp)

set(glfw_PREFIX "${glfw_checkout_Dir}")
set(glfw_INSTALL_DIR "${glfw_build_dir}/install")

set(glfw_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${glfw_INSTALL_DIR} -DGLFW_BUILD_DOCS=OFF —DGLFW_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
set(glfw_DIR "${glfw_INSTALL_DIR}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(glfw_CMAKE_ARGS ${glfw_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD="c++11" -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY="libc++" -DGLFW_USE_RETINA=ON)
endif()


if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	#set(glfw_CMAKE_ARGS "${glfw_CMAKE_ARGS} -std=c++11")
endif()

if (WIN32)
    set(glfw_CMAKE_ARGS ${glfw_CMAKE_ARGS} -DUSE_MSVC_RUNTIME_LIBRARY_DLL=OFF -DGLFW_USE_HYBRID_HPG=ON)
endif()
   
ExternalProject_add(glfw
		PREFIX ${glfw_PREFIX}
		GIT_REPOSITORY https://github.com/glfw/glfw.git
		INSTALL_DIR ${glfw_INSTALL_DIR}
		CMAKE_ARGS ${glfw_CMAKE_ARGS}
		UPDATE_COMMAND ""
		DOWNLOAD_DIR ${glfw_checkout_Dir}
		SOURCE_DIR ${glfw_checkout_Dir}
		TMP_DIR ${glfw_tmp_dir}
		STAMP_DIR ${glfw_stamp_dir}
		BINARY_DIR ${glfw_build_dir}
)
 
#set_property(TARGET glfw PROPERTY FOLDER "Dependencies")

set(GLFW_INCLUDE_DIR ${glfw_INSTALL_DIR}/include CACHE INTERNAL "Directory of GLFW header files")
if(MSVC)
	set(GLFW_LIBRARY ${glfw_INSTALL_DIR}/lib/glfw3.lib CACHE INTERNAL "GLFW lib file")
else()
	set(GLFW_LIBRARY ${glfw_INSTALL_DIR}/lib/libglfw3.a CACHE INTERNAL "GLFW lib file")
endif()	
include_directories(${GLFW_INCLUDE_DIR})


#Assimp
set(assimp_checkout_Dir ${PROJECT_SOURCE_DIR}/deps/assimp)
make_directory(${assimp_checkout_Dir})

set(assimp_build_dir ${CMAKE_BINARY_DIR}/assimp)
set(assimp_stamp_dir ${assimp_build_dir}/stamp)
set(assimp_tmp_dir ${assimp_build_dir}/tmp)

set(assimp_PREFIX "${assimp_checkout_Dir}")
set(assimp_INSTALL_DIR "${assimp_build_dir}/install")

set(assimp_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${assimp_INSTALL_DIR} -DBUILD_SHARED_LIBS=OFF  —DASSIMP_BUILD_TESTS=OFF —DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DCMAKE_BUILD_TYPE=RELEASE -DASSIMP_INSTALL_PDB=OFF)
set(assimp_DIR "${assimp_INSTALL_DIR}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set( assimp_CMAKE_ARGS ${assimp_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD="c++11")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(assimp_CMAKE_ARGS "${assimp_CMAKE_ARGS} -std=c++11")
IF (PKG_CONFIG_FOUND)
	PKG_CHECK_MODULES(UNZIP minizip)
ENDIF()

endif()

if (WIN32)
    set(assimp_CMAKE_ARGS ${assimp_CMAKE_ARGS} -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON)
endif()
   
ExternalProject_add(assimp
		PREFIX ${assimp_PREFIX}
		GIT_REPOSITORY https://github.com/assimp/assimp.git
		INSTALL_DIR ${assimp_INSTALL_DIR}
		CMAKE_ARGS ${assimp_CMAKE_ARGS}
		UPDATE_COMMAND ""
		DOWNLOAD_DIR ${assimp_checkout_Dir}
		SOURCE_DIR ${assimp_checkout_Dir}
		TMP_DIR ${assimp_tmp_dir}
		STAMP_DIR ${assimp_stamp_dir}
		BINARY_DIR ${assimp_build_dir}
)
 
#set_property(TARGET assimp PROPERTY FOLDER "Dependencies")

set(ASSIMP_INCLUDE_DIR ${assimp_INSTALL_DIR}/include CACHE INTERNAL "Directory of Assimp header files")
if(MSVC)
  # in order to prevent DLL hell, each of the DLLs have to be suffixed with the major version and msvc prefix
  if( MSVC70 OR MSVC71 )
    set(MSVC_PREFIX "vc70")
  elseif( MSVC80 )
    set(MSVC_PREFIX "vc80")
  elseif( MSVC90 )
    set(MSVC_PREFIX "vc90")
  elseif( MSVC10 )
    set(MSVC_PREFIX "vc100")
  elseif( MSVC11 )
    set(MSVC_PREFIX "vc110")
  elseif( MSVC12 )
    set(MSVC_PREFIX "vc120")
  elseif( MSVC14 )
    set(MSVC_PREFIX "vc140")
  else()
    set(MSVC_PREFIX "vc150")
  endif()
  set(ASSIMP_LIBRARY_SUFFIX "-${MSVC_PREFIX}-mt" CACHE STRING "the suffix for the assimp windows library" FORCE)
	set(ASSIMP_LIBRARY ${assimp_INSTALL_DIR}/lib/assimp${ASSIMP_LIBRARY_SUFFIX}.lib CACHE INTERNAL "Assimp lib file")
else()
	set(ASSIMP_LIBRARY ${assimp_INSTALL_DIR}/lib/libassimp.a CACHE INTERNAL "Assimp lib file")
endif()	
include_directories(${ASSIMP_INCLUDE_DIR})

find_package(ZLIB QUIET)
if (NOT ZLIB_FOUND)
	if (MSVC)
		message("Zlib not found. Setting it to zlib built with assimp")
		set(ZLIB_LIBRARY debug ${assimp_INSTALL_DIR}/lib/zlibstaticd.lib)
		set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
		SET(ZLIB_FOUND 1)
	endif()
else()
include_directories(${ZLIB_INCLUDE_DIRS})
endif()


#SOIL
set(soil_checkout_dir ${PROJECT_SOURCE_DIR}/deps/SOIL)
make_directory(${soil_checkout_dir})

set(soil_patch_dir ${PROJECT_SOURCE_DIR}/src/soil)

set(soil_build_dir ${CMAKE_BINARY_DIR}/soil)
set(soil_stamp_dir ${soil_build_dir}/stamp)
set(soil_tmp_dir ${soil_build_dir}/tmp)

set(soil_PREFIX "${soil_checkout_dir}")
set(soil_INSTALL_DIR "${soil_build_dir}/install")

set(soil_PATCH ${CMAKE_COMMAND} -E copy ${soil_patch_dir}/CMakeLists.txt <SOURCE_DIR>/CMakeLists.txt)
set(soil_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${soil_INSTALL_DIR}")
   
ExternalProject_add(SOIL
	PREFIX ${soil_PREFIX}
	URL http://www.lonesock.net/files/soil.zip
	INSTALL_DIR ${soil_INSTALL_DIR}
	CMAKE_ARGS ${soil_CMAKE_ARGS}
	PATCH_COMMAND ${soil_PATCH}
	UPDATE_COMMAND ""
	DOWNLOAD_DIR ${soil_checkout_dir}
	SOURCE_DIR ${soil_checkout_dir}
	TMP_DIR ${soil_tmp_dir}
	STAMP_DIR ${soil_stamp_dir}
	BINARY_DIR ${soil_build_dir}
 )

set(SOIL_INCLUDE_DIR ${soil_INSTALL_DIR}/include CACHE INTERNAL "Directory of SOIL header files")

if(MSVC)
	set(SOIL_LIBRARY ${soil_INSTALL_DIR}/lib/SOIL.lib CACHE INTERNAL "SOIL lib file")
else()
	set(SOIL_LIBRARY ${soil_INSTALL_DIR}/lib/libSOIL.a CACHE INTERNAL "SOIL lib file")
endif()	
include_directories(${SOIL_INCLUDE_DIR})


# OpenGL
find_package(OpenGL REQUIRED)
set(OPENGL_LIBRARIES ${OPENGL_gl_LIBRARY})
set(OPENGL_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIRS})


############################################################

set (CMAKE_VERBOSE_MAKEFILE TRUE)

############################################################
#OS Specific flags should go here
############################################################

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
 	 # Linux specific code
  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11") 
	find_package(Threads)
	find_package(X11)
	set(LIBS_ALL ${LIBS_ALL} ${CMAKE_THREAD_LIBS_INIT} rt Xrandr Xxf86vm Xi m dl Xcursor Xinerama ${X11_LIBRARIES})
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # Apple-specific code
  set(CMAKE_CXX_FLAGS "-DOSX")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	#set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	find_library(COCOA_LIB Cocoa)
	find_library(IOKIT_LIB IOKit)
	find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    	find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
	set(LIBS_ALL ${LIBS_ALL} ${COCOA_LIB} ${IOKIT_LIB} ${CORE_FOUNDATION_FRAMEWORK} ${CORE_VIDEO_FRAMEWORK})
	message(STATUS "${CORE_VIDEO_FRAMEWORK}")

endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if (WIN32)
  # Windows-specific
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_taget_properties(${WINDOWS_BINARIES} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif (WIN32)


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/glad/include
)

add_executable(${PROJECT_NAME} ${SOURCEFILES} ${HEADERFILES})
target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY} ${ASSIMP_LIBRARY} ${ZLIB_LIBRARIES} ${SOIL_LIBRARY} ${OPENGL_LIBRARIES} ${LIBS_ALL})
add_dependencies(${PROJECT_NAME} glfw assimp SOIL)

if (WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "./Debug")
	set_target_properties(${WINDOWS_BINARIES} PROPERTIES VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()


# Organize folder structure for Xcode, Visual Studio, etc.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#copy resource files to build folder so that the executable can find them in the working directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>)

