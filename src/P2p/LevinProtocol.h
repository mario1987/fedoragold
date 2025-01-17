// Copyright (c) 2011-2016 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "CryptoNote.h"
#include <Common/MemoryInputStream.h>
#include <Common/VectorOutputStream.h>
#include "Serialization/KVBinaryInputStreamSerializer.h"
#include "Serialization/KVBinaryOutputStreamSerializer.h"

#include "Logging/LoggerRef.h"

namespace System {
class TcpConnection;
}

namespace CryptoNote {

enum class LevinError: int32_t {
  OK = 0,
  ERROR_CONNECTION = -1,
  ERROR_CONNECTION_NOT_FOUND = -2,
  ERROR_CONNECTION_DESTROYED = -3,
  ERROR_CONNECTION_TIMEDOUT = -4,
  ERROR_CONNECTION_NO_DUPLEX_PROTOCOL = -5,
  ERROR_CONNECTION_HANDLER_NOT_DEFINED = -6,
  ERROR_FORMAT = -7,
};

const int32_t LEVIN_PROTOCOL_RETCODE_SUCCESS = 1;

using namespace Logging;

class LevinProtocol {
public:

  LevinProtocol(System::TcpConnection& connection);

  template <typename Request, typename Response>
  bool invoke(uint32_t command, const Request& request, Response& response, Logging::LoggerRef& logger) {
    sendMessage(command, encode(request), true, logger);

    // a false return from readCommand is NOT an error here, can just be
    // the end of the transmission, so keep going!
    Command cmd;
    readCommand(cmd, logger);

    if (!cmd.isResponse) {
      return false;
    }

    return decode(cmd.buf, response); 
  }

  template <typename Request>
  void notify(uint32_t command, const Request& request, int, Logging::LoggerRef& logger) {
    sendMessage(command, encode(request), false, logger);
  }

  struct Command {
    uint32_t command;
    bool isNotify;
    bool isResponse;
    BinaryArray buf;

    bool needReply() const;
  };

  bool readCommand(Command& cmd, Logging::LoggerRef& logger);

  void sendMessage(uint32_t command, const BinaryArray& out, bool needResponse, Logging::LoggerRef& logger);
  void sendReply(uint32_t command, const BinaryArray& out, int32_t returnCode, Logging::LoggerRef& logger);

  template <typename T>
  static bool decode(const BinaryArray& buf, T& value) {
    try {
      Common::MemoryInputStream stream(buf.data(), buf.size());
      KVBinaryInputStreamSerializer serializer(stream);
      serialize(value, serializer);
    } catch (std::exception&) {
      return false;
    }

    return true;
  }

  template <typename T>
  static BinaryArray encode(const T& value) {
    BinaryArray result;
    KVBinaryOutputStreamSerializer serializer;
    serialize(const_cast<T&>(value), serializer);
    Common::VectorOutputStream stream(result);
    serializer.dump(stream);
    return result;
  }

private:

  bool readStrict(uint8_t* ptr, size_t size, Logging::LoggerRef &logger, bool bSynchronous=false);
  void writeStrict(const uint8_t* ptr, size_t size, Logging::LoggerRef &logger);
  System::TcpConnection& m_conn;
};

}
