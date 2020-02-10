#include <civetweb.h>
#include "cJSON.h"
#include <string>
#include <iostream>
#include "signal.h"
#include "helpers.h"
#include "cors.h"
#include "spdlog/spdlog.h"

int postEncryptKey(struct mg_connection *conn, void *cbdata, char *dbPath, char* password);
int postEncryptEmail(struct mg_connection *conn, void *cbdata, char *dbPath, char* password);