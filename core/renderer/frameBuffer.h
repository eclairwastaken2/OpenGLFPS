#pragma once
#include <cstdint>

class FrameBuffer
{
public:
	FrameBuffer(uint32_t width, uint32_t height); 
	~FrameBuffer(); 

	void bind(); 
	void unbind(); 

private:
	int32_t m_FBO = 0; 
	int32_t m_Color = 0; 
	int32_t m_Depth = 0;
};