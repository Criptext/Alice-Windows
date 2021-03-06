#include "SessionRecord.h"

using namespace sqlite;
using namespace std;

CriptextDB::SessionRecord CriptextDB::getSessionRecord(database db, int accountId, string recipientId, long int deviceId) {
  string myRecord = "";
  int myLen = 0;
  db << "Select * from sessionrecord where recipientId == ? and deviceId == ? and accountId == ?;"
     << recipientId
     << deviceId
     << accountId
     >> [&] (int id, string recipientId, int deviceId, string record, int recordLength) {
        myLen = recordLength;
		myRecord = record;
	};
  if (myLen == 0) {
    throw std::invalid_argument("row not available");
  }
  SessionRecord sessionRecord = { 
    recipientId, 
    deviceId, 
    myRecord, 
    (size_t)myLen 
  };

  return sessionRecord;
}

vector<CriptextDB::SessionRecord> CriptextDB::getSessionRecords(database db, int accountId, string recipientId) {
  vector<CriptextDB::SessionRecord> sessionRecords;
  try {
	  db << "Select * from sessionrecord where recipientId == ? and accountId == ?;"
     << recipientId
     << accountId
     >> [&] (int id, string recipientId, int deviceId, string record, int recordLength) {
        SessionRecord mySessionRecord = { 
          recipientId, 
          deviceId, 
          record, 
          (size_t)recordLength 
        };
        sessionRecords.push_back(mySessionRecord);
    };
  } catch (exception& e) {
    std::cout << e.what() << std::endl; 
    return sessionRecords;
  }

  return sessionRecords;
}

bool CriptextDB::createSessionRecord(database db, int accountId, string recipientId, long int deviceId, char* record, size_t len) {
  try {
  	bool hasRow = false;
    db << "begin;";
    db << "Select * from sessionrecord where recipientId == ? and deviceId == ? and accountId == ?;"
     << recipientId
     << deviceId
     << accountId
     >> [&] (string recipientId, int deviceId, string record, int recordLength) {
        hasRow = true;
    };
    if (hasRow) {
      db << "update sessionrecord set record = ?, recordLength = ? where recipientId == ? and deviceId == ? and accountId == ?;"
        << record
        << static_cast<int>(len)
        << recipientId
        << deviceId
        << accountId;
    } else {
      db << "insert into sessionrecord (recipientId, deviceId, record, recordLength, accountId) values (?,?,?,?,?);"
        << recipientId
        << deviceId
        << record
        << static_cast<int>(len)
        << accountId;
    }
    db << "commit;";
  } catch (exception& e) {
    std::cout << "ERROR Creating Session Record : " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool CriptextDB::deleteSessionRecord(database db, int accountId, string recipientId, long int deviceId) {
  try {
    db << "delete from sessionrecord where recipientId == ? and deviceId == ? and accountId == ?;"
      << recipientId
      << deviceId
      << accountId;
  } catch (exception& e) {
    std::cout << "ERROR : " << e.what() << std::endl;
    return false;
  }
  return true;
}

bool CriptextDB::deleteSessionRecords(database db, int accountId, string recipientId) {
  try {
    db << "delete from sessionrecord where recipientId == ? and accountId == ?;"
      << recipientId
      << accountId;
  } catch (exception& e) {
    std::cout << "ERROR : " << e.what() << std::endl;
    return false;
  }
  return true;
}