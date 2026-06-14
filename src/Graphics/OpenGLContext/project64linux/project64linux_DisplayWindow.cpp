#include <GL/gl.h>

#include <Config.h>
#include <DisplayWindow.h>
#include <FrameBuffer.h>
#include <Graphics/Context.h>
#include <Graphics/OpenGLContext/GLFunctions.h>
#include <Graphics/OpenGLContext/ThreadedOpenGl/opengl_Wrapper.h>
#include <N64.h>

#include <cstdlib>

extern "C" void Project64LinuxSwapBuffers();

using namespace opengl;

class DisplayWindowProject64Linux : public DisplayWindow
{
private:
	bool _start() override;
	void _stop() override {}
	void _restart() override {}
	void _swapBuffers() override;
	void _saveScreenshot() override {}
	void _saveBufferContent(graphics::ObjectHandle /*_fbo*/, CachedTexture * /*_pTexture*/) override {}
	void _changeWindow() override {}
	bool _resizeWindow() override;
	void _readScreen(void **_pDest, long *_pWidth, long *_pHeight) override;
	void _readScreen2(void * _dest, int * _width, int * _height, int _front) override;
	graphics::ObjectHandle _getDefaultFramebuffer() override;
};

DisplayWindow & DisplayWindow::get()
{
	static DisplayWindowProject64Linux video;
	return video;
}

bool DisplayWindowProject64Linux::_start()
{
	FunctionWrapper::setThreadedMode(config.video.threadedVideo);
	if (!_resizeWindow())
		return false;
	initGLFunctions();
	return true;
}

void DisplayWindowProject64Linux::_swapBuffers()
{
	FunctionWrapper::WaitForSwapBuffersQueued();
	Project64LinuxSwapBuffers();
}

bool DisplayWindowProject64Linux::_resizeWindow()
{
	m_bFullscreen = false;
	m_screenWidth = config.video.windowedWidth;
	m_screenHeight = config.video.windowedHeight;
	m_heightOffset = 0;
	_setBufferSize();
	return true;
}

void DisplayWindowProject64Linux::_readScreen(void **_pDest, long *_pWidth, long *_pHeight)
{
	if (_pWidth != nullptr)
		*_pWidth = m_width;
	if (_pHeight != nullptr)
		*_pHeight = m_height;
	if (_pDest == nullptr)
		return;

	*_pDest = std::malloc(m_width * m_height * 3);
	if (*_pDest == nullptr)
		return;

	glReadBuffer(GL_FRONT);
	glReadPixels(0, m_heightOffset, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, *_pDest);
}

void DisplayWindowProject64Linux::_readScreen2(void * _dest, int * _width, int * _height, int _front)
{
	if (_width != nullptr)
		*_width = static_cast<int>(m_screenWidth);
	if (_height != nullptr)
		*_height = static_cast<int>(m_screenHeight);
	if (_dest == nullptr)
		return;

	glReadBuffer(_front != 0 ? GL_FRONT : GL_BACK);
	glReadPixels(0, m_heightOffset, m_screenWidth, m_screenHeight, GL_RGB, GL_UNSIGNED_BYTE, _dest);
}

graphics::ObjectHandle DisplayWindowProject64Linux::_getDefaultFramebuffer()
{
	return graphics::ObjectHandle::null;
}
