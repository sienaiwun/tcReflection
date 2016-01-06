/*

	Copyright 2014 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <iostream>
#include <fstream>
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#include "ogldev_util.h"

bool ReadFile(const char* pFileName, string& outFile)
{
    ifstream f(pFileName);
    
    bool ret = false;
    
    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
            outFile.append(line);
            outFile.append("\n");
        }
        
        f.close();
        
        ret = true;
    }
    else {
        OGLDEV_FILE_ERROR(pFileName);
    }
    
    return ret;
}


void OgldevFileError(const char* pFileName, uint line, const char* pFileError)
{
#ifdef WIN32
	char msg[1000];
	_snprintf_s(msg, sizeof(msg), "%s:%d: unable to open file `%s`", pFileName, line, pFileError);
    MessageBoxA(NULL, msg, NULL, 0);
#else
    fprintf(stderr, "%s:%d: unable to open file `%s`\n", pFileName, line, pFileError);
#endif    
}


long long GetCurrentTimeMillis()
{
#ifdef WIN32    
	return GetTickCount();
#else
	timeval t;
	gettimeofday(&t, NULL);

	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
#endif    
}
//void SaveBMP(const char *, BYTE *, int, int);
/*void SaveBMP(const char *filename, unsigned char *pixBuf, int width, int height){

	FILE *pFile;
	fopen_s(&pFile, filename, "wb");
	if (pFile == NULL){
		printf("Save file to %s : failed !\n", filename);
		return;
	}
	//------交换顺序---------------------
	BYTE *m_PixChanged = new BYTE[width* height * 3];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int inx = 3 * (i * width + j);
			m_PixChanged[inx + 0] = pixBuf[inx + 2];
			m_PixChanged[inx + 1] = pixBuf[inx + 1];
			m_PixChanged[inx + 2] = pixBuf[inx + 0];
		}
	}
	//-------- Create a new file for writing -----------------
	//-------------------- 位图信息头 -----------------------
	BITMAPINFOHEADER BMIH;                 //位图信息头
	BMIH.biSize = sizeof(BITMAPINFOHEADER);//指定这个结构的长度，40字节

	//指定图象的宽度，单位是象素。
	BMIH.biWidth = width;

	//	指定图象的高度，单位是象素。
	BMIH.biHeight = height;

	//目标绘图设备包含的层数，必须设置为1，不用考虑。
	BMIH.biPlanes = 1;

	//指定表示颜色时要用到的位数，常用的值为1(黑白二色图), 4(16色图), 8(256色), 24(真彩色图)
	BMIH.biBitCount = 24;

	//指定位图是否压缩，有效的值为BI_RGB，BI_RLE8，BI_RLE4，BI_BITFIELDS。BI_RGB不压缩
	BMIH.biCompression = BI_RGB;

	//指定实际的位图数据占用的字节数
	BMIH.biSizeImage = width * height * 3;

	//-------------------- 位图文件头 -----------------------
	BITMAPFILEHEADER bmfh;

	//位图信息头与文件头大小
	int nBitsOffset = sizeof(BITMAPFILEHEADER) + BMIH.biSize;

	//BMP图像数据的大小
	LONG lImageSize = BMIH.biSizeImage;

	//BMP图像文件的大小
	LONG lFileSize = nBitsOffset + lImageSize;

	//位图类别，根据不同的操作系统而不同，在Windows中，此字段的值总为‘BM’
	bmfh.bfType = 'B' + ('M' << 8);

	//BMP图像数据的地址
	bmfh.bfOffBits = nBitsOffset;

	//BMP图像文件的大小
	bmfh.bfSize = lFileSize;

	//二者总为0
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;

	//Write the bitmap file header
	//将位图文件头写入pFile
	UINT nWrittenFileHeaderSize = fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), pFile);

	//And then the bitmap info header
	//将位图信息头写入pFile
	UINT nWrittenInfoHeaderSize = fwrite(&BMIH, 1, sizeof(BITMAPINFOHEADER), pFile);

	//Finally, write the image data itself 
	//将位图文件图像数据写入pFile
	//-- the data represents our drawing

	UINT nWrittenDIBDataSize = fwrite(m_PixChanged, 1, lImageSize, pFile);

	fclose(pFile);

	delete[]m_PixChanged;
}
*/



#ifdef WIN32
/*float fmax(float a, float b)
{
    if (a > b)
        return a;
    else
        return b;
}*/
#endif