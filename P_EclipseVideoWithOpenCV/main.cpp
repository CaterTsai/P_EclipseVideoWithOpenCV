#include "constParameter.h"
#include "eclipseVideoCreater.h"
int main()
{
	eclipseVideoCreater VideoCreater_;
	if(!VideoCreater_.setup())
	{
		cout<<"[ERROR]eclipseVideoCreater setup failed"<<endl;
		return -1;
	}
	//frameInfoManager::GetInstance()->SetupFrameInfoManager("data/_EclipseFrameSetting.xml");

	while(true)
	{
		if(GetAsyncKeyState(VK_ESCAPE&0x8000))
		{
			break;
		}
		VideoCreater_.update();
		Sleep(cCHECK_NEW_DATA_MS);
	}

	return 0;
}

