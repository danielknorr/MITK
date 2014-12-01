/**
* @author Daniel Knorr
*/

#include "mitkIgtlCommandClient.h"

namespace mitk {

IgtlCommandClient::IgtlCommandClient()
{
}

IgtlCommandClient::~IgtlCommandClient()
{
}

void IgtlCommandClient::Receive()
{
}

}  // end of namespace mitk

/********************************** TESTCODE **********************************
igtl::StringMessage::Pointer strMsg = igtl::StringMessage::New();
  strMsg->SetDeviceName("CMD_commandUid");

  std::cout << "Sending command: " << "RequestChannelIds" << std::endl;
  strMsg->SetString("<Command Name=\"RequestChannelIds\" />");
  strMsg->Pack();
  socket->Send(strMsg->GetPackPointer(), strMsg->GetPackSize());
  

  int loop = 0;

  while (1)
    {
    //------------------------------------------------------------
    // Wait for a reply
    igtl::MessageHeader::Pointer headerMsg;
    headerMsg = igtl::MessageHeader::New();
    headerMsg->InitPack();
    int rs = socket->Receive(headerMsg->GetPackPointer(), headerMsg->GetPackSize());
    if (rs == 0)
      {
      std::cerr << "Connection closed." << std::endl;
      socket->CloseSocket();
      exit(0);
      }
    if (rs != headerMsg->GetPackSize())
      {
      std::cerr << "Message size information and actual data size don't match." << std::endl; 
      socket->CloseSocket();
      exit(0);
      }
    
    headerMsg->Unpack();
    if (strcmp(headerMsg->GetDeviceType(), "STRING") == 0)
      {
      ReceiveTrackingData(socket, headerMsg);
      }
    else
      {
      std::cerr << "Receiving : " << headerMsg->GetDeviceType() << std::endl;
      socket->Skip(headerMsg->GetBodySizeToRead(), 0);
      }
    }


  if(socket.IsNotNull() && socket->GetConnected()){
    socket->CloseSocket();
  }
}

int ReceiveTrackingData(igtl::ClientSocket::Pointer& socket, igtl::MessageHeader::Pointer& header)
{
  
  //------------------------------------------------------------
  // Allocate TrackingData Message Class

  igtl::StringMessage::Pointer trackingData;
  trackingData = igtl::StringMessage::New();
  trackingData->SetMessageHeader(header);
  trackingData->AllocatePack();

  // Receive body from the socket
  socket->Receive(trackingData->GetPackBodyPointer(), trackingData->GetPackBodySize());

  // Deserialize the transform data
  // If you want to skip CRC check, call Unpack() without argument.
  int c = trackingData->Unpack(1);

  if (c & igtl::MessageHeader::UNPACK_BODY) // if CRC check is OK
  {
    std::cerr << trackingData->GetString();
    // Kommt nicht soweit!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }
  return 0;
}
*/
