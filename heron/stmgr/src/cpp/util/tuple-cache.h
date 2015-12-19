#ifndef SRC_CPP_SVCS_STMGR_SRC_UTIL_TUPLE_CACHE_H_
#define SRC_CPP_SVCS_STMGR_SRC_UTIL_TUPLE_CACHE_H_

#include <list>
#include <map>

namespace heron {
namespace stmgr {

class StMgr;

class TupleCache {
 public:
  TupleCache(EventLoop* eventLoop, sp_uint32 _drain_threshold);
  virtual ~TupleCache();

  template <class T>
  void RegisterDrainer(void (T::*method)(sp_int32,
                                         proto::system::HeronTupleSet*),
                       T* _t) {
    drainer_ = std::bind(method, _t, std::placeholders::_1,
                              std::placeholders::_2);
  }

  // returns tuple key
  sp_int64 add_data_tuple(sp_int32 _task_id,
                          const proto::api::StreamId& _streamid,
                          const proto::system::HeronDataTuple& _tuple);
  void add_ack_tuple(sp_int32 _task_id, const proto::system::AckTuple& _tuple);
  void add_fail_tuple(sp_int32 _task_id, const proto::system::AckTuple& _tuple);
  void add_emit_tuple(sp_int32 _task_id, const proto::system::AckTuple& _tuple);

 private:
  void drain(EventLoop::Status);
  void drain_impl();

  class TupleList {
    // not accessible to anyone else
    friend class TupleCache;

   private:
    TupleList();
    ~TupleList();

    sp_int64 add_data_tuple(const proto::api::StreamId& _streamid,
                            const proto::system::HeronDataTuple& _tuple,
                            sp_uint64* total_size_,
                            sp_uint64* _tuples_cache_max_tuple_size);
    void add_ack_tuple(const proto::system::AckTuple& _tuple,
                       sp_uint64* total_size_);
    void add_fail_tuple(const proto::system::AckTuple& _tuple,
                        sp_uint64* total_size_);
    void add_emit_tuple(const proto::system::AckTuple& _tuple,
                        sp_uint64* total_size_);

    void drain(sp_int32 _task_id,
               std::function<void(sp_int32, proto::system::HeronTupleSet*)>
                   _drainer);

   private:
    std::list<proto::system::HeronTupleSet*> tuples_;
    proto::system::HeronTupleSet* current_;
    sp_uint64 current_size_;
    sp_int32 last_drained_count_;
  };

  TupleList* get(sp_int32 _task_id);

  // map from task_id to the TupleList
  std::map<sp_int32, TupleList*> cache_;
  EventLoop* eventLoop_;
  std::function<void(sp_int32, proto::system::HeronTupleSet*)> drainer_;
  sp_uint64 total_size_;
  sp_uint32 drain_threshold_bytes_;

  // Configs to be read
  sp_int32 cache_drain_frequency_ms_;
  sp_uint64 tuples_cache_max_tuple_size_;
};

}  // namespace stmgr
}  // namespace heron

#endif  // SRC_CPP_SVCS_STMGR_SRC_UTIL_TUPLE_CACHE_H_
