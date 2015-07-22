/**
* @author Daniel Knorr
*/

#ifndef MITKIGTLRECEIVECLIENT_H
#define MITKIGTLRECEIVECLIENT_H

#include <QRunnable>

#include <igtlClientSocket.h>
#include <igtlMessageHeader.h>

namespace mitk {

class IgtlReceiveClient : public QRunnable
{
public:

  void Connect();
  virtual void Receive() = 0;

  void Disconnect();
  bool IsConnected();

  std::string m_ServerIp;
  int m_ServerPort;

  volatile bool m_IsRunning;

protected:

  IgtlReceiveClient();
  ~IgtlReceiveClient();

  igtl::ClientSocket::Pointer m_Socket;
  igtl::MessageHeader::Pointer m_Header;

};

} // end of namespace mitk

#endif // MITKIGTLRECEIVECLIENT_H
