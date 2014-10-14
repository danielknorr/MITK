/*===================================================================

@author Daniel Knorr

===================================================================*/

#include "org_mitk_gui_qt_openigtlclient_Activator.h"

#include <QtPlugin>

namespace mitk {

ctkPluginContext* org_mitk_gui_qt_openigtlclient_Activator::m_Context = 0;

ctkPluginContext* org_mitk_gui_qt_openigtlclient_Activator::GetContext()
{
  return m_Context;
}

void org_mitk_gui_qt_openigtlclient_Activator::start(ctkPluginContext* context)
{
  this->m_Context = context;

  BERRY_REGISTER_EXTENSION_CLASS(QmitkXnatTreeBrowserView, context)
}

void org_mitk_gui_qt_openigtlclient_Activator::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

}

Q_EXPORT_PLUGIN2(org_mitk_gui_qt_openigtlclient, mitk::org_mitk_gui_qt_openigtlclient_Activator)
