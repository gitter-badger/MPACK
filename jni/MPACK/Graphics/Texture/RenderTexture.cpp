#include "RenderTexture.hpp"

namespace MPACK
{
	namespace Graphics
	{
		RenderTexture::RenderTexture()
			: m_FBOId(0), m_colorTex(NULL), m_depthRBOId(0)
		{
		}

		RenderTexture::~RenderTexture()
		{
			if(m_FBOId)
			{
				GL_CHECK( glDeleteFramebuffers(1,&m_FBOId) );
			}
			if(m_colorTex)
			{
				delete m_colorTex;
				m_colorTex = NULL;
			}
			if(m_depthRBOId)
			{
				GL_CHECK( glDeleteRenderbuffers(1,&m_depthRBOId) );
			}
		}

		void RenderTexture::Init(GLuint width, GLuint height)
		{
			GL_CHECK( glDeleteFramebuffers(1,&m_FBOId) );
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

			if(m_depthRBOId)
			{
				GL_CHECK( glDeleteRenderbuffers(1,&m_depthRBOId) );
				m_depthRBOId=0;
			}
			GL_CHECK( glGenRenderbuffers(1, &m_depthRBOId) );
			GL_CHECK( glBindRenderbuffer(GL_RENDERBUFFER, m_depthRBOId) );
			GL_CHECK( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height) );


			GL_CHECK( glGenFramebuffers(1, &m_FBOId) );
			GL_CHECK( glBindFramebuffer(GL_FRAMEBUFFER, m_FBOId) );

			GL_CHECK( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
									  GL_TEXTURE_2D, m_colorTex->m_texId, 0) );
			GL_CHECK( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRBOId) );

			GL_CHECK( glBindFramebuffer(GL_FRAMEBUFFER, 0) );

			GLenum status;
			GL_CHECK( status = glCheckFramebufferStatus(GL_FRAMEBUFFER) );
			if ( status != GL_FRAMEBUFFER_COMPLETE )
			{
				LOGE("RenderTexture::Init(): Failed to init FBO");
			}
		}

		void RenderTexture::Bind()
		{
			GL_CHECK( glBindFramebuffer(GL_FRAMEBUFFER, m_FBOId) );
			GL_CHECK( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
		}

		void RenderTexture::Unbind()
		{
			GL_CHECK( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
		}
	}
}
