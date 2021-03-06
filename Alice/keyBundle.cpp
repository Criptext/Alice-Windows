#include "keyBundle.h"

int createKeyBundle(struct mg_connection *conn, void *cbdata, char *dbPath, char* password) {
  bool corsResult = cors(conn);
  if (corsResult) {
    return 201;
  }

  if (password == 0) {
    sendError(conn, 401, "Missing Password Setup");
    return 401;
  }

  std::cout << "/keybundle Receiving Request " << std::endl;

  char buffer[1024];
  int dlen = mg_read(conn, buffer, sizeof(buffer) - 1);

  if ((dlen < 1) || (dlen >= sizeof(buffer))) {
    sendError(conn, 400, "Empty Request");
    return 400;
  }
  buffer[dlen] = 0;
  cJSON *obj = cJSON_Parse(buffer);
  
  if (obj == NULL) {
    sendError(conn, 422, "Not a JSON Object");
    return 422;
  }
  std::cout << "Request -> " << cJSON_Print(obj) << std::endl;

  cJSON *recipientId;
  recipientId = cJSON_GetObjectItemCaseSensitive(obj, "recipientId");

  if (!cJSON_IsString(recipientId)) {
    sendError(conn, 400, "Missing Params");
    return 400;
  }

  database db = initializeDB(dbPath, password);
  CriptextSignal signal(recipientId->valuestring, db);
  cJSON *bundle = cJSON_CreateObject();
  signal.generateKeyBundle(bundle, recipientId->valuestring);

  (void)[v = std::move(db)]{};
  return SendJSON(conn, bundle);
}

int createAccount(struct mg_connection *conn, void *cbdata, char *dbPath, char* password) {
  bool corsResult = cors(conn);
  if (corsResult) {
    return 201;
  }

  std::cout << "/account Receiving Request" << std::endl;

  if (password == 0) {
    sendError(conn, 401, "Missing Password Setup");
    return 401;
  }

  char buffer[1024];
  int dlen = mg_read(conn, buffer, sizeof(buffer) - 1);

  if ((dlen < 1) || (dlen >= sizeof(buffer))) {
    sendError(conn, 400, "Empty Request");
    return 400;
  }
  buffer[dlen] = 0;
  cJSON *obj = cJSON_Parse(buffer);
  
  if (obj == NULL) {
    sendError(conn, 422, "Not a JSON Object");
    return 422;
  }
  std::cout << "Request -> " << cJSON_Print(obj) << std::endl;

  cJSON *recipientId, *deviceId, *name;
  recipientId = cJSON_GetObjectItemCaseSensitive(obj, "recipientId");
  deviceId = cJSON_GetObjectItemCaseSensitive(obj, "deviceId");
  name = cJSON_GetObjectItemCaseSensitive(obj, "name");

  if (!cJSON_IsString(recipientId) || !cJSON_IsNumber(deviceId) || !cJSON_IsString(name)) {
    sendError(conn, 400, "Missing Params");
    return 400;
  }

  char *publicKey;
  char *privKey;
  int regId;
  int result = CriptextSignal::createAccountCredentials(&publicKey, &privKey, &regId);
  database db = initializeDB(dbPath, password);
  result = CriptextDB::createAccount(db, recipientId->valuestring, name->valuestring, deviceId->valueint, publicKey, privKey, regId);

  (void)[v = std::move(db)]{};

  if (result < 0) {
    sendError(conn, 500, "Unable to Create Account");
    return 500;
  }

  sendOK(conn, "Done");
  return 1;
}

int processKeyBundle(struct mg_connection *conn, void *cbdata, char *dbPath, char* password) {
  bool corsResult = cors(conn);
  if (corsResult) {
    return 201;
  }

  std::cout << "/session/create Receiving Request" << std::endl;

  if (password == 0) {
    sendError(conn, 401, "Missing Password Setup");
    return 401;
  }

  string bufferData = parseBody(conn);
  int readLength = bufferData.length();

  if (readLength <= 0) {
    sendError(conn, 413, "Request Data Too Big");
    return 413;
  }
  
  cJSON *obj = cJSON_Parse(bufferData.c_str());
  
  if (obj == NULL) {
    sendError(conn, 422, "Not a JSON Object");
    return 422;
  }
  std::cout << "Request -> " << cJSON_Print(obj) << std::endl;

  cJSON *accountRecipientId, *keybundleArray;
  accountRecipientId = cJSON_GetObjectItemCaseSensitive(obj, "accountRecipientId");
  keybundleArray = cJSON_GetObjectItemCaseSensitive(obj, "keybundles");
  
  if (!cJSON_IsString(accountRecipientId) || !cJSON_IsArray(keybundleArray)) {
    sendError(conn, 400, "Missing Params");
    return 400;
  }

  database db = initializeDB(dbPath, password);
  CriptextSignal signal(accountRecipientId->valuestring, db);
  cJSON *keyBundleObj = NULL;
  cJSON_ArrayForEach(keyBundleObj, keybundleArray) {

	  cJSON* recipientId, * domain, * deviceId, * registrationId, * signedPreKeyId, * signedPreKey, * signature, * identityKey, * preKeyObj;
	  cJSON* preKeyId = 0; 
	  cJSON *preKey = 0;

    recipientId = cJSON_GetObjectItemCaseSensitive(keyBundleObj, "recipientId");
    deviceId = cJSON_GetObjectItemCaseSensitive(keyBundleObj, "deviceId");
    registrationId = cJSON_GetObjectItemCaseSensitive(keyBundleObj, "registrationId");
    signedPreKeyId = cJSON_GetObjectItemCaseSensitive(keyBundleObj, "signedPreKeyId");
    signedPreKey = cJSON_GetObjectItemCaseSensitive(keyBundleObj, "signedPreKeyPublic");
    signature = cJSON_GetObjectItemCaseSensitive(keyBundleObj, "signedPreKeySignature");
    identityKey = cJSON_GetObjectItemCaseSensitive(keyBundleObj, "identityPublicKey");

    if (!cJSON_IsString(recipientId) || !cJSON_IsNumber(deviceId) || !cJSON_IsNumber(registrationId) 
      || !cJSON_IsNumber(signedPreKeyId) || !cJSON_IsString(signedPreKey) || !cJSON_IsString(signature) 
      || !cJSON_IsString(identityKey)) {
      continue;
    }
    if (cJSON_HasObjectItem(keyBundleObj, "preKey")) {
      preKeyObj = cJSON_GetObjectItemCaseSensitive(keyBundleObj, "preKey");
      preKey = cJSON_GetObjectItemCaseSensitive(preKeyObj, "publicKey");
      preKeyId = cJSON_GetObjectItemCaseSensitive(preKeyObj, "id");
    }

    Keybundle kb = {
      recipientId->valuestring,
      deviceId->valueint,
      registrationId->valueint,
      signedPreKeyId->valueint,
      signedPreKey->valuestring,
      signature->valuestring,
      identityKey->valuestring,
      preKeyId != 0 ? preKeyId->valueint : 0,
      preKey != 0 ? preKey->valuestring : 0,
    };
    signal.processKeyBundle(&kb);
  }
  (void)[v = std::move(db)]{};
  sendOK(conn, "");
  return 1;
}

int createPreKeys(struct mg_connection *conn, void *cbdata, char *dbPath, char* password) {
  bool corsResult = cors(conn);
  if (corsResult) {
    return 201;
  }

  std::cout << "/prekey Receiving Request" << std::endl;

  if (password == 0) {
    sendError(conn, 401, "Missing Password Setup");
    return 401;
  }

  char buffer[1024];
  int dlen = mg_read(conn, buffer, sizeof(buffer) - 1);

  if ((dlen < 1) || (dlen >= sizeof(buffer))) {
    sendError(conn, 400, "Empty Request");
    return 400;
  }
  buffer[dlen] = 0;
  cJSON *obj = cJSON_Parse(buffer);
  
  if (obj == NULL) {
    sendError(conn, 422, "Not a JSON String");
    return 422;
  }
  std::cout << "Request -> " << cJSON_Print(obj) << std::endl;

  cJSON *accountId;
  cJSON *newPreKeys;
  accountId = cJSON_GetObjectItemCaseSensitive(obj, "accountId");
  newPreKeys = cJSON_GetObjectItemCaseSensitive(obj, "newPreKeys");

  if (!cJSON_IsString(accountId) || !cJSON_IsArray(newPreKeys)) {
    sendError(conn, 400, "Missing Params");
    return 400;
  }

  database db = initializeDB(dbPath, password);
  CriptextSignal signal(accountId->valuestring, db);
  cJSON* bundle = cJSON_CreateObject();
  signal.generateMorePreKeys(bundle, newPreKeys);
  (void)[v = std::move(db)]{};
  return SendJSON(conn, bundle);
}