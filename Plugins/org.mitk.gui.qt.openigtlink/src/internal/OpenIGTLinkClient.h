#pragma once
#include <QRunnable>
#include <qstring.h>

// OpenIGTLink
#include "igtlOSUtil.h"
#include "igtlImageMessage.h"
#include "igtlServerSocket.h"
#include <mitkImage.h>
#include "OpenIGTLinkConnector.h"

class OpenIGTLinkClient : public OpenIGTLinkConnector
{

public:
  OpenIGTLinkClient(mitk::Image::Pointer image, int port, int fps, char* ip);
  virtual void Connect();
  ~OpenIGTLinkClient(void);
  void run();
protected:
  QString _ip;
};

