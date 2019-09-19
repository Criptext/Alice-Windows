#include <civetweb.h>
#include "cJSON.h"
#include "cors.h"
#include <string>
#include <iostream>
#include <signal/session_pre_key.h>
#include "signal.h"
#include "helpers.h"

int createKeyBundle(struct mg_connection *conn, void *cbdata, char *dbPath);
int createPreKeys(struct mg_connection *conn, void *cbdata, char *dbPath);
int processKeyBundle(struct mg_connection *conn, void *cbdata, char *dbPath);
int createAccount(struct mg_connection *conn, void *cbdata, char *dbPath);