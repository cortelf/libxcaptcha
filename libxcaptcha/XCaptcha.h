#pragma once

#include <string>
#include <curl/curl.h>
#include <map>
#include <deque>


typedef std::string TaskID;

class XCaptcha
{
public:
	XCaptcha(std::string apiKey);
	~XCaptcha();
private:
	std::string _apiKey;
	CURL* _curl;

	std::string sendGet(std::string url, std::deque<std::pair<std::string, std::string>>& parameters);
	std::string parseResponse(std::string& response);
public:
	float getBalance();
	
	TaskID sendCaptcha(std::string googleKey, std::string pageURL);
	std::string getTaskResponse(TaskID id);
};

