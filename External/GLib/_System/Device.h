#pragma once

#include <Windows.h>
#include <d3d11_2.h>

namespace GLib
{
	extern D3D_DRIVER_TYPE			g_DriverType;
	extern D3D_FEATURE_LEVEL		g_FeatureLevel;
	extern ID3D11Device *			g_pD3DDevice;
	extern ID3D11Device1 *			g_pD3DDevice1;
	extern ID3D11DeviceContext *	g_pImmediateContext;
	extern ID3D11DeviceContext1 *	g_pImmediateContext1;
	extern IDXGISwapChain *			g_pSwapChain;
	extern IDXGISwapChain1 *		g_pSwapChain1;
	extern ID3D11RenderTargetView *	g_pRenderTargetView;
	extern ID3D11DepthStencilView *	g_pDepthStencilView;

	HRESULT				CreateDevice( HWND i_hWnd );
	void				ReleaseDevice();

	unsigned int		GetDisplayWidth();
	unsigned int		GetDisplayHeight();
} // namespace GLib