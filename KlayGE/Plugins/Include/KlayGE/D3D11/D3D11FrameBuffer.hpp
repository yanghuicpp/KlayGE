// D3D11FrameBuffer.hpp
// KlayGE D3D11֡������ ͷ�ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2009
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ���ν��� (2009.1.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D11FRAMEBUFFER_HPP
#define _D3D11FRAMEBUFFER_HPP

#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/D3D11/D3D11Typedefs.hpp>

namespace KlayGE
{
	class D3D11FrameBuffer : public FrameBuffer
	{
	public:
		D3D11FrameBuffer();
		virtual ~D3D11FrameBuffer();

		ID3D11RenderTargetViewPtr D3DRTView(uint32_t n) const;
		ID3D11DepthStencilViewPtr D3DDSView() const;

		virtual std::wstring const & Description() const;

		virtual void OnBind();
		virtual void OnUnbind();

		bool RequiresFlipping() const
		{
			return true;
		}

		void Clear(uint32_t flags, Color const & clr, float depth, int32_t stencil);
	};

	typedef boost::shared_ptr<D3D11FrameBuffer> D3D11FrameBufferPtr;
}

#endif			// _D3D11RENDERTEXTURE_HPP