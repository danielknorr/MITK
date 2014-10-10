/**
* @author Daniel Knorr
*/

#ifndef MITKIGTLCLIENTMANAGER_H_
#define MITKIGTLCLIENTMANAGER_H_

#include <usServiceRegistration.h>

#include "MitkOPENIGTLINKExports.h"
#include "mitkIgtlClientSocket.h"

namespace mitk {

class MITK_OPENIGTLINK_EXPORT IgtlClientManager
{
public:

  IgtlClientManager();
  ~IgtlClientManager();

  void StartClient(char* serverIp, int serverPort);
  void StopClient();

private:

  us::ServiceRegistration<IgtlClientSocket> m_ClientRegistration;
  IgtlClientSocket::Pointer m_Client;

};

#endif // MITKIGTLCLIENTMANAGER_H_

} // end of namespace mitk
