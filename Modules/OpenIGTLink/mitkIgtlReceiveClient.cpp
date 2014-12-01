/**
* @author Daniel Knorr
*/

#include "mitkIgtlReceiveClient.h"

namespace mitk {

IgtlReceiveClient::IgtlReceiveClient()
{
}

IgtlReceiveClient::~IgtlReceiveClient()
{
}

void IgtlReceiveClient::Connect()
{
  m_Socket = igtl::ClientSocket::New();
  int r = m_Socket->ConnectToServer(m_ServerIp, m_ServerPort);

  if (r != 0)
    {
    std::cerr << "Cannot connect to the server." << std::endl;
    Disconnect();
  }
}

void IgtlReceiveClient::Disconnect()
{
  m_Socket->CloseSocket();
}

void IgtlReceiveClient::SetServerAddress(char* serverIp, int serverPort)
{
  m_ServerIp = serverIp;
  m_ServerPort = serverPort;
}

void IgtlReceiveClient::run()
{
  this->Connect();
  this->Receive();
}

}  // end of namespace mitk
