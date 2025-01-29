// !Marked to remove!

#include "EmulatorWindow.h"
#include "CPUFactory.h"

#include <QString>
#include <QFileDialog>
#include <iostream>

#define RAYGUI_IMPLEMENTATION
#include "raylib_wrapper.h"
#include "raygui.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 490


// EmulatorWindow::EmulatorWindow(CPUType type) {
//     std::cout << "EmulatorWindow::EmulatorWindow(CPUType type) start" << std::endl;
//     //cpu = createCPU(type);
//     std::cout << "EmulatorWindow::EmulatorWindow(CPUType type) end" << std::endl;
// }

void EmulatorWindow::start() {
    cpu->reset();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Emulator");
    SetTargetFPS(60);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(LIGHTGRAY));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(GREEN));
    font = LoadFont("assets/fonts/DejaVuSansMono.ttf");

    running = true;

    char prgPath[512] = "\0";

    while(!WindowShouldClose()) {
        if (IsKeyPressed(KEY_BACKSPACE)) {
            cpu->getIO().getTerminal().removeLastChar();
        } else if (IsKeyPressed(KEY_ENTER)) {
            // cpu->getIO().getTerminal().processCommand(cpu->getIO().getTerminal().getLastCommand(), cpu);
            cpu->getIO().getTerminal().printChar('\n');
        } else if (IsKeyPressed(KEY_SPACE)) {
            cpu->getIO().getTerminal().printChar(' ');
        } else {
            std::string ch = "";
            for (int key = KEY_A; key <= KEY_Z; key++) {
                if (IsKeyPressed(key)) {
                    char chKey = (char)(key - KEY_A + 'a');
                    cpu->getIO().getTerminal().printChar(chKey);
                    ch = chKey;
                }
            }
            for (int key = KEY_ZERO; key <= KEY_NINE; key++) {
                if (IsKeyPressed(key)) {
                    char chKey = (char)(key - KEY_ZERO + '0');
                    cpu->getIO().getTerminal().printChar(chKey);
                    ch = chKey;
                }
            }
            // cpu->getIO().getTerminal().setLastCommand(ch);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        float winWidth = GetScreenWidth();
        float termX = 20 + 80*9 + 20;
        float panelWidth = winWidth - termX;
        float pX = termX + panelWidth - 80;

        GuiGroupBox(Rectangle{termX - 10, 20, panelWidth, 135}, "REGISTERS");
        // GuiRegisterBox(termX, 30, "A:", cpu->regs->A);
        // GuiRegisterBox(termX, 55, "B:", cpu->regs->B);
        // GuiRegisterBox(pX + 5, 55, "C:", cpu->regs->C);
        // GuiRegisterBox(termX, 80, "D:", cpu.regs.D);
        // GuiRegisterBox(pX + 5, 80, "E:", cpu.regs.E);
        // GuiRegisterBox(termX, 105, "H:", cpu.regs.H);
        // GuiRegisterBox(pX + 5, 105, "L:", cpu.regs.L);
        // GuiRegisterBox(termX - 5, 130, "PC:", cpu.regs.PC);
        // GuiRegisterBox(pX, 130, "SP:", cpu.regs.SP);

        GuiGroupBox(Rectangle{termX - 10, 165, panelWidth, 60}, "FLAGS");
        // GuiRegisterBox(termX, 175, "Z:", cpu.regs.Z);
        // GuiRegisterBox(termX + 45, 175, "S:", cpu.regs.S);
        // GuiRegisterBox(termX + 90, 175, "P:", cpu.regs.P);
        // GuiRegisterBox(termX - 5, 200, "CY:", cpu.regs.CY);
        // GuiRegisterBox(termX + 40, 200, "AC:", cpu.regs.AC);

        GuiGroupBox(Rectangle{termX - 10, 235, panelWidth, 100}, "ASM");
        for(float i=4; i>=0; i--) {
            if(i == 4) {
                DrawRectangleRec(Rectangle{termX, 245 + i*15 + 5, panelWidth - 20, 10}, DARKGREEN);
            }
            std::string label = " ";
            // if(cpu.lastInstructions.size() > i) label = cpu.lastInstructions[i];
            GuiLabel(Rectangle{termX + 15, 245 + i*15, panelWidth - 30, 20}, label.c_str());
        }

        // drawTerminal(cpu.io.getTerminal());
        // if(memoryViewVisible) drawMemoryView(cpu.memory, memoryViewStartAddress);

        // if(GuiButton(Rectangle{20, buttonsY, 100, 30}, cpu.isRunning() ? "Stop" : "Start")) {
        //     cpu.setRunning(!cpu.isRunning());
        // }
        if(GuiButton(Rectangle{130, buttonsY, 100, 30}, "Load PRG")) {
            QString fileName = QFileDialog::getOpenFileName(nullptr, "Load Program", ".", "Binary files (*.bin)");
            if(!fileName.isEmpty()) {
                // if(cpu.memory.loadProgram(fileName.toStdString(), 0x0100)) {
                //     cpu.regs.PC = 0x0100;
                //     strncpy(prgPath, fileName.toStdString().c_str(), sizeof(prgPath) - 1);
                //     cpu.io.getTerminal().printString("Loaded program from %s to address: %04Xh\n", prgPath, cpu.regs.PC);
                // }
            }
        }
        if(GuiButton(Rectangle{240, buttonsY, 100, 30}, "Load DSK")) {
            QString fileName = QFileDialog::getOpenFileName(nullptr, "Load Disk", ".", "Disk files (*.dsk)");
            if(!fileName.isEmpty()) {
                // cpu.disk.loadDiskFromFile(fileName.toStdString(), cpu.memory, 0x0100);
                // cpu.io.getTerminal().printString("Loaded disk from %s\n", fileName.toStdString().c_str());
            }
        }
        if(GuiButton(Rectangle{350, buttonsY, 100, 30}, "Step >")) {
            // cpu.step();
        }
        if(GuiButton(Rectangle{460, buttonsY, 100, 30}, "Reset")) {
            cpu->reset();
        }
        if(GuiButton(Rectangle{570, buttonsY, 100, 30}, "Dump CPU")) {
            // cpu.consoleDump();
        }
        if(GuiButton(Rectangle{680, buttonsY, 100, 30}, "Dump MEM")) {
            memoryViewVisible = !memoryViewVisible;
            if(memoryViewVisible) {
                SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT + 350);
                memoryViewStartAddress = 0x0100;
            } else {
                SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
            }
        }
        if(GuiButton(Rectangle{790, buttonsY, 100, 30}, "Test Opcodes")) {
            std::cout << "EmulatorWindow::start() Test Opcodes" << std::endl;
            cpu->testOpcodes();
        }

        // if(cpu.isRunning()) {
        //     cpu.step();
        // }

        EndDrawing();
    }

    running = false;
    UnloadFont(font);

    CloseWindow();
}

// void EmulatorWindow::drawTerminal(const TerminalBase& terminal) {
//     int charWidth = MeasureTextEx(font, "A", 16, 1).x + 1;
//     int charHeight = 16;
//     int offsetX = 20, offsetY = 20;

//     DrawRectangle(offsetX, offsetY, charWidth * 80, charHeight * 25, DARKGRAY);

//     for(int y = 0; y < terminal.getBuffer().size(); y++) {
//         DrawTextEx(font, terminal.getBuffer()[y].c_str(), Vector2{(float)(offsetX), (float)(offsetY + y * charHeight)}, charHeight, 1, GREEN);
//     }

//     DrawRectangle(terminal.getCursorPos().first * charWidth + offsetX, terminal.getCursorPos().second * charHeight + offsetY, charWidth, charHeight, GREEN);
// }

// void EmulatorWindow::GuiRegisterBox(int x, int y, const char* label, Register8& reg, bool editable) {
//     int tempValue = static_cast<int>(reg.value);
//     char registerValue[4];
//     snprintf(registerValue, sizeof(registerValue), "%02Xh", tempValue);
//     GuiLabel(Rectangle{(float)x, (float)y, 20, 15}, label);
//     GuiTextBox(Rectangle{(float)x + 15, (float)y, 40, 15}, registerValue, sizeof(registerValue), editable);
// }

// void EmulatorWindow::GuiRegisterBox(int x, int y, const char* label, Register16& reg, bool editable) {
//     int tempValue = static_cast<int>(reg.value);
//     char registerValue[6];
//     snprintf(registerValue, sizeof(registerValue), "%04Xh", tempValue);
//     GuiLabel(Rectangle{(float)x, (float)y, 20, 15}, label);
//     GuiTextBox(Rectangle{(float)x + 20, (float)y, 40, 15}, registerValue, sizeof(registerValue), editable);
// }

// void EmulatorWindow::GuiRegisterBox(int x, int y, const char* label, bool& reg, bool editable) {
//     char tempValue[2];
//     snprintf(tempValue, sizeof(tempValue), "%d", reg);
//     GuiLabel(Rectangle{(float)x, (float)y, 20, 15}, label);
//     if(strlen(label) == 3) {
//         GuiTextBox(Rectangle{(float)x + 20, (float)y, 20, 15}, tempValue, 2, editable);
//     } else {
//         GuiTextBox(Rectangle{(float)x + 15, (float)y, 20, 15}, tempValue, 2, editable);
//     }
// }

// void EmulatorWindow::drawMemoryView(MemoryBase& memory, int startAddress) {
//     float posY = buttonsY + 40;
//     GuiGroupBox(Rectangle{20, posY, WINDOW_WIDTH - 40, 350}, "Memory Dump");

//     // every single byte
//     for(int i=0; i<=0xF; i++) {
//         char colLabel[3];
//         snprintf(colLabel, sizeof(colLabel), "%02Xh", i);
//         float posX = 100 + i*50;
//         DrawTextEx(font, colLabel, Vector2{posX, posY + 10}, 18, 1, WHITE);
//     }

//     // every 16 bytes
//     for(int i=0; i<=0xF; i++) {
//         char rowLabel[5];
//         snprintf(rowLabel, sizeof(rowLabel), "%04Xh", startAddress + i*16);
//         float posX = 30;
//         float posY = buttonsY + 80 + i*20;
//         DrawTextEx(font, rowLabel, Vector2{posX, posY}, 18, 1, WHITE);
//     }

//     // memory dump
//     for(int i=0; i<=0xF; i++) {
//         for(int j=0; j<=0xF; j++) {
//             uint16_t pc = startAddress + i*16 + j;
//             // if(pc == cpu.regs.PC) {
//             //     DrawRectangle(100 + j*50, buttonsY + 80 + i*20, 30, 15, RED);
//             // }
//             auto color = WHITE;
//             if(j % 2 == 0) color = LIGHTGRAY;
//             char memLabel[3];
//             snprintf(memLabel, sizeof(memLabel), "%02Xh", memory.read(startAddress + i*16 + j));
//             float posX = 100 + j*50;
//             float posY = buttonsY + 80 + i*20;
//             DrawTextEx(font, memLabel, Vector2{posX, posY}, 16, 1, color);
//         }
//     }
// }
