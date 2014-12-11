project(org_mitk_gui_qt_openigtlclient)

find_package(OpenIGTLink)

include(${OpenIGTLink_USE_FILE})

MACRO_CREATE_MITK_CTK_PLUGIN(
  EXPORT_DIRECTIVE OPENIGTLINKCLIENT_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDS MitkOpenIGTLink
)
