#include <d3d11.h>
#include <DirectXColors.h>

#include "..\_System\Device.h"

namespace GLib
{
bool BeginRendering( void )
{
	assert( g_pImmediateContext );
	assert( g_pSwapChain );

	// Just clear the backbuffer
	g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, DirectX::Colors::Transparent );

	if( g_pDepthStencilView )
		g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	return true;
}

void EndRendering( void )
{		
	HRESULT hResult = g_pSwapChain->Present( 0, 0 );
	assert( hResult == S_OK );
}

} // namespace GLib