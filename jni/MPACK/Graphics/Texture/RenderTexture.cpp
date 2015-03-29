#include "RenderTexture.hpp"

namespace MPACK
{
	namespace Graphics
	{
		RenderTexture::RenderTexture()
			: m_fboId(0), m_depthTex(NULL), m_colorTex(NULL)
		{
		}
		RenderTexture::~RenderTexture()
		{
			GL_CHECK( glDeleteFramebuffers(1,&m_fboId) );
			if(m_colorTex)
			{
				delete m_colorTex;
				m_colorTex = NULL;
			}
			if(m_depthTex)
			{
				delete m_depthTex;
				m_depthTex = NULL;
			}
		}
		void RenderTexture::Init(GLuint width, GLuint height)
		{
			GL_CHECK( glDeleteFramebuffers(1,&m_fboId) );
			if(m_colorTex)
			{
				GL_CHECK( glDeleteTextures(1,&m_colorTex->m_texId) );
			}
			else
			{
				m_colorTex=new Texture2D();
			}
			GL_CHECK( glGenTextures(1, &m_colorTex->m_texId) );
			GL_CHECK( glBindTexture(GL_TEXTURE_2D, m_colorTex->m_texId) );
			GL_CHECK( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
			GL_CHECK( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
			GL_CHECK( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
			GL_CHECK( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
			GL_CHECK( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
						 GL_RGBA, GL_UNSIGNED_BYTE, 0) );
			GL_CHECK( glBindTexture(GL_TEXTURE_2D, 0) );

			if(m_depthTex)
			{
				GL_CHECK( glDeleteTextures(1,&m_depthTex->m_texId) );
			}
			else
			{
				m_depthTex=new Texture2D();
			}
			/*
			GL_CHECK( glGenTextures(1, &m_depthTex->m_texId) );
			GL_CHECK( glBindTexture(GL_TEXTURE_2D, m_depthTex->m_texId) );
			GL_CHECK( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
			GL_CHECK( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
			GL_CHECK( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
			GL_CHECK( glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
			GL_CHECK( glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
						 GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL) );
			GL_CHECK( glBindTexture(GL_TEXTURE_2D, 0) );
			 */

			GLuint depthRenderbuffer;
			GL_CHECK( glGenRenderbuffers(1, &depthRenderbuffer) );
			GL_CHECK( glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer) );
			GL_CHECK( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height) );


			GL_CHECK( glGenFramebuffers(1, &m_fboId) );
			GL_CHECK( glBindFramebuffer(GL_FRAMEBUFFER, m_fboId) );

			GL_CHECK( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
									  GL_TEXTURE_2D, m_colorTex->m_texId, 0) );
			//GL_CHECK( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			//						  GL_TEXTURE_2D, m_depthTex->m_texId, 0) );

			GL_CHECK ( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer) );

			GL_CHECK( glBindFramebuffer(GL_FRAMEBUFFER, 0) );

			GLenum status;
			GL_CHECK( status = glCheckFramebufferStatus(GL_FRAMEBUFFER) );
			if ( status == GL_FRAMEBUFFER_COMPLETE )
			{
				LOGI("FBO successfully created!");
			}
			else
			{
				LOGE("ERROR: Failed to init FBO");
			}
		}
		void RenderTexture::Bind()
		{
			GL_CHECK( glBindFramebuffer(GL_FRAMEBUFFER, m_fboId) );
		}
		void RenderTexture::Unbind()
		{
			GL_CHECK( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
		}
	}
}
