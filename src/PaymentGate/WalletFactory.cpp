// Copyright (c) 2011-2016 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "WalletFactory.h"

#include "NodeRpcProxy/NodeRpcProxy.h"
#include "Wallet/WalletGreen.h"
#include "CryptoNoteCore/Currency.h"

#include <stdlib.h>
#include <future>

#include <Logging/LoggerManager.h>
using namespace Logging;
LoggerManager fManager;
LoggerRef flogger(fManager, "Wallet Factory tests");

namespace PaymentService {

WalletFactory WalletFactory::factory;

WalletFactory::WalletFactory() {
}

WalletFactory::~WalletFactory() {
}

CryptoNote::IWallet* WalletFactory::createWallet(const CryptoNote::Currency& currency, CryptoNote::INode& node, System::Dispatcher& dispatcher, Logging::ILogger &logger) {
  CryptoNote::IWallet* wallet = new CryptoNote::WalletGreen(dispatcher, currency, node, flogger);
  return wallet;
}

}
