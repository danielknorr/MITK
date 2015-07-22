/**
* @author Daniel Knorr
*/

#ifndef MITKIGTLTRANSFORMCLIENT_H
#define MITKIGTLTRANSFORMCLIENT_H

#include "MitkOPENIGTLINKExports.h"

#include <mitkIgtlReceiveClient.h>
#include <mitkMatrix.h>

namespace mitk {

class MITK_OPENIGTLINK_EXPORT IgtlTransformClient : public IgtlReceiveClient
{
public:

  IgtlTransformClient();
  ~IgtlTransformClient();

  void run();

  void Receive();

  mitk::Matrix<float, 4, 4> GetTransformMatrix();

private:

  mitk::Matrix<float, 4, 4> m_TransformMatrix;
};

} // end of namespace mitk

#endif // MITKIGTLTRANSFORMCLIENT_H
