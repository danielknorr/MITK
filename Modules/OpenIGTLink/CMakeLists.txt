find_package(OpenIGTLink)

include(${OpenIGTLink_USE_FILE})

MITK_CREATE_MODULE(
  DEPENDS MitkCore
  ADDITIONAL_LIBS ${OpenIGTLink_LIBRARIES}
)
