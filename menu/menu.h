#pragma once
#include <imgui/imgui.h>
#include <iostream>
#include <D3DX11.h>

static class menu
{
public:
	static void draw();
	static bool custom_tab(const char* name, bool boolean);
};

enum tabs {
    aim_tab,
    visual_tab,
    misc_tab
};

inline static tabs tab_selected = aim_tab;

class fonts_
{
public:
    ImFont* medium;
    ImFont* semibold;
    ImFont* large;
    ImFont* fortnite;

    ImFont* logo;
};
inline std::unique_ptr<fonts_> fonts = std::make_unique<fonts_>();


namespace photos
{
    inline ID3D11ShaderResourceView* skeleton = nullptr;
    inline ID3D11ShaderResourceView* skeleton_box = nullptr;
    inline ID3D11ShaderResourceView* skeleton_filled_box = nullptr;
    inline ID3D11ShaderResourceView* box = nullptr;
    inline ID3D11ShaderResourceView* filled_box = nullptr;
};