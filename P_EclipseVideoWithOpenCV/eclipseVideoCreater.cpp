#include "eclipseVideoCreater.h"

//--------------------------------------------------------------
bool eclipseVideoCreater::setup()
{
	bool bSuccess_ = true;
	bSuccess_ &= frameInfoManager::GetInstance()->setup("data/_EclipseFrameInfo.xml");

	//COVER
	return bSuccess_;
}

//--------------------------------------------------------------
void eclipseVideoCreater::update()
{
	string strOrderName_ = "";
	if(!checkNewFile(strOrderName_))
	{
		return;
	}
	//load order xml
	tinyxml2::XMLDocument xml_;
	if(xml_.LoadFile( (cORDER_XML_FOLDER + strOrderName_).c_str() ) != XMLError::XML_SUCCESS)
	{
		cout<<"[ERROR]Loading order xml failed : " << strOrderName_ <<endl; 
		moveOrder(strOrderName_, false);
		return;
	}

	XMLElement* pElement_ = xml_.FirstChildElement();
	string strTID_ = pElement_->FirstChildElement("TID")->GetText();
	string strTreeCode_ = pElement_->FirstChildElement("TREE_CODE")->GetText();
	string strOutputName_ = pElement_->FirstChildElement("VIDEO_NAME")->GetText();
	
	//Decode
	int treeCode_[cNUM_TREE_CODE] = {0};
	if(!decode(strTreeCode_, treeCode_))
	{
		moveOrder(strOrderName_, false);
		return;
	}

	cout<<"[NOTICE]Get New Order :" << strOrderName_ << endl;
	if(!mixVideo(strTID_, treeCode_, strOutputName_))
	{
		moveOrder(strOrderName_, false);
		return;
	}
	moveOrder(strOrderName_, true);

}

//--------------------------------------------------------------
bool eclipseVideoCreater::checkNewFile(string& strXMLName)
{
	strXMLName.clear();

	WIN32_FIND_DATAA Fd_;
	HANDLE pH_ = FindFirstFileA((cORDER_XML_FOLDER + "*.xml").c_str(), &Fd_);

	if(pH_ == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		strXMLName = string(Fd_.cFileName);
		return true;
	}
}

//--------------------------------------------------------------
bool eclipseVideoCreater::checkVideo(string strVideoName)
{
	WIN32_FIND_DATAA Fd_;
	HANDLE pH_ = FindFirstFileA((cOUTPUT_VIDEO_FOLDER + strVideoName).c_str(), &Fd_);

	if(pH_ == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//--------------------------------------------------------------
bool eclipseVideoCreater::mixVideo(string strTID_, int(&arr)[cNUM_TREE_CODE], string videoName)
{
	//Load Text
	Mat MatTo_, MatMsg_, MatFrom_;
	MatTo_ = imread(cTEXT_FOLDER + strTID_ + "_to.png");
	MatMsg_ = imread(cTEXT_FOLDER + strTID_ + "_msg.png");
	MatFrom_ = imread(cTEXT_FOLDER + strTID_ + "_from.png");

	if(MatTo_.dims != 2 || MatMsg_.dims != 2 || MatFrom_.dims !=2)
	{
		cout<<"[ERROR]Loading text files failed"<<endl;
		return false;
	}

	//Set frame index
	list<int> IndexList_[cNUM_THREAD];
	for(int idx_ = 0; idx_ < frameInfoManager::GetInstance()->GetFrameNum(); ++idx_)
	{
		int TId_ = idx_ % cNUM_THREAD;
		IndexList_[TId_].push_back(idx_);
	}

	//Start all thread
	FrameMixUnit Unit_[cNUM_THREAD];
	thread threadSet_[cNUM_THREAD];
	for(int idx_ = 0; idx_ < cNUM_THREAD; idx_++)
	{
		Unit_[idx_].setTreeCode(arr);
		Unit_[idx_].setText(MatFrom_, MatTo_, MatMsg_, IndexList_[idx_]);
		threadSet_[idx_] = thread(Unit_[idx_]);
	}
	cout<<"[NOTICE]Start All thread"<<endl;

	//Wait all thread finish
	for(int idx_ = 0; idx_ < cNUM_THREAD; idx_++)
	{
		threadSet_[idx_].join();
	}
	cout<<"[NOTICE]Image Mix Success!!\n";

	//Create Cover
	string strCoverName_ =  videoName + ".jpg";
	string strCoverCmd_ = cCOVER_FOLDER + strCoverName_;

	//Create Video
	string strOutputName_ = videoName + ".mp4";
	string strCmd_ = cOUTPUT_VIDEO_FOLDER + strOutputName_;
	switch(arr[1])
	{
	case 0:
		{
			strCoverCmd_ = COVER_CMD_1 + strCoverCmd_;
			strCmd_ = FFMPEG_CMD_1 + strCmd_;
		}
		break;
	case 1:
		{
			strCoverCmd_ = COVER_CMD_2 + strCoverCmd_;
			strCmd_ = FFMPEG_CMD_2 + strCmd_;
		}
		break;
	case 2:
		{
			strCoverCmd_ = COVER_CMD_3 + strCoverCmd_;
			strCmd_ = FFMPEG_CMD_3 + strCmd_;
		}
		break;
	case 3:
		{
			strCoverCmd_ = COVER_CMD_4 + strCoverCmd_;
			strCmd_ = FFMPEG_CMD_4 + strCmd_;
		}
		break;
	}

	system(strCoverCmd_.c_str());
	system(strCmd_.c_str());

	//Check the video
	if(!this->checkVideo(strOutputName_))
	{
		cout<<"[ERROR]Create video failed!!\n";
		return false;
	}
	else
	{
		cout<<"[NOTICE]Create video success!!\n";
		return true;
	}
}

//--------------------------------------------------------------
bool eclipseVideoCreater::decode(string strCode, int(&arr)[cNUM_TREE_CODE])
{
	if(strCode.size() != 5)
	{
		cout<<"[ERROR]wraning tree code"<<endl;
		return false;
	}

	int idx_ = 0;
	for(auto Iter_ : strCode)
	{
		arr[idx_] = Iter_ - '0';
		idx_++;
	}
	return true;
}

//--------------------------------------------------------------
void eclipseVideoCreater::moveOrder(string strOrderName, bool isSuccess)
{
	string strMoveCmd_ = "move " + cORDER_XML_FOLDER + strOrderName + " ";
	if(isSuccess)
	{
		strMoveCmd_ += cORDER_XML_SUCCESS_FOLDER;
	}
	else
	{
		strMoveCmd_ += cORDER_XML_FAILED_FOLDER;
	}
	system(strMoveCmd_.c_str());
}