#ifndef _ECLIPSE_VIDEO_CREATER_
#define _ECLIPSE_VIDEO_CREATER_

#include "constParameter.h"
#include "frameInfoManager.h"
#include "eclipseFrameMixUnit.h"
#include <thread>

enum eMIX_VIDEO_STATE
{
	eMIX_VIDEO_UNINITIAL,
	eMIX_VIDEO_SUCCESS,
	eMIX_VIDEO_LOAD_TEXT_FAILED,
	eMIX_VIDEO_TEXT_FORMATE_FAILED,
	eMIX_VIDEO_COVER_FAILED,
	eMIX_VIDEO_FFMPEG_FAILED,
};

class eclipseVideoCreater
{
public:
	bool setup();
	void update();

private:
	bool checkNewFile(string& strXMLName);
	bool checkVideo(string strVideoName);
	bool mixVideo(string strTID_, int(&arr)[cNUM_TREE_CODE], string videoName);

	bool decode(string strCode, int(&arr)[cNUM_TREE_CODE]);
	void moveOrder(string strOrderName, bool isSuccess);
	
};


#endif // !_ECLIPSE_VIDEO_CREATER_
