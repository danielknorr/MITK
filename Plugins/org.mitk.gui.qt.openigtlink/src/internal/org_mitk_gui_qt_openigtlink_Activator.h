#ifndef org_mitk_gui_qt_openigtlink_Activator_h
#define org_mitk_gui_qt_openigtlink_Activator_h

#include <ctkPluginActivator.h>

namespace mitk {

class org_mitk_gui_qt_openigtlink_Activator :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)

public:

  static ctkPluginContext* GetContext();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

private:

  static ctkPluginContext* m_Context;

}; // org_mitk_gui_qt_openigtlink_Activator

}

#endif // org_mitk_gui_qt_openigtlink_Activator_h
