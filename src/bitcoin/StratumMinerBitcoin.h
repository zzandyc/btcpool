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
#ifndef STRATUM_MINER_BITCOIN_H_
#define STRATUM_MINER_BITCOIN_H_

#include "StratumServerBitcoin.h"
#include "StratumMiner.h"

class StratumMinerBitcoin : public StratumMinerBase<StratumTraitsBitcoin> {
public:
  StratumMinerBitcoin(StratumSessionBitcoin &session,
                      const DiffController &diffController,
                      const std::string &clientAgent,
                      const std::string &workerName,
                      int64_t workerId);

  void handleRequest(const std::string &idStr,
                     const std::string &method,
                     const JsonNode &jparams,
                     const JsonNode &jroot) override;
  void handleExMessage(const std::string &exMessage) override;

private:
  void handleRequest_Submit(const std::string &idStr, const JsonNode &jparams);
  void handleRequest_SuggestTarget(const std::string &idStr, const JsonNode &jparams);
  void handleExMessage_SubmitShare(const std::string &exMessage, bool isWithTime);
  void handleRequest_Submit(const std::string &idStr,
                            uint8_t shortJobId,
                            uint64_t extraNonce2,
                            uint32_t nonce,
                            uint32_t nTime);
};

#endif // #ifndef STRATUM_MINER_BITCOIN_H_
