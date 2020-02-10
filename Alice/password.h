#include <civetweb.h>
#include <cjson/cJSON.h>
#include "cors.h"
#include <string>
#include <iostream>
#include <signal/session_pre_key.h>
#include "signal.h"
#include "helpers.h"
#include "spdlog/spdlog.h"
#include "dbUtils.h"

string decryptPassword(struct mg_connection* conn, void* cbdata, char* token, string currentPassword);