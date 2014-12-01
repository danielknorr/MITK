#include "OpenIGTLinkClient.h"
#include "OpenIGTLinkConnector.h"

// OpenIGTLink
#include "igtlOSUtil.h"
#include "igtlImageMessage.h"
#include "igtlServerSocket.h"
#include "igtlClientSocket.h"
#include <mitkImage.h>
#include <mitkImageReadAccessor.h>
#include <qstring.h>


OpenIGTLinkClient::OpenIGTLinkClient(mitk::Image::Pointer image, int port, int fps, char* ip)
{
  _isRunning = true;
  _usImage = image;
  _port = port;
  _fps = fps;
  _ip = ip;
}



OpenIGTLinkClient::~OpenIGTLinkClient(void)
{
}

void OpenIGTLinkClient::run(){

  this->Connect();
}


void OpenIGTLinkClient::Connect()
{

  int r;
  igtl::TimeStamp::Pointer ts = igtl::TimeStamp::New();

  igtl::ClientSocket::Pointer socket;
  socket = igtl::ClientSocket::New();
  r = socket->ConnectToServer(_ip.toLocal8Bit().data(), _port);


  if (r < 0)
  {
    std::cerr << "Cannot create a client socket to " << _ip.toLocal8Bit().data() <<";" << std::endl;
    return;
  }

  std::cerr << "Connecting" << std::endl;
  // Waiting for Connection
  while(_isRunning){
    ts->GetTime();
    sendImage(socket, ts);
    if(!socket->GetConnected()){
      _isRunning = false;
    }
  }
  
  if(socket.IsNotNull() && socket->GetConnected()){
    socket->CloseSocket();
  }
}