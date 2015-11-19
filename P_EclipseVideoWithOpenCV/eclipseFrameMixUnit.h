#ifndef _ECLIPSE_FRAME_MIX_UNIT
#define _ECLIPSE_FRAME_MIX_UNIT

#include "constParameter.h"
#include "frameInfoManager.h"

class FrameMixUnit
{
public:
	FrameMixUnit()
		:_bIsSetup(false)
		,_bIsFinish(false)
		,_starType(0)
		,_treeType(0)
		,_itemType1(0)
		,_itemType2(0)
		,_itemType3(0)
	{}
	void operator()();
	void setTreeCode( int tree, int star, int item1, int item2, int item3);
	void setTreeCode( int (&arr)[cNUM_TREE_CODE]);
	void setText(Mat& from, Mat& to, Mat& message, list<int>& frameIdxList);
	
private:
	void mixFrame();
	void mixTree(int index, Mat& MatFrameB, stFrameInfo& FrameInfo);

	void mixImg(Mat& MatTarget, Mat& MatSource, stPoint pos, bool center);
	void mixImg(Mat& MatTarget, Mat& MatSource, stPoint pos, int alpha, bool center);

private:
	bool		_bIsSetup, _bIsFinish;
	int			_starType, _treeType, _itemType1, _itemType2, _itemType3;
	Mat			_from, _to, _message;
	list<int>	_FrameIndexList;
};

#endif // !_ECLIPSE_FRAME_MIX_UNIT
