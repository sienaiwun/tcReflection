#include "glslShader.h"
#ifndef GBUFFERSHADER_H
#define GBUFFERSHADER_H
class GbufferShader:public glslShader
{
public :
	GbufferShader()
	{
		m_vertexFileName = "Shader/gBuffer.vert";
		m_fragmentFileName = "Shader/gBuffer.frag";
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
    virtual void setCamera(CCamera * pCamera);
	virtual void setScene(scene * pScene);
	virtual void setGeometry(MyGeometry * pGeometry);
	virtual void setGeomtryIndex(int i);
private:
	
	GLuint m_vmpBinding;
	GLuint m_modelViewBinding;
	float* m_mvp;
	float* m_modelView;
	GLuint m_objectTexBinding;
	GLuint m_objectDiffuseBinding;
	GLuint m_cameraPosBinding;
	GLuint m_lightPosBinding;
	GLuint m_hasTex;
	GLuint m_objectId;
	GLuint m_reflectFactor;
	
	nv::vec3f m_lightPos;
	nv::vec3f m_cameraPos;

	
};
#endif