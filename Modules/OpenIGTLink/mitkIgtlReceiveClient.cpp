/**
* @author Daniel Knorr
*/

#include "mitkIgtlReceiveClient.h"

namespace mitk {

IgtlReceiveClient::IgtlReceiveClient()
  : m_Header(igtl::MessageHeader::New())
  , m_IsRunning(true)
{
}

IgtlReceiveClient::~IgtlReceiveClient()
{
}

void IgtlReceiveClient::Connect()
{
  m_Socket = igtl::ClientSocket::New();
  int r = m_Socket->ConnectToServer(m_ServerIp.c_str(), m_ServerPort);

  if (r != 0)
  {
    std::cerr << "Cannot connect to the server." << std::endl;
    return;
  }
  else
  {
    std::cout << "Connected." << std::endl;
  }
}

void IgtlReceiveClient::Disconnect()
{
  m_IsRunning = false;
  m_Socket->CloseSocket();
  std::cout << "Disconnected." << std::endl;
}

bool IgtlReceiveClient::IsConnected()
{
  return m_Socket->GetConnected();
}

}  // end of namespace mitk
