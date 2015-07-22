/*===================================================================

@author Daniel Knorr

===================================================================*/

#include "QmitkOpenIgtLinkClientView.h"


// Blueberry
#include <berryIWorkbenchPage.h>

// Qt
#include <QThreadPool>
#include <QRunnable>

const std::string QmitkOpenIgtLinkClientView::VIEW_ID = "org.mitk.views.openigtl.clientview";

QmitkOpenIgtLinkClientView::QmitkOpenIgtLinkClientView()
  : m_CommandClient(new mitk::IgtlCommandClient())
  , m_TransformClient(new mitk::IgtlTransformClient())
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

  connect( m_Controls.startRecordingButton, SIGNAL(clicked()), this, SLOT(StartRecording()) );
  connect( m_Controls.stopRecordingButton, SIGNAL(clicked()), this, SLOT(StopRecording()) );
  connect( m_Controls.reconstructButton, SIGNAL(clicked()), this, SLOT(ReconstructRecording()) );
  connect( m_Controls.connectButton, SIGNAL(clicked()), this, SLOT(StartConnection()) );
}

void QmitkOpenIgtLinkClientView::StartRecording()
{
  m_CommandClient->StartRecording(m_Controls.recordingPathLineEdit->text());
}

void QmitkOpenIgtLinkClientView::StopRecording()
{
  m_CommandClient->StopRecording();
}

void QmitkOpenIgtLinkClientView::ReconstructRecording()
{
  m_CommandClient->ReconstructVolume(m_Controls.recordingPathLineEdit->text(), m_Controls.reconstructionPathLineEdit->text());
}

void QmitkOpenIgtLinkClientView::StartConnection()
{
  //try
  //{
  m_CommandClient->m_ServerIp = m_Controls.serverIpLineEdit->text().toStdString();
  m_CommandClient->m_ServerPort = m_Controls.serverPortLineEdit->text().toInt();
  QThreadPool::globalInstance()->start(m_CommandClient, 0);
  //}catch(std::exception &e){}

  //m_TransformClient->m_ServerIp = m_Controls.serverIpLineEdit->text().toStdString();
  //m_TransformClient->m_ServerPort = m_Controls.serverPortLineEdit->text().toInt();
  //QThreadPool::globalInstance()->start((QRunnable*)m_TransformClient, 0);
}
