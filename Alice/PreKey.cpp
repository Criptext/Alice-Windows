#include "PreKey.h"

using namespace std;
using namespace sqlite;

CriptextDB::PreKey CriptextDB::getPreKey(database db, short int id) {
  string myPreKey = "";
  size_t myLen = 0;
  db << "Select * from prekeyrecord where preKeyId == ?;"
     << id
     >> [&] (int preKeyId, string record, int recordLength) {
		myPreKey = record;
        myLen = (size_t)recordLength;
    };

  if (myLen == 0 || myPreKey.empty()) {
    throw std::invalid_argument("row not available");
  }
  PreKey preKey = { 
    id, 
    myPreKey, 
    myLen
  };
  return preKey;
}

bool CriptextDB::createPreKey(database db, short int id, char* keyRecord, size_t len) {
  try {
    db << "insert into prekeyrecord (preKeyId, record, recordLength) values (?,?,?);"
     << id
     << keyRecord
     << static_cast<int>(len);
    return true;
  } catch (exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
}

bool CriptextDB::deletePreKey(database db, short int id) {
  try {
    db << "delete from prekeyrecord where preKeyId == ?;"
     << id;
    return true;
  } catch (exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
}
