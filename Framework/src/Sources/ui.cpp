#include "main.h"
#include "settings.h"
#include "font_awesome.h"
#include "fonts.h"
#include "images.h"
#include "particles.h"

#include <algorithm>

#include "auth.hpp"
#include "skStr.h"

Framework::CUSTOM c;
Framework::GUI g;

using namespace KeyAuth;

//FILL THIS UP WITH YOUR CREDENTIALS

std::string name = skCrypt("").decrypt(); // Application Name
std::string ownerid = skCrypt("").decrypt(); // Owner ID
std::string secret = skCrypt("").decrypt(); // Application Secret
std::string version = skCrypt("").decrypt();; // Application Version
std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting
std::string path = skCrypt("").decrypt(); // (OPTIONAL) see tutorial here https://www.youtube.com/watch?v=I9rxt821gMk&t=1s

api KeyAuthApp(name, ownerid, secret, version, url, path);

void Framework::ui::BeforeLoop()
{
    //name.clear(); ownerid.clear(); secret.clear(); version.clear(); url.clear();
    //KeyAuthApp.init();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true; icons_config.OversampleH = 3; icons_config.OversampleV = 3;

    if (images::valo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, valo_p, sizeof(valo_p), nullptr, nullptr, &images::valo, 0);
    if (images::rust == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, rust_p, sizeof(rust_p), nullptr, nullptr, &images::rust, 0);
    if (images::fn == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, fn_p, sizeof(fn_p), nullptr, nullptr, &images::fn, 0);
    if (images::eft == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, EFT_p, sizeof(EFT_p), nullptr, nullptr, &images::eft, 0);
    if (images::mw == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, mw_p, sizeof(mw_p), nullptr, nullptr, &images::mw, 0);
    if (images::circle == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, circle_p, sizeof(circle_p), nullptr, nullptr, &images::circle, 0);
    
    if (fonts::Inter_Regular == nullptr) fonts::Inter_Regular = io.Fonts->AddFontFromMemoryTTF(inter_regular_p, sizeof(inter_regular_p), 16);
    if (fonts::Sansation_Light== nullptr) fonts::Sansation_Light = io.Fonts->AddFontFromMemoryTTF(sansation_light_p, sizeof(sansation_light_p), 13);
    if (fonts::Sansation_Regular == nullptr) fonts::Sansation_Regular = io.Fonts->AddFontFromMemoryTTF(sansation_regular_p, sizeof(sansation_regular_p), 25);
    if (fonts::Sansation_Bold == nullptr) fonts::Sansation_Bold = io.Fonts->AddFontFromMemoryTTF(sansation_bold_p, sizeof(sansation_bold_p), 28);
    if (fonts::FontAwesome == nullptr) fonts::FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);

    InitializeParticles();
    g.Blur(hwnd);

}

void Framework::ui::Render()
{
    g.begin("Main Window", window::size_max);
    {
        UpdateParticles(0.03);
        RenderParticles();

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& s = ImGui::GetStyle();
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        float time = io.DeltaTime * 5;
        static float timer = io.DeltaTime;

        c.IconButton("###Close", ICON_FA_XMARK, window::size_max.x - 20, 18 / 2, 1);
        c.Text(15, 22, fonts::Sansation_Regular, 24, "FLAMMED", colors::White);
        c.Text(125, 28, fonts::Sansation_Light, 15, "LITE", colors::MainColor);
        c.Text(window::size_max.x - 120, window::size_max.y - 30, fonts::Inter_Regular, 15, "Made by Baikzz #0", colors::White);
        s.Alpha = alphaColor;

        if (TAB == 0)
        {
            alphaColor = std::clamp(alphaColor + (1.f * ImGui::GetIO().DeltaTime * 1.f), 0.0f, 1.f);
            c.ShadowText(190, 140, fonts::Sansation_Bold, 100, "FLAMMED", colors::White, colors::MainColor);
            c.ShadowText(320, 140, fonts::Sansation_Bold, 65, "V3", colors::MainColor, colors::MainColor);
            c.ShadowText(360, 140, fonts::Sansation_Bold, 100, "LOGIN", colors::White, colors::MainColor);

            c.InputText("KEY", "LICENSE KEY", 190, 210, 250, License, 50, 0);
            if (c.Button("Login", "Sign in", 190, 260, 250, 30)) 
            { 
                //KeyAuthApp.license(License);
                /*if (KeyAuthApp.response.success)*/ { TAB = 1; alphaColor = 0; }
            }
        }

        if (TAB == 1)
        {
            if (timer <= 5) { timer += 0.5; return; }

            alphaColor = std::clamp(alphaColor + (1.f * ImGui::GetIO().DeltaTime * 1.f), 0.0f, 1.f);
            window->DrawList->AddLine({ 165, 28 }, { 165, 42 }, ImGui::GetColorU32(colors::White));

            if (c.Tab("HOME", "Home", NULL, 180, 28, ImGui::CalcTextSize("Home").x, ImGui::CalcTextSize("Home").y, subtab == 0)) { subtab = 0; }

            if (subtab == 0)
            {
                SelectablesPos = ImLerp(SelectablesPos, 15.f, time);
                ButtonPos = ImLerp(ButtonPos, 245.f, time);
                ParametersPos = ImLerp(ParametersPos, window::size_max.x - 15, time);

                c.Text(SelectablesPos, 87, fonts::Inter_Regular, 16, "Select a game", colors::White);
                c.Text(ParametersPos - 272, 87, fonts::Inter_Regular, 16, "Optional", colors::White);

                g.ChildBegin("Selectable items", SelectablesPos, 106, 315, 300);
                {
                    if (c.SelectableItem("Valo", images::valo, 12.5, 15, 290, 80, selected == 1)) { selected = 1; }
                    if (c.SelectableItem("Rust", images::rust, 12.5, 110, 290, 80, selected == 2)) { selected = 2; }
                    if (c.SelectableItem("FN", images::fn, 12.5, 205, 290, 80, selected == 3)) { selected = 3; }
                    if (c.SelectableItem("EFT", images::eft, 12.5, 300, 290, 80, selected == 4)) { selected = 4; }
                    if (c.SelectableItem("MW", images::mw, 12.5, 395, 290, 80, selected == 5)) { selected = 5; }
                    ImGui::Spacing();
                }
                g.ChildEnd();

                g.ChildBegin("Parameters", ParametersPos - 272, 105, 275, 115);
                {
                    c.Checkbox2("MAC", "Mac Changer", 235, 15, &MAC, 190);
                    c.Checkbox2("Cleaner", "Cleaner", 235, 40, &Clean, 208);
                    c.Checkbox2("Disk", "Disk Spoof", 235, 65, &Disk, 198);
                    c.Checkbox2("Save", "Save Config", 235, 90, &Save, 195);
                }
                g.ChildEnd();

                if (c.Button("Spoof", "Spoof", 340, ButtonPos, 270, 30)) { subtab = 5; alphaColor = 0; }
                if (c.Button("SpoofASUS", "Asus Spoof", 340, ButtonPos + 37, 132, 30)) selected = 0;
                if (c.Button("TEMPSpoof", "Temp Spoof", 478, ButtonPos + 37, 132, 30)) selected = 0;
            }

            if (subtab == 5)
            {
                alphaColor = std::clamp(alphaColor + (1.f * ImGui::GetIO().DeltaTime * 1.f), 0.0f, 1.f);

                c.Text((window::size_max.x - ImGui::CalcTextSize("Spoofig").x) / 2, (window::size_max.y - ImGui::CalcTextSize("Spoofig").y) / 2, fonts::Inter_Regular, 17, "Spoofing", colors::White);
                g.AddImageRotated(images::circle, (window::size_max.x - 20) / 2, (window::size_max.y - 35) * 0.6, 35, 35);

                timer += 0.1;
                if (timer > 100) { timer = 0; alphaColor = 0; ButtonPos = window::size_max.y + 5; SelectablesPos = -335; ParametersPos = window::size_max.x + 340; subtab = 0; }
            }
        }
        c.AddBorders(window::size_max);
    }
    g.end();
}