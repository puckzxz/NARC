#pragma once

#include <filesystem>
#include <string>
#include "imgui.h"
#include "TextEditor.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

using UserPalette = std::unordered_map<ImU32 , std::string>;

enum AppTheme
{
    Dark,
    Light,
    Classic
};

struct AppSettings
{
    int32_t windowWidth;
    int32_t windowHeight;
    bool maximized;
    AppTheme theme;
    TextEditor::Palette palette;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AppSettings, windowWidth, windowHeight, maximized, theme, palette);
};

class Settings
{
public:
    static void Save(const AppSettings& s);

    static const AppSettings& Get();

    static TextEditor::Palette GetPalette();

    static TextEditor::Palette GetDefaultPalette()
    {
        return TextEditor::GetDarkPalette();
    }

    static void Init();

    inline static const std::array<std::string, (unsigned)TextEditor::PaletteIndex::Max> PaletteIndexNames = {
        "Default",
        "Key",
        "String",
        "Keyword",
        "Number",
        "Punctuation",
        "Background",
        "Cursor",
        "Selection",
        "Line number",
        "Current line fill",
        "Current line fill (inactive)",
        "Current line edge"
    };
private:
    Settings() = default;
};
