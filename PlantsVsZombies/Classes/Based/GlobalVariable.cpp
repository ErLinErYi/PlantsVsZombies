#include "Based/GlobalVariable.h"
#include "AudioEngine.h"

using namespace cocos2d::experimental;

Global* Global::_instance = nullptr;

Global* Global::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)Global;
	}
	return _instance;
}

Global::Global():
	prohibitId(-1)
,	userInformation(new UserInformation())
{
}

Global::~Global()
{
	if(userInformation)delete userInformation;
}

void Global::resumeProhibit()
{
	AudioEngine::setFinishCallback(prohibitId, [=](int i, string name)
		{
			prohibitId = -1;
		});
}

