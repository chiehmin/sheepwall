#include "http.h"

#include <sstream>
#include <boost/algorithm/string.hpp>

using namespace std;

// revised: https://github.com/lgandx/PCredz/blob/master/Pcredz#L65
const vector<string> Http::acctFields {
	"log","login", "wpname", "ahd_username", "unickname", "nickname", "user", "user_name",
	"alias", "pseudo", "email", "username", "_username", "userid", "form_loginname", "loginname",
	"login_id", "loginid", "session_key", "sessionkey", "pop_login", "uid", "id", "user_id", "screename",
	"uname", "ulogin", "acctname", "account", "member", "mailaddress", "membername", "login_username",
	"login_email", "loginusername", "loginemail", "uin", "sign-in", "j_username", "login-account"
};

// revised: https://github.com/lgandx/PCredz/blob/master/Pcredz#L71
const vector<string> Http::passwdFields {
	"ahd_password", "pass", "password", "_password", "passwd", "session_password", "sessionpassword",
	"login_password", "loginpassword", "form_pw", "pw", "userpassword", "pwd", "upassword", "login_password",
	"passwort", "passwrd", "wppassword", "upasswd", "j_password", "login-pwd"
};

const vector<string> Http::sessionFields {
	"sess",
	/* "sessionid", "sessid", "sess_id", "session_key", */
	/* "PHPSESSID", "ASP.NET_SessionId", "_rb_sess_id", "JSESSIONID " */
};

string Http::ToString()
{
	ostringstream oss;
	oss << "Method: " << method_ << endl
		<< "Url: " << host_ << path_ << endl
		<< "Cookie: " << cookie_ << endl
		<< "Payload: " << payload_ << endl;

	return oss.str();
}

bool Http::IsCredz()
{
	return IsContainFields(payload_, acctFields) ||
		IsContainFields(payload_, passwdFields) ||
		IsContainFields(cookie_, sessionFields);
}

bool Http::IsContainFields(const string &content, const vector<string> &fields)
{
	auto lower_content = boost::to_lower_copy<string>(content);
	for (const auto &field : fields) {
		if (string::npos != lower_content.find(field)) {
			return true;
		}
	}
	return false;
}

