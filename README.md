# libxcaptcha - C++ API wrapper x-captcha

## Init API object
```cpp
XCaptcha x("Your key");
```

## Get Balance
```cpp
float balance = x.getBalance();
```

## Send request to service
```cpp
TaskID id = x.sendCaptcha("google key", "page url");
```

## Get captcha from id
```cpp
std::string response = x.getTaskResponse(id);
```

## Usage
* Wait 20-25 seconds between send captcha and get response from task id
