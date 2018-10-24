/*
 The MIT License (MIT)

 Copyright (c) [2016] [BTC.COM]

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

#ifndef STRATUM_MESSAGE_DISPATCHER_H
#define STRATUM_MESSAGE_DISPATCHER_H

#include "utilities_js.hpp"

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

class IStratumSession;
class StratumJobEx;
class StratumMiner;
class DiffController;
struct LocalJob;

class StratumMessageDispatcher {
public:
  virtual ~StratumMessageDispatcher() = default;

  virtual void handleRequest(const std::string &idStr, const std::string &method, const JsonNode &jparams, const JsonNode &jroot) = 0;
  virtual void handleExMessage(const std::string &exMessage) = 0;
  virtual void responseShareAccepted(const std::string &idStr) = 0;
  virtual void responseShareError(const std::string &idStr, int32_t status) = 0;
  virtual void setMinDiff(uint64_t minDiff) = 0;
  virtual void resetCurDiff(uint64_t curDiff) = 0;
  virtual void addLocalJob(LocalJob &localJob) = 0;
  virtual void removeLocalJob(LocalJob &localJob) = 0;
};

class StratumMessageMinerDispatcher : public StratumMessageDispatcher {
public:
  StratumMessageMinerDispatcher(IStratumSession &session, std::unique_ptr<StratumMiner> miner);

  void handleRequest(const std::string &idStr, const std::string &method, const JsonNode &jparams, const JsonNode &jroot) override;
  void handleExMessage(const std::string &exMessage) override;
  void responseShareAccepted(const std::string &idStr) override;
  void responseShareError(const std::string &idStr, int32_t status) override;
  void setMinDiff(uint64_t minDiff) override;
  void resetCurDiff(uint64_t curDiff) override;
  void addLocalJob(LocalJob &localJob) override;
  void removeLocalJob(LocalJob &localJob) override;

protected:
  IStratumSession &session_;
  std::unique_ptr<StratumMiner> miner_;
};

class StratumMessageAgentDispatcher : public StratumMessageDispatcher {
public:
  explicit StratumMessageAgentDispatcher(IStratumSession &session);

  void handleRequest(const std::string &idStr, const std::string &method, const JsonNode &jparams, const JsonNode &jroot) override;
  void handleExMessage(const std::string &exMessage) override;
  void responseShareAccepted(const std::string &idStr) override {}
  void responseShareError(const std::string &idStr, int32_t status) override {}
  void setMinDiff(uint64_t minDiff) override;
  void resetCurDiff(uint64_t curDiff) override;
  void addLocalJob(LocalJob &localJob) override;
  void removeLocalJob(LocalJob &localJob) override;

protected:
  void handleExMessage_RegisterWorker(const std::string &exMessage);
  void handleExMessage_UnregisterWorker(const std::string &exMessage);
  void handleExMessage_SessionSpecific(const std::string &exMessage);

public:
  // These are public for unittests...
  void registerWorker(uint32_t sessionId, const std::string &clientAgent, const std::string &workerName, int64_t workerId);
  void unregisterWorker(uint32_t sessionId);
  static void getSetDiffCommand(std::map<uint8_t, std::vector<uint16_t>> &diffSessionIds, std::string &exMessage);

protected:
  IStratumSession &session_;
  std::map<uint16_t, std::unique_ptr<StratumMiner>> miners_;
};

#endif // #ifndef STRATUM_MESSAGE_DISPATCHER_H
