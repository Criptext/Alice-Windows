#include "signal/signal_protocol.h"
#include "IdentityKey.h"
#include "Account.h"
#include "decode_utils.h"
#include <string>
#include <vector>
#include <iostream>

void setup_identity_key_store(signal_protocol_store_context *context, signal_context *global_context, CriptextDB::Account *account);