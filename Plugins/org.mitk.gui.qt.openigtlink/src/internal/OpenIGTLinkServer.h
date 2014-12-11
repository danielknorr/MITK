#pragma once
#include <QRunnable>
#include <qstring.h>

// OpenIGTLink
#include "igtlOSUtil.h"
#include "igtlImageMessage.h"
#include "igtlServerSocket.h"
#include <mitkImage.h>
#include "OpenIGTLinkConnector.h"

class OpenIGTLinkServer : public OpenIGTLinkConnector
{

public:
  OpenIGTLinkServer(mitk::Image::Pointer image, int port, int fps);
  virtual void Connect();

  ~OpenIGTLinkServer(void);
  void run();
};

