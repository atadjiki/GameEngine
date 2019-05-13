#include <assert.h>
#include <d3d11_1.h>

#include "DXHelpers.h"

//#define CREATE_DEBUG_DEVICE

namespace GLib
{
D3D_DRIVER_TYPE				g_DriverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL			g_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device *				g_pD3DDevice = nullptr;
ID3D11Device1 *				g_pD3DDevice1 = nullptr;
ID3D11DeviceContext *		g_pImmediateContext = nullptr;
ID3D11DeviceContext1 *		g_pImmediateContext1 = nullptr;
IDXGISwapChain *			g_pSwapChain = nullptr;
IDXGISwapChain1 *			g_pSwapChain1 = nullptr;
ID3D11RenderTargetView *	g_pRenderTargetView = nullptr;
ID3D11DepthStencilView *	g_pDepthStencilView = nullptr;

unsigned int				m_DisplayWidth;
unsigned int				m_DisplayHeight;

HRESULT CreateDevice( HWND i_hWnd )
{
	UINT createDeviceFlags = 0;
#ifdef CREATE_DEBUG_DEVICE
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hResult = S_OK;

	const D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	const size_t numDriverTypes = ARRAYSIZE( driverTypes );

	const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	const size_t numFeatureLevels = ARRAYSIZE( featureLevels );

	for( size_t indexDriverType = 0; indexDriverType < numDriverTypes; indexDriverType++ )
	{
		D3D_FEATURE_LEVEL featureLevel;

		hResult = D3D11CreateDevice( nullptr, driverTypes[indexDriverType], nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &g_pD3DDevice, &featureLevel, &g_pImmediateContext );

		if( hResult == E_INVALIDARG )
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hResult = D3D11CreateDevice( nullptr, driverTypes[indexDriverType], nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &g_pD3DDevice, &g_FeatureLevel, &g_pImmediateContext );
		}

		if( Succeeded( hResult ) )
		{
			g_DriverType = driverTypes[indexDriverType];
			break;
		}
	}

	if( !Succeeded( hResult ) )
		return hResult;

	IDXGIFactory1 * pDXGIFactory = nullptr;
	IDXGIDevice *	pDXGIDevice = nullptr;
	
	hResult = g_pD3DDevice->QueryInterface( __uuidof( IDXGIDevice ), reinterpret_cast<void**>( &pDXGIDevice ) );
	if( Succeeded( hResult ) )
	{
		IDXGIAdapter * pAdapter = nullptr;
			
		hResult = pDXGIDevice->GetAdapter( &pAdapter );
		if( Succeeded( hResult ) )
		{
			hResult = pAdapter->GetParent( __uuidof( IDXGIFactory1 ), reinterpret_cast<void**>( &pDXGIFactory ) );
			pAdapter->Release();
		}

		pDXGIDevice->Release();
	}

	if( !Succeeded( hResult ) )
		return hResult;

	RECT			rc;
	GetClientRect( i_hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// Create swap chain
	IDXGIFactory2 *	pDXGIFactory2 = nullptr;
	hResult = pDXGIFactory->QueryInterface( __uuidof( IDXGIFactory2 ), reinterpret_cast<void**>( &pDXGIFactory2 ) );
	if( pDXGIFactory2 )
	{
		// DirectX 11.1 or later
		hResult = g_pD3DDevice->QueryInterface( __uuidof( ID3D11Device1 ), reinterpret_cast<void**>( &g_pD3DDevice1 ) );
		if( Succeeded( hResult ) )
		{
			(void) g_pImmediateContext->QueryInterface( __uuidof( ID3D11DeviceContext1 ), reinterpret_cast<void**>( &g_pImmediateContext1 ) );
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hResult = pDXGIFactory2->CreateSwapChainForHwnd( g_pD3DDevice, i_hWnd, &sd, nullptr, nullptr, &g_pSwapChain1 );
		if( Succeeded( hResult ) )
		{
			hResult = g_pSwapChain1->QueryInterface( __uuidof( IDXGISwapChain ), reinterpret_cast<void**>( &g_pSwapChain ) );
		}

		pDXGIFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = i_hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hResult = pDXGIFactory->CreateSwapChain( g_pD3DDevice, &sd, &g_pSwapChain );
	}

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	if( Succeeded( hResult ) )
		hResult = pDXGIFactory->MakeWindowAssociation( i_hWnd, DXGI_MWA_NO_ALT_ENTER );
	
	pDXGIFactory->Release();

	if( !Succeeded( hResult ) )
		return hResult;

	// Create a render target view
	ID3D11Texture2D * pBackBuffer = nullptr;
	hResult = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
	if( !Succeeded( hResult ) )
		return hResult;

	hResult = g_pD3DDevice->CreateRenderTargetView( pBackBuffer, nullptr, &g_pRenderTargetView );
	pBackBuffer->Release();
	if( !Succeeded( hResult ) )
		return hResult;

	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width = width;
	dsd.Height = height;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.SampleDesc.Count = 1;
	dsd.SampleDesc.Quality = 0;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags = 0;
	dsd.MiscFlags = 0;
	
	ID3D11Texture2D *  pDepthStencil;
	
	hResult = g_pD3DDevice->CreateTexture2D( &dsd, nullptr, &pDepthStencil );
	assert( hResult == S_OK );

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	dsvd.Format = dsd.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	dsvd.Texture2D.MipSlice = 0;

	hResult = g_pD3DDevice->CreateDepthStencilView( pDepthStencil, &dsvd, &g_pDepthStencilView );
	assert( hResult == S_OK );
	pDepthStencil->Release();

	g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = FLOAT( width );
	vp.Height = FLOAT( height );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	g_pImmediateContext->RSSetViewports( 1, &vp );

	m_DisplayWidth = width;
	m_DisplayHeight = height;

	return S_OK;
}

unsigned int GetDisplayWidth()
{
	return m_DisplayWidth;
}
unsigned int GetDisplayHeight()
{
	return m_DisplayHeight;
}

void ReleaseDevice()
{
	if( g_pImmediateContext ) g_pImmediateContext->ClearState();

	if( g_pDepthStencilView )
	{
		ULONG refCount = g_pDepthStencilView->Release();
		assert( refCount == 0 );
	}

	if( g_pRenderTargetView )
	{
		ULONG refCount = g_pRenderTargetView->Release();
		assert( refCount == 0 );

		g_pRenderTargetView = nullptr;
	}

	if( g_pImmediateContext )
	{
		g_pImmediateContext->Release();
		g_pImmediateContext = nullptr;
	}

	if( g_pImmediateContext1 )
	{
		ULONG refCount = g_pImmediateContext1->Release();
		assert( refCount == 0 );

		g_pImmediateContext1 = nullptr;
	}

	if( g_pSwapChain )
	{
		g_pSwapChain->Release();
		g_pSwapChain = nullptr;
	}

	if( g_pSwapChain1 )
	{
		ULONG refCount = g_pSwapChain1->Release();
		assert( refCount == 0 );

		g_pSwapChain1 = nullptr;
	}

#if defined( REPORT_LIVE_DXOBJECTS )
	ID3D11Debug * pDebug = nullptr;
	HRESULT hResult = g_pD3DDevice->QueryInterface( __uuidof( ID3D11Debug ), reinterpret_cast<void**>( &pDebug ) );
	if( ( hResult == S_OK ) && ( pDebug ) )
	{
		hResult = pDebug->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
		pDebug->Release();
	}
#endif // REPORT_LIVE_OBJECTS

	if( g_pD3DDevice )
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = nullptr;
	}

	if( g_pD3DDevice1 )
	{
		ULONG refCount = g_pD3DDevice1->Release();
		assert( refCount == 0 );

		g_pD3DDevice1 = nullptr;
	}

}

} // namespace GLib
