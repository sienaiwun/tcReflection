#include "Geometry.h"
#include "cgShader.h"
#include "glslShader.h"
nv::vec3f MyGeometry::modelBBMax = nv::vec3f (-999.0f,-999.0f,-999.0f);
nv::vec3f MyGeometry::modelBBMin = nv::vec3f (999.0f,999.0f,999.0f);
optix::Context MyGeometry::ms_rtContext = optix::Context::create();
CGparameter * MyGeometry::mspCgisTex = 0;
CGparameter * MyGeometry::mspCgshiness = 0;
CGparameter * MyGeometry::mspCgIsModel = 0;
CGparameter * MyGeometry::mspCgIsWall = 0;
CGparameter * MyGeometry::mspCgDiffuseColor = 0;
CGparameter * MyGeometry::msgCgDiffuseTexParam = 0;
CGparameter * MyGeometry::msgCgreflectVale = 0;
MyMeterial* MyGeometry::s_mat = 0;
MyGeometry::MyGeometry(char *s):filename(s)
{
	//initGeometry(s);
}

void MyGeometry::initGeometry(char* name)
{
	m_model = new nv::Model();
	if(!m_model->loadModelFromFile(name)) 
	{
		printf("Unable to load model %s\n",name);
		exit(-1);
	}

	//m_model->removeDegeneratePrims();



	//m_model->computeNormals();

	// model->clearTexCoords();
	m_model->clearColors();
	m_model->clearTangents();

	if(0) 
	{
		nv::vec3f* vertices = (nv::vec3f*)m_model->getPositions();
		nv::vec3f* normals  = (nv::vec3f*)m_model->getNormals();
		for(int i = 0; i < m_model->getPositionCount(); ++i) {
			std::swap(vertices[i].y, vertices[i].z);
			vertices[i].z *= -1;

			std::swap(normals[i].y, normals[i].z);
			normals[i].z *= -1;
		}
	}

	m_model->compileModel();
	m_model->computeBoundingBox(m_modelBBMin, m_modelBBMax);
	for(int i =0;i<3;i++)
	{
		if(MyGeometry::modelBBMax[i]<m_modelBBMax[i])
			MyGeometry::modelBBMax[i] = m_modelBBMax[i];
		if(MyGeometry::modelBBMin[i]>m_modelBBMin[i])
			MyGeometry::modelBBMin[i] = m_modelBBMin[i];
	}

	glGenBuffers(1, &m_ModelVb);
	glBindBuffer(GL_ARRAY_BUFFER, m_ModelVb);
	int size = m_model->getCompiledVertexSize();
	glBufferData(GL_ARRAY_BUFFER,
		m_model->getCompiledVertexCount()*size*sizeof(float),
		m_model->getCompiledVertices(), GL_STATIC_READ);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_ModelIb);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ModelIb);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		m_model->getCompiledIndexCount()*sizeof(int),
		m_model->getCompiledIndices(), GL_STATIC_READ);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &optixVb);
	glBindBuffer(GL_ARRAY_BUFFER, optixVb);
	size = 9;
	glBufferData(GL_ARRAY_BUFFER,
		m_model->getCompiledVertexCount()*size*sizeof(float),
		m_model->getCompiledOptixVertices(), GL_STATIC_READ);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void MyGeometry::initGeometry()
{
	initGeometry(filename);	

}
void MyGeometry::drawQuad(glslShader& shader)
{
	shader.begin();
	glEnable(GL_TEXTURE_2D);
	glBegin( GL_QUADS);
	glTexCoord2f( 0.0f, 0.0f);
	glVertex2f( -1.0f, -1.0f);
	glTexCoord2f( 1.0f, 0.0f);
	glVertex2f( 1.0f,-1.0f);
	glTexCoord2f( 1.0f, 1.0f);
	glVertex2f( 1.0f, 1.0f);
	glTexCoord2f( 0.0f, 1.0f);
	glVertex2f( -1.0f, 1.0f);
	glEnd();
	shader.end();
}
void MyGeometry::drawQrad(CgShader& shader)
{
	//glDisable(GL_CULL_FACE);
	CGtechnique tech= shader.getTechnique();
	CGpass pass = cgGetFirstPass(tech);
	while(pass) 
	{
		cgSetPassState(pass);
		glBegin( GL_QUADS);
		glVertex2f( 0, 0);
		glVertex2f( 1.0, 0);
		glVertex2f( 1.0, 1.0);
		glVertex2f( 0, 1.0);	
		glEnd();
		cgResetPassState(pass);
		pass = cgGetNextPass(pass);
	}
}
void MyGeometry::initGeometry(GeoPara gp)
{
	assert(s_mat);
	shiness=gp.shiness;
	reflectValue = gp.reflectValue;
	initGeometry(gp.name);
	setMaterial(&s_mat[gp.matNumber]);
	actionNumber = gp.actionNum;
}
/*extern float delta;
extern GLuint blendFactorTex;
extern transform1 transformArray[];
extern action ActArray[];*/
void MyGeometry::drawGeometry(glslShader& shader,int actionNumber)
{
	CHECK_ERRORS();
	glBindBuffer(GL_ARRAY_BUFFER, m_ModelVb);
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(  m_model->getPositionSize(),
		GL_FLOAT,
		m_model->getCompiledVertexSize()*sizeof(float),
		(void*) (m_model->getCompiledPositionOffset()*sizeof(float)));

	if ( m_model->hasNormals() ) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(    GL_FLOAT,
			m_model->getCompiledVertexSize()*sizeof(float),
			(void*) (m_model->getCompiledNormalOffset()*sizeof(float)));
	}
	if (m_model->hasTexCoords()) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(   m_model->getPositionSize(),
			GL_FLOAT,
			m_model->getCompiledVertexSize()*sizeof(float),
			(void*) (m_model->getCompiledTexCoordOffset()*sizeof(float)));
	}*/
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_model->getCompiledVertexSize()*sizeof(float), (void*) (m_model->getCompiledPositionOffset()*sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, m_model->getCompiledVertexSize()*sizeof(float), (void*) (m_model->getCompiledNormalOffset()*sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, m_model->getCompiledVertexSize()*sizeof(float), (void*) (m_model->getCompiledTexCoordOffset()*sizeof(float)));

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ModelIb);

	
	glDrawElements( GL_TRIANGLES, m_model->getCompiledIndexCount(), GL_UNSIGNED_INT, 0 );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	glDisableVertexAttribArray(2);
	//glDisableClientState( GL_VERTEX_ARRAY );
	//glDisableClientState( GL_NORMAL_ARRAY );
	//glDisableClientState( GL_TEXTURE_COORD_ARRAY );


	CHECK_ERRORS();
}
void MyGeometry::drawGeometry(CGtechnique& tech,int actionNumber)
{
	
	glBindBuffer(GL_ARRAY_BUFFER, m_ModelVb);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glVertexPointer(  m_model->getPositionSize(),
		GL_FLOAT,
		m_model->getCompiledVertexSize()*sizeof(float),
		(void*) (m_model->getCompiledPositionOffset()*sizeof(float)));

	if ( m_model->hasNormals() ) 
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(    GL_FLOAT,
			m_model->getCompiledVertexSize()*sizeof(float),
			(void*) (m_model->getCompiledNormalOffset()*sizeof(float)));
	}
	if (m_model->hasTexCoords()) 
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(   m_model->getPositionSize(),
			GL_FLOAT,
			m_model->getCompiledVertexSize()*sizeof(float),
			(void*) (m_model->getCompiledTexCoordOffset()*sizeof(float)));
	}

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ModelIb);
	
	
	cgSetParameter1i(*MyGeometry::mspCgisTex, mp_met->hasTx());
	cgSetParameter1i(*MyGeometry::mspCgshiness, shiness);
	cgSetParameter1f(*MyGeometry::msgCgreflectVale, reflectValue);

	if(mp_met->hasTx())
	{
		cgGLSetTextureParameter(*MyGeometry::msgCgDiffuseTexParam, mp_met->getTxId());
	
	}
	else
	{
		float3 diffuse = mp_met->getDiffuse();
		cgSetParameter3fv(*MyGeometry::mspCgDiffuseColor,(float*)&diffuse);

	}
	CGpass pass = cgGetFirstPass(tech);
	while(pass) {
		cgSetPassState(pass);
		glDrawElements( GL_TRIANGLES, m_model->getCompiledIndexCount(), GL_UNSIGNED_INT, 0 );
		cgResetPassState(pass);
		pass = cgGetNextPass(pass);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );


	CHECK_ERRORS();
}



void MyGeometry::drawTex(GLuint mapId)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,mapId);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D); 
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

			glBegin( GL_QUADS);
				glTexCoord2f(0,0);
				glVertex2f( -1.0, -1.0);
				glTexCoord2f(1,0);
				glVertex2f( 1.0, -1.0);
				glTexCoord2f(1,1);
				glVertex2f( 1.0, 1.0);
				glTexCoord2f(0,1);
				glVertex2f( -1.0, 1.0);				
			glEnd();
		glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D,0);


	
	
}
optix::Geometry MyGeometry::getOptixGeometry()
{
	optix::Context    rtContext = MyGeometry::ms_rtContext;
	optix::Geometry geometry = rtContext->createGeometry();
	geometry ->setPrimitiveCount( m_model->getCompiledIndexCount()/3 );
	geometry ->setIntersectionProgram( rtContext->createProgramFromPTXFile( ptxpath("triangle_mesh_fat.cu"), "mesh_intersect" ) );
	geometry ->setBoundingBoxProgram( rtContext->createProgramFromPTXFile( ptxpath("triangle_mesh_fat.cu"), "mesh_bounds" ) );

	int num_vertices = m_model->getCompiledVertexCount();
	optix::Buffer vertex_buffer = rtContext->createBufferFromGLBO(RT_BUFFER_INPUT, optixVb);
	vertex_buffer->setFormat(RT_FORMAT_USER);
	vertex_buffer->setElementSize(3*3*sizeof(float));
	vertex_buffer->setSize(num_vertices);
	geometry ["vertex_buffer"]->setBuffer(vertex_buffer);

	optix::Buffer index_buffer = rtContext->createBufferFromGLBO(RT_BUFFER_INPUT, m_ModelIb);
	index_buffer->setFormat(RT_FORMAT_INT3);
	int indexCount = m_model->getCompiledIndexCount()/3;
	index_buffer->setSize(indexCount);
	geometry ["index_buffer"]->setBuffer(index_buffer);

	optix::Buffer material_buffer = rtContext->createBuffer(RT_BUFFER_INPUT);
	material_buffer->setFormat(RT_FORMAT_UNSIGNED_INT);
	material_buffer->setSize(m_model->getCompiledIndexCount()/3);
	void* material_data = material_buffer->map();
	memset(material_data, 0, m_model->getCompiledIndexCount()/3*sizeof(unsigned int));
	material_buffer->unmap();
	geometry ["material_buffer"]->setBuffer(material_buffer);
	return geometry;
	
}
MyGeometry::MyGeometry():mp_met(0),shiness(0)
{
	
}
optix::GeometryInstance MyGeometry::getInstance()
{
	optix::Context    rtContext = MyGeometry::ms_rtContext;
	optix::Geometry sofaGemetry = getOptixGeometry();
	optix::GeometryInstance instance = rtContext->createGeometryInstance();
	instance->setMaterialCount(1);
	instance->setMaterial(0, mp_met->getMaterial(this));
	instance->setGeometry(sofaGemetry);
	return instance;
}
void MyGeometry::setMaterial(MyMeterial *m)
{
	mp_met = m;
}
MyMeterial* MyGeometry::getMaterial() const
{
	return mp_met;
}
optix::TextureSampler MyGeometry::initMaterial()
{
	optix::Context    rtContext = MyGeometry::ms_rtContext;
	char *szPathName;
	GLuint texid;
	char fileName[] = "book.jpg";
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

	glGenTextures(1, &texid);                      // Create The Texture

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texid);                // Bind To The Texture ID
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
		tex_data[i] = make_float4(static_cast<float>(pixel_data[i].x)/255.00f,
			static_cast<float>(pixel_data[i].y)/255.00f,
			static_cast<float>(pixel_data[i].z)/255.00f,
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