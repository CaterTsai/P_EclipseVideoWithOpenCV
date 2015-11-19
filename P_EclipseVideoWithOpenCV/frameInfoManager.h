#ifndef _FRAME_INFO_MANAGER_
#define _FRAME_INFO_MANAGER_

#include "constParameter.h"

typedef struct _stPoint
{
	_stPoint()
		:x(0)
		,y(0){}
	int x, y;
}stPoint;

typedef struct _stFrameInfo
{
	_stFrameInfo()
		:itemType(true)
		,toAlpha(0)
		,fromAlpha(0)
		,msgAlpha(0)
	{}

	bool			 itemType;
	int toAlpha, fromAlpha, msgAlpha;
}stFrameInfo;

class frameInfoManager
{
public:
	frameInfoManager():_bIsSetup(false){};
	~frameInfoManager()
	{
		Destroy();
	}

	bool setup(string strXmlName);
	int GetFrameNum() const;
	bool GetFrameInfo(int iFrameID, stFrameInfo& refFrameInfo) const;

	Mat& getItemMat(int iType, bool bChangeType);
	Mat& getStarMat(int iStarType);
	Mat& getTreeMat(int iTreeType);

	stPoint getTreePos();
	stPoint getToPos();
	stPoint getFromPos();
	stPoint getMsgPos();
	stPoint getItemPos(int itemNo, int posNo);

private:
	void loadImage();
	bool ImportXml(string strXmlName);
	static string int2str(int ivalue);

private:
	bool										_bIsSetup;
	map<int, stFrameInfo>	_FrameInfoMap;

	int										_iTotalFrame;
	Mat										_itemMat[cNUM_ITEM_TYPE][cNUM_ITEM_CHANGE_TYPE];
	Mat										_treeMat[cNUM_TREE_TYPE];
	Mat										_starMat[cNUM_STAR_TYPE];

	stPoint								_itemPos[cNUM_ITEM_POSITION_TYPE][cNUM_ITEM_POSITION];
	stPoint								_tree, _to, _from, _msg;

//-------------------
//Singleton
//-------------------
public:
	static frameInfoManager* GetInstance();
	static void Destroy();

private:
	static frameInfoManager *pInstance;
};

#endif // !_FRAME_INFO_MANAGER_
