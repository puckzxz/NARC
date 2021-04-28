#pragma once

#include <filesystem>
#include <string>
#include "imgui.h"
#include "TextEditor.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

using UserPalette = std::unordered_map<ImU32 , std::string>;

static TextEditor::Palette s_p = { {
        0xff7f7f7f,	// Default
        0xffd69c56,	// Key
        0xff7070e0,	// String
        0xff70a0e0, // Keyword
        0xff00ff00,	// Number
        0xffffffff, // Punctuation
        0xff101010, // Background
        0xffe0e0e0, // Cursor
        0x80a06020, // Selection
        0xff707000, // Line number
        0x40000000, // Current line fill
        0x40808080, // Current line fill (inactive)
        0x40a0a0a0, // Current line edge
    } };

enum AppTheme
{
    Dark,
    Light,
    Classic
};

struct Settings
{
    int32_t windowWidth;
    int32_t windowHeight;
    bool maximized;
    AppTheme theme;
    TextEditor::Palette palette;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, windowWidth, windowHeight, maximized, theme, palette);
};

class SettingsManager
{
public:
    static void SaveSettings(const Settings& s);

    static const Settings& GetSettings();

    static TextEditor::Palette GetPalette();

    static TextEditor::Palette GetDefaultPalette()
    {
        return s_p;
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
    SettingsManager() = default;;
};
