#include <stdlib.h>
#include <string>
#include <civetweb.h>
#include "Account.h"
#include "decrypt.h"
#include "encrypt.h"
#include "keyBundle.h"

void http_init(char* dbPath, char* port, char* pass);
void http_shutdown();