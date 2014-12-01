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

// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

// Qmitk
#include "OpenIGTLink.h"
#include "OpenIGTLinkServer.h"
#include "OpenIGTLinkClient.h"
#include "OpenIGTLinkConnector.h"

#include <mitkImage.h>

// Qt
#include <QMessageBox>
#include <QThreadPool>
#include <QRunnable>
#include <qvalidator.h>

const std::string OpenIGTLink::VIEW_ID = "org.mitk.views.openigtlink";

void OpenIGTLink::SetFocus()
{
  m_Controls.buttonToggleServer->setFocus();
}

void OpenIGTLink::CreateQtPartControl( QWidget *parent )
{
  igtl::MessageHeader::Pointer headerMsg = igtl::MessageHeader::New();
  // create GUI widgets from the Qt Designer's .ui file
  m_Controls.setupUi( parent );
  _serverRunning = false;
  _port = 18944;
  _fps = 5;
  _isClient = false;
  connect( m_Controls.buttonToggleServer, SIGNAL(clicked()), this, SLOT(ToggleServer()) );
  connect( m_Controls.editPort, SIGNAL(editingFinished()), this, SLOT(ChangePort()) );
  connect( m_Controls.sliderFPS, SIGNAL(valueChanged(int)), this, SLOT(ChangeFPS()) );
  connect( m_Controls.checkClient, SIGNAL(stateChanged(int)), this, SLOT(ChangeClient()) );
  connect( m_Controls.editIP, SIGNAL(editingFinished()), this, SLOT(ChangeIP()) );

  QRegExpValidator *v = new QRegExpValidator(this);
  //IP v4 RegEx
  QRegExp rx("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
  v->setRegExp(rx);
  m_Controls.editIP->setValidator(v);
  m_Controls.labelFPS->setText(QString::number(_fps));
  m_Controls.editPort->setValidator(new QIntValidator(1, 65535, this));
}



void OpenIGTLink::ToggleServer(){
  if(_serverRunning){
    m_Controls.sliderFPS->setEnabled(true);
    m_Controls.editPort->setEnabled(true);
    m_Controls.editIP->setEnabled(_isClient);
    m_Controls.checkClient->setEnabled(true);
    if(_isClient)
      m_Controls.buttonToggleServer->setText("Start Client");
    else
      m_Controls.buttonToggleServer->setText("Start Server");
    std::cerr << "Stopping OpenIGTLink" << std::endl;
    _openIGTConnector->stop();
    _serverRunning = !_serverRunning;
  }else{
    std::cerr << "Starting OpenIGTLink...";
    mitk::DataNode::Pointer node = this->GetDataStorage()->GetNamedNode("US Image Stream");
    mitk::Image::Pointer image;
    if(node.IsNotNull() && NULL != node->GetData())
    {
      m_Controls.sliderFPS->setEnabled(false);
      m_Controls.editPort->setEnabled(false);
      m_Controls.checkClient->setEnabled(false);
      m_Controls.editIP->setEnabled(false);
      image = dynamic_cast<mitk::Image*>(node->GetData());

      if(_isClient){
        m_Controls.buttonToggleServer->setText("Close Client");
        _openIGTConnector = new OpenIGTLinkClient(image, _port, _fps, _ip.toLocal8Bit().data());

      }
      else{
        m_Controls.buttonToggleServer->setText("Close Server");
        _openIGTConnector = new OpenIGTLinkServer(image, _port, _fps);

      }
      std::cerr << "Image available" << std::endl;

      QThreadPool::globalInstance()->start((QRunnable*)_openIGTConnector, 0);
      std::cerr << "done"<< std::endl;
      _serverRunning = !_serverRunning;
    }else{
      std::cerr << "Image not available" << std::endl;

    }
  }
}

void OpenIGTLink::ChangePort(){
  if(!_serverRunning){
    _port = m_Controls.editPort->text().toInt();
  }
}

void OpenIGTLink::ChangeIP(){
  if(!_serverRunning){
    _ip = m_Controls.editIP->text();
  }
}

void OpenIGTLink::ChangeClient(){
  if(!_serverRunning){
    _isClient = m_Controls.checkClient->isChecked();
    if(m_Controls.checkClient->isChecked()){
      m_Controls.editIP->setEnabled(true);
      if(_serverRunning)
        m_Controls.buttonToggleServer->setText("Close Client");
      else
        m_Controls.buttonToggleServer->setText("Start Client");
    }else{
      m_Controls.editIP->setEnabled(false);
      if(_serverRunning)
        m_Controls.buttonToggleServer->setText("Close Server");
      else
        m_Controls.buttonToggleServer->setText("Start Server");
    }
  }
}

void OpenIGTLink::ChangeFPS(){
  if(!_serverRunning){
    _fps = m_Controls.sliderFPS->value();
    m_Controls.labelFPS->setText(QString::number(_fps));
  }
}