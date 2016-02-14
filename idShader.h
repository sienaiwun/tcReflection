#include "glslShader.h"
#ifndef IDSHADER_H
#define IDSHADER_H
class IDShader:public glslShader
{
public :
	IDShader()
	{
		m_vertexFileName = "Shader/idShader.vert";
		m_geometryFileName = "Shader/idShader.geom";
		m_fragmentFileName = "Shader/idShader.frag";
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
	float* m_mvp;
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