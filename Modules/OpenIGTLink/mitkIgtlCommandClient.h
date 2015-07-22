/**
* @author Daniel Knorr
*/

#ifndef MITKIGTLCOMMANDCLIENT_H
#define MITKIGTLCOMMANDCLIENT_H

#include "MitkOPENIGTLINKExports.h"

#include <mitkIgtlReceiveClient.h>

namespace mitk {

class MITK_OPENIGTLINK_EXPORT IgtlCommandClient : public IgtlReceiveClient
{
public:

  IgtlCommandClient();
  ~IgtlCommandClient();

  void run();

  virtual void Receive();

  void StartRecording(QString outFilenamePath);
  void StopRecording();

  void ReconstructVolume(QString inFilenamePath, QString outFilenamePath);

private:

  void ReceiveCommand();
  void ReceiveImage();

};

} // end of namespace mitk

#endif // MITKIGTLCOMMANDCLIENT_H
