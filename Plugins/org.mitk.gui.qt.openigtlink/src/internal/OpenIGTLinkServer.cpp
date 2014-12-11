#include "OpenIGTLinkServer.h"
#include "OpenIGTLinkConnector.h"
// OpenIGTLink
#include "igtlOSUtil.h"
#include "igtlImageMessage.h"
#include "igtlServerSocket.h"
#include "igtlClientSocket.h"
#include <mitkImage.h>
#include <mitkImageReadAccessor.h>
#include <qstring.h>


OpenIGTLinkServer::OpenIGTLinkServer(mitk::Image::Pointer image, int port, int fps)
{
  _isRunning = true;
  _usImage = image;
  _port = port;
  _fps = fps;
}


OpenIGTLinkServer::~OpenIGTLinkServer(void)
{
}

void OpenIGTLinkServer::run(){

  this->Connect();
}


void OpenIGTLinkServer::Connect()
{

  int r;
  igtl::TimeStamp::Pointer ts = igtl::TimeStamp::New();

  igtl::ClientSocket::Pointer socket;
  igtl::ServerSocket::Pointer serverSocket;
  serverSocket = igtl::ServerSocket::New();
  r = serverSocket->CreateServer(_port);


  if (r < 0)
  {
    std::cerr << "Cannot create a server socket." << std::endl;
    return;
  }


  while (_isRunning)
  {

    std::cerr << "Waiting" << std::endl;
    // Waiting for Connection

    socket = serverSocket->WaitForConnection(1000);
    if(!socket.IsNotNull()) // if client connected
    {
      continue;
    }

    while(_isRunning){
      ts->GetTime();
      sendImage(socket, ts);
      if(!socket->GetConnected()){
        _isRunning = false;
      }
    }
  }
  if(socket.IsNotNull() && socket->GetConnected()){
    socket->CloseSocket();
  }
}