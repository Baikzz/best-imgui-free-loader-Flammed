#pragma once
#include "main.h"

inline ID3D11Device* g_pd3dDevice = nullptr;
inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
inline IDXGISwapChain* g_pSwapChain = nullptr;
inline UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
inline ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

const wchar_t project_name[] = L"FLAMMED";

inline int TAB = 0;
inline int subtab = 0;

inline char License[50];

inline float alphaColor = 0.f;

inline bool MAC;
inline bool Disk;
inline bool Clean;
inline bool Save;

inline float ButtonPos = window::size_max.y + 5;
inline float SelectablesPos = -335;
inline float ParametersPos = window::size_max.x + 340;

inline int selected = 0;

inline ImVec4 lbg2 = ImVec4(colors::bg.x, colors::bg.y, colors::bg.z, alphaColor);