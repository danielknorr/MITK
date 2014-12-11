/**
* @autor Daniel Knorr
*/

#include "mitkIgtlClientSocketTracker.h"

namespace mitk {

  IgtlClientSocketTracker::IgtlClientSocketTracker(us::ModuleContext* context)
    : Superclass(context),
    m_Context(context)
  {
  }

  IgtlClientSocket* IgtlClientSocketTracker::AddingService(const ServiceReferenceType &reference)
  {
    IgtlClientSocket* socket = Superclass::AddingService(reference);
    return socket;
  }

  void IgtlClientSocketTracker::RemovedService(const ServiceReferenceType& reference, IgtlClientSocket* tracked)
  {
    Superclass::RemovedService(reference, tracked);
  }

} // end of namespace mitk
