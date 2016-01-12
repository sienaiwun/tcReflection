#include "glslShader.h"
#ifndef REFLECTIONSHADER_H
#define REFLECTIONSHADER_H
class reflectShader:public glslShader
{
public :
	reflectShader()
	{
		m_vertexFileName = "Shader/gBuffer.vert";
		m_fragmentFileName = "Shader/reflection.frag";
	}
	virtual void init();
	virtual void bindParemeter();
	virtual void begin();
	virtual void end();
	inline void setParemeter(GLuint reflectTex,GLuint worldPosTex,GLuint lastWorld,const float* lastmvp)
	{
		
	}
	virtual void setCamera(CCamera * pCamera);
	virtual void setScene(scene * pScene);
	virtual void setGeometry(MyGeometry * pGeometry);
	virtual void setGeomtryIndex(int i);
	void setReflectMap(GLuint reflectMap);
	void setWindowSize(int w,int h);
private:
	nv::vec2f m_windowSize;
	GLuint m_vmpBinding;
	float* m_mvp;
	GLuint m_objectTexBinding;
	GLuint m_objectDiffuseBinding;
	GLuint m_cameraPosBinding;
	GLuint m_lightPosBinding;
	GLuint m_hasTex;
	GLuint m_objectId;

	GLuint m_reflectValueBinding;
	GLuint m_reflectMapBinding;
	GLuint m_windowsSizeSlot;
	GLuint m_reflectionMap;

};
#endif