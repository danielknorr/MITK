/**
* @author Daniel Knorr
*/

#ifndef MITKIGTLCOMMANDCLIENT_H
#define MITKIGTLCOMMANDCLIENT_H

#include "MitkOPENIGTLINKExports.h"

#include "mitkIgtlReceiveClient.h"

namespace mitk {

class MITK_OPENIGTLINK_EXPORT IgtlTransformClient : IgtlReceiveClient
{
public:

  IgtlTransformClient();
  ~IgtlTransformClient();

protected:

  virtual void Receive();

private:

};

} // end of namespace mitk

#endif // MITKIGTLCOMMANDCLIENT_H
