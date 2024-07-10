#include <menu/menu.h>
#include <imgui/imgui_internal.h>
#include <map>
#include <utils/config.h>
#include <utils/encryption/xor.h>

bool unload = false;

struct tab_element {
    float element_opacity;
};

bool menu::custom_tab(const char* name, bool boolean)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = ImGui::CalcTextSize(name, NULL, true);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + label_size.x, pos.y + label_size.y));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x + 10, rect.Max.y), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, tab_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.8f : hovered ? 0.6f : 0.4f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddText(rect.Min, ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity), name);

    return pressed;
}
const char* parts[] = { "head", "neck", "chest", "pelvis" };

bool isChangingKey = false;
void menu::draw()
{
    ImGui::SetNextWindowSize({ 500, 370 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin(enc("sin.wtf"), nullptr, ImGuiWindowFlags_NoDecoration );
    {
        auto draw = ImGui::GetWindowDrawList();
        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();

        draw->AddRectFilled(pos, ImVec2(2000, 2000), ImColor(9, 9, 9, 255), 0, 0);
        draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + 51), ImColor(24, 24, 24, 255), 0, 0);

        draw->AddText(fonts->large, 20.0f, ImVec2(pos.x + 25, pos.y + 16), ImColor(204, 107, 219), enc("sin."));
        draw->AddText(fonts->large, 20.0f, ImVec2(pos.x + 54, pos.y + 16), ImColor(0, 247, 255), enc("wtf"));

        ImGui::SetCursorPos({ 320, 19 });
        ImGui::BeginGroup(); {
            ImGui::PushFont(fonts->medium);
            if (menu::custom_tab(enc("Aimbot"), tab_selected == aim_tab)) tab_selected = aim_tab;
            ImGui::SameLine();
            if (menu::custom_tab(enc("Visuals"), tab_selected == visual_tab)) tab_selected = visual_tab;
            ImGui::SameLine();
            if (menu::custom_tab(enc("Misc"), tab_selected == misc_tab)) tab_selected = misc_tab;
            ImGui::PopFont();
        }
        ImGui::EndGroup();

        ImVec2 menuSize(500, 370);

        float middleX = pos.x + menuSize.x / 2;

        float paddingTop = 60;
        float paddingBottom = 10;

        ImVec2 lineStart(middleX, pos.y + paddingTop);
        ImVec2 lineEnd(middleX, pos.y + menuSize.y - paddingBottom);

        draw->AddLine(lineStart, lineEnd, ImColor(0, 247, 255, 255), 1.0f);

        switch (tab_selected) {
        case aim_tab:
            ImGui::PushFont(fonts->medium);
            draw->AddText(fonts->medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Aim");

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild(enc("##container"), ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar); {
                ImGui::Checkbox(enc("Aimbot"), &config->aim.aimbot);
                ImGui::Checkbox(enc("Prediction"), &config->aim.prediction);
                ImGui::Checkbox(enc("FOV Circle"), &config->aim.draw.fov);

                ImGui::SliderInt(enc("FOV"), &config->aim.fov, 5, 500, enc("%d%%"), ImGuiSliderFlags_NoInput);
                ImGui::SliderInt(enc("Smoothing"), &config->aim.smooth, 1, 30, enc("%d%%"), ImGuiSliderFlags_NoInput);

                if (ImGui::BeginCombo("Select Aim Bone", parts[config->aim.aimbone])) {
                    for (int i = 0; i < IM_ARRAYSIZE(parts); i++) {
                        bool isSelected = (config->aim.aimbone == i);
                        if (ImGui::Selectable(parts[i], isSelected))
                            config->aim.aimbone = i;

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                // Logic for changing key
                if (!isChangingKey && ImGui::Button("Change Key")) {
                    isChangingKey = true;
                }

                // Logic for changing key
                if (isChangingKey) {
                    ImGui::Text("Press a key...");
                    for (int i = 0; i < 256; i++) {
                        if (GetAsyncKeyState(i) & 0x8000 && i != VK_ESCAPE && i != VK_RETURN) {
                            config->aim.aim_key = i;
                            isChangingKey = false;
                            break;
                        }
                    }
                }

                // Display currently set key
                ImGui::Text("Aim Key: %d", config->aim.aim_key);
            }
            ImGui::EndChild();

            draw->AddText(fonts->medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), enc("Aim Utils"));

            ImGui::SetCursorPos({ 285, 85 });
            ImGui::BeginChild(enc("##container1"), ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar); {
                ImGui::Checkbox(enc("Draw Held Weapon"), &config->aim.held_weapon);
                ImGui::Checkbox(enc("Draw Target line"), &config->aim.draw.line);
            }
            ImGui::PopFont();
            ImGui::EndChild();
            break;

        case visual_tab:
            ImGui::PushFont(fonts->medium);
            draw->AddText(fonts->medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), enc("Visuals"));

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild(enc("##container"), ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar); {
                ImGui::Checkbox(enc("Box"), &config->visual.box);
                if (config->visual.box)
                {
                    config->visual.filled_box = false;
                }
                ImGui::Checkbox(enc("Filled Box"), &config->visual.filled_box);
                if (config->visual.filled_box)
                {
                    config->visual.box = false;
                }
                ImGui::Checkbox(enc("Skeleton"), &config->visual.skeleton);
                ImGui::Checkbox(enc("Distance"), &config->visual.distance);
                ImGui::Checkbox(enc("Visible Check"), &config->visual.visible_check);
                ImGui::Checkbox(enc("Name"), &config->visual.name);
            }
            ImGui::EndChild();

            draw->AddText(fonts->medium, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), enc("Visual Preview"));

            ImGui::SetCursorPos({ 285, 85 });
            ImGui::BeginChild(enc("##container1"), ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar);
            {
                ImVec2 imageSize = ImVec2(330, 300);

                ImVec2 childSize = ImGui::GetContentRegionAvail();
                float xOffset = (childSize.x - imageSize.x) * 0.57f;
                float yOffset = (childSize.y - imageSize.y) * 0.8f;

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + xOffset);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + yOffset);

                if (config->visual.skeleton && config->visual.box)
                {
                    ImGui::Image((void*)(intptr_t)photos::skeleton_box, imageSize);
                }
                else if (config->visual.skeleton && config->visual.filled_box)
                {
                    ImGui::Image((void*)(intptr_t)photos::skeleton_filled_box, imageSize);
                }
                else if (config->visual.skeleton)
                {
                    ImGui::Image((void*)(intptr_t)photos::skeleton, imageSize);
                }
                else if (config->visual.filled_box)
                {
                    ImGui::Image((void*)(intptr_t)photos::filled_box, imageSize);
                }
                else if (config->visual.box)
                {
                    ImGui::Image((void*)(intptr_t)photos::box, imageSize);
                }
            }
            ImGui::EndChild();
            ImGui::PopFont();
            break;

        case misc_tab:
            ImGui::PushFont(fonts->medium);
            draw->AddText(fonts->medium, 14.0f, ImVec2(pos.x + 25, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Misc");

            ImGui::SetCursorPos({ 25, 85 });
            ImGui::BeginChild("##container", ImVec2(190, 275), false, ImGuiWindowFlags_NoScrollbar); {
                ImGui::Checkbox(enc("Draw Watermark"), &config->misc.water_mark);
                ImGui::Checkbox(enc("Draw Fps"), &config->misc.fps);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Checkbox(enc("Un-Inject"), &unload);

                if (unload)
                {
                    system(enc("taskkill /F /IM MedalEncoder.exe"));
                    exit(0);
                }
            }
            ImGui::PopFont();
            ImGui::EndChild();
            break;
        }
    }
    ImGui::End();

    ImGui::PopStyleVar();
}