// D3D11AdapterList.hpp
// KlayGE D3D11�������б� ͷ�ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2009
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// ���ν��� (2009.1.30)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D11ADAPTERLIST_HPP
#define _D3D11ADAPTERLIST_HPP

#include <KlayGE/PreDeclare.hpp>
#include <boost/smart_ptr.hpp>
#include <KlayGE/D3D11/D3D11Adapter.hpp>

#include <KlayGE/D3D11/D3D11MinGWDefs.hpp>
#include <d3d11.h>

namespace KlayGE
{
	class D3D11AdapterList
	{
	public:
		D3D11AdapterList();

		void Enumerate(IDXGIFactoryPtr const & gi_factory);

		size_t NumAdapter() const;
		D3D11AdapterPtr const & Adapter(size_t index) const;

		uint32_t CurrentAdapterIndex() const;
		void CurrentAdapterIndex(uint32_t index);

	private:
		std::vector<D3D11AdapterPtr> adapters_;
		uint32_t			current_adapter_;
	};

	typedef boost::shared_ptr<D3D11AdapterList> D3D11AdapterListPtr;
}

#endif			// _D3D11ADAPTERLIST_HPP