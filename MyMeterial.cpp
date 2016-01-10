#include "MyMeterial .h"
 
optix::Context MyMeterial::ms_rtContext = optix::Context::create();

MyMeterial::MyMeterial(char* filename):m_fileName(filename)
{
	isTex = 1;
}
MyMeterial::MyMeterial(float3 diffuse):m_diffuseColor(diffuse)
{
	isTex = 0;
}
MyMeterial::MyMeterial()
{
	isTex = 0;
}
const int MyMeterial::hasTx() const
{
	return isTex;
}
const GLuint MyMeterial::getTxId() const
{
	return m_texId;
}
const float3 MyMeterial::getDiffuse() const
{
	return m_diffuseColor;
}
optix::Material MyMeterial::getMaterial()
{
   optix::Context    rtContext = MyMeterial::ms_rtContext;
   if(isTex)
   {
	   
	   optix::Material flat_tex = rtContext->createMaterial();
	   flat_tex->setClosestHitProgram(0, rtContext->createProgramFromPTXFile( ptxpath("flat_tex_isg.cu"), "closest_hit_radiance"));
	   flat_tex["diffuse_texture"]->setTextureSampler(getTexture());
	   return flat_tex;
   }
   else
   {
	   optix::Material glossy = rtContext->createMaterial();
	   glossy->setClosestHitProgram(0, rtContext->createProgramFromPTXFile( ptxpath("glossy_isg.cu"), "closest_hit_radiance") );
	   glossy->setAnyHitProgram(1, rtContext->createProgramFromPTXFile( ptxpath("glossy_isg.cu"), "any_hit_shadow") );
	   glossy["diffuse_Color"]->setFloat( m_diffuseColor.x,m_diffuseColor.y,m_diffuseColor.z );
	   return glossy;
   }
}

optix::TextureSampler MyMeterial::getTexture()
{
	char* fileName = m_fileName;
	optix::Context    rtContext = MyMeterial::ms_rtContext;
	char *szPathName;
	szPathName = fileName;
	HDC      hdcTemp;                        // The DC To Hold Our Bitmap
	HBITMAP    hbmpTemp;                        // Holds The Bitmap Temporarily
	IPicture  *pPicture;                        // IPicture Interface
	OLECHAR    wszPath[MAX_PATH+1];                  // Full Path To Picture (WCHAR)
	char    szPath[MAX_PATH+1];                    // Full Path To Picture
	long    lWidth;                          // Width In Logical Units
	long    lHeight;                        // Height In Logical Units
	long    lWidthPixels;                      // Width In Pixels
	long    lHeightPixels;                      // Height In Pixels
	GLint    glMaxTexDim ;                      // Holds Maximum Texture Size

	if (strstr(szPathName, "http://"))                  // If PathName Contains http:// Then...
	{
		strcpy(szPath, szPathName);                    // Append The PathName To szPath
	}
	else                                // Otherwise... We Are Loading From A File
	{
		GetCurrentDirectory(MAX_PATH, szPath);              // Get Our Working Directory
		strcat(szPath, "\\");                      // Append "\" After The Working Directory
		strcat(szPath, szPathName);                    // Append The PathName
	}

	MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);    // Convert From ASCII To Unicode
	HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);

	if(FAILED(hr))                            // If Loading Failed
		return NULL;                          // Return False

	hdcTemp = CreateCompatibleDC(GetDC(0));                // Create The Windows Compatible Device Context
	if(!hdcTemp)                            // Did Creation Fail?
	{
		pPicture->Release();                      // Decrements IPicture Reference Count
		return NULL;                          // Return False (Failure)
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);          // Get Maximum Texture Size Supported

	pPicture->get_Width(&lWidth);                    // Get IPicture Width (Convert To Pixels)
	lWidthPixels  = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
	pPicture->get_Height(&lHeight);                    // Get IPicture Height (Convert To Pixels)
	lHeightPixels  = MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

	// Resize Image To Closest Power Of Two
	if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f); 
	else // Otherwise Set Width To "Max Power Of Two" That The Card Can Handle
		lWidthPixels = glMaxTexDim;

	if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
	else // Otherwise Set Height To "Max Power Of Two" That The Card Can Handle
		lHeightPixels = glMaxTexDim;

	//  Create A Temporary Bitmap
	BITMAPINFO  bi = {0};                        // The Type Of Bitmap We Request
	DWORD    *pBits = 0;                        // Pointer To The Bitmap Bits

	bi.bmiHeader.biSize      = sizeof(BITMAPINFOHEADER);        // Set Structure Size
	bi.bmiHeader.biBitCount    = 32;                  // 32 Bit
	bi.bmiHeader.biWidth    = lWidthPixels;              // Power Of Two Width
	bi.bmiHeader.biHeight    = lHeightPixels;            // Make Image Top Up (Positive Y-Axis)
	bi.bmiHeader.biCompression  = BI_RGB;                // RGB Encoding
	bi.bmiHeader.biPlanes    = 1;                  // 1 Bitplane

	//  Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
	hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);

	if(!hbmpTemp)                            // Did Creation Fail?
	{
		DeleteDC(hdcTemp);                        // Delete The Device Context
		pPicture->Release();                      // Decrements IPicture Reference Count
		return NULL;                          // Return False (Failure)
	}

	SelectObject(hdcTemp, hbmpTemp);                  // Select Handle To Our Temp DC And Our Temp Bitmap Object

	// Render The IPicture On To The Bitmap
	pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

	// Convert From BGR To RGB Format And Add An Alpha Value Of 255
	for(long i = 0; i < lWidthPixels * lHeightPixels; i++)        // Loop Through All Of The Pixels
	{
		BYTE* pPixel  = (BYTE*)(&pBits[i]);              // Grab The Current Pixel
		BYTE temp    = pPixel[0];                  // Store 1st Color In Temp Variable (Blue)
		pPixel[0]    = pPixel[2];                  // Move Red Value To Correct Position (1st)
		pPixel[2]    = temp;                      // Move Temp Value To Correct Blue Position (3rd)

		// This Will Make Any Black Pixels, Completely Transparent    (You Can Hardcode The Value If You Wish)
		if ((pPixel[0]==0) && (pPixel[1]==0) && (pPixel[2]==0))      // Is Pixel Completely Black
			pPixel[3]  = 0;                      // Set The Alpha Value To 0
		else                              // Otherwise
			pPixel[3]  = 255;                      // Set The Alpha Value To 255
	}

	glGenTextures(1, &m_texId);                      // Create The Texture

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, m_texId);                // Bind To The Texture ID
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);    // (Modify This For The Type Of Filtering You Want)
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // (Modify This For The Type Of Filtering You Want)
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);  // (Modify This If You Want Mipmaps)
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA16,lWidthPixels, lHeightPixels, GL_RGBA, GL_UNSIGNED_BYTE, pBits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 


	//return TRUE;                            // Return True (All Good)
	optix::Buffer floor_data = rtContext->createBuffer(RT_BUFFER_INPUT);
	floor_data->setFormat(RT_FORMAT_FLOAT4);
	floor_data->setSize(lWidthPixels, lHeightPixels);
	float4* tex_data = (float4*)floor_data->map();
	uchar4* pixel_data = (uchar4*)pBits;
	for(unsigned int i = 0; i < lWidthPixels * lHeightPixels; ++i) {
		tex_data[i] = make_float4(static_cast<float>(pixel_data[i].x)/255.99f,
			static_cast<float>(pixel_data[i].y)/255.99f,
			static_cast<float>(pixel_data[i].z)/255.99f,
			1.f);
	}
	floor_data->unmap();


	optix::TextureSampler floor_tex = rtContext->createTextureSampler();
	floor_tex->setWrapMode( 0, RT_WRAP_REPEAT );
	floor_tex->setWrapMode( 1, RT_WRAP_REPEAT );
	floor_tex->setIndexingMode( RT_TEXTURE_INDEX_NORMALIZED_COORDINATES );
	floor_tex->setReadMode( RT_TEXTURE_READ_NORMALIZED_FLOAT );
	floor_tex->setMaxAnisotropy( 1.0f );
	floor_tex->setMipLevelCount( 1u );
	floor_tex->setArraySize( 1u );
	floor_tex->setFilteringModes( RT_FILTER_LINEAR, RT_FILTER_LINEAR, RT_FILTER_NONE );
	floor_tex->setBuffer(0,0, floor_data);
	DeleteObject(hbmpTemp);                        // Delete The Object
	DeleteDC(hdcTemp);                          // Delete The Device Context

	pPicture->Release();                        // Decrements IPicture Reference Count

	return floor_tex;

}