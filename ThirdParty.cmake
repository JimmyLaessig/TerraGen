include(FetchContent)

set(FETCHCONTENT_BASE_DIR "${PROJECT_SOURCE_DIR}/thirdParty")

set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
set(CPM_VERSION 0.15.1)

if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
  message(STATUS "Downloading CPM.cmake")
  file(DOWNLOAD https://raw.githubusercontent.com/TheLartians/CPM.cmake/v${CPM_VERSION}/cmake/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})

include(${CPM_DOWNLOAD_LOCATION})



##################################################
# GLM
##################################################

CPMAddPackage(
  NAME glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.5
  DOWNLOAD_ONLY True
)

##################################################
# libnoise
##################################################

CPMAddPackage(
  NAME libnoise
  GIT_REPOSITORY https://github.com/qknight/libnoise.git
  GIT_TAG master
  OPTIONS
  "BUILD_LIBNOISE_EXAMPLES OFF"
  "BUILD_LIBNOISE_UTILS OFF"
)

