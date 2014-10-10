/**
* @author Daniel Knorr
*/

#ifndef MITKIGTLCLIENTSOCKET_H
#define MITKIGTLCLIENTSOCKET_H

#include <igtlClientSocket.h>

#include <usServiceInterface.h>

namespace mitk {

  typedef igtl::ClientSocket IgtlClientSocket;

} // end of namespace mitk
US_DECLARE_SERVICE_INTERFACE(mitk::IgtlClientSocket, "org.mitk.services.IgtlClientSocket")

#endif // MITKIGTLCLIENTSOCKET_H
