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
  else
  {
    std::cout << "Connected." << std::endl;
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

bool IgtlReceiveClient::IsConnected()
{
  return m_Socket->GetConnected();
}

void IgtlReceiveClient::run()
{
  this->Connect();
  this->Receive();
}

}  // end of namespace mitk
