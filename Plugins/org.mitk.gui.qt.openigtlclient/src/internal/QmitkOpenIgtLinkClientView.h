/*===================================================================

@author Daniel Knorr

===================================================================*/

#ifndef QMITKOPENIGTLINKCLIENTVIEW_H
#define QMITKOPENIGTLINKCLIENTVIEW_H

#include <QmitkAbstractView.h>

#include "ui_QmitkOpenIgtLinkClientViewControls.h"


/*!
\brief QmitkOpenIgtLinkClientView

\warning This class is not yet documented. Use "git blame" and ask the author to provide basic documentation.

\sa QmitkFunctionality
\ingroup ${plugin_target}_internal
*/
class QmitkOpenIgtLinkClientView : public QmitkAbstractView
{
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT

public:

  QmitkOpenIgtLinkClientView();
  ~QmitkOpenIgtLinkClientView();

  static const std::string VIEW_ID;

  virtual void CreateQtPartControl(QWidget *parent);

  protected slots:
    void DoSomething();

protected:

  virtual void SetFocus();

  Ui::QmitkOpenIgtLinkClientViewControls m_Controls;

private:

};

#endif // QMITKOPENIGTLINKCLIENTVIEW_H
