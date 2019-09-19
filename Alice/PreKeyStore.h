#include "signal/signal_protocol.h"
#include "PreKey.h"
#include "Account.h"
#include "decode_utils.h"
#include <string>
#include <vector>
#include <iostream>

void setup_pre_key_store(signal_protocol_store_context *context, CriptextDB::Account *account);