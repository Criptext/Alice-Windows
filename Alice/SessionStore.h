#include <sstream>
#include <codecvt>
#include "signal/signal_protocol.h"
#include "SessionRecord.h"
#include "Account.h"
#include <inttypes.h>
#include <string>
#include <vector>
#include <iostream>

extern "C" {
  #include "utf8.h"
  #include "base64.h"
}

void setup_session_store(signal_protocol_store_context *context, CriptextDB::Account *account);
