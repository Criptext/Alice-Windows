#include <signal/signal_protocol.h>
#include "IdentityKeyStore.h"
#include "PreKeyStore.h"
#include "SessionStore.h"
#include "SignedPreKeyStore.h"
#include <string>

void setup_store_context(signal_protocol_store_context **context, signal_context *global_context, CriptextDB::Account *account);
