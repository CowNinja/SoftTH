/*
SoftTH, Software multihead solution for Direct3D
Copyright (C) 2005-2012 Keijo Ruotsalainen, www.kegetys.fi
              2014-     C. Justin Ratcliff, www.softth.net

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//
// Main DLL loader
//

#include <windows.h>
#include <stdio.h>
//#include <d3d10_1.h>
#include "d3d10main.h"


// SoftTH main dll import function/variable prototypes
void (__stdcall * dbg)(char *first, ...) = NULL;
void (__stdcall * ShowMessage)(char *first, ...) = NULL;
//void (__stdcall * addNoHookModule)(HMODULE mod) = NULL;
DLL configFile config; // Main configuration
DLL HINSTANCE hLibD3D11;


/* D3D 10 real function prototypes */
/*HRESULT (WINAPI*dllD3D10CreateDevice)(IDXGIAdapter *adapter,
                                      D3D10_DRIVER_TYPE DriverType,
                                      HMODULE Software,
                                      UINT Flags,
                                      UINT SDKVersion,
                                      ID3D10Device** ppDevice) = NULL;

extern "C" __declspec(dllexport) HRESULT (WINAPI*dllD3D10CreateDeviceAndSwapChain)(IDXGIAdapter *adapter,
                                                                                   D3D10_DRIVER_TYPE DriverType,
                                                                                   HMODULE Software,
                                                                                   UINT Flags,
                                                                                   UINT SDKVersion,
                                                                                   DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
                                                                                   IDXGISwapChain **ppSwapChain,
                                                                                   ID3D10Device **ppDevice) = NULL;
*/




BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpReserved)
{

  SoftTHMod = NULL;
  //D3D10Mod = NULL;
  hLibSoftTH = NULL; // Main SoftTH dll (dxgi.dll)
  //hLibD3D10 = NULL; // Real d3d10.dll

  switch (reason)
  {
    case DLL_PROCESS_ATTACH:
    {
      bool hooks = true;

      /* Load main SoftTH library */
      {
        // Load the library
        SoftTHMod = new Module;
        if(SoftTHMod->SetHandle(".\\dxgi.dll"))
          hLibSoftTH = SoftTHMod->GetHandle();
        else
          ShowMessage("Main SoftTH library not found (dxgi.dll)!"), exit(0);

        // Capture functions from main SoftTH library
        dbg = (void(__stdcall *)(char*,...)) GetProcAddress(hLibSoftTH,"dbg");
        ShowMessage = (void(__stdcall *)(char*,...)) GetProcAddress(hLibSoftTH,"ShowMessage");
        //addNoHookModule = (void(__stdcall *)(HMODULE)) GetProcAddress(hLibSoftTH,"addNoHookModule");

        dbg("d3d10: Main SoftTH functions captured.");
      }

      /* Load D3D10 Library */
      {
        /*char path[256];
        if(strlen(config.main.dllPathD3D10) < 2)
          sprintf(path, "%s\\system32\\%s", getenv("SystemRoot"), "d3d10.dll");
        else
          strcpy(path, config.main.dllPathD3D10);
        dbg("D3D10 DLL Path: <%s>", path);

        D3D10Mod = new Module;
        if(D3D10Mod->SetHandle(path))
          hLibD3D10 = D3D10Mod->GetHandle();
        //hLibD3D10 = LoadLibrary(path);

	      if(!hLibD3D10)
		      ShowMessage("D3D10 DLL not found!\n'%s'", path), exit(0);

        dllD3D10CreateDevice = (HRESULT(__stdcall *)(IDXGIAdapter*,
                                                     D3D10_DRIVER_TYPE,
                                                     HMODULE,
                                                     UINT,
                                                     UINT,
                                                     ID3D10Device**))
                                                     GetProcAddress(hLibD3D10, "D3D10CreateDevice");

        dllD3D10CreateDeviceAndSwapChain = (HRESULT(__stdcall *)(IDXGIAdapter*,
                                                                 D3D10_DRIVER_TYPE,
                                                                 HMODULE,
                                                                 UINT,
                                                                 UINT,
                                                                 DXGI_SWAP_CHAIN_DESC *,
                                                                 IDXGISwapChain **,
                                                                 ID3D10Device **))
                                                                 GetProcAddress(hLibD3D10, "D3D10CreateDeviceAndSwapChain");

        if(!dllD3D10CreateDevice)
		      ShowMessage("D3D10CreateDevice not in DLL!\nWindows 7 or newer required!\n'%s'", path), exit(0);
        if(!dllD3D10CreateDeviceAndSwapChain)
		      ShowMessage("D3D10CreateDeviceAndSwapChain not in DLL!\nWindows 7 or newer required!\n'%s'", path), exit(0);*/
      }

      if(hooks) {
        // Pin our DLL - cannot allow unloading since hook code is stored by us
        char fn[256];
        GetModuleFileName(hModule, fn, 256);
        HMODULE foo;
        HRESULT ret = GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN, fn, &foo);
        if(!ret)
          dbg("Failed to pin DLL: error %d", GetLastError());
        else
          dbg("Pinned DLL: <%s>", fn);

        /*if(hLibSoftTH) addNoHookModule(hLibSoftTH);*/
        /*if(hLibD3D10) addNoHookModule(hLibD3D10);*/
      }

      break;
    }

    case DLL_PROCESS_DETACH:
    {

			/*if(hLibD3D10) {
				FreeLibrary(hLibD3D10);
				hLibD3D10 = NULL;
			}*/
      if(hLibSoftTH) {
        FreeLibrary(hLibSoftTH);
        hLibSoftTH = NULL;
      }

			/*if(D3D10Mod)
      {
        D3D10Mod->Release();
        delete D3D10Mod;
        D3D10Mod = NULL;
      }*/
      if(SoftTHMod)
      {
        SoftTHMod->Release();
        delete SoftTHMod;
        SoftTHMod = NULL;
      }

      break;
    }
  }
  return true;
}

/*    Direct3D 10     */
////D3D10CreateDevice
//extern "C" _declspec(dllexport) HRESULT WINAPI newD3D10CreateDevice(IDXGIAdapter *adapter,
//                                                                    D3D10_DRIVER_TYPE DriverType,
//                                                                    HMODULE Software,
//                                                                    UINT Flags,
//                                                                    UINT SDKVersion,
//                                                                    ID3D10Device** ppDevice)
//{
//  dbg("d3d10: D3D10CreateDevice 0x%08X 0x%08X", adapter, *adapter);
//
//  HRESULT ret = dllD3D10CreateDevice(adapter, DriverType, Software, Flags, SDKVersion, ppDevice);
//
//  /*IDXGIAdapter1New *anew;
//  if(adapter->QueryInterface(IID_IDXGIAdapter1New, (void**) &anew) == S_OK) {
//    adapter = anew->getReal();
//    anew->Release();
//  }*/
//
//  return ret;
//}
//
////D3D10CreateDeviceAndSwapChain
//extern "C" _declspec(dllexport) HRESULT WINAPI newD3D10CreateDeviceAndSwapChain(IDXGIAdapter *adapter,
//                                                                                D3D10_DRIVER_TYPE DriverType,
//                                                                                HMODULE Software,
//                                                                                UINT Flags,
//                                                                                UINT SDKVersion,
//                                                                                DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
//                                                                                IDXGISwapChain **ppSwapChain,
//                                                                                ID3D10Device **ppDevice)
//{
//  dbg("d3d10: D3D10CreateDeviceAndSwapChain 0x%08X 0x%08X", adapter, *adapter);
//
//  /*dbg("Mode: %dx%d %d.%dHz %s", pSwapChainDesc->BufferDesc.Width, pSwapChainDesc->BufferDesc.Height, pSwapChainDesc->BufferDesc.RefreshRate.Numerator, pSwapChainDesc->BufferDesc.RefreshRate.Denominator, pSwapChainDesc->Windowed?"Windowed":"Fullscreen");
//  dbg("Multisample: %d samples, quality %d", pSwapChainDesc->SampleDesc.Count, pSwapChainDesc->SampleDesc.Quality);
//  dbg("Buffers: %d (Usage %s), Swapeffect: %s", pSwapChainDesc->BufferCount, getUsageDXGI(pSwapChainDesc->BufferUsage), pSwapChainDesc->SwapEffect==DXGI_SWAP_EFFECT_DISCARD?"DISCARD":"SEQUENTIAL");
//
//  dbg("Flags: %s %s %s", pSwapChainDesc->Flags&DXGI_SWAP_CHAIN_FLAG_NONPREROTATED?"NONPREROTATED":"",
//                         pSwapChainDesc->Flags&DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH?"ALLOW_MODE_SWITCH":"",
//                         pSwapChainDesc->Flags&DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE?"GDI_COMPATIBLE":"");*/
//
//  HRESULT ret;
//
//  /*ret = D3D10CreateDevice(adapter,
//                          D3D10_DRIVER_TYPE_HARDWARE,
//                          NULL,
//                          0,
//                          D3D10_SDK_VERSION,
//                          ppDevice
//                          );*/
//
//  ret = D3D10CreateDevice(adapter,
//                          DriverType,
//                          Software,
//                          Flags,
//                          SDKVersion,
//                          ppDevice
//                          );
//
//  /*IDXGIAdapter1New *anew;
//  if(adapter->QueryInterface(IID_IDXGIAdapter, (void**) &anew) == S_OK) {
//    adapter = anew->getReal();
//    anew->Release();
//  }*/
//
//  IDXGIFactory1 *factory;
//  //IDXGIFactory1New *fnew;
//  //CreateDXGIFactory1()
//  if(adapter->GetParent(IID_IDXGIFactory, (void**) &factory) == S_OK)
//  {
//    //factory = fnew->getReal();
//    //fnew->Release();
//    dbg("d3d10: Got parent factory");
//  }
//
//  //ret =  dllD3D10CreateDeviceAndSwapChain(anew, DriverType, Software, Flags, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice);
//
//  (*ppSwapChain) = new IDXGISwapChainNew(factory, factory, *ppDevice, pSwapChainDesc);
//
//  /*IDXGISwapChainNew *scnew;
//  if((*ppSwapChain)->QueryInterface(IID_IDXGISwapChainNew, (void**) &scnew) == S_OK) {
//    (*ppSwapChain) = scnew->getReal();
//    scnew->Release();
//  } else dbg("Booh! No real swap chain!");*/
//
//  /*if(fnew)
//  {
//    fnew->Release();
//    delete fnew;
//    fnew = NULL;
//  }*/
//
//  return ret;
//}



#ifndef _WIN64
//#if 1

#ifdef _WIN64
#define STACK_BASE rbp // x64: rbp is stack base
#else
#define STACK_BASE ebp // x86: ebp is stack base
#endif

// Passthrough exports
#pragma warning (disable : 4731)
#define DEXPORT(hLib, x) _declspec(dllexport) void x(void) {\
	static FARPROC foo = GetProcAddress((HMODULE) hLib, #x);\
	__asm {pop STACK_BASE};\
	__asm {jmp foo};\
	}

#define DEXPORTD(hLib, x) _declspec(dllexport) void x(void) {\
	static FARPROC foo = GetProcAddress((HMODULE) hLib, #x);\
	if (!foo) { dbg("Error getting address (Code: %d)", GetLastError()); }\
  GetModuleFileName((HMODULE) hLib,libfn,256);\
  dbg("DEXPORTD: %s, %s, 0x%08X - %s, 0x%08X", #hLib, libfn, &hLib, #x, foo);\
	__asm {pop STACK_BASE};\
	__asm {jmp foo};\
	}

extern "C" {
/* Direct3D 10 */
DEXPORTD(hLibD3D10, D3D10CompileEffectFromMemory         );
DEXPORTD(hLibD3D10, D3D10CreateBlob                      );
//DEXPORTD(hLibD3D10, D3D10CreateDevice                    );
//DEXPORTD(hLibD3D10, D3D10CreateDeviceAndSwapChain        );
DEXPORTD(hLibSoftTH, D3D10CreateDeviceAndSwapChain        );
DEXPORTD(hLibD3D10, D3D10CreateEffectFromMemory          );
DEXPORTD(hLibD3D10, D3D10CreateEffectPoolFromMemory      );
DEXPORTD(hLibD3D10, D3D10CreateStateBlock                );
DEXPORTD(hLibD3D10, D3D10DisassembleEffect               );
DEXPORTD(hLibD3D10, D3D10DisassembleShader               );
DEXPORTD(hLibD3D10, D3D10GetGeometryShaderProfile        );
DEXPORTD(hLibD3D10, D3D10GetInputAndOutputSignatureBlob  );
DEXPORTD(hLibD3D10, D3D10GetInputSignatureBlob           );
DEXPORTD(hLibD3D10, D3D10GetOutputSignatureBlob          );
DEXPORTD(hLibD3D10, D3D10GetPixelShaderProfile           );
DEXPORTD(hLibD3D10, D3D10GetShaderDebugInfo              );
DEXPORTD(hLibD3D10, D3D10GetVersion                      );
DEXPORTD(hLibD3D10, D3D10GetVertexShaderProfile          );
DEXPORTD(hLibD3D10, D3D10PreprocessShader                );
DEXPORTD(hLibD3D10, D3D10ReflectShader                   );
DEXPORTD(hLibD3D10, D3D10RegisterLayers                  );
DEXPORTD(hLibD3D10, D3D10StateBlockMaskDifference        );
DEXPORTD(hLibD3D10, D3D10StateBlockMaskDisableAll        );
DEXPORTD(hLibD3D10, D3D10StateBlockMaskDisableCapture    );
DEXPORTD(hLibD3D10, D3D10StateBlockMaskEnableAll         );
DEXPORTD(hLibD3D10, D3D10StateBlockMaskEnableCapture     );
DEXPORTD(hLibD3D10, D3D10StateBlockMaskGetSetting        );
DEXPORTD(hLibD3D10, D3D10StateBlockMaskIntersect         );
DEXPORTD(hLibD3D10, D3D10StateBlockMaskUnion             );
DEXPORTD(hLibD3D10, RevertToOldImplementation            );
}
#pragma warning (default : 4731)

#endif
