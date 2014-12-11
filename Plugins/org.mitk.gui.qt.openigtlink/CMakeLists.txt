project(org_mitk_gui_qt_openigtlink)

find_package(OpenIGTLink)

include(${OpenIGTLink_USE_FILE})

MACRO_CREATE_MITK_CTK_PLUGIN(
  EXPORT_DIRECTIVE OPENIGTLINK_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDS MitkQtWidgetsExt MitkUS MitkUSUI
)
