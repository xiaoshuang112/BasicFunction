#include "stdafx.h"
#include"funclibrary.h"
#include "direct.h"

void  WriteInidata(int*In, int StrAr, int length)
{
	CString   DataINI = GetModulePath() + "Inidata.ini"; // 设定参 数文档路径
	FILE *File = NULL;
	fopen_s(&File,DataINI , "w");
	fclose(File);

	CStdioFile MyFile(DataINI, CFile::modeWrite | CFile::typeText);

	CString A ="";

	for (int i = StrAr; i < length; i++)
	{
		A.Format("%d,%d\n",i, In[i]);

		MyFile.WriteString(A);
	}

	ShellExecute(NULL,"open",DataINI,NULL,NULL,SW_SHOW);// 打开外部文件程序

}

CString GetFolderName()
{
	static TCHAR strDirName[MAX_PATH];

	BROWSEINFO bi;
	CString szString = TEXT("选择一个源文件子文件夹");
	bi.hwndOwner = ::GetFocus();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = strDirName;
	bi.lpszTitle = szString;
	bi.ulFlags = BIF_BROWSEFORCOMPUTER | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST pItemIDList = ::SHBrowseForFolder(&bi);
	if(pItemIDList == NULL)
	{
		return ;
	}

	::SHGetPathFromIDList(pItemIDList, strDirName);

	CString str = strDirName;
	if(str != "" && str.Right(1) != '//')
		str+= "\\";
	return str;
}

CString  GetFilename( CString FileName)//èŽ·å¾—æ–‡ä»¶åï¼›
{
	CString Suffix = "0";
	char* fileName = (LPSTR)(LPCTSTR)FileName;

	char c = '.';
	char d = '\\' ;
	char *ptr1 = strrchr(fileName, c);    //æœ€åŽä¸€ä¸ªå‡ºçŽ°cçš„ä½ç½®
	char *ptr2 = strrchr(fileName, d);    //æœ€åŽä¸€ä¸ªå‡ºçŽ°dçš„ä½ç½®

	int  n = (ptr1 - ptr2 - 1) ; //åŽç¼€åé•¿åº¦ï¼›
	char stbuf[50] ={0};
	strncpy_s(stbuf,  ptr2 + 1, n);
	Suffix.Format(stbuf);
	return Suffix;
}

CString  GetModulePath()  //1
{
	int  i    = 0;
	int  len  = 0;
	bool flag = false;
	CString myStr;
	char ModulePath[128] = {0};
	char path[128]={0};
	GetModuleFileName(NULL, ModulePath, 128);//return  real  lenghth
	len = strlen(ModulePath);
	for (i = len - 1; i >= 0; i--)
	{
		if (ModulePath[i] == '\\')
		{
			ModulePath[i + 1] = 0;
			flag = true;
			strcpy_s(path, ModulePath);
			break;
		}
	}

	if (!flag)
	{
		strcpy_s(path, "");

	}
	myStr.Format(path);
	return myStr;
}


CString  GetFileSuffix( CString FileName)//2 èŽ·å¾—æ–‡ä»¶åŽç¼€åï¼›
{
	CString Suffix = "0";
	char* fileName = (LPSTR)(LPCTSTR)FileName;

	char c = '.';
	char *ptr = strrchr(fileName, c);    //æœ€åŽä¸€ä¸ªå‡ºçŽ°cçš„ä½ï¿½ï¿½?
	int  pos = ptr-fileName;             //ç”¨æŒ‡é’ˆç›¸ï¿½ï¿½?æ±‚å¾—cç´¢å¼•
	int  n = strlen(fileName) - (pos + 1) ; //åŽç¼€åé•¿åº¦ï¼›
	char stbuf[20] ={0};
	strncpy_s(stbuf,  ptr + 1, n);
	Suffix.Format(stbuf);
	return Suffix;
}

CString  OpenFile()
{
	CFileDialog pHoto(TRUE,0,0,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"All Files(*.*)|*.*||", NULL);

	pHoto.m_ofn.lpstrTitle = "Open File";

	if (pHoto.DoModal()!=IDOK)
	{
		return "";
	}


	CString FileName = pHoto.GetPathName();

	return FileName;
}
/**************************************getFiles ÓÃ·¨*********************************************************/
/*  vector<string> files;

	CString Path = GetModulePath();
	Path = Path +"1";
	string filePath = Path.GetBuffer(0);
	////»ñÈ¡¸ÃÂ·¾¶ÏÂµÄËùÓÐÎÄ¼þ
	getFiles(filePath, files );

	vector<CString> cFiles;

	CString buffer;
	int size = files.size();
	for (int i = 0;i < size;i++)
	{
		buffer = files[i].c_str();
	 	cFiles.push_back(buffer);
	}
*/
/************************************************************************************************************/
void getFiles( string path, vector<string>& files )  //»ñÈ¡ÎÄ¼þ¼ÐÏÂµÄËùÓÐÎÄ¼þ£»
{
	//ÎÄ¼þ¾ä±ú
	long   hFile   =   0;
	//ÎÄ¼þÐÅÏ¢
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//Èç¹ûÊÇÄ¿Â¼,µü´úÖ®
			//Èç¹û²»ÊÇ,¼ÓÈëÁÐ±í
			if((fileinfo.attrib &  _A_SUBDIR))
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}

string Cstring2string(Cstring cs)
{
	std::string strStl;

    strStl=cs.GetBuffer(0);
    return strStl;
}

CString string2Cstring(string s)
{
	CString strMfc;
	strMfc=s.c_str();
	return strMfc;
}

void Bmp2IplImage(BYTE*Bmp, IplImage*cvPhoto)
{
	//	memcpy(cvPhoto->imageData,(char*)(Bmp),1920*1440*3); //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ãµï¿½ï¿½ï¿½Í¼ï¿½ï¿½ï¿½Çµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	for (int y = 0 ;y < cvPhoto->height; y++)
	{
		memcpy(cvPhoto->imageData+y*cvPhoto->widthStep, Bmp + 3*(cvPhoto->height - y - 1)*cvPhoto->width,3*cvPhoto->width);//ï¿½ï¿½Ò»ï¿½ï¿½ï¿½ï¿½×ªï¿½ï¿½
	}

}

void MatchHist(IplImage* srcImg, IplImage* dstImg) //æ”¹å˜çš„æ˜¯SRC
{
	// ****** å¦‚æžœï¿½ï¿½?RGB å›¾ç‰‡åˆ™è½¬ä¸ºç°åº¦å›¾ç‰‡æ“ï¿½ï¿½?******

	int grayArr[256];
	int srcRow = srcImg->height;
	int srcCol = srcImg->width;
	int dstRow = dstImg->height;
	int dstCol = dstImg->width;
	float srcCdfArr[256]  = {0.f};
	float dstCdfArr[256]  = {0.f};
	float tmp;

	// *** æ±‚è§£æºå›¾ç‰‡çš„ç´¯ç§¯ç›´æ–¹å›¾ï¼ˆæ¦‚çŽ‡ï¼‰åˆ†ï¿½ï¿½?***
	memset(grayArr, 0, sizeof(grayArr));
	for(size_t nrow = 0; nrow < srcRow; nrow++)
	{
		UCHAR*pTr = (UCHAR*)(srcImg->imageData + nrow*srcImg->width);
		for(size_t ncol = 0; ncol < srcCol; ncol++)
		{
			int tag = pTr[ncol];
			grayArr[tag]++;
		}
	}


	tmp = 0;
	for(int i=0; i<256; i++)
	{
		tmp += grayArr[i];
		srcCdfArr[i] = tmp / (srcRow * srcCol);
		// std::cout<<srcCdfArr[i]<<std::endl;
	}

	// *** æ±‚è§£ç›®æ ‡å›¾ç‰‡çš„ç´¯ç§¯ç›´æ–¹å›¾ï¼ˆæ¦‚çŽ‡ï¼‰åˆ†å¸ƒ ***
	memset(grayArr, 0, sizeof(grayArr));
	for(size_t nrow = 0; nrow < dstRow; nrow++)
	{
		UCHAR*pTr = (UCHAR*)(dstImg->imageData + nrow*dstImg->width);
		for(size_t ncol = 0; ncol < dstCol; ncol++)
		{
			int tag = pTr[ncol];
			grayArr[tag]++;
		}
	}


	tmp = 0;
	for(int i=0; i<256; i++)
	{
		tmp += grayArr[i];
		dstCdfArr[i] = tmp / (dstRow * dstCol);
	}

	// *** ç›´æ–¹å›¾åŒ¹é…ç®—ï¿½ï¿½?***
	int histMap[256];
	int minTag;
	for(int i=0; i<256; i++)
	{
		float minMap = 10.f;
		for(int j=0; j<256; j++)
		{
			if (minMap > abs(srcCdfArr[i] - dstCdfArr[j])) //æ±‚æœ€å°å€¼ï¼›
			{
				minMap = abs(srcCdfArr[i] - dstCdfArr[j]);
				minTag = j;
			}
		}
		histMap[i] = minTag;
	}

	for(size_t nrow = 0; nrow < srcRow; nrow++)
	{
		UCHAR*pTr = (UCHAR*)(srcImg->imageData + nrow*srcImg->width);
		for(size_t ncol = 0; ncol < srcCol; ncol++)
		{
			int tag = pTr[ncol];
			pTr[ncol] = histMap[tag];
		}
	}

}

void   Savetestdata(CString name,int NO,int wide, int height)
{
	CString path= GetModulePath()+"Data\\";
	_mkdir(path);

	FILE  *fp  = NULL;


	CString FileName = path +name + ".xls";
	fp=freopen(FileName,"at",stdout);
	if(fp==NULL)
	{
		return;
	}

	long fileSizeTemp=ftell(fp);
	fseek(fp,0,SEEK_END );
	fileSizeTemp=ftell(fp);
	if(fileSizeTemp==0)
	{
		printf("N0\tWide\tHeight");
	}

	printf("\n");

	CString sdata;
	sdata.Format("%d\t%d\t%d",NO,wide,height);
	printf(sdata);
	fclose(fp);
}
