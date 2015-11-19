#include "eclipseFrameMixUnit.h"

//--------------------------------------------------------------
void FrameMixUnit::operator()()
{
	if(_bIsSetup)
	{
		mixFrame();
	}
}

//--------------------------------------------------------------
void FrameMixUnit::setTreeCode(int tree, int star, int item1, int item2, int item3)
{
	_starType = star;
	_treeType = tree;
	_itemType1 = item1;
	_itemType2 = item2;
	_itemType3 = item3;
}

//--------------------------------------------------------------
void FrameMixUnit::setTreeCode( int (&arr)[cNUM_TREE_CODE])
{
	_treeType = arr[0];
	_starType = arr[1];	
	_itemType1 = arr[2];
	_itemType2 = arr[3];
	_itemType3 = arr[4];
}

//--------------------------------------------------------------
void FrameMixUnit::setText(Mat& from, Mat& to, Mat& message, list<int>& frameIdxList)
{
	if(_bIsSetup)
	{
		return;
	}

	_from = from.clone();
	_to = to.clone();
	_message = message.clone();

	_FrameIndexList = frameIdxList;
	_bIsSetup = true;
}

//--------------------------------------------------------------
void FrameMixUnit::mixFrame()
{
	for(auto& Iter_ : _FrameIndexList)
	{
		char acLoadNameF_[64], acLoadNameB_[64];
		sprintf_s(acLoadNameB_,cVIDEO_FRAME_B_SOURCE_FOLDER.c_str(), _starType + 1, Iter_);
		sprintf_s(acLoadNameF_,cVIDEO_FRAME_F_SOURCE_FOLDER.c_str(), _starType + 1, Iter_);
		Mat MatFrameF_ = imread(acLoadNameF_, CV_LOAD_IMAGE_UNCHANGED);
		Mat MatFrameB_ = imread(acLoadNameB_);

		//Get the frame info
		stFrameInfo stFrameInfo_;
		if(frameInfoManager::GetInstance()->GetFrameInfo(Iter_, stFrameInfo_))
		{
			mixTree(Iter_, MatFrameB_, stFrameInfo_);
		}

		//Mix Forground
		this->mixImg(MatFrameB_, MatFrameF_, stPoint(), false);

		//Save the result
		char acSaveName_[64];
		sprintf_s(acSaveName_, cVIDEO_FRAME_RESULT_FOLDER.c_str(), _starType + 1, Iter_);
		imwrite(acSaveName_, MatFrameB_);
	}
	_bIsFinish = true;
}

//--------------------------------------------------------------
void FrameMixUnit::mixTree(int index, Mat& MatFrameB, stFrameInfo& FrameInfo)
{
	//Get image
	Mat MatTree_, MatStar_;
	Mat MatItem1_, MatItem2_, MatItem3_;
		
	MatTree_ = frameInfoManager::GetInstance()->getTreeMat(_treeType);
	//MatStar_ = frameInfoManager::GetInstance()->getStarMat(_starType);
	MatItem1_ = frameInfoManager::GetInstance()->getItemMat(_itemType1, FrameInfo.itemType);
	MatItem2_ = frameInfoManager::GetInstance()->getItemMat(_itemType2, FrameInfo.itemType);
	MatItem3_ = frameInfoManager::GetInstance()->getItemMat(_itemType3, FrameInfo.itemType);

	//Mix tree
	this->mixImg(MatFrameB, MatTree_, frameInfoManager::GetInstance()->getTreePos(), false);

	//Mix star
	//this->mixImg(MatFrameB, MatStar_, frameInfoManager::GetInstance()->getTreePos(), false);

	//Mix Item
	for(int posIdx_ = 0; posIdx_ < cNUM_ITEM_POSITION; posIdx_++)
	{
		stPoint item1Pos_ = frameInfoManager::GetInstance()->getItemPos(0, posIdx_);
		stPoint item2Pos_ = frameInfoManager::GetInstance()->getItemPos(1, posIdx_);
		stPoint item3Pos_ = frameInfoManager::GetInstance()->getItemPos(2, posIdx_);
			
		this->mixImg(MatFrameB, MatItem1_, item1Pos_, true);
		this->mixImg(MatFrameB, MatItem2_, item2Pos_, true);
		this->mixImg(MatFrameB, MatItem3_, item3Pos_, true);
	}

	//Mix To
	this->mixImg(MatFrameB, _to, frameInfoManager::GetInstance()->getToPos(), FrameInfo.toAlpha, false);

	//Mix Msg
	this->mixImg(MatFrameB, _message, frameInfoManager::GetInstance()->getMsgPos(), FrameInfo.msgAlpha, false);

	//Mix From
	this->mixImg(MatFrameB, _from, frameInfoManager::GetInstance()->getFromPos(), FrameInfo.fromAlpha, false);
}

//--------------------------------------------------------------
void FrameMixUnit::mixImg(Mat& MatB,  Mat& MatF, stPoint pos, bool center)
{
	if(MatF.channels() != 4)
	{
		return;
	}

	stPoint start_, end_;
	if(center)
	{
		start_.x = (int)pos.x - MatF.cols/2.0;
		start_.y = (int)pos.y - MatF.rows/2.0;
		end_.x = (int)pos.x + MatF.cols/2.0;
		end_.y = (int)pos.y + MatF.rows/2.0;
	}
	else
	{
		start_ = pos;
		end_.x = (int)pos.x + MatF.cols;
		end_.y = (int)pos.y + MatF.rows;
	}

	//Check bounding box
	if(start_.x < 0 || start_.y < 0 || end_.x > MatB.cols || end_.y > MatB.rows)
	{
		return;
	}

	//Draw
	for(int y_ = 0; y_ < MatF.rows; ++y_)
	{		
		for(int x_ = 0; x_ < MatF.cols; ++x_)
		{
			stPoint pos_ = start_;
			pos_.y += y_;
			pos_.x += x_;			
			
			unsigned char ucFR_ = MatF.at<Vec4b>(y_, x_)[0];
			unsigned char ucFG_ = MatF.at<Vec4b>(y_, x_)[1];
			unsigned char ucFB_ = MatF.at<Vec4b>(y_, x_)[2];
			float fAlpha_ =  (float)MatF.at<Vec4b>(y_, x_)[3]/255.0;
			
			unsigned char ucBR_ = MatB.at<Vec3b>(pos_.y, pos_.x)[0];
			unsigned char ucBG_ = MatB.at<Vec3b>(pos_.y, pos_.x)[1];
			unsigned char ucBB_ = MatB.at<Vec3b>(pos_.y, pos_.x)[2];
			
			MatB.at<Vec3b>(pos_.y, pos_.x)[0] = 
				(unsigned char)ucBR_ * (1 - fAlpha_) + (ucFR_ * fAlpha_);
			MatB.at<Vec3b>(pos_.y, pos_.x)[1] = 
				(unsigned char)ucBG_ * (1 - fAlpha_) + (ucFG_ * fAlpha_);
			MatB.at<Vec3b>(pos_.y, pos_.x)[2] = 
				(unsigned char)ucBB_ * (1 - fAlpha_) + (ucFB_ * fAlpha_);
		}
	}
}

//--------------------------------------------------------------
void FrameMixUnit::mixImg(Mat& MatB,  Mat& MatF, stPoint pos, int alpha, bool center)
{
	if(alpha <= 0)
	{
		return;
	}

	stPoint start_, end_;
	if(center)
	{
		start_.x = (int)pos.x - MatF.cols/2.0;
		start_.y = (int)pos.y - MatF.rows/2.0;
		end_.x = (int)pos.x + MatF.cols/2.0;
		end_.y = (int)pos.y + MatF.rows/2.0;
	}
	else
	{
		start_ = pos;
		end_.x = (int)pos.x + MatF.cols;
		end_.y = (int)pos.y + MatF.rows;
	}

	//Check bounding box
	if(start_.x < 0 || start_.y < 0 || end_.x > MatB.cols || end_.y > MatB.rows)
	{
		return;
	}

	//Draw
	for(int y_ = 0; y_ < MatF.rows; ++y_)
	{		
		for(int x_ = 0; x_ < MatF.cols; ++x_)
		{
			stPoint pos_ = start_;
			pos_.y += y_;
			pos_.x += x_;			
			
			unsigned char ucFR_ = MatF.at<Vec3b>(y_, x_)[0];
			unsigned char ucFG_ = MatF.at<Vec3b>(y_, x_)[1];
			unsigned char ucFB_ = MatF.at<Vec3b>(y_, x_)[2];
			float fAlpha_ = (alpha / 255.0);
			
			unsigned char ucBR_ = MatB.at<Vec3b>(pos_.y, pos_.x)[0];
			unsigned char ucBG_ = MatB.at<Vec3b>(pos_.y, pos_.x)[1];
			unsigned char ucBB_ = MatB.at<Vec3b>(pos_.y, pos_.x)[2];

			if(ucFR_ > 0)
			{
				MatB.at<Vec3b>(pos_.y, pos_.x)[0] = 
					(unsigned char)MIN(255, ucBR_  + (ucFR_ * fAlpha_));
			}

			if(ucFG_ > 0)
			{
				MatB.at<Vec3b>(pos_.y, pos_.x)[1] = 
					(unsigned char)MIN(255, ucBG_  + (ucFG_ * fAlpha_));
					//(unsigned char)ucBG_ * (1 - fAlpha_) + (ucFG_ * fAlpha_);
			}

			if(ucFB_ > 0)
			{
				MatB.at<Vec3b>(pos_.y, pos_.x)[2] = 
					(unsigned char)MIN(255, ucBB_  + (ucFB_ * fAlpha_));
					//(unsigned char)ucBB_ * (1 - fAlpha_) + (ucFB_ * fAlpha_);
			}
		}
	}
}