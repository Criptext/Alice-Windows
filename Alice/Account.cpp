#include "Account.h"
#include <iostream>

using namespace std;
using namespace sqlite;

CriptextDB::Account CriptextDB::getAccount(database db, char* recipientId) {
	string myPrivKey = "";
	string myPubKey = "";
	int regId = 0;
	int accountId = 0;
	db << "select id, privKey, pubKey, registrationId from account where recipientId == ?;"
		<< recipientId
		>> [&](int id, string privKey, string pubKey, int registrationId) {
		accountId = id;
		myPrivKey = privKey;
		myPubKey = pubKey;
		regId = registrationId;
	};

	connection_type con = db.connection();
	Account account = {
		accountId,
	  myPrivKey,
	  myPubKey,
	  regId,
	  con
	};
	return account;
}

int CriptextDB::createAccount(database db, char* recipientId, char* name, int deviceId, char* pubKey, char* privKey, int registrationId) {
	try {
		bool hasRow = false;

		db << "begin;";
		db << "Select recipientId from account where recipientId == ?;"
			<< recipientId
			>> [&](string recipientId) {
			hasRow = true;
		};
		if (hasRow) {
			db << "update account set name = ?, deviceId = ?, privKey = ?, pubKey = ?, registrationId = ?, isLoggedIn = false, isActive = false where recipientId == ?;"
				<< name
				<< deviceId
				<< privKey
				<< pubKey
				<< registrationId
				<< recipientId;
		}
		else {
			db << "insert into account (recipientId, name, deviceId, jwt, refreshToken, privKey, pubKey, registrationId, isLoggedIn, isActive) values (?,?,?,?,?,?,?,?,?,?);"
				<< recipientId
				<< name
				<< deviceId
				<< ""
				<< ""
				<< privKey
				<< pubKey
				<< registrationId
				<< false
				<< false;
		}
		db << "commit;";
	} catch (exception & e) {
		std::cout << "ERROR Creating Account : " << e.what() << std::endl;
		return false;
	}
	return true;
}
