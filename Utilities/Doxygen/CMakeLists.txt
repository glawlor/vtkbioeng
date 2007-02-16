#
# Build the documentation
#

INCLUDE (${CMAKE_ROOT}/Modules/Documentation.cmake OPTIONAL)

IF (BUILD_DOCUMENTATION)
  OPTION (DOCUMENTATION_DOWNLOAD_VTK_TAGFILE
         "Download the VTK tag file." ON)
  MARK_AS_ADVANCED (
    DOCUMENTATION_DOWNLOAD_VTK_TAGFILE
  )

  SET(VTKBIOENG_SOURCE_DIR ${PROJECT_SOURCE_DIR})
  SET(VTKBIOENG_BINARY_DIR ${PROJECT_BINARY_DIR})

  #
  # Configure the script and the doxyfile, then add target
  #
  IF(NOT DOT_PATH)
    GET_FILENAME_COMPONENT(DOT_PATH ${DOT} PATH)
  ENDIF(NOT DOT_PATH)

  CONFIGURE_FILE(
    ${VTKBIOENG_SOURCE_DIR}/Utilities/Doxygen/doxyfile.in
    ${VTKBIOENG_BINARY_DIR}/Utilities/Doxygen/doxyfile)

  CONFIGURE_FILE(
    ${VTKBIOENG_SOURCE_DIR}/Utilities/Doxygen/doc_makeall.sh.in
    ${VTKBIOENG_BINARY_DIR}/Utilities/Doxygen/doc_makeall.sh)

  ADD_CUSTOM_TARGET(vtkBioengDoxygenDoc
    ${BASH}
    ${VTKBIOENG_BINARY_DIR}/Utilities/Doxygen/doc_makeall.sh)

ENDIF (BUILD_DOCUMENTATION)
