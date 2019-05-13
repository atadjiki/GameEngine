#pragma once

#include <assert.h>

#include "..\_System\Device.h"
#include "..\_DDSLoader\DDSTextureLoader.h"

namespace GLib
{
	struct Texture
	{
		ID3D11Resource *			m_pTextureResource;
		ID3D11ShaderResourceView *	m_pShaderResource;

		Texture( ID3D11Resource * i_pTextureResource, ID3D11ShaderResourceView * i_pShaderResource ) :
			m_pTextureResource( i_pTextureResource ),
			m_pShaderResource( i_pShaderResource )
		{

		}
	};

	Texture * CreateTexture( void * i_pTextureData, size_t i_sizeTextureData );
	void Release( Texture * i_pTexture );
} // namespace GLib