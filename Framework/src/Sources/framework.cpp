#include <map>
#include <vector>

#include "main.h"
#include "settings.h"

DWORD window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground;

//UI ITEMS

void Framework::GUI::begin(const char* name, ImVec2 size)
{
    ImGuiStyle& s = ImGui::GetStyle();

    s.WindowRounding = window::rounding;
    s.Alpha = 1;

    ImGui::Begin(name, NULL, window_flags);
    ImGui::SetWindowSize(size);
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    window->DrawList->AddRectFilled({ 0,0 }, ImVec2(size.x - 1, size.y - 1), ImGui::GetColorU32(colors::bg), window::rounding);
    ImGui::PopStyleVar(1);
    style();
    mw();
}

void Framework::GUI::end()
{
    ImGui::End();
}

void Framework::GUI::ChildBegin(const char* name, float posX, float posY, float sizeX, float sizeY)
{
    auto s = ImGui::GetStyle();
    ImGui::SetNextWindowPos({ posX, posY });

    ImGui::BeginChild(name, { sizeX, sizeY });
}
void Framework::GUI::ChildEnd() 
{
    ImGui::EndChild();
}

void Framework::GUI::style()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowBorderSize = 0.f;
    style.WindowRounding = window::rounding;
    style.ChildRounding = window::rounding;
    style.FrameBorderSize = 0.f;
    style.FrameRounding = items::rounding;
    style.WindowPadding = ImVec2(0, 0);
    style.Colors[ImGuiCol_ChildBg] = colors::lbg;
    style.ScrollbarSize = 5.f;
    style.ScrollbarRounding = 10;
    style.ItemSpacing = { 10, 15 };
}

void Framework::GUI::Blur(HWND hwnd)
{
    struct ACCENTPOLICY
    {
        int na;
        int nf;
        int nc;
        int nA;
    };
    struct WINCOMPATTRDATA
    {
        int na;
        PVOID pd;
        ULONG ul;
    };

    const HINSTANCE hm = LoadLibrary(L"user32.dll");
    if (hm)
    {
        typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);

        const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hm, "SetWindowCompositionAttribute");
        if (SetWindowCompositionAttribute)
        {
            ACCENTPOLICY policy = { 4, 0, 155, 0 }; // 4,0,155,0 (Acrylic blur) 3,0,0,0 
            WINCOMPATTRDATA data = { 19, &policy,sizeof(ACCENTPOLICY) };
            SetWindowCompositionAttribute(hwnd, &data);
        }
        FreeLibrary(hm);
    }
}

void Framework::GUI::mw()
{
    GetWindowRect(hwnd, &rc);
    MoveWindow(hwnd, rc.left + ImGui::GetWindowPos().x, rc.top + ImGui::GetWindowPos().y, window::size_max.x, window::size_max.y, TRUE);
    ImGui::SetWindowPos(ImVec2(0.f, 0.f));
}

void Framework::GUI::pf(ImFont* font)
{
    ImGui::PushFont(font);
}

void Framework::GUI::AddImage(ID3D11ShaderResourceView* pic, float posX, float posY, float sizeX, float sizeY)
{
    ImGui::SetCursorPos({ posX, posY });
    ImGui::Image(pic, { sizeX, sizeY });
}

void Framework::GUI::AddImageRotated(ID3D11ShaderResourceView* pic, float posX, float posY, float sizeX, float sizeY)
{
    auto style = ImGui::GetStyle();
    style.Alpha = alphaColor;

    ImGui::SetCursorPos({ posX, posY });
    ImGui::ImageRotation(pic, {sizeX, sizeY}, ImVec2(1, 1), ImVec2(0, 0), ImGui::GetColorU32(colors::MainColor), 0.1f);
}

void CircleImage(ID3D11ShaderResourceView* user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1))
{
    auto style = ImGui::GetStyle();
    style.Alpha = alphaColor;

    ImVec2 current_cursor_pos = ImGui::GetCursorPos();
    ImGui::SetCursorPos(current_cursor_pos);

    ImVec2 p_min = ImGui::GetCursorScreenPos();
    ImVec2 p_max = ImVec2(p_min.x + size.x, p_min.y + size.y);
    float rounding = 5.0f;

    ImGui::GetWindowDrawList()->AddImageRounded(user_texture_id, p_min, p_max, uv0, uv1, ImGui::GetColorU32(tint_col), rounding);
    ImGui::Dummy(size);

    ImGui::SetCursorPos(current_cursor_pos);
}

void Framework::GUI::ApplyImage(int i)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    auto style = ImGui::GetStyle();
    style.Alpha = alphaColor;

    ImVec2 MIN = ImGui::GetItemRectMin();
    ImVec2 MAX = ImGui::GetItemRectMax();

    window->DrawList->AddRectFilledMultiColor({ MIN.x, MIN.y }, { MAX.x - 160, MAX.y }, ImGui::GetColorU32(colors::bg), ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0), ImGui::GetColorU32(colors::bg));
}

//Framework::CUSTOM ITEMS

struct ButtonState {
    ImVec4 ButtonColor;
    ImVec4 BorderColor;
    ImVec4 ShadowColor;
    ImVec4 TextColor;
    float timer;
    ButtonState() : ButtonColor(0, 0, 0, 0), BorderColor(0, 0, 0, 0), TextColor(0, 0, 0, 0), timer(0.0f) {}
};
bool Framework::CUSTOM::Button(const char* label, const char* text, float posX, float posY, float sizeX, float sizeY) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiIO& io = ImGui::GetIO();
    style.Alpha = alphaColor;

    ImGui::SetCursorPos({ posX, posY });
    bool result = ImGui::InvisibleButton(label, { sizeX, sizeY });

    ImGuiID id = window->GetID(label);
    static std::map <ImGuiID, ButtonState> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end()) {
        anim.insert({ id, ButtonState() });
        it_anim = anim.find(id);
    }

    float time = io.DeltaTime;

    ImVec4 target = ImGui::IsItemActive() ? colors::MainColor : ImGui::IsItemHovered() ? colors::MainColor: colors::SecondColor;
    ImVec4 ShadowColorTarget = ImGui::IsItemActive() ? colors::MainColor : ImGui::IsItemHovered() ? colors::SecondColor : ImVec4(0, 0, 0, 0);
    ImVec4 targetText = ImGui::IsItemActive() || ImGui::IsItemHovered() ? colors::White : colors::lwhite;

    it_anim->second.ButtonColor = ImVec4(ImLerp(it_anim->second.ButtonColor.x, target.x, time * 6), ImLerp(it_anim->second.ButtonColor.y, target.y, time * 6), ImLerp(it_anim->second.ButtonColor.z, target.z, time * 6), ImLerp(it_anim->second.ButtonColor.w, target.w, time * 6));
    it_anim->second.TextColor = ImVec4(ImLerp(it_anim->second.TextColor.x, targetText.x, time * 6), ImLerp(it_anim->second.TextColor.y, targetText.y, time * 6), ImLerp(it_anim->second.TextColor.z, targetText.z, time * 6), ImLerp(it_anim->second.TextColor.w, targetText.w, time * 6));
    it_anim->second.ShadowColor = ImVec4(ImLerp(it_anim->second.ShadowColor.x, ShadowColorTarget.x, time * 6), ImLerp(it_anim->second.ShadowColor.y, ShadowColorTarget.y, time * 6), ImLerp(it_anim->second.ShadowColor.z, ShadowColorTarget.z, time * 6), ImLerp(it_anim->second.ShadowColor.w, ShadowColorTarget.w / 2, time * 6));

    ImVec2 MIN = ImGui::GetItemRectMin();
    ImVec2 MAX = ImGui::GetItemRectMax();

    window->DrawList->AddRectFilled(MIN, MAX, ImGui::GetColorU32(it_anim->second.ButtonColor), items::rounding, 0);
    window->DrawList->AddShadowRect(MIN, MAX, ImGui::GetColorU32(it_anim->second.ShadowColor), 50, ImVec2(0,0), 0, 3.f);

    if (text) {
        ImVec2 textSize = ImGui::CalcTextSize(text);
        ImVec2 textPos = { MIN.x + (sizeX - textSize.x) * 0.5f, MIN.y + (sizeY - textSize.y) * 0.5f };
        window->DrawList->AddText(textPos, ImGui::GetColorU32(it_anim->second.TextColor), text);
    }

    return result;
}

struct TabState {
    ImVec4 TabColor;
    ImVec4 TextColor;
    ImVec4 IconColor;
    ImVec4 ShadowColor;
};

bool Framework::CUSTOM::Tab(const char* label, const char* text, const char* icon, float posX, float posY, float sizeX, float sizeY, bool v)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiIO& io = ImGui::GetIO();
    style.Alpha = alphaColor;

    ImGui::SetCursorPos({ posX, posY });
    bool result = ImGui::InvisibleButton(label, { sizeX, sizeY });

    ImGuiID id = window->GetID(label);
    static std::map<ImGuiID, TabState> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end()) {
        anim.insert({ id, TabState() });
        it_anim = anim.find(id);
    }

    float time = io.DeltaTime;

    if (result)
        v = !v;

    ImVec4 targetText = v? colors::White : ImGui::IsItemHovered() ? colors::lwhite : colors::Gray;
   
    it_anim->second.TextColor = ImVec4(ImLerp(it_anim->second.TextColor.x, targetText.x, time * 6), ImLerp(it_anim->second.TextColor.y, targetText.y, time * 6), ImLerp(it_anim->second.TextColor.z, targetText.z, time * 6), ImLerp(it_anim->second.TextColor.w, targetText.w, time * 6));
    
    ImVec2 MIN = ImGui::GetItemRectMin();
    ImVec2 MAX = ImGui::GetItemRectMax();


    if (text) {
        ImVec2 textSize = ImGui::CalcTextSize(text);
        ImVec2 textPos = { MIN.x, MIN.y + (sizeY - textSize.y) * 0.5f };
        window->DrawList->AddText(textPos, ImGui::GetColorU32(it_anim->second.TextColor), text);
    }

    return result;
}

struct SelectableItemState {
    ImVec4 ShadowColor;
    ImVec4 FilledColor;
};

bool Framework::CUSTOM::SelectableItem(const char* label, ID3D11ShaderResourceView* pic, float posX, float posY, float sizeX, float sizeY, bool v)
{
    Framework::GUI gui;
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiIO& io = ImGui::GetIO();
    style.Alpha = alphaColor;

    ImGui::SetCursorPos({ posX, posY });
    bool result = ImGui::InvisibleButton(label, { sizeX, sizeY });

    ImGuiID id = window->GetID(label);
    static std::map<ImGuiID, SelectableItemState> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end()) {
        anim.insert({ id, SelectableItemState() });
        it_anim = anim.find(id);
        it_anim->second.FilledColor = ImVec4(1, 1, 1, 1);
    }

    float time = io.DeltaTime;

    if (result)
        v = !v;

    ImVec4 ShadowColorTarget = v ? colors::MainColor : ImGui::IsItemHovered() ? colors::SecondColor : ImVec4(0, 0, 0, 0);
    ImVec4 FilledColorTarget = v ? ImVec4(0, 0, 0, 0) : ImGui::IsItemHovered() ? ImVec4(0.1, 0.1, 0.1, 0.5) : ImVec4(0.1, 0.1, 0.1, 0.7);

    it_anim->second.ShadowColor = ImVec4(ImLerp(it_anim->second.ShadowColor, ShadowColorTarget, time * 6));
    it_anim->second.FilledColor = ImVec4(ImLerp(it_anim->second.FilledColor, FilledColorTarget, time * 6));

    ImVec2 MIN = ImGui::GetItemRectMin();
    ImVec2 MAX = ImGui::GetItemRectMax();

    ImVec2 p_min = { MIN.x, MIN.y };
    ImVec2 p_max = { MAX.x, MAX.y };

    window->DrawList->AddShadowRect(p_min, p_max, ImGui::GetColorU32(it_anim->second.ShadowColor), 15, ImVec2(0, 0), 0, 3.f);

    window->DrawList->AddImage((void*)pic, p_min, p_max);
    window->DrawList->AddRectFilled({p_min.x - 1, p_min.y - 1}, p_max, ImGui::GetColorU32(it_anim->second.FilledColor), items::rounding);

    return result;
}

struct Checkbox2State {
    ImVec4 ButtonColor;
    ImVec4 FilledRectColor;
    ImVec4 TextColor;
    
    ImVec2 Button_Pos;
};

bool Framework::CUSTOM::Checkbox2(const char* label, const char* text, float posX, float posY, bool* v, float space)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiIO& io = ImGui::GetIO();
    style.Alpha = alphaColor;

    ImGui::SetCursorPos({ posX, posY });
    bool result = ImGui::InvisibleButton(label, { 25, 10 });

    ImVec2 MIN = ImGui::GetItemRectMin();
    ImVec2 MAX = ImGui::GetItemRectMax();

    ImGuiID id = window->GetID(label);
    static std::map<ImGuiID, Checkbox2State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end()) {
        anim.insert({ id, Checkbox2State() });
        it_anim = anim.find(id);
        it_anim->second.Button_Pos.x = MIN.x;
        it_anim->second.TextColor = *v ? colors::White : colors::Gray;
        it_anim->second.FilledRectColor = *v ? colors::DarkGray : ImVec4(.071f, 0.071f, 0.071f, 1.0f);
        it_anim->second.ButtonColor = *v ? colors::MainColor : colors::Gray;
    }

    float time = io.DeltaTime * 6;

    if (result)
        *v = !*v;

    ImVec4 Button_Target = *v ? colors::MainColor : colors::Gray;
    ImVec4 FilledRect_Target = *v ? colors::DarkGray : ImVec4(.071f, 0.071f, 0.071f, 1.0f);
    ImVec4 Text_Target = *v ? colors::White : ImGui::IsItemHovered() ? colors::lwhite : colors::Gray;
    ImVec2 ButtonPos_Target = *v ? ImVec2(MAX.x - 12, (MAX.y + MIN.y) / 2 - 6) : ImVec2(MIN.x, (MAX.y + MIN.y) / 2 - 6);

    it_anim->second.ButtonColor = ImVec4(ImLerp(it_anim->second.ButtonColor, Button_Target, time));
    it_anim->second.FilledRectColor = ImVec4(ImLerp(it_anim->second.FilledRectColor, FilledRect_Target, time));
    it_anim->second.TextColor = ImVec4(ImLerp(it_anim->second.TextColor, Text_Target, time));
    it_anim->second.Button_Pos = ImLerp(it_anim->second.Button_Pos, ButtonPos_Target, time);

    window->DrawList->AddRectFilled(MIN, MAX, ImGui::GetColorU32(it_anim->second.FilledRectColor), 2.f);

    if (text) {
        ImVec2 textSize = ImGui::CalcTextSize(text);
        ImVec2 textPos = { MIN.x + (MAX.x - MIN.x - textSize.x) * 0.5f, MIN.y + (MAX.y - MIN.y - textSize.y) * 0.5f };
        window->DrawList->AddText({ textPos.x - space, textPos.y }, ImGui::GetColorU32(it_anim->second.TextColor), text);
    }

    ImVec2 buttonMin = { it_anim->second.Button_Pos.x, (MAX.y + MIN.y) / 2 - 6 };
    ImVec2 buttonMax = { buttonMin.x + 12, buttonMin.y + 12 };
    window->DrawList->AddRectFilled(buttonMin, buttonMax, ImGui::GetColorU32(it_anim->second.ButtonColor), 2.f);

    return result;
}

struct InputState {
    ImVec4 TextColor;
    ImVec4 ShadowColor;
    float Shadow_size;
};

bool Framework::CUSTOM::InputText(const char* label, const char* hint, float posX, float posY, float sizeX, static char buf[], size_t buf_size, ImGuiInputTextFlags flag)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiIO& io = ImGui::GetIO();

    style.Colors[ImGuiCol_FrameBg] = ImVec4(0, 0, 0, 0);
    style.FramePadding = ImVec2(0, 5);
    style.Alpha = alphaColor;


    ImGui::PushFont(fonts::Inter_Regular);
    ImGui::SetNextItemWidth(sizeX);
    ImGui::SetCursorPos({ posX, posY });

    ImGuiID id = window->GetID(label);
    static std::map<ImGuiID, InputState> anim;
    auto it_anim = anim.find(id);
    float time = io.DeltaTime;

    if (it_anim == anim.end()) {
        anim.insert({ id, InputState() });
        it_anim = anim.find(id);
        it_anim->second.TextColor = colors::Gray;
        it_anim->second.ShadowColor = colors::SecondColor;
    }

    style.Colors[ImGuiCol_TextDisabled] = it_anim->second.TextColor;

    ImVec2 MIN = ImVec2(posX, posY);
    ImVec2 MAX = ImVec2(posX + sizeX, posY + io.Fonts->Fonts[0]->FontSize + style.FramePadding.y * 2);

    window->DrawList->AddShadowRect({MIN.x, MAX.y - 1}, MAX, ImGui::GetColorU32(it_anim->second.ShadowColor), it_anim->second.Shadow_size, {0,0}, 0, items::rounding);
    window->DrawList->AddLine({ MIN.x, MAX.y }, { MAX.x, MAX.y }, ImGui::GetColorU32(colors::MainColor), 1.f);

    bool result = ImGui::InputTextEx(label, hint, buf, (int)buf_size, ImVec2(0, 0), flag, 0, 0); //LINE 5063

    ImVec4 TextTarget = ImGui::IsItemActive() ? colors::White : ImGui::IsItemHovered() ? colors::White : colors::Gray;
    ImVec4 ShadowTarget = ImGui::IsItemActive() ? colors::MainColor : colors::SecondColor;

    float Target_Size = ImGui::IsItemActive() ? 25.f : ImGui::IsItemHovered() ? 20.f : 5.f;

    it_anim->second.Shadow_size = ImLerp(it_anim->second.Shadow_size, Target_Size, time);
    it_anim->second.TextColor = ImVec4(ImLerp(it_anim->second.TextColor.x, TextTarget.x, time), ImLerp(it_anim->second.TextColor.y, TextTarget.y, time), ImLerp(it_anim->second.TextColor.z, TextTarget.z, time), ImLerp(it_anim->second.TextColor.w, TextTarget.w, time));
    it_anim->second.ShadowColor = ImVec4(ImLerp(it_anim->second.ShadowColor, ShadowTarget, time));

    ImGui::PopStyleVar();
    ImGui::TreePop();
    ImGui::PopStyleColor(2);
    ImGui::PopID();
    return result;
}

struct IconButtonState {
    ImVec4 TextColor;
};

bool Framework::CUSTOM::IconButton(const char* label, const char* icon, float posX, float posY, int action)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiIO& io = ImGui::GetIO();
    //style.Alpha = alphaColor;

    ImGui::SetCursorPos({ posX, posY });
    bool result = ImGui::InvisibleButton(label, { 15, 15 });

    ImGuiID id = window->GetID(label);
    static std::map<ImGuiID, IconButtonState> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end()) {
        anim.insert({ id, IconButtonState() });
        it_anim = anim.find(id);
    }

    float time = io.DeltaTime;

    ImVec4 target = ImGui::IsItemHovered() ? colors::lwhite : colors::Gray;

    it_anim->second.TextColor = ImVec4(ImLerp(it_anim->second.TextColor.x, target.x, time * 6), ImLerp(it_anim->second.TextColor.y, target.y, time * 6), ImLerp(it_anim->second.TextColor.z, target.z, time * 6), ImLerp(it_anim->second.TextColor.w, target.w, time * 6));

    ImVec2 MIN = ImGui::GetItemRectMin();
    ImVec2 MAX = ImGui::GetItemRectMax();

    ImFont* current = ImGui::GetFont();
    ImGui::PushFont(fonts::FontAwesome);

    ImVec2 textSize = ImGui::CalcTextSize(icon);
    ImVec2 textPos = { MIN.x + (15 - textSize.x) * 0.5f, MIN.y + (5 - textSize.y) * 0.5f };

    window->DrawList->AddText(textPos, ImGui::GetColorU32(it_anim->second.TextColor), icon);
    ImGui::PushFont(current);

    switch (action)
    {
    case 1:
        if (result)
            exit(0);
        break;
    case 2:
        if (result)
            ::ShowWindow(hwnd, SW_MINIMIZE);
        break;
    }

    return result;
}

void Framework::CUSTOM::AddBorders(ImVec2 pos)
{
    const int vtx_idx_1 = ImGui::GetWindowDrawList()->VtxBuffer.Size;
    ImGui::GetWindowDrawList()->AddRect(ImVec2(1, 1), ImVec2(pos.x - 1, pos.y - 1), ImGui::GetColorU32(colors::MainColor), window::rounding, ImDrawFlags_None, 2.f);

    ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImGui::GetWindowDrawList(), vtx_idx_1, ImGui::GetWindowDrawList()->VtxBuffer.Size, ImVec2(1, 1), ImVec2(pos.x / 4, pos.y / 3), ImGui::GetColorU32(colors::MainColor), ImColor(24, 24, 24, 24));

}

void Framework::CUSTOM::Text(float posX, float posY, ImFont* font, float size, const char* text, ImVec4 col)
{
    ImFont* current = ImGui::GetFont();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    window->DrawList->AddText(font, size, { posX, posY }, ImGui::GetColorU32(col), text);
    ImGui::PushFont(current);
}

void Framework::CUSTOM::ShadowText(float posX, float posY, ImFont* font, float size, const char* text, ImVec4 col, ImVec4 shadow_col)
{
    auto cfont = ImGui::GetFont();
    ImGui::PushFont(font);

    ImGui::ShadowText(text, ImGui::GetColorU32(col), ImGui::GetColorU32(shadow_col), size, {posX, posY});
}