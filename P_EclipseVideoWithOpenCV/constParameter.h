#ifndef _ECLIPSE_VIDEO_PARAMETER_
#define _ECLIPSE_VIDEO_PARAMETER_

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <windows.h>
//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//Tinyxml
#include "Tinyxml/tinyxml2.h"

//#define AMZ

using namespace std;
using namespace cv;
using namespace tinyxml2;

static const int cNUM_ITEM_TYPE = 5;
static const int cNUM_ITEM_POSITION_TYPE = 3;
static const int cNUM_ITEM_POSITION = 4;
static const int cNUM_ITEM_CHANGE_TYPE = 2;
static const int cNUM_STAR_TYPE = 4;
static const int cNUM_TREE_TYPE = 4;
static const int cNUM_TREE_CODE = 5;

static const int cCHECK_NEW_DATA_MS = 10000;
static const int cNUM_THREAD = 5;

static const int cCOVER_FRAME_NO = 300;

//cover
static const string COVER_CMD_1 = "copy data\\video_result\\video_1\\video_300.jpg ";
static const string COVER_CMD_2 = "copy data\\video_result\\video_1\\video_300.jpg ";
static const string COVER_CMD_3 = "copy data\\video_result\\video_1\\video_300.jpg ";
static const string COVER_CMD_4 = "copy data\\video_result\\video_1\\video_300.jpg ";

//ffmpeg
static const string FFMPEG_CMD_1 = "ffmpeg -framerate 24 -y -i data/video_result/video_1/video_%3d.jpg -i data/audio/bgm.mp3 -c:v libx264 -r 24 -pix_fmt yuv420p -c:a aac -strict -2 -threads 4 -shortest ";
static const string FFMPEG_CMD_2 = "ffmpeg -framerate 24 -y -i data/video_result/video_2/video_%3d.jpg -i data/audio/bgm.mp3 -c:v libx264 -r 24 -pix_fmt yuv420p -c:a aac -strict -2 -threads 4 -shortest ";
static const string FFMPEG_CMD_3 = "ffmpeg -framerate 24 -y -i data/video_result/video_3/video_%3d.jpg -i data/audio/bgm.mp3 -c:v libx264 -r 24 -pix_fmt yuv420p -c:a aac -strict -2 -threads 4 -shortest ";
static const string FFMPEG_CMD_4 = "ffmpeg -framerate 24 -y -i data/video_result/video_4/video_%3d.jpg -i data/audio/bgm.mp3 -c:v libx264 -r 24 -pix_fmt yuv420p -c:a aac -strict -2 -threads 4 -shortest ";

//default folder
static const string cVIDEO_FRAME_F_SOURCE_FOLDER = "data/video_source/video_%d_f/video_%03d.png";
static const string cVIDEO_FRAME_B_SOURCE_FOLDER = "data/video_source/video_%d_b/video_%03d.png";
static const string cVIDEO_FRAME_RESULT_FOLDER = "data/video_result/video_%d/video_%03d.jpg";

#ifdef AMZ
static const string cORDER_XML_FOLDER = "C:\\xampp\\htdocs\\eclipse\\s\\orders\\";
static const string cORDER_XML_SUCCESS_FOLDER = "C:\\xampp\\htdocs\\eclipse\\s\\orders_success\\";
static const string cORDER_XML_FAILED_FOLDER = "C:\\xampp\\htdocs\\eclipse\\s\\orders_failed\\";
static const string cTEXT_FOLDER = "C:\\xampp\\htdocs\\eclipse\\s\\text\\";
static const string cOUTPUT_VIDEO_FOLDER = "C:\\xampp\\htdocs\\eclipse\\s\\output_video\\";
static const string cCOVER_FOLDER = "C:\\xampp\\htdocs\\eclipse\\s\\covers\\";
#else //AMZ
//static const string cORDER_XML_FOLDER = "data\\orders\\";
//static const string cORDER_XML_SUCCESS_FOLDER = "data\\orders_success\\";
//static const string cORDER_XML_FAILED_FOLDER = "data\\orders_failed\\";
//static const string cTEXT_FOLDER = "data\\text\\";
//static const string cOUTPUT_VIDEO_FOLDER = "data\\output_video\\";
static const string cORDER_XML_FOLDER = "D:\\xampp\\htdocs\\eclipse\\s\\orders\\";
static const string cORDER_XML_SUCCESS_FOLDER = "D:\\xampp\\htdocs\\eclipse\\s\\orders_success\\";
static const string cORDER_XML_FAILED_FOLDER = "D:\\xampp\\htdocs\\eclipse\\s\\orders_failed\\";
static const string cTEXT_FOLDER = "D:\\xampp\\htdocs\\eclipse\\s\\text\\";
static const string cOUTPUT_VIDEO_FOLDER = "D:\\xampp\\htdocs\\eclipse\\s\\output_video\\";
static const string cCOVER_FOLDER = "D:\\xampp\\htdocs\\eclipse\\s\\covers\\";
#endif//AMZ



#endif // !_ECLIPSE_VIDEO_PARAMETER_
