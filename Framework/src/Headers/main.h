#pragma once
#include <d3d11.h>
#include <windows.h>
#include <dwmapi.h>
#include <string>
#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


#define size_of IM_ARRAYSIZE

inline HWND hwnd;
inline RECT rc;

namespace images {
	inline ID3D11ShaderResourceView* valo = nullptr;
	inline ID3D11ShaderResourceView* rust = nullptr;
	inline ID3D11ShaderResourceView* fn = nullptr;
	inline ID3D11ShaderResourceView* eft = nullptr;
	inline ID3D11ShaderResourceView* mw = nullptr;
	inline ID3D11ShaderResourceView* circle = nullptr;
}

namespace window {
	const ImVec2 size_max = { 625, 425 };
	const float rounding = 6.f;
}

namespace fonts {
	inline ImFont* Inter_Regular = nullptr;
	inline ImFont* Sansation_Light = nullptr;
	inline ImFont* Sansation_Regular = nullptr;
	inline ImFont* Sansation_Bold = nullptr;
	inline ImFont* FontAwesome = nullptr;
}

namespace items {
	const float rounding = 2.f;
}

class Framework {
public:
	class ui {
	public:
		void Render();
		void BeforeLoop();
	};

	class GUI
	{
	public:
		void begin(const char* name, ImVec2 size);
		void end();
		void ChildBegin(const char* name, float posX, float posY, float sizeX, float sizeY);
		void ChildEnd();
		void mw();
		void style();
		void Blur(HWND hwnd);
		void AddImage(ID3D11ShaderResourceView* pic, float posX, float posY, float sizeX, float sizeY);
		void AddImageRotated(ID3D11ShaderResourceView* pic, float posX, float posY, float sizeX, float sizeY);
		void pf(ImFont* font);
		void ApplyImage(int i);
	};

	class CUSTOM
	{
	public:
		bool Button(const char* label, const char* text, float posX, float posY, float sizeX, float sizeY);
		bool Tab(const char* label, const char* text, const char* icon, float posX, float posY, float sizeX, float sizeY, bool v);
		bool SelectableItem(const char* label, ID3D11ShaderResourceView* pic, float posX, float posY, float sizeX, float sizeY, bool v);
		bool Checkbox2(const char* label, const char* text, float posX, float posY, bool* v, float space);
		bool InputText(const char* label, const char* hint, float posX, float posY, float sizeX, static char buf[], size_t buf_size, ImGuiInputTextFlags flag);

		void AddBorders(ImVec2 pos);
		bool IconButton(const char* label, const char* icon, float posX, float posY, int action);
		void Text(float posX, float posY, ImFont* font, float size, const char* text, const ImVec4 col);
		void ShadowText(float posX, float posY, ImFont* fofnt, float size, const char* text, ImVec4 col, ImVec4 shadow_col);
	};
};

namespace colors {
	const ImVec4 bg = { 0.059f, 0.059f, 0.059f, .8f };
	const ImVec4 lbg = { 0.086f, 0.086f, 0.086f, .95f };

	const ImVec4 SecondColor = { 1.0f, 0.384f, 0.016f, .8f };
	const ImVec4 MainColor = { SecondColor.x, SecondColor.y, SecondColor.z, SecondColor.w * 3 };
	const ImVec4 ItemBgColor = { 0.1176f, 0.1176f, 0.1176f, 1.0f };

	const ImVec4 White = { 1, 1, 1, 1 };
	const ImVec4 lwhite = { 1, 1, 1, 0.8 };
	const ImVec4 Gray = { 0.235f, 0.235f, 0.235f, 1 };
	const ImVec4 DarkGray = { 0.137, 0.137, 0.137, 1 };
	const ImVec4 Green = { 0.247,1.0,0.247,1.0 };
	const ImVec4 Red = { 0.6824, 0.1608, 0.1608, 1.0 };
	const ImVec4 Orange = { 0.7451f, 0.6118f, 0.2471f, 1.0f };
}