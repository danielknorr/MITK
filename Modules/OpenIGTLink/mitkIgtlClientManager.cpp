/**
* @author Daniel Knorr
*/

#include "mitkIgtlClientManager.h"

#include <iostream>
#include <iomanip>

#include "igtlOSUtil.h"
#include "igtlMessageHeader.h"
#include "igtlTransformMessage.h"
#include "igtlPositionMessage.h"
#include "igtlImageMessage.h"
#include "igtlClientSocket.h"
#include "igtlStatusMessage.h"

int ReceiveTransform(igtl::Socket * socket, igtl::MessageHeader::Pointer& header);
int ReceivePosition(igtl::Socket * socket, igtl::MessageHeader::Pointer& header);
int ReceiveImage(igtl::Socket * socket, igtl::MessageHeader::Pointer& header);
int ReceiveStatus(igtl::Socket * socket, igtl::MessageHeader::Pointer& header);

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

    if (r != 0)
    {
      std::cerr << "Cannot connect to the server." << std::endl;
      return;
    }

    //------------------------------------------------------------
    // Create a message buffer to receive header
    igtl::MessageHeader::Pointer headerMsg;
    headerMsg = igtl::MessageHeader::New();

    //------------------------------------------------------------
    // Allocate a time stamp
    igtl::TimeStamp::Pointer ts;
    ts = igtl::TimeStamp::New();

    while (m_Client->GetConnected())
    {
      //------------------------------------------------------------
      // loop
      for (int i = 0; i < 100; i ++)
      {
        // Initialize receive buffer
        headerMsg->InitPack();

        // Receive generic header from the socket
        int r = m_Client->Receive(headerMsg->GetPackPointer(), headerMsg->GetPackSize());
        if (r == 0)
        {
          m_Client->CloseSocket();
          return;
        }
        if (r != headerMsg->GetPackSize())
        {
          continue;
        }

        // Deserialize the header
        headerMsg->Unpack();

        // Get time stamp
        igtlUint32 sec;
        igtlUint32 nanosec;

        headerMsg->GetTimeStamp(ts);
        ts->GetTimeStamp(&sec, &nanosec);

        std::cerr << "Time stamp: "
          << sec << "." << std::setw(9) << std::setfill('0')
          << nanosec << std::endl;

        // Check data type and receive data body
        if (strcmp(headerMsg->GetDeviceType(), "TRANSFORM") == 0)
        {
          ReceiveTransform(m_Client, headerMsg);
        }
        else if (strcmp(headerMsg->GetDeviceType(), "POSITION") == 0)
        {
          ReceivePosition(m_Client, headerMsg);
        }
        else if (strcmp(headerMsg->GetDeviceType(), "IMAGE") == 0)
        {
          ReceiveImage(m_Client, headerMsg);
        }
        else if (strcmp(headerMsg->GetDeviceType(), "STATUS") == 0)
        {
          ReceiveStatus(m_Client, headerMsg);
        }
        else
        {
          std::cerr << "Receiving : " << headerMsg->GetDeviceType() << std::endl;
          m_Client->Skip(headerMsg->GetBodySizeToRead(), 0);
        }
      }
    }
  }

  void IgtlClientManager::StopClient()
  {
    m_Client->CloseSocket();
  }
} // end of namespace mitk

int ReceiveTransform(igtl::Socket * socket, igtl::MessageHeader::Pointer& header)
{
  std::cerr << "Receiving TRANSFORM data type." << std::endl;

  // Create a message buffer to receive transform data
  igtl::TransformMessage::Pointer transMsg;
  transMsg = igtl::TransformMessage::New();
  transMsg->SetMessageHeader(header);
  transMsg->AllocatePack();

  // Receive transform data from the socket
  socket->Receive(transMsg->GetPackBodyPointer(), transMsg->GetPackBodySize());

  // Deserialize the transform data
  // If you want to skip CRC check, call Unpack() without argument.
  int c = transMsg->Unpack(1);

  if (c & igtl::MessageHeader::UNPACK_BODY) // if CRC check is OK
  {
    // Retrive the transform data
    igtl::Matrix4x4 matrix;
    transMsg->GetMatrix(matrix);
    igtl::PrintMatrix(matrix);
    std::cerr << std::endl;
    return 1;
  }

  return 0;
}

int ReceivePosition(igtl::Socket * socket, igtl::MessageHeader::Pointer& header)
{
  std::cerr << "Receiving POSITION data type." << std::endl;

  // Create a message buffer to receive transform data
  igtl::PositionMessage::Pointer positionMsg;
  positionMsg = igtl::PositionMessage::New();
  positionMsg->SetMessageHeader(header);
  positionMsg->AllocatePack();

  // Receive position position data from the socket
  socket->Receive(positionMsg->GetPackBodyPointer(), positionMsg->GetPackBodySize());

  // Deserialize the transform data
  // If you want to skip CRC check, call Unpack() without argument.
  int c = positionMsg->Unpack(1);

  if (c & igtl::MessageHeader::UNPACK_BODY) // if CRC check is OK
  {
    // Retrive the transform data
    float position[3];
    float quaternion[4];

    positionMsg->GetPosition(position);
    positionMsg->GetQuaternion(quaternion);

    std::cerr << "position   = (" << position[0] << ", " << position[1] << ", " << position[2] << ")" << std::endl;
    std::cerr << "quaternion = (" << quaternion[0] << ", " << quaternion[1] << ", "
      << quaternion[2] << ", " << quaternion[3] << ")" << std::endl << std::endl;

    return 1;
  }

  return 0;
}

int ReceiveImage(igtl::Socket * socket, igtl::MessageHeader::Pointer& header)
{
  std::cerr << "Receiving IMAGE data type." << std::endl;

  // Create a message buffer to receive transform data
  igtl::ImageMessage::Pointer imgMsg;
  imgMsg = igtl::ImageMessage::New();
  imgMsg->SetMessageHeader(header);
  imgMsg->AllocatePack();

  // Receive transform data from the socket
  socket->Receive(imgMsg->GetPackBodyPointer(), imgMsg->GetPackBodySize());

  // Deserialize the transform data
  // If you want to skip CRC check, call Unpack() without argument.
  int c = imgMsg->Unpack(1);

  if (c & igtl::MessageHeader::UNPACK_BODY) // if CRC check is OK
  {
    // Retrive the image data
    int   size[3];          // image dimension
    float spacing[3];       // spacing (mm/pixel)
    int   svsize[3];        // sub-volume size
    int   svoffset[3];      // sub-volume offset
    int   scalarType;       // scalar type

    scalarType = imgMsg->GetScalarType();
    imgMsg->GetDimensions(size);
    imgMsg->GetSpacing(spacing);
    imgMsg->GetSubVolume(svsize, svoffset);

    std::cerr << "Device Name           : " << imgMsg->GetDeviceName() << std::endl;
    std::cerr << "Scalar Type           : " << scalarType << std::endl;
    std::cerr << "Dimensions            : ("
      << size[0] << ", " << size[1] << ", " << size[2] << ")" << std::endl;
    std::cerr << "Spacing               : ("
      << spacing[0] << ", " << spacing[1] << ", " << spacing[2] << ")" << std::endl;
    std::cerr << "Sub-Volume dimensions : ("
      << svsize[0] << ", " << svsize[1] << ", " << svsize[2] << ")" << std::endl;
    std::cerr << "Sub-Volume offset     : ("
      << svoffset[0] << ", " << svoffset[1] << ", " << svoffset[2] << ")" << std::endl << std::endl;
    return 1;
  }

  return 0;
}

int ReceiveStatus(igtl::Socket * socket, igtl::MessageHeader::Pointer& header)
{
  std::cerr << "Receiving STATUS data type." << std::endl;

  // Create a message buffer to receive transform data
  igtl::StatusMessage::Pointer statusMsg;
  statusMsg = igtl::StatusMessage::New();
  statusMsg->SetMessageHeader(header);
  statusMsg->AllocatePack();

  // Receive transform data from the socket
  socket->Receive(statusMsg->GetPackBodyPointer(), statusMsg->GetPackBodySize());

  // Deserialize the transform data
  // If you want to skip CRC check, call Unpack() without argument.
  int c = statusMsg->Unpack(1);

  if (c & igtl::MessageHeader::UNPACK_BODY) // if CRC check is OK
  {
    std::cerr << "========== STATUS ==========" << std::endl;
    std::cerr << " Code      : " << statusMsg->GetCode() << std::endl;
    std::cerr << " SubCode   : " << statusMsg->GetSubCode() << std::endl;
    std::cerr << " Error Name: " << statusMsg->GetErrorName() << std::endl;
    std::cerr << " Status    : " << statusMsg->GetStatusString() << std::endl;
    std::cerr << "============================" << std::endl << std::endl;
  }

  return 0;
}
