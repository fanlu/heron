#ifndef __DUMMY_MTMGR_H
#define __DUMMY_MTMGR_H

#include "threads/spcountdownlatch.h"
#include "network/network_error.h"

namespace heron {
  namespace proto {
    namespace tmaster {
      class TMasterLocation;
    }
  }
}

class DummyMtrMgr : public Server
{
 public:
  DummyMtrMgr(EventLoopImpl* ss, const NetworkOptions& options,
              const sp_string& stmgr_id, CountDownLatch* tmasterLatch,
              CountDownLatch* connectionCloseLatch);
  virtual ~DummyMtrMgr();

  heron::proto::tmaster::TMasterLocation* get_tmaster();

 protected:
  // handle an incoming connection from server
  virtual void
  HandleNewConnection(Connection* newConnection);

  // handle a connection close
  virtual void HandleConnectionClose(Connection* connection, NetworkErrorCode status);

  // Handle metrics publisher request
  virtual void HandleMetricPublisherRegisterRequest(REQID _id, Connection* _conn,
                                       heron::proto::system::MetricPublisherRegisterRequest* _request);
  virtual void HandleMetricPublisherPublishMessage(Connection* _conn,
                                        heron::proto::system::MetricPublisherPublishMessage* _message);
  virtual void HandleTMasterLocationMessage(Connection*,
                                            heron::proto::system::TMasterLocationRefreshMessage* _message);

 private:
  sp_string stmgr_id_expected_;
  heron::proto::tmaster::TMasterLocation* location_;
  // Used to signal that tmaster location has been received
  CountDownLatch* tmasterLatch_;
  // Used to signal that connection to stmgr has been closed
  CountDownLatch* connectionCloseLatch_;
};

#endif
