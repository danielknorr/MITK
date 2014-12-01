/**
* @author Daniel Knorr
*/

#ifndef MITKIGTLRECEIVECLIENT_H
#define MITKIGTLRECEIVECLIENT_H

#include <QRunnable>

#include <igtlClientSocket.h>
#include <igtlMessageHeader.h>

namespace mitk {

class IgtlReceiveClient : QRunnable
{
public:

  virtual void run();

  void Disconnect();
  bool IsConnected();

  void SetServerAddress(char* serverIp, int serverPort);

protected:

  IgtlReceiveClient();
  ~IgtlReceiveClient();

  void Connect();

  virtual void Receive() = 0;

  igtl::ClientSocket::Pointer m_Socket;
  igtl::MessageHeader::Pointer m_Header;

private:

  char* m_ServerIp;
  int m_ServerPort;

};

} // end of namespace mitk

#endif // MITKIGTLRECEIVECLIENT_H
