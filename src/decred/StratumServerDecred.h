/*
 The MIT License (MIT)

 Copyright (c) [2018] [BTC.COM]

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#ifndef STRATUM_SERVER_DECRED_H_
#define STRATUM_SERVER_DECRED_H_

#include "StratumServer.h"
#include "StratumDecred.h"

class ServerDecred;

class JobRepositoryDecred : public JobRepositoryBase<ServerDecred> {
public:
  JobRepositoryDecred(const char *kafkaBrokers, const char *consumerTopic, const string &fileLastNotifyTime, ServerDecred *server);

  StratumJob* createStratumJob() override;
  void broadcastStratumJob(StratumJob *sjob) override;

private:
  uint32_t lastHeight_;
};

class ServerDecred : public ServerBase<JobRepositoryDecred> {
public:
  explicit ServerDecred(int32_t shareAvgSeconds, const libconfig::Config &config);
  unique_ptr<StratumSession> createConnection(bufferevent *bev, sockaddr *saddr, uint32_t sessionID) override;

  int checkShare(ShareDecred &share, shared_ptr<StratumJobEx> exJobPtr, const vector<uint8_t> &extraNonce2,
                 uint32_t ntime, uint32_t nonce, const string &workerFullName);

protected:
  JobRepository* createJobRepository(const char *kafkaBrokers, const char *consumerTopic, const string &fileLastNotifyTime) override;

private:
  void sendSolvedShare2Kafka(const FoundBlockDecred &foundBlock);
  
  unique_ptr<StratumProtocolDecred> protocol_;
};

#endif
