#include "stdafx.h"
#include"funclibrary.h"
#include "direct.h"


CString  GetFilename( CString FileName)//获得文件名；
{
	CString Suffix = "0";
	char* fileName = (LPSTR)(LPCTSTR)FileName;

	char c = '.'; 
	char d = '\\' ;
	char *ptr1 = strrchr(fileName, c);    //最后一个出现c的位置
	char *ptr2 = strrchr(fileName, d);    //最后一个出现d的位置

	int  n = (ptr1 - ptr2 - 1) ; //后缀名长度；
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


CString  GetFileSuffix( CString FileName)//2 获得文件后缀名；
{
	CString Suffix = "0";
	char* fileName = (LPSTR)(LPCTSTR)FileName;

	char c = '.';
	char *ptr = strrchr(fileName, c);    //最后一个出现c的位��?
	int  pos = ptr-fileName;             //用指针相��?求得c索引
	int  n = strlen(fileName) - (pos + 1) ; //后缀名长度；
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

void Bmp2IplImage(BYTE*Bmp, IplImage*cvPhoto)
{
	//	memcpy(cvPhoto->imageData,(char*)(Bmp),1920*1440*3); //�������õ���ͼ���ǵ�������
	for (int y = 0 ;y < cvPhoto->height; y++)
	{
		memcpy(cvPhoto->imageData+y*cvPhoto->widthStep, Bmp + 3*(cvPhoto->height - y - 1)*cvPhoto->width,3*cvPhoto->width);//��һ����ת��
	}

}	

void MatchHist(IplImage* srcImg, IplImage* dstImg) //改变的是SRC
{
	// ****** 如果��?RGB 图片则转为灰度图片操��?******

	int grayArr[256];
	int srcRow = srcImg->height;
	int srcCol = srcImg->width;
	int dstRow = dstImg->height;
	int dstCol = dstImg->width;
	float srcCdfArr[256]  = {0.f};
	float dstCdfArr[256]  = {0.f};
	float tmp;

	// *** 求解源图片的累积直方图（概率）分��?***
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

	// *** 求解目标图片的累积直方图（概率）分布 ***
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

	// *** 直方图匹配算��?***
	int histMap[256];
	int minTag;
	for(int i=0; i<256; i++)
	{
		float minMap = 10.f;
		for(int j=0; j<256; j++)
		{
			if (minMap > abs(srcCdfArr[i] - dstCdfArr[j])) //求最小值；
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
