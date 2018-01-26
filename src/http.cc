#include "http.h"

#include <sstream>

using namespace std;

// from: https://github.com/lgandx/PCredz/blob/master/Pcredz#L65
const vector<string> Http::acctFields {
	"log","login", "wpname", "ahd_username", "unickname", "nickname", "user", "user_name",
	"alias", "pseudo", "email", "username", "_username", "userid", "form_loginname", "loginname",
	"login_id", "loginid", "session_key", "sessionkey", "pop_login", "uid", "id", "user_id", "screename",
	"uname", "ulogin", "acctname", "account", "member", "mailaddress", "membername", "login_username",
	"login_email", "loginusername", "loginemail", "uin", "sign-in", "j_username"
};

// from: https://github.com/lgandx/PCredz/blob/master/Pcredz#L71
const vector<string> Http::passwdFields {
	"ahd_password", "pass", "password", "_password", "passwd", "session_password", "sessionpassword",
	"login_password", "loginpassword", "form_pw", "pw", "userpassword", "pwd", "upassword", "login_password",
	"passwort", "passwrd", "wppassword", "upasswd", "j_password"
};

string Http::ToString()
{
	ostringstream oss;
	oss << "Method: " << method_ << endl
		<< "Url: " << host_ << path_ << endl
		<< "Payload: " << payload_ << endl;

	return oss.str();
}

bool Http::IsCredz()
{
	return IsPayloadContainFields(acctFields) ||
		IsPayloadContainFields(passwdFields);
}

bool Http::IsPayloadContainFields(vector<string> fields)
{
	for (auto &field : fields) {
		if (string::npos != payload_.find(field)) {
			return true;
		}
	}
	return false;
}

