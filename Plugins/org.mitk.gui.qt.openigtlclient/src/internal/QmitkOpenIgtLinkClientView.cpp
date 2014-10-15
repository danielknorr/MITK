/*===================================================================

@author Daniel Knorr

===================================================================*/

#include "QmitkOpenIgtLinkClientView.h"


// Blueberry
#include <berryIWorkbenchPage.h>

const std::string QmitkOpenIgtLinkClientView::VIEW_ID = "org.mitk.views.openigtl.clientview";

QmitkOpenIgtLinkClientView::QmitkOpenIgtLinkClientView()
{
}

QmitkOpenIgtLinkClientView::~QmitkOpenIgtLinkClientView()
{
}

void QmitkOpenIgtLinkClientView::SetFocus()
{
}

void QmitkOpenIgtLinkClientView::CreateQtPartControl( QWidget *parent )
{
  // Create GUI widgets from the Qt Designer's .ui file
  m_Controls.setupUi( parent );

  connect( m_Controls.pushButton, SIGNAL(clicked()), this, SLOT(DoSomething()) );
}

void QmitkOpenIgtLinkClientView::DoSomething()
{
}
