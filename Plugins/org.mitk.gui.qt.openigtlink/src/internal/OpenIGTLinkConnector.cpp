#include "OpenIGTLinkConnector.h"
// OpenIGTLink
#include "igtlOSUtil.h"
#include "igtlImageMessage.h"
#include "igtlServerSocket.h"
#include "igtlClientSocket.h"
#include <mitkImage.h>
#include <mitkImageReadAccessor.h>
#include <qstring.h>




void OpenIGTLinkConnector::stop(){
  _isRunning = false;
}

int OpenIGTLinkConnector::ApplyImageToMessage(igtl::ImageMessage::Pointer& msg)
{
  if(_usImage.IsNotNull() && _usImage->IsInitialized() ){
    try
    {
      mitk::ImageReadAccessor readAccess(_usImage, _usImage->GetVolumeData(0));
      const void* cPointer = readAccess.GetData();
      if(cPointer != NULL && _usImage->IsInitialized())
      {
        if(*((unsigned char*)cPointer )==205){
        std::cerr << "buffer is clearing or cleared" << std::endl;
          return 0;
        }
        memcpy(msg->GetScalarPointer(), cPointer, msg->GetImageSize());

      }
      else
      {
        std::cerr << "image.GetData() is null" << std::endl;
      }
    }
    catch(mitk::Exception& e)
    {
      std::cerr << "image is null;" << e.GetDescription() << std::endl;
    }
  }else{
    std::cerr << "image is null or not initialized" << std::endl;

  }
  return 1;
}

void OpenIGTLinkConnector::sendImage(igtl::ClientSocket::Pointer socket, igtl::TimeStamp::Pointer ts){
  //------------------------------------------------------------
  // size parameters

  // image dimension 
  float spacing[]  = {1.0, 1.0, 5.0};     // spacing (mm/pixel)
  int   svsize[]   = {_usImage->GetDimension(0), _usImage->GetDimension(1), 1};       // sub-volume size
  int   svoffset[] = {0, 0, 0};           // sub-volume offset
  int   scalarType = igtl::ImageMessage::TYPE_UINT8;// scalar type

  //------------------------------------------------------------
  // Create a new IMAGE type message
  igtl::ImageMessage::Pointer imgMsg = igtl::ImageMessage::New();
  imgMsg->SetDimensions(_usImage->GetDimension(0),_usImage->GetDimension(1),1);
  imgMsg->SetSpacing(spacing);
  if(_usImage->GetPixelType().GetPixelType() != itk::ImageIOBase::SCALAR)
    return;

  switch (_usImage->GetPixelType().GetComponentType())
  {
    case itk::ImageIOBase::UCHAR:
      scalarType = igtl::ImageMessage::TYPE_UINT8;
      break;
    case itk::ImageIOBase::CHAR:
      scalarType = igtl::ImageMessage::TYPE_INT8;
      break;
    case itk::ImageIOBase::USHORT:
      scalarType = igtl::ImageMessage::TYPE_UINT16;
      break;
    case itk::ImageIOBase::SHORT:
      scalarType = igtl::ImageMessage::TYPE_INT16;
      break;
  }
  imgMsg->SetScalarType(scalarType); 
  imgMsg->SetDeviceName("MITK");
  imgMsg->SetSubVolume(svsize, svoffset);
  imgMsg->AllocateScalars();
  imgMsg->SetTimeStamp(ts);
  igtlUint32 sec;
  igtlUint32 nsec;
  ts->GetTimeStamp(&sec, &nsec);
  std::cerr << "Time Stamp: sec = " << sec << ", nsec = " << nsec << std::endl;
  //bdata->
  //------------------------------------------------------------
  // Set image data (See GetTestImage() bellow for the details)
  int success = ApplyImageToMessage(imgMsg);
  if(success == 0)
    return;

  //------------------------------------------------------------
  // Get orientation matrix and set it.
  igtl::Matrix4x4 matrix;
  igtl::IdentityMatrix(matrix);
  imgMsg->SetMatrix(matrix);

  //------------------------------------------------------------
  // Pack (serialize) and send
  imgMsg->Pack();
  socket->Send(imgMsg->GetPackPointer(), imgMsg->GetPackSize());

  igtl::Sleep((int) (1000.0 / _fps)); // wait
}