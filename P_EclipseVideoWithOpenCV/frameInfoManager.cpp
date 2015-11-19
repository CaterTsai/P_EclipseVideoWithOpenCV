#include "frameInfoManager.h"

//--------------------------------------------------------------
bool frameInfoManager::setup(string strXmlName)
{
	if(!_bIsSetup)
	{
		if(this->ImportXml(strXmlName))
		{
			_bIsSetup = true;
		}
		else
		{
			cout<<"[WARNING]Loading xml file failed."<<endl;
		}
		loadImage();
		return true;
	}
	else
	{
		cout<<"[WARNING]Xml has been loading before."<<endl;
		return false;
	}
}

//--------------------------------------------------------------
int frameInfoManager::GetFrameNum() const
{
	if(!_bIsSetup)
	{
		cout<<"[WARNING]Need setup before using. at frameInfoManager\n";
		return 0;
	}
	return _iTotalFrame;
}

//--------------------------------------------------------------
bool frameInfoManager::GetFrameInfo(int iFrameID, stFrameInfo& refFrameInfo) const
{
	if(!_bIsSetup)
	{
		cout<<"[WARNING]Need setup before using. at frameInfoManager\n";
		return false;
	}

	auto Iter_ = _FrameInfoMap.find(iFrameID);
	if(Iter_ == _FrameInfoMap.end())
	{
		return false;
	}
	else
	{
		refFrameInfo = Iter_->second;
		return true;
	}
}

//--------------------------------------------------------------
Mat& frameInfoManager::getItemMat(int iType, bool bChangeType)
{
	if(iType < 0 || iType > cNUM_ITEM_TYPE)
	{
		return _itemMat[0][0];
	}

	return _itemMat[iType][bChangeType];
}

//--------------------------------------------------------------
Mat& frameInfoManager::getStarMat(int iStarType)
{
	if(iStarType < 0 || iStarType > cNUM_ITEM_TYPE)
	{
		return _starMat[0];
	}
	return _starMat[iStarType];
}

//--------------------------------------------------------------
Mat& frameInfoManager::getTreeMat(int iTreeType)
{
	if(iTreeType < 0 || iTreeType > cNUM_ITEM_TYPE)
	{
		return _treeMat[0];
	}
	return _treeMat[iTreeType];
}

//--------------------------------------------------------------
stPoint frameInfoManager::getTreePos()
{
	return _tree;
}

//--------------------------------------------------------------
stPoint frameInfoManager::getToPos()
{
	return _to;
}

//--------------------------------------------------------------
stPoint frameInfoManager::getFromPos()
{
	return _from;
}

//--------------------------------------------------------------
stPoint frameInfoManager::getMsgPos()
{
	return _msg;
}

//--------------------------------------------------------------
stPoint frameInfoManager::getItemPos(int itemNo, int posNo)
{
	stPoint  pos_;
	if((itemNo >= 0 && itemNo < cNUM_ITEM_POSITION_TYPE) && (posNo >= 0 && posNo < cNUM_ITEM_POSITION))
	{
		pos_ = _itemPos[itemNo][posNo];
	}
	return pos_;
}

//--------------------------------------------------------------
void frameInfoManager::loadImage()
{
	//Tree
	for(int idx_ = 0; idx_ < cNUM_TREE_TYPE; idx_++)
	{
		string strName_ = "data/images/tree_" + int2str(idx_ + 1) + ".png";
		_treeMat[idx_] = imread(strName_, CV_LOAD_IMAGE_UNCHANGED);
	}

	//Star
	for(int idx_ = 0; idx_ < cNUM_STAR_TYPE; idx_++)
	{
		string strName_ = "data/images/topdeco_" + int2str(idx_ + 1) + ".png";
		_starMat[idx_] = imread(strName_, CV_LOAD_IMAGE_UNCHANGED);
	}

	//Item
	for(int idx_ = 0; idx_ < cNUM_ITEM_TYPE; idx_++)
	{
		string strName1_ = "data/images/deco_" + int2str(idx_ + 1) + "_1.png";
		string strName2_ = "data/images/deco_" + int2str(idx_ + 1) + "_2.png";

		_itemMat[idx_][0] = imread(strName1_, CV_LOAD_IMAGE_UNCHANGED);
		_itemMat[idx_][1] = imread(strName2_, CV_LOAD_IMAGE_UNCHANGED);
	}
}

//--------------------------------------------------------------
bool frameInfoManager::ImportXml(string strXmlName)
{
	tinyxml2::XMLDocument xml_;
	
	if(xml_.LoadFile(strXmlName.c_str()) != XMLError::XML_SUCCESS)
	{
		return false;
	}

	//Setting
	XMLElement* pSetting_ = xml_.FirstChildElement("SETTING");
	pSetting_->FirstChildElement("TOTAL_FRAME")->QueryIntText(&_iTotalFrame);
	pSetting_->FirstChildElement("TREE_POS_X")->QueryIntText(&_tree.x);
	pSetting_->FirstChildElement("TREE_POS_Y")->QueryIntText(&_tree.y);
	pSetting_->FirstChildElement("TO_POS_X")->QueryIntText(&_to.x);
	pSetting_->FirstChildElement("TO_POS_Y")->QueryIntText(&_to.y);
	pSetting_->FirstChildElement("MSG_POS_X")->QueryIntText(&_msg.x);
	pSetting_->FirstChildElement("MSG_POS_Y")->QueryIntText(&_msg.y);
	pSetting_->FirstChildElement("FROM_POS_X")->QueryIntText(&_from.x);
	pSetting_->FirstChildElement("FROM_POS_Y")->QueryIntText(&_from.y);

	for(int itemIdx_ = 0; itemIdx_ < cNUM_ITEM_POSITION_TYPE; itemIdx_++)
	{
		string Item_ = "ITEM_" + int2str(itemIdx_ + 1);
		XMLElement* pItem_ = pSetting_->FirstChildElement(Item_.c_str());
		for(int idx_ = 0; idx_ < cNUM_ITEM_POSITION; idx_++)
		{
			string PosIdx_ = int2str(idx_ + 1);
			string X_ = "X_" + PosIdx_;
			string Y_ = "Y_" + PosIdx_;

			pItem_->FirstChildElement(X_.c_str())->QueryIntText(&_itemPos[itemIdx_][idx_].x);
			pItem_->FirstChildElement(Y_.c_str())->QueryIntText(&_itemPos[itemIdx_][idx_].y);
		}
	}

	//Frame Info
	XMLElement* pFrameInfo_ = xml_.FirstChildElement("ECLPISE_FRAME_INFO_SET")->FirstChildElement();
	while(pFrameInfo_)
	{
		stFrameInfo newFrameInfo_;
		int iFrameID_ = 0;

		pFrameInfo_->FirstChildElement("ID")->QueryIntText(&iFrameID_);
		pFrameInfo_->FirstChildElement("ITEM_TYPE")->QueryBoolText(&newFrameInfo_.itemType);
		pFrameInfo_->FirstChildElement("TO_ALPHA")->QueryIntText(&newFrameInfo_.toAlpha);
		pFrameInfo_->FirstChildElement("MSG_ALPHA")->QueryIntText(&newFrameInfo_.msgAlpha);
		pFrameInfo_->FirstChildElement("FROM_ALPHA")->QueryIntText(&newFrameInfo_.fromAlpha);

		_FrameInfoMap.insert(make_pair(iFrameID_, newFrameInfo_));

		pFrameInfo_ = pFrameInfo_->NextSiblingElement();
	}

	return true;
	
}

//--------------------------------------------------------------
string frameInfoManager::int2str(int ivalue)
{
	string strResult_;
	stringstream	ss_;
	ss_<<ivalue;
	ss_>>strResult_;

	return strResult_;
}

//--------------------------------------------------------------
frameInfoManager* frameInfoManager::pInstance = nullptr;
frameInfoManager* frameInfoManager::GetInstance()
{
	if(pInstance == nullptr)
	{
		pInstance = new frameInfoManager();
	}
	return pInstance;
}

//--------------------------------------------------------------
void frameInfoManager::Destroy()
{
	if(pInstance != nullptr)
	{
		delete pInstance;
	}
}