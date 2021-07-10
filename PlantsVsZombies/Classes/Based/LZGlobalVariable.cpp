/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Email: 2117610943@qq.com
 */
#include "LZGlobalVariable.h"
#include "LZUserData.h"
#include "AudioEngine.h"

using namespace cocos2d::experimental;

Global* Global::_instance = nullptr;
string Global::_computerUniqueIdentificationString= string("");

Global* Global::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)Global;
	}
	return _instance;
}

Global::Global() :
  _sumAnimationInterval(0)
, _frequence(0)
, userInformation(new UserInformation())
{
}

Global::~Global()
{
	if(userInformation)delete userInformation;
}

void Global::checkAnimationInterval()
{
	++_frequence;
	if (_sumAnimationInterval <= 0)
	{
		_sumAnimationInterval = 1.f / Director::getInstance()->getSecondsPerFrame();
	}
	else
	{
		_sumAnimationInterval = (_sumAnimationInterval + 1.f / Director::getInstance()->getSecondsPerFrame()) / 2;
	}

	if (_sumAnimationInterval <= 15 && _frequence > 60)
	{
		_frequence = 0;
		Director::getInstance()->getRunningScene()->runAction(
			Sequence::create(DelayTime::create(20.f), 
			CallFunc::create([=]()
				{
					if (_sumAnimationInterval <= 15)
					{
						AudioEngine::stopAll();
						Director::getInstance()->stopAnimation();

						wstring str = L"        很抱歉！检测到程序运行的平均帧数太低对游戏的可玩性有极大的影响。所以请点击确定按钮关闭此游戏！关闭不使用的其他程序或提升电脑硬件配置等操作后再次尝试运行此游戏。";
						if (MessageBoxW(Director::getInstance()->getOpenGLView()->getWin32Window(), str.c_str(), L"游戏平均帧数太低(平均FPS <= 15FPS)", MB_ICONERROR | MB_OK) == MB_OKCANCEL)
						{
							Director::getInstance()->end();
						}
					}
					else
					{
						_frequence = 40;
					}
				}), nullptr));
	}
}

void Global::checkUserDataFile()
{
	string computerId;
	if (getComputerUniqueIdentification(computerId)) // 如果成功获取数据
	{
		_computerUniqueIdentificationString = computerId;
		if (_computerUniqueIdentificationString.empty()) //如果获取结果为空
		{
			return;
		}
	}
	else
	{
		return;
	}

	auto data = UserDefault::getInstance()->getStringForKey("COMPUTERID");
	if (!data.empty()) // 如果已有记录
	{
		if (data.compare(UserData::encryption(_computerUniqueIdentificationString))) // 如果数据不同 删除所有存档
		{
			showWanging();
			return;
		}
	}

	/* 读取用户存档名称 */
	for (unsigned int i = 0; i < 8; ++i)
	{
		if (!UserDefault::getInstance()->getStringForKey(Global::getInstance()->userInformation->getUserCaveFileNameKey(i).c_str()).empty()) // 如果有存档
		{
			Global::getInstance()->userInformation->setUserCaveFileNumber(i);      /* 设置存档编号 */
			UserData::getInstance()->createNewUserDataDocument();
			string data = UserData::getInstance()->openStringUserData(const_cast<char*>("COMPUTERID"));
			if (!data.empty())
			{
				if (_computerUniqueIdentificationString.compare(data)) // 如果数据不同 删除所有存档
				{
					showWanging();
					return;
				}
			}
		}
	}
}

void Global::writeComputerUniqueIdentification()
{
	if (_computerUniqueIdentificationString.empty())
	{
		return;
	}

	auto data = UserDefault::getInstance()->getStringForKey("COMPUTERID");
	if (data.empty()) // 如果没有记录
	{
		UserDefault::getInstance()->setStringForKey("COMPUTERID", UserData::encryption(_computerUniqueIdentificationString));
	}

	for (unsigned int i = 0; i < 8; ++i)
	{
		if (!UserDefault::getInstance()->getStringForKey(Global::getInstance()->userInformation->getUserCaveFileNameKey(i).c_str()).empty()) // 如果有存档
		{
			Global::getInstance()->userInformation->setUserCaveFileNumber(i);      /* 设置存档编号 */
			UserData::getInstance()->createNewUserDataDocument();
			string data = UserData::getInstance()->openStringUserData(const_cast<char*>("COMPUTERID"));
			if (data.empty()) //如果没有记录
			{
				UserData::getInstance()->caveUserData(const_cast<char*>("COMPUTERID"), const_cast<char*>(_computerUniqueIdentificationString.c_str()));
			}
		}
	}
}

bool Global::getComputerUniqueIdentification(string& identification)
{
	const long MAX_COMMAND_SIZE = 1000;            // 命令行输出缓冲大小	

	WCHAR szFetCmd[] = L"wmic csproduct get UUID"; // 获取BOIS命令行	
	const string strEnSearch = "UUID";             // 主板序列号的前导信息

	BOOL   bret = FALSE;
	HANDLE hReadPipe = NULL;                       //读取管道
	HANDLE hWritePipe = NULL;                      //写入管道	
	PROCESS_INFORMATION pi;                        //进程信息	
	memset(&pi, 0, sizeof(pi));
	STARTUPINFO	si;	                               //控制命令行窗口信息
	memset(&si, 0, sizeof(si));
	SECURITY_ATTRIBUTES sa;                        //安全属性
	memset(&sa, 0, sizeof(sa));

	char szBuffer[MAX_COMMAND_SIZE + 1] = { 0 };   // 放置命令行结果的输出缓冲区
	string	strBuffer;
	unsigned long count = 0;
	long ipos = 0;

	pi.hProcess = NULL;
	pi.hThread = NULL;
	si.cb = sizeof(STARTUPINFO);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//1.创建管道
	bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (!bret) {
		goto END;
	}

	//2.设置命令行窗口的信息为指定的读写管道
	GetStartupInfo(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = SW_HIDE; //隐藏命令行窗口
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	//3.创建获取命令行的进程
	bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (!bret) {
		goto END;
	}
	Sleep(200);
	//4.读取返回的数据
	WaitForSingleObject(pi.hProcess, 200);
	bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
	if (!bret) {
		goto END;
	}

	//5.查找主板ID
	bret = FALSE;
	strBuffer = szBuffer;
	ipos = strBuffer.find(strEnSearch);

	if (ipos < 0) { // 没有找到
		goto END;
	}
	else {
		strBuffer = strBuffer.substr(ipos + strEnSearch.length());
	}

	memset(szBuffer, 0x00, sizeof(szBuffer));
	strcpy_s(szBuffer, strBuffer.c_str());

	//去掉中间的空格 \r \n
	for (unsigned int i = 0; i < strlen(szBuffer); i++) {
		if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r') {
			identification += szBuffer[i];
		}
	}

	bret = true;

END:
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return bret;
}

void Global::showWanging()
{
	wstring str = L"        很抱歉！检测到您的存档存在异常，程序将自动重置存档。玩游戏循序渐进才有乐趣，禁止使用他人存档。点击确定关闭对话框。";
	MessageBoxW(NULL, str.c_str(), L"游戏存档异常", MB_ICONERROR | MB_OK);

	FileUtils::getInstance()->removeDirectory(FileUtils::getInstance()->getWritablePath());
	UserDefault::destroyInstance();
}
