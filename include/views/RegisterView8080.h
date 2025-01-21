#pragma once

#include "CPU8080.h"
#include "RegistersView.h"
#include "raylib_wrapper.h"
#include "raygui.h"

class RegisterView8080 : public RegistersView {
    public:
        RegisterView8080(CPU8080& cpu) : cpu(cpu) {}

        void render() override {
            float winWidth = GetScreenWidth();
            float termX = 20 + 80*9 + 20;
            float panelWidth = winWidth - termX;
            float pX = termX + panelWidth - 80;

            GuiGroupBox(Rectangle{termX - 10, 20, panelWidth, 135}, "REGISTERS");
            GuiRegisterBox(termX, 30, "A:", cpu.regs.A);
            GuiRegisterBox(termX, 55, "B:", cpu.regs.B);
            GuiRegisterBox(pX + 5, 55, "C:", cpu.regs.C);
            GuiRegisterBox(termX, 80, "D:", cpu.regs.D);
            GuiRegisterBox(pX + 5, 80, "E:", cpu.regs.E);
            GuiRegisterBox(termX, 105, "H:", cpu.regs.H);
            GuiRegisterBox(pX + 5, 105, "L:", cpu.regs.L);
            GuiRegisterBox(termX - 5, 130, "PC:", cpu.regs.PC);
            GuiRegisterBox(pX, 130, "SP:", cpu.regs.SP);

            GuiGroupBox(Rectangle{termX - 10, 165, panelWidth, 60}, "FLAGS");
            GuiRegisterBox(termX, 175, "Z:", cpu.regs.Z);
            GuiRegisterBox(termX + 45, 175, "S:", cpu.regs.S);
            GuiRegisterBox(termX + 90, 175, "P:", cpu.regs.P);
            GuiRegisterBox(termX - 5, 200, "CY:", cpu.regs.CY);
            GuiRegisterBox(termX + 40, 200, "AC:", cpu.regs.AC);
        }

    private:
        CPU8080& cpu;
};
