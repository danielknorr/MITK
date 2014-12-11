/**
* @author Daniel Knorr
*/

#include "mitkIgtlCommandClient.h"

#include <mitkImage.h>
#include <mitkDataNode.h>

#include <igtlStringMessage.h>
#include <igtlImageMessage.h>

#include <qfile.h>

namespace mitk {

  IgtlCommandClient::IgtlCommandClient()
  {
  }

  IgtlCommandClient::~IgtlCommandClient()
  {
  }

  void IgtlCommandClient::Receive()
  {
    /********************************** TESTCODE **********************************
    igtl::StringMessage::Pointer strMsg = igtl::StringMessage::New();
    strMsg->SetDeviceName("CMD_commandUid");

    std::cout << "Sending command: " << "RequestChannelIds" << std::endl;
    strMsg->SetString("<Command Name=\"RequestChannelIds\" />");
    strMsg->Pack();
    m_Socket->Send(strMsg->GetPackPointer(), strMsg->GetPackSize());
    */

    while (true)
    {
      // Wait for a reply
      m_Header = igtl::MessageHeader::New();
      m_Header->InitPack();
      int rs = m_Socket->Receive(m_Header->GetPackPointer(), m_Header->GetPackSize());
      if (rs == 0)
      {
        std::cerr << "Connection closed." << std::endl;
        m_Socket->CloseSocket();
        return;
      }
      if (rs != m_Header->GetPackSize())
      {
        std::cerr << "Message size information and actual data size don't match." << std::endl;
        //m_Socket->CloseSocket();
        continue;
      }

      m_Header->Unpack();
      if (strcmp(m_Header->GetDeviceType(), "STRING") == 0)
      {
        ReceiveCommand();
        break;
      }
      else if (strcmp(m_Header->GetDeviceType(), "IMAGE") == 0)
      {
        ReceiveImage();
        break;
      }
      else
      {
        std::cerr << "Receiving : " << m_Header->GetDeviceType() << std::endl;
        m_Socket->Skip(m_Header->GetBodySizeToRead(), 0);
      }
    }

    if(m_Socket.IsNotNull() && m_Socket->GetConnected()){
      m_Socket->CloseSocket();
    }
  }

  void IgtlCommandClient::ReceiveCommand()
  {
    // Allocate command Message Class
    igtl::StringMessage::Pointer command;
    command = igtl::StringMessage::New();
    command->SetMessageHeader(m_Header);
    command->AllocatePack();

    // Receive body from the socket
    m_Socket->Receive(command->GetPackBodyPointer(), command->GetPackBodySize());

    // Deserialize the transform data
    // If you want to skip CRC check, call Unpack() without argument.
    int c = command->Unpack(1);

    if (c & igtl::MessageHeader::UNPACK_BODY) // if CRC check is OK
    {
      std::cout << command->GetString();
    }
  }

  void IgtlCommandClient::ReceiveImage()
  {
    // Allocate image Message Class
    igtl::ImageMessage::Pointer image;
    image = igtl::ImageMessage::New();
    image->SetMessageHeader(m_Header);
    image->AllocatePack();

    // Receive body from the socket
    m_Socket->Receive(image->GetPackBodyPointer(), image->GetPackBodySize());

    // Deserialize the transform data
    // If you want to skip CRC check, call Unpack() without argument.
    int c = image->Unpack(1);

    if (c & igtl::MessageHeader::UNPACK_BODY) // if CRC check is OK
    {
      // Retrive the image data
      int   size[3];          // image dimension
      float spacing[3];       // spacing (mm/pixel)
      int   svsize[3];        // sub-volume size
      int   svoffset[3];      // sub-volume offset
      int   scalarType;       // scalar type

      scalarType = image->GetScalarType();
      image->GetDimensions(size);
      image->GetSpacing(spacing);
      image->GetSubVolume(svsize, svoffset);

      // Creating a mitk::ImageDescriptor
      mitk::ImageDescriptor::Pointer desc;
      //desc: Create form file

      // Initializing a mitk::Image
      mitk::Image::Pointer mitkImage = mitk::Image::New();
      mitkImage->Initialize(desc);

      mitk::DataNode::Pointer node = mitk::DataNode::New();
      node->SetData(mitkImage);
      //Add to DataManager
    }
  }

  void IgtlCommandClient::StartRecording(QString outFilenamePath)
  {/*
    QFile file(outFilenamePath);
    if ( !file.exists() )
    {
      std::cerr << "No file with filename " << outFilenamePath << " exists." << std::endl;
      return;
    }*/
    igtl::StringMessage::Pointer strMsg = igtl::StringMessage::New();
    strMsg->SetDeviceName("CMD_commandUid");

    std::cout << "Sending command: " << "StartRecording " << "with OutputFilename: " << outFilenamePath.toStdString() << std::endl;
    QString sendStr("<Command Name=\"StartRecording\" OutputFilename=\"" + outFilenamePath + "\" />");
    strMsg->SetString(sendStr.toStdString());
    strMsg->Pack();
    m_Socket->Send(strMsg->GetPackPointer(), strMsg->GetPackSize());

    Receive();
  }

  void IgtlCommandClient::StopRecording()
  {
    igtl::StringMessage::Pointer strMsg = igtl::StringMessage::New();
    strMsg->SetDeviceName("CMD_commandUid");

    std::cout << "Sending command: " << "StopRecording" << std::endl;
    char* sendStr = "<Command Name=\"StopRecording\" />";
    strMsg->SetString(sendStr);
    strMsg->Pack();
    m_Socket->Send(strMsg->GetPackPointer(), strMsg->GetPackSize());

    Receive();
  }

  void IgtlCommandClient::ReconstructVolume(QString inFilenamePath, QString outFilenamePath)
  {
    igtl::StringMessage::Pointer strMsg = igtl::StringMessage::New();
    strMsg->SetDeviceName("CMD_commandUid");

    std::cout << "Sending command: " << "RecontructVolume " << "with InputSeqFilename: " <<
      inFilenamePath.toStdString() << " and OutputVolFilename: " << outFilenamePath.toStdString() << std::endl;
    QString sendStr("<Command Name=\"StopRecording\" InputSeqFilename=\"" + inFilenamePath + "\" OutputVolFilename=\"" + outFilenamePath + "\" />");
    strMsg->SetString(sendStr.toStdString());
    strMsg->Pack();
    m_Socket->Send(strMsg->GetPackPointer(), strMsg->GetPackSize());

    Receive();
  }

}  // end of namespace mitk
