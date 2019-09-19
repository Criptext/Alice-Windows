#include "signal/signal_protocol.h"
#include "signal/curve.h"
#include "SignedPreKey.h"
#include "Account.h"
#include "types.h"
#include "decode_utils.h"
#include <string>
#include <vector>
#include <iostream>

extern "C" {
    #include "base64.h"
    #include "crypto.h"
}

void setup_signed_pre_key_store(signal_protocol_store_context *context, CriptextDB::Account *account);