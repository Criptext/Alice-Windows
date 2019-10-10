#include "PreKey.h"

using namespace std;
using namespace sqlite;

CriptextDB::PreKey CriptextDB::getPreKey(string dbPath, short int id) {
  sqlite_config config;
  config.flags = OpenFlags::FULLMUTEX | OpenFlags::SHAREDCACHE | OpenFlags::READONLY;
  database db(dbPath, config);

  string myPreKey = "";
  size_t myLen = 0;
  std::cout << 7 << std::endl;
  db << "Select * from prekeyrecord where preKeyId == ?;"
     << id
     >> [&] (int preKeyId, string record, int recordLength) {
		myPreKey = record;
        myLen = (size_t)recordLength;
    };

  if (myLen == 0 || myPreKey.empty()) {
    std::cout << 8 << std::endl;
    throw std::invalid_argument("row not available");
  }
  PreKey preKey = { 
    id, 
    myPreKey, 
    myLen
  };
  std::cout << 9 << std::endl;
  return preKey;
}

bool CriptextDB::createPreKey(string dbPath, short int id, char *keyRecord, size_t len) {
  try {
    std::cout << 10 << std::endl;

    sqlite_config config;
    config.flags = OpenFlags::FULLMUTEX | OpenFlags::SHAREDCACHE | OpenFlags::READWRITE;
    database db(dbPath, config);

    db << "insert into prekeyrecord (preKeyId, record, recordLength) values (?,?,?);"
     << id
     << keyRecord
     << static_cast<int>(len);
    return true;
    std::cout << 11 << std::endl;
  } catch (exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
}

bool CriptextDB::deletePreKey(string dbPath, short int id) {
  try {
    std::cout << 12 << std::endl;
    sqlite_config config;
    config.flags = OpenFlags::FULLMUTEX  | OpenFlags::SHAREDCACHE | OpenFlags::READWRITE;
    database db(dbPath, config);
    db << "delete from prekeyrecord where preKeyId == ?;"
     << id;
    std::cout << 13 << std::endl;
    return true;
  } catch (exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
}
