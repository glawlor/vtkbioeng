CMAKE_MINIMUM_REQUIRED(VERSION 2.0)

# Set the project/library name here.  Classes should be declared as
# "class VTK_BIOENG_EXPORT vtkFoo"
# project set here.
PROJECT(vtkBioeng)

# Set your list of sources here.  Do not change the name of the
# vtkLocal_SRCS variable.
SET(vtkBioeng_SRCS
vtkBufferOutputWindow.cxx
vtkCollisionDetectionFilter.cxx
vtkCellsFilter.cxx
vtkRemoveCellsFilter.cxx
vtkSelectCellsFilter.cxx
vtkTesselateBoundaryLoops.cxx
)

SET_SOURCE_FILES_PROPERTIES(
vtkCellsFilter
ABSTRACT
)

# List the kits from VTK that are needed by this project.
SET(vtkBioeng_LIBS
vtkCommon
vtkFiltering 
vtkGraphics 
vtkRendering 
)

#-----------------------------------------------------------------------------
# Most users should not need to change anything below this line.

# Need to include class headers and the configuration header.
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

# Setup output paths.
SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH
     "Single output directory for building all libraries.")
SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH
     "Single output directory for building all executables.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

# Find and load VTK settings.
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

# Give user option of building shared or static libraries.  Default to
# the choice made for VTK.
OPTION(BUILD_SHARED_LIBS "Build with shared libraries."
       ${VTK_BUILD_SHARED_LIBS})

# Set VTKBIOENG_BUILD_SHARED_LIBS to 1 or 0 for use in creating the
# configuration header.
IF(BUILD_SHARED_LIBS)
  SET(VTKBIOENG_BUILD_SHARED_LIBS_CONFIG 1)
ELSE(BUILD_SHARED_LIBS)
  SET(VTKBIOENG_BUILD_SHARED_LIBS_CONFIG 0)
ENDIF(BUILD_SHARED_LIBS)

IF(VTK_FOUND)
  # If this is a build tree, provide an option for putting this
  # project's executables and libraries in with VTK's.
  IF (EXISTS ${VTK_DIR}/bin)
    OPTION(USE_VTK_OUTPUT_PATHS
     "Use VTK's output directory for this project's executables and libraries."
     OFF)
    MARK_AS_ADVANCED (USE_VTK_OUTPUT_PATHS)
    IF (USE_VTK_OUTPUT_PATHS)
      SET (LIBRARY_OUTPUT_PATH ${VTK_DIR}/bin)
      SET (EXECUTABLE_OUTPUT_PATH ${VTK_DIR}/bin)
    ENDIF (USE_VTK_OUTPUT_PATHS)
  ENDIF (EXISTS ${VTK_DIR}/bin)

  # Create the header to configure the classes.
  CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/vtkBioengConfigure.h.in
                 ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Configure.h
                 @ONLY IMMEDIATE)

  # Create the instantiator for these classes.
  SET(VTK_USE_INSTANTIATOR_NEW 1)
  VTK_MAKE_INSTANTIATOR3("${PROJECT_NAME}Instantiator"
    vtkBioengInstantiator_SRCS
    "${vtkBioeng_SRCS}"
    "VTK_BIOENG_EXPORT"
    ${PROJECT_BINARY_DIR}
    "${PROJECT_NAME}Configure.h")

  # Create the library.
  ADD_LIBRARY(${PROJECT_NAME} ${vtkBioeng_SRCS} ${vtkBioengInstantiator_SRCS})
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${vtkBioeng_LIBS})

  # Create Tcl wrappers if VTK is wrapped in Tcl.
  IF(VTK_WRAP_TCL)
    INCLUDE(${VTK_CMAKE_DIR}/vtkWrapTcl.cmake)
    SET(VTK_WRAP_TCL2_INIT_DIR "${PROJECT_SOURCE_DIR}")
    VTK_WRAP_TCL2(${PROJECT_NAME}TCL vtkBioengTCL_SRCS ${vtkBioeng_SRCS} ${Kit_TCL_EXTRA_CMDS})
    ADD_LIBRARY(${PROJECT_NAME}TCL ${vtkBioengTCL_SRCS})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME}TCL ${PROJECT_NAME})
    FOREACH(c ${vtkBioeng_LIBS})
      TARGET_LINK_LIBRARIES(${PROJECT_NAME}TCL ${c}TCL)
    ENDFOREACH(c)

    # Create the list of kits that are required.
    SET(VTKBIOENG_KITS "")
    FOREACH(c ${vtkBioeng_LIBS})
      SET(VTKBIOENG_KITS "${VTKBIOENG_KITS} ${c}")
    ENDFOREACH(c)
    FOREACH(c ${vtkBioeng_SRCS})
      GET_FILENAME_COMPONENT(VTKBIOENG_LAST_CLASS ${c} NAME_WE)
    ENDFOREACH(c)

    IF(CMAKE_CONFIGURATION_TYPES)
      FOREACH(config ${CMAKE_CONFIGURATION_TYPES})
        SET(VTKBIOENG_TCL_LIBRARY_DIR "${LIBRARY_OUTPUT_PATH}/${config}")
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/pkgIndex.tcl.in
                       ${PROJECT_BINARY_DIR}/${config}/pkgIndex.tcl
                       @ONLY IMMEDIATE)
      ENDFOREACH(config)
    ELSE(CMAKE_CONFIGURATION_TYPES)
      SET(VTKBIOENG_TCL_LIBRARY_DIR "${LIBRARY_OUTPUT_PATH}")
      CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/pkgIndex.tcl.in
                     ${PROJECT_BINARY_DIR}/pkgIndex.tcl
                     @ONLY IMMEDIATE)
    ENDIF(CMAKE_CONFIGURATION_TYPES)
    
  ENDIF(VTK_WRAP_TCL)

#  Nothing set up to test... TODO!
#  SUBDIRS(Testing)

  SUBDIRS(Utilities)
ENDIF(VTK_FOUND)
