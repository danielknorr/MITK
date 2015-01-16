/**
* @author Daniel Knorr
*/

#include "mitkIgtlTransformClient.h"

#include <igtlOSUtil.h>
#include <igtlTransformMessage.h>

namespace mitk {

IgtlTransformClient::IgtlTransformClient()
{
}

IgtlTransformClient::~IgtlTransformClient()
{
}

void IgtlTransformClient::Receive()
{
  // Allocate image Message Class
  igtl::TransformMessage::Pointer transMsg;
  transMsg = igtl::TransformMessage::New();
  transMsg->SetMessageHeader(m_Header);
  transMsg->AllocatePack();

  // Receive transform data from the socket
  m_Socket->Receive(transMsg->GetPackBodyPointer(), transMsg->GetPackBodySize());

  // Deserialize the transform data
  // If you want to skip CRC check, call Unpack() without argument.
  int c = transMsg->Unpack(1);

  if (c & igtl::MessageHeader::UNPACK_BODY) // if CRC check is OK
  {
    // Retrive the transform data
    igtl::Matrix4x4 matrix;
    transMsg->GetMatrix(matrix);
    // TODO
  }
}

}  // end of namespace mitk
