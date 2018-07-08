#pragma once

#include <d3d9.h>
#include <d3d11.h>
#include <dxgi.h>

class D3DImageHelper
{
public:

	D3DImageHelper() : d3d(NULL), device(NULL), dx9_texture(NULL), dx9_surface(NULL), dx11_texture(NULL)
	{
		
	}
	
	~D3DImageHelper()
	{
		if (dx9_texture) {		
			dx9_texture->Release();
			dx9_surface->Release();
		}

		if (device) {
			device->Release();
			d3d->Release();
		}
	}

	bool Active() const // see somebody us using shared surfaces (AKA are we in wpf_image)
	{
		return dx9_surface != NULL;
	}

	IDirect3DSurface9 * Get_DX9Ex_Shared_Surface(HWND hwnd, ID3D11Texture2D * dx11_texture)
	{
		if (device == NULL) {

			HRESULT hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d);
			if (FAILED(hr)) {
				HE_ERROR(HEC_DX11_DRIVER, HES_CANNOT_OPEN_DEVICE, "Failed to create a Direct3D 9 Ex object.");
				return NULL;
			}

			D3DPRESENT_PARAMETERS d3dpp;
			ZeroMemory(&d3dpp, sizeof(d3dpp));
			d3dpp.Windowed = TRUE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

			DWORD behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

			hr = d3d->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, behaviorFlags, &d3dpp, NULL, &device);
			if (FAILED(hr)) {
				HE_ERROR(HEC_DX11_DRIVER, HES_CANNOT_OPEN_DEVICE, "Failed to create a Direct3D 9 Ex device.");
				return NULL;
			}
		}
		
		if (this->dx11_texture != dx11_texture) {

			this->dx11_texture = dx11_texture;		

			if (dx9_texture) {		
				dx9_texture->Release();
				dx9_texture = NULL;
				dx9_surface->Release();
				dx9_surface = NULL;
			}

			IDXGIResource * dxgi_resource = NULL;
			if (FAILED(dx11_texture->QueryInterface(__uuidof(IDXGIResource), (void**)&dxgi_resource)))
				return NULL;

			// TODO: In D3D 11.1 we're supposed to use IDXGIResource1::CreateSharedHandle instead and use NT handles.
			HANDLE shared_handle = NULL;
			if (FAILED(dxgi_resource->GetSharedHandle(&shared_handle))) {
				dxgi_resource->Release();
				return NULL;
			}

			dxgi_resource->Release();

			D3D11_TEXTURE2D_DESC desc;
			dx11_texture->GetDesc(&desc);

			if (FAILED(device->CreateTexture(desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET, DXGIToCrossAPID3D9Format(desc.Format), D3DPOOL_DEFAULT, &dx9_texture, &shared_handle)))
				return NULL;

			if (FAILED(dx9_texture->GetSurfaceLevel(0, &dx9_surface)))
				return NULL;
		}

		return dx9_surface;
	}

private:

	static D3DFORMAT DXGIToCrossAPID3D9Format(DXGI_FORMAT Format)
	{
		switch (Format)
		{
			case DXGI_FORMAT_B8G8R8A8_UNORM:
				return D3DFMT_A8R8G8B8;
			case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: 
				return D3DFMT_A8R8G8B8;
			case DXGI_FORMAT_B8G8R8X8_UNORM:
				return D3DFMT_X8R8G8B8;
			case DXGI_FORMAT_R8G8B8A8_UNORM: 
				return D3DFMT_A8B8G8R8;
			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: 
				return D3DFMT_A8B8G8R8;
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				return D3DFMT_A2B10G10R10;
			case DXGI_FORMAT_R16G16B16A16_FLOAT: 
				return D3DFMT_A16B16G16R16F;
			default:
				return D3DFMT_UNKNOWN;
		};
	}

	IDirect3D9Ex * d3d;
	IDirect3DDevice9Ex * device;	
	IDirect3DTexture9 * dx9_texture;
	IDirect3DSurface9 * dx9_surface;
	ID3D11Texture2D * dx11_texture;
};