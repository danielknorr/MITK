/**
* @author Daniel Knorr
*/

#include "mitkIgtlClientManager.h"

#include <igtlOSUtil.h>
#include <igtlServerSocket.h>
#include <igtlImageMetaMessage.h>

namespace mitk {

IgtlClientManager::IgtlClientManager()
{
}

IgtlClientManager::~IgtlClientManager()
{
}

void IgtlClientManager::StartClient(char* serverIp, int serverPort)
{
  m_Client = igtl::ClientSocket::New();
  int r = m_Client->ConnectToServer(serverIp, serverPort);

  if(r < 0)
  {
    std::cerr << "CONNECTION FAILED";
    return;
  }
  if(m_Client->GetConnected() >= 0)
  {
    igtl::GetImageMetaMessage::Pointer getImageMetaMsg;
    getImageMetaMsg = igtl::GetImageMetaMessage::New();
    getImageMetaMsg->SetDeviceName("Client");
    getImageMetaMsg->Pack();
    m_Client->Send(getImageMetaMsg->GetPackPointer(), getImageMetaMsg->GetPackSize());

    //------------------------------------------------------------
    // Wait for a reply
    igtl::MessageHeader::Pointer headerMsg;
    headerMsg = igtl::MessageHeader::New();
    headerMsg->InitPack();
    int rs = m_Client->Receive(headerMsg->GetPackPointer(), headerMsg->GetPackSize());
    if (rs == 0)
    {
      std::cerr << "Connection closed." << std::endl;
      m_Client->CloseSocket();
      return;
    }
    if (rs != headerMsg->GetPackSize())
    {
      std::cerr << "Message size information and actual data size don't match." << std::endl; 
      return;
    }

    headerMsg->Unpack();
    if (strcmp(headerMsg->GetDeviceType(), "IMGMETA") == 0)
    {
      ReceiveImageMeta(m_Client, headerMsg);
    }
    else
    {
      std::cerr << "Invalid response from the server:" << headerMsg->GetDeviceName() << std::endl; 
      return;
    }

    igtl::Sleep(500); // wait
  }
}

void IgtlClientManager::StopClient()
{
}

} // end of namespace mitk

int ReceiveImageMeta(igtl::ClientSocket::Pointer& socket, igtl::MessageHeader::Pointer& header);
