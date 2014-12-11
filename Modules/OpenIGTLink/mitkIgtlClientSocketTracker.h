/**
* @autor Daniel Knorr
*/

#ifndef MITKIGTLCLIENTSOCKETTRACKER_H
#define MITKIGTLCLIENTSOCKETTRACKER_H

#include <usServiceTracker.h>

#include "MitkOPENIGTLINKExports.h"
#include "mitkIgtlClientSocket.h"

namespace mitk {

class MITK_OPENIGTLINK_EXPORT IgtlClientSocketTracker : public us::ServiceTracker<IgtlClientSocket>
{

public:
  IgtlClientSocketTracker(us::ModuleContext* context);

private:
  typedef us::ServiceTracker<IgtlClientSocket> Superclass;

  us::ModuleContext* m_Context;

  virtual TrackedType AddingService(const ServiceReferenceType &reference);
  virtual void RemovedService(const ServiceReferenceType& reference, TrackedType tracked);
};

} // end of namespace mitk

#endif // MITKIGTLCLIENTSOCKETTRACKER_H
