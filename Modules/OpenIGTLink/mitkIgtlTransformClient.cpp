/**
* @author Daniel Knorr
*/

#include "mitkIgtlTransformClient.h"

#include <igtlOSUtil.h>
#include <igtlTransformMessage.h>
#include <itkMatrix.h>

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
    // Retrieve the transform data
    igtl::Matrix4x4 matrix;
    transMsg->GetMatrix(matrix);
    typedef itk::Matrix<float, 4, 4> KalibrationMatrix;
    KalibrationMatrix kalibMatrix;
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        kalibMatrix(i,j) = matrix[i][j];
        std::cout << "Matrix: " << i << ". row, " << j << ". col - value: " << matrix[i][j];
      }
    }

    // itk::matrix is put into mitk::matrix
    m_TransformMatrix = kalibMatrix;
    std::cout << "Transform Matrix received.";
  }
}

mitk::Matrix<float, 4, 4> IgtlTransformClient::GetTransformMatrix()
{
  return m_TransformMatrix;
}

void IgtlTransformClient::run()
{
  Connect();
  this->Receive();
}

}  // end of namespace mitk
