
#include "opencv2/opencv.hpp"
#include<io.h>
#include<stdio.h>

using namespace std;

CString GetFolderName();
CString   GetModulePath(); // 
CString  GetFileSuffix( CString FileName);// 
void MatchHist(IplImage* srcImg, IplImage* dstImg);
CString  OpenFile();
void Bmp2IplImage(BYTE*Bmp, IplImage*cvPhoto);
CString  GetFilename( CString FileName);//获得文件名；
void  Savetestdata(CString name,int NO,int wide, int height);
void getFiles( string path, vector<string>& files );
string Cstring2string(CString cs);

CString string2Cstring(string s);
void  WriteInidata(int*In, int StrAr, int length);