CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0 FATAL_ERROR)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -Wshadow")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules"  ${CMAKE_MODULE_PATH})
include(HalMacros)
SetFairBase()
SET(FAIRROOT_LIBRARY_PROPERTIES ${FAIRROOT_LIBRARY_PROPERTIES} VERSION "${HAL_VERSION}" SOVERSION "${HAL_MAJOR_VERSION}")

include(FairMacros)
include(Hal)
include(CheckCompiler)
include(WriteConfig)

PathInfo()
   
Set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
Set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
Set(INCLUDE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/include")
    
IF(NOT UNIX)
  MESSAGE(FATAL_ERROR "You're not on an UNIX system. The project was up to now only tested on UNIX systems, so we break here. If you want to go on please edit the CMakeLists.txt in the source directory.")
ENDIF(NOT UNIX)  
    
    
IF(NOT DEFINED ENV{SIMPATH})
  MESSAGE(FATAL_ERROR "You did not define the environment variable SIMPATH which is nedded to find the external packages. Please set this variable and execute cmake again.")
ENDIF(NOT DEFINED ENV{SIMPATH})
    
find_package(ROOT 6.00.00 REQUIRED)
  if("${ROOT_VERSION_MAJOR}.${ROOT_VERSION_MINOR}" VERSION_GREATER 6.16)
    Execute_Process(COMMAND ${ROOT_CONFIG_EXECUTABLE} --has-vmc OUTPUT_VARIABLE ROOT_vmc_FOUND)
    String(STRIP ${ROOT_vmc_FOUND} ROOT_vmc_FOUND)
  If(NOT ROOT_vmc_FOUND)
    set(CMAKE_PREFIX_PATH $ENV{SIMPATH} ${CMAKE_PREFIX_PATH})
    find_package2(PUBLIC VMC REQUIRED)
    set(VMCLIB VMCLibrary)
  endif()
endif()
Set(SIMPATH $ENV{SIMPATH})
Set(Boost_NO_SYSTEM_PATHS TRUE)
Set(Boost_NO_BOOST_CMAKE TRUE)
Set(BOOST_ROOT ${SIMPATH})
Set(GSL_DIR ${SIMPATH})

#Message("-- Looking for Boost ...")
# If an older version of boost is found both of the variables below are
# cached and in a second cmake run, a good boost version is found even 
# if the version is to old. 
# To overcome this problem both variables are cleared before checking
# for boost.
#Unset(Boost_INCLUDE_DIR CACHE)
#Unset(Boost_LIBRARY_DIRS CACHE)
#find_package(Boost 1.48 COMPONENTS program_options thread system timer serialization regex log log_setup filesystem)
#If (Boost_FOUND)
#  Set(Boost_Avail 1)
#Else (Boost_FOUND)
#  Set(Boost_Avail 0)
#EndIf (Boost_FOUND)

Message("-- Looking for GSL ...")
find_package(GSL)
if(NOT GSL_FOUND)
  if(APPLE)
    set(GSL_DIR /usr/local)
  else()
    #unset(GSL_DIR)
    execute_process(COMMAND gsl-config --prefix OUTPUT_VARIABLE GSL_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
  endif()
  find_package(GSL REQUIRED)
endif()
Find_Package(FairLogger)
Find_Package(FairMQ)  
  
SetBasicVariables()
Set(INCLUDE_HAL_SUBIDIR TRUE)
    
Set(BASE_INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES} ${VMC_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRECTORIES} )
SET(HALLIBDIR ${CMAKE_BINARY_DIR}/lib)  
SET(LD_LIBRARY_PATH  ${HALLIBDIR} ${GSL_LIBRARY_DIR} ${LD_LIBRARY_PATH})

If(CMAKE_SYSTEM_NAME MATCHES Darwin)
  Include(CheckIncludeFile)
  Check_Include_File("_types/_uint8_t.h" _UINT8_T_EXIST)
EndIf()

Check_Compiler()


