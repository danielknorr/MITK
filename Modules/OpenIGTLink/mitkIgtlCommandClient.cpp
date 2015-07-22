/**
* @author Daniel Knorr
*/

#include "mitkIgtlCommandClient.h"

#include <usGetModuleContext.h>

#include <mitkImage.h>
#include <mitkPixelType.h>
#include <mitkDataNode.h>
#include <mitkOpenCVToMitkImageFilter.h>
#include <mitkImageWriteAccessor.h>
#include <mitkRenderWindowBase.h>
#include <mitkDataStorage.h>

#include <igtlStringMessage.h>
#include <igtlImageMessage.h>

//#include <qfile.h>
#include <qstring.h>

#include <cv.h>

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

    while (m_IsRunning)
    {
      // Wait for a reply
      m_Header = igtl::MessageHeader::New();
      m_Header->InitPack();
      int rs = m_Socket->Receive(m_Header->GetPackPointer(), m_Header->GetPackSize());

      if (rs == 0)
      {
        Disconnect();
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

      if(!IsConnected()) m_IsRunning = false;
    }

    if(m_Socket.IsNotNull() && IsConnected()){
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

    // if CRC check is OK
    if (c & igtl::MessageHeader::UNPACK_BODY)
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

    // if CRC check is OK
    if (c & igtl::MessageHeader::UNPACK_BODY)
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

      // 
      unsigned int dims[3];
      dims[0]= size[0];
      dims[1]= size[1];
      dims[2]= size[2];

      // Initializing a mitk::Image
      mitk::Image::Pointer mitkImage = mitk::Image::New();

      switch (scalarType)
      {
      case igtl::ImageMessage::TYPE_UINT8:
        mitkImage->Initialize(MakePixelType<uchar,3>(1),3,dims);
        break;
      case igtl::ImageMessage::TYPE_INT8:
        mitkImage->Initialize(MakePixelType<char,3>(1),3,dims);
        break;
      case igtl::ImageMessage::TYPE_UINT16:
        mitkImage->Initialize(MakePixelType<ushort,3>(1),3,dims);
        break;
      case igtl::ImageMessage::TYPE_INT16:
        mitkImage->Initialize(MakePixelType<short,3>(1),3,dims);
        break;
      }

      mitk::ImageWriteAccessor writeAccess(mitkImage);
      void* vPointer = writeAccess.GetData();

      memcpy(vPointer, image->GetScalarPointer(), image->GetImageSize());
      /*
      cv::Mat cvImage(3, size, scalarType, vPointer);

      //Transform with Filter from cv::Mat to mitk::Image
      mitk::OpenCVToMitkImageFilter::Pointer filter = mitk::OpenCVToMitkImageFilter::New();
      filter->SetOpenCVMat(cvImage);
      filter->Update();

      //mitkImage = filter->GetOutput();
      mitkImage->Initialize(filter->GetOutput());
      */
      // Create a new DataNode and enter the image
      mitk::DataNode::Pointer node = mitk::DataNode::New();
      node->SetData(mitkImage);

      // Add DataNode to DataStorage (MANAGER)
      static us::ModuleContext* moduleContext = us::GetModuleContext();
      mitk::DataStorage::Pointer ds =
        moduleContext->GetService(moduleContext->GetServiceReference<mitk::DataStorage>());
      ds->Add(node);
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

void IgtlCommandClient::run()
{
  //this->setAutoDelete(false);
  Connect();
  this->Receive();
}

}  // end of namespace mitk
