#include "Account.h"
#include <iostream>

using namespace std;
using namespace sqlite;

CriptextDB::Account CriptextDB::getAccount(string dbPath, string password, char* recipientId) {
	database db = initializeDB(dbPath, password);

	string myPrivKey = "";
	string myPubKey = "";
	int regId = 0;
	db << "select privKey, pubKey, registrationId from account where recipientId == ?;"
		<< recipientId
		>> [&](string privKey, string pubKey, int registrationId) {
		myPrivKey = privKey;
		myPubKey = pubKey;
		regId = registrationId;
	};
	Account account = {
	  myPrivKey,
	  myPubKey,
	  regId,
	  dbPath,
	  password
	};
	return account;
}

int CriptextDB::createAccount(string dbPath, string password, char* recipientId, char* name, int deviceId, char* pubKey, char* privKey, int registrationId) {
	try {
		std::cout << "DB: " << dbPath << " || pass: " << password << std::endl;
		database db = initializeDB(dbPath, password);
		bool hasRow = false;
		db << "Select recipientId from account where recipientId == ?;"
			<< recipientId
			>> [&](string recipientId) {
			hasRow = true;
		};

		if (hasRow) {
			db << "update account set name = ?, deviceId = ?, privKey = ?, pubKey = ?, registrationId = ? where recipientId == ?;"
				<< name
				<< deviceId
				<< privKey
				<< pubKey
				<< registrationId
				<< recipientId;
		}
		else {
			db << "insert into account (recipientId, name, deviceId, jwt, refreshToken, privKey, pubKey, registrationId) values (?,?,?,?,?,?,?,?);"
				<< recipientId
				<< name
				<< deviceId
				<< ""
				<< ""
				<< privKey
				<< pubKey
				<< registrationId;
		}

		string myPrivKey = "";
		string myPubKey = "";
		int regId = 0;
		db << "select privKey, pubKey, registrationId from account where recipientId == ?;"
			<< recipientId
			>> [&](string privKey, string pubKey, int registrationId) {
			myPrivKey = privKey;
			myPubKey = pubKey;
			regId = registrationId;
		};
		Account account = {
		  myPrivKey,
		  myPubKey,
		  regId,
		  dbPath
		};
		std::cout << "STORED : " << account.registrationId << std::endl;
	}
	catch (exception& e) {
		std::cout << "ERROR : " << e.what() << std::endl;
		return false;
	}


	return true;
}
