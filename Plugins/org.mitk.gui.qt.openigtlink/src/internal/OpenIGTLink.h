/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$ 
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef OpenIGTLink_h
#define OpenIGTLink_h

#include <berryISelectionListener.h>

#include <QmitkAbstractView.h>
#include "igtlOSUtil.h"
#include "igtlImageMessage.h"
#include "igtlServerSocket.h"
#include "ui_OpenIGTLinkControls.h"

#include "OpenIGTLinkServer.h"



/*!
  \brief OpenIGTLink

  \warning  This class is not yet documented. Use "git blame" and ask the author to provide basic documentation.

  \sa QmitkFunctionality
  \ingroup ${plugin_target}_internal
*/
class OpenIGTLink : public QmitkAbstractView
{  
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT
  
  public:  

    static const std::string VIEW_ID;

    virtual void CreateQtPartControl(QWidget *parent);

  protected slots:
  
    /// \brief Called when the user clicks the GUI button
    void ToggleServer();
    void ChangePort();
    void ChangeFPS();
    void ChangeClient();
    void ChangeIP();

  protected:
  
    virtual void SetFocus();

    bool _serverRunning;
    int _port;
    int _fps;
    QString _ip;
    bool _isClient;
    OpenIGTLinkConnector* _openIGTConnector;
    Ui::OpenIGTLinkControls m_Controls;

};

#endif // OpenIGTLink_h

