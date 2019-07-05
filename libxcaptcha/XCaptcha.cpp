#include "stdafx.h"
#include "XCaptcha.h"
#include <vector>
#include <cctype>
#include <algorithm>
#include <deque>

using namespace std;


std::vector<std::string> split(const std::string& s, const char separator) {
	std::vector<string> commands; 
	std::string buffer = "";     
	for (int i = 0; i < s.size(); i++) {
		if (s[i] != separator) {     
			buffer += s[i];
		}
		else {
			commands.push_back(buffer);
			buffer = "";
		}
	}
	commands.push_back(buffer);

	return commands;
}

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s)
{
	size_t newLength = size * nmemb;
	try
	{
		s->append((char*)contents, newLength);
	}
	catch (std::bad_alloc &e)
	{
		//handle memory problem
		return 0;
	}
	return newLength;
}


template<typename T>
string join(vector<T> arr, string sep) {
	string res = "";
	for (size_t i = 0; i < arr.size(); i++)
	{
		string el = to_string(arr[i]);
		res = res + el;
		if (i != arr.size() - 1) {
			res += sep;
		}
	}
	return res;
}


string join(vector<string> arr, string sep) {
	string res = "";
	for (size_t i = 0; i < arr.size(); i++)
	{
		string el = arr[i];
		res = res + el;
		if (i != arr.size() - 1) {
			res += sep;
		}
	}
	return res;
}

XCaptcha::XCaptcha(string apiKey)
{
	this->_apiKey = apiKey;
	this->_curl = curl_easy_init();
	if (!this->_curl) {
		throw "Curl not init";
	}
}

XCaptcha::~XCaptcha()
{
	curl_easy_cleanup(this->_curl);
}

std::string XCaptcha::sendGet(std::string url, deque<pair<string, string>>& parameters)
{
	parameters.push_front(pair<string, string>("key", this->_apiKey));

	vector<string> pathParts;
	for (const auto &param : parameters)
	{
		pathParts.push_back(param.first + "=" + param.second);
	}

	auto part = join(pathParts, "&");
	string response;

	curl_easy_setopt(this->_curl, CURLOPT_URL, (url + "?" + part).c_str());
	curl_easy_setopt(this->_curl, CURLOPT_HTTPGET, 1L);

	curl_easy_setopt(this->_curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
	curl_easy_setopt(this->_curl, CURLOPT_WRITEDATA, &response);
	//curl_easy_setopt(this->_curl, CURLOPT_VERBOSE, 0L); 

	CURLcode res = curl_easy_perform(this->_curl);

	if (res != CURLE_OK)
	{
		throw curl_easy_strerror(res);
	}

	return response;
}




float XCaptcha::getBalance()
{
	deque<pair<string, string>> params;

	params.push_back(pair<string, string>("action","getbalance"));
	auto res = sendGet("http://x-captcha2.ru/res.php", params);
	
	auto parsed = parseResponse(res);
	return ::stof(parsed.c_str());
}


std::string XCaptcha::parseResponse(string& response)
{
	size_t found = response.find("OK|");
	if (found != string::npos) {
		auto parts = split(response, '|');

		if (parts.size() == 2) {
			return parts.at(1);
		}
		else {
			throw ("Format error in:" + response).c_str();
		}
	}
	else {

		if (response == "ERROR_KEY_USER")
			throw "Wrong API key";
		else if (response == "ERROR_PAUSE_SERVICE")
			throw "Service now in unavailable";
		else if (response == "ERROR_ZERO_BALANCE")
			throw "You haven't money";
		else if (response == "ERROR_KEY_OR_URL")
			throw "Google parametrs is incorrect or not found";
		else if (response == "ERROR_NOT_SLOT_ZERO")
			throw "Service haven't workers";
		else if (response == "ERROR_NOT_SLOT_BUSY")
			throw "Service so busy. Wait";
		else if (response == "CAPCHA_NOT_READY")
			throw "Captcha not ready";
		else if (response == "ERROR_BAD_REZ_MANYBEK")
			throw "Wrong captcha result. Worker's error. Service give back moneys";
		else if (response == "ERROR_BAD_TIME_MANYBEK")
			throw "For this time captcha is not complated. Service give back moneys";
		else if (response == "ERROR_NOT_CAPCHA_ID")
			throw "Wrong captcha ID";
		else {
			throw (("Unknow error: " + response).c_str());
		}
	}
}


TaskID XCaptcha::sendCaptcha(std::string googleKey, std::string pageURL)
{
	deque<pair<string, string>> params;

	params.push_back(pair<string, string>("method", "userrecaptcha"));
	params.push_back(pair<string, string>("googlekey", googleKey));
	params.push_back(pair<string, string>("pageurl", pageURL));
	auto res = sendGet("http://x-captcha2.ru/in.php", params);

	auto parsed = parseResponse(res);
	// TODO: Добавьте сюда код реализации.
	return parsed;
}


std::string XCaptcha::getTaskResponse(TaskID id)
{
	deque<pair<string, string>> params;

	params.push_back(pair<string, string>("id", id));
	auto res = sendGet("http://x-captcha2.ru/res.php", params);

	auto parsed = parseResponse(res);
	// TODO: Добавьте сюда код реализации.
	return parsed;
}
