#pragma once
#include <QRunnable>
#include "igtlOSUtil.h"
#include "igtlImageMessage.h"
#include "igtlClientSocket.h"
#include <mitkImage.h>



class OpenIGTLinkConnector : QRunnable
{

public:
  virtual void Connect(){};
  int ApplyImageToMessage(igtl::ImageMessage::Pointer& msg);
  void stop();
  void sendImage(igtl::ClientSocket::Pointer socket, igtl::TimeStamp::Pointer ts);
protected:
  volatile bool _isRunning;
  mitk::Image::Pointer _usImage;
  int _port;
  int _fps;
};

