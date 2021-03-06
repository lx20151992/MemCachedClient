#ifndef _MEMCACHEDCLIENT_H
#define _MEMCACHEDCLIENT_H
#include <functional>
#include <boost/atomic.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/pool/object_pool.hpp>
#include "memcachedClient/RequestItem.h"
#include "serverManager/ServerList.h"
#include "MemLog.h"
class MemcachedClient
    :boost::noncopyable
{
public:
    MEMCACHEDCLIENT_EXPORT explicit MemcachedClient(boost::asio::io_service& ioService);

    ServerList Servers;


    MEMCACHEDCLIENT_EXPORT MemGetResult::Ptr Get(const std::string& key,Callback callback = DefaultCallback);
    MEMCACHEDCLIENT_EXPORT MemGetResult::Ptr Get(const std::string& key,const Buffer& buf,Callback callback = DefaultCallback);

    MEMCACHEDCLIENT_EXPORT MemSetResult::Ptr Set(const std::string& key,uint32_t flag,uint32_t expiry,const ConstBuffer& buf,Callback callback = DefaultCallback);
    MEMCACHEDCLIENT_EXPORT MemAddResult::Ptr Add(const std::string& key,uint32_t flag,uint32_t expiry,const ConstBuffer& buf,Callback callback = DefaultCallback);
    MEMCACHEDCLIENT_EXPORT MemReplaceResult::Ptr Replace(const std::string& key,uint32_t flag,uint32_t expiry,const ConstBuffer& buf,Callback callback = DefaultCallback);
    MEMCACHEDCLIENT_EXPORT MemDeleteResult::Ptr Delete(const std::string& key,Callback callback = DefaultCallback);
   
    MEMCACHEDCLIENT_EXPORT MemIncResult::Ptr Increment(const std::string& key,uint64_t delta,uint64_t initValue,uint32_t expiry = 0xffffffff,Callback callback = DefaultCallback);
    MEMCACHEDCLIENT_EXPORT MemDecResult::Ptr Decrement(const std::string& key,uint64_t delta,uint64_t initValue,uint32_t expiry = 0xffffffff,Callback callback = DefaultCallback);
    
    MEMCACHEDCLIENT_EXPORT MemAppendResult::Ptr Append(const std::string& key,const ConstBuffer& value,Callback callback = DefaultCallback);
    MEMCACHEDCLIENT_EXPORT MemPreAppendResult::Ptr PreAppend(const std::string& key,const ConstBuffer& value,Callback callback = DefaultCallback);
    
    MEMCACHEDCLIENT_EXPORT MemFlushResult::Ptr Flush(ServerItem::Ptr pServer,uint32_t expiry,Callback callback = DefaultCallback);
    
    MEMCACHEDCLIENT_EXPORT MemVersionResult::Ptr Version(ServerItem::Ptr pServer,Callback callback = DefaultCallback);
     
    MEMCACHEDCLIENT_EXPORT MemStatResult::Ptr Stat(ServerItem::Ptr pServer,const std::string& key,Callback callback = DefaultCallback);
private:
    typedef boost::unordered_map<int,RequestItem> RequestMap;

    void OnServerAdded(const ServerItem::Ptr& item);
    void OnServerRemoved(const ServerItem::Ptr& item);
    bool OnHeaderReaded(void* header,VBuffer& body);
    void OnBodayReaded(const void* header,const VBuffer& boday);
    void FinishRequest(RequestMap::iterator,ERequestStatus err);

    MemResult::Ptr Store(uint8_t cmd,const std::string& key,uint32_t flag,uint32_t expiry,const ConstBuffer& buf,Callback callback);
    MemIncResult::Ptr SelfOp(uint8_t cmd,const std::string& key,uint64_t delta,uint64_t initValue,uint32_t expiry,Callback callback);
    MemAppendResult::Ptr Append(uint8_t cmd,const std::string& key,const ConstBuffer& value,Callback callback = DefaultCallback);

    static void DefaultCallback(const MemResult::Ptr&){}
    static void AdjustEndian(protocol_binary_response_header* response);
    static void AdjustEndian(protocol_binary_request_header* request);
private:

    boost::asio::io_service& mIoService;

    boost::atomic_int mNextRequestId;

    RequestMap mRequests;

    boost::mutex mSyncRequest;

//    MemLog mLog;
};
#endif
 
