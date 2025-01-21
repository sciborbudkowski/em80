#pragma once

#include "RegistersBase.h"
#include "raygui.h"

#include <cstdio>
#include <cstring>

class RegistersView {
    public:
        virtual ~RegistersView() = default;

        virtual void render() = 0;

        void GuiRegisterBox(int x, int y, const char* label, Register8& reg, bool editable = false) {
            int tempValue = static_cast<int>(reg.value);
            char registerValue[4];
            snprintf(registerValue, sizeof(registerValue), "%02Xh", tempValue);
            GuiLabel(Rectangle{(float)x, (float)y, 20, 15}, label);
            GuiTextBox(Rectangle{(float)x + 15, (float)y, 40, 15}, registerValue, sizeof(registerValue), editable);
        }

        void GuiRegisterBox(int x, int y, const char* label, Register16& reg, bool editable = false) {
            int tempValue = static_cast<int>(reg.value);
            char registerValue[6];
            snprintf(registerValue, sizeof(registerValue), "%04Xh", tempValue);
            GuiLabel(Rectangle{(float)x, (float)y, 20, 15}, label);
            GuiTextBox(Rectangle{(float)x + 20, (float)y, 40, 15}, registerValue, sizeof(registerValue), editable);
        }

        void GuiRegisterBox(int x, int y, const char* label, Flag& reg, bool editable = false) {
            char tempValue[2];
            snprintf(tempValue, sizeof(tempValue), "%d", reg);
            GuiLabel(Rectangle{(float)x, (float)y, 20, 15}, label);
            if(strlen(label) == 3) {
                GuiTextBox(Rectangle{(float)x + 20, (float)y, 20, 15}, tempValue, 2, editable);
            } else {
                GuiTextBox(Rectangle{(float)x + 15, (float)y, 20, 15}, tempValue, 2, editable);
            }
        }
};
