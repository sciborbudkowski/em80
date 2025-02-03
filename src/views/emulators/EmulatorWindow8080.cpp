#include "EmulatorWindow8080.h"
#include "AssetsLoader.h"
#include "CPU8080.h"
#include "Memory8080.h"
#include "IO8080.h"
#include "StartAddressDialog.h"

#define RAYGUI_IMPLEMENTATION
#include "raylib_wrapper.h"
#include "raygui.h"

#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <fstream>

void EmulatorWindow8080::start() {
    cpu->reset();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Emulator | Intel 8080");
    SetTargetFPS(60);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(LIGHTGRAY));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(GREEN));
    
    try {
        font = AssetsLoader::loadFont("assets/fonts/DejaVuSansMono.ttf");
    } catch (const std::runtime_error& e) {
        QMessageBox::critical(nullptr, "Error", e.what());
        CloseWindow();
        return;
    }

    cpu->setRunning(false);
    char prgPath[512] = "\0";

    while(!WindowShouldClose()) {
        if(IsKeyPressed(KEY_BACKSPACE)) {
            cpu->getIO().terminal->removeLastChar();
            cpu->getIO().terminal->setLastCommand(cpu->getIO().terminal->getLastCommand().substr(0, cpu->getIO().terminal->getLastCommand().size() - 1));
        } else if(IsKeyPressed(KEY_ENTER)) {
            cpu->getIO().terminal->printNewLine();
            cpu->getIO().terminal->handleInput(cpu->getIO().terminal->getLastCommand());
            cpu->getIO().terminal->printNewLine();
            cpu->getIO().terminal->setLastCommand("");
        } else {
            std::string ch = "";
            for(int key=KEY_A; key<=KEY_Z; key++) {
                if(IsKeyPressed(key)) {
                    char chKey = (char)(key - KEY_A + 'a');
                    cpu->getIO().terminal->printChar(chKey);
                    ch = chKey;
                }
            }
            for(int key=KEY_ZERO; key<=KEY_NINE; key++) {
                if(IsKeyPressed(key)) {
                    char chKey = (char)(key - KEY_ZERO + '0');
                    cpu->getIO().terminal->printChar(chKey);
                    ch = chKey;
                }
            }
            if(IsKeyPressed(KEY_SPACE)) {
                cpu->getIO().terminal->printChar(' ');
                ch = ' ';
            }

            if(!ch.empty()) {
                cpu->getIO().terminal->setLastCommand(cpu->getIO().terminal->getLastCommand() + ch);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        winWidth = GetScreenWidth();
        termX = 20 + 80*9 + 20;
        panelWidth = winWidth - termX;
        pX = termX + panelWidth - 80;

        renderTerminal();
        renderRegistersBox();
        renderFlagsBox();
        if(memoryViewVisible) renderMemoryBox(memoryViewStartAddress);
        renderDisassemblyBox();
        renderAndHandleButtons();

        if(cpu->isRunning()) {
            cpu->step();
        }

        EndDrawing();
    }

    cpu->setRunning(false);
    UnloadFont(font);
    CloseWindow();
}

void EmulatorWindow8080::renderTerminal() {
    int charWidth = MeasureTextEx(font, "A", 16, 1).x + 1;
    int charHeight = 16;
    int offsetX = 20, offsetY = 20;

    DrawRectangle(offsetX, offsetY, charWidth * 80, charHeight * 25, DARKGRAY);

    for(int y=0; y<cpu->getIO().terminal->getBuffer().size(); y++) {
        DrawTextEx(font, cpu->getIO().terminal->getBuffer()[y].c_str(), Vector2{(float)(offsetX), (float)(offsetY + y * charHeight)}, charHeight, 1, GREEN);
    }

    DrawRectangle(cpu->getIO().terminal->getCursorPos().first * charWidth + offsetX, cpu->getIO().terminal->getCursorPos().second * charHeight + offsetY, charWidth, charHeight, GREEN);
}

void EmulatorWindow8080::renderRegistersBox() {
    GuiGroupBox(Rectangle{termX - 10, 20, panelWidth, 135}, "REGISTERS");
    guiRegisterBox(termX, 30.0f, "A:", cpu->getRegisters().A);
    guiRegisterBox(termX, 55.0f, "B:", cpu->getRegisters().B);
    guiRegisterBox(pX + 5, 55.0f, "C:", cpu->getRegisters().C);
    guiRegisterBox(termX, 80.0f, "D:", cpu->getRegisters().D);
    guiRegisterBox(pX + 5, 80.0f, "E:", cpu->getRegisters().E);
    guiRegisterBox(termX, 105.0f, "H:", cpu->getRegisters().H);
    guiRegisterBox(pX + 5, 105.0f, "L:", cpu->getRegisters().L);
    guiRegisterBox(termX - 5, 130.0f, "PC:", cpu->getRegisters().PC);
    guiRegisterBox(pX, 130.0f, "SP:", cpu->getRegisters().SP);
}

void EmulatorWindow8080::renderFlagsBox() {
    GuiGroupBox(Rectangle{termX - 10, 165, panelWidth, 60}, "FLAGS");
    guiFlagBox(termX, 175, "Z:", cpu->getRegisters().getFlag(Registers8080::Flag::ZERO));
    guiFlagBox(termX + 45, 175, "S:", cpu->getRegisters().getFlag(Registers8080::Flag::SIGN));
    guiFlagBox(termX + 90, 175, "P:", cpu->getRegisters().getFlag(Registers8080::Flag::PARITY));
    guiFlagBox(termX - 5, 200, "C:", cpu->getRegisters().getFlag(Registers8080::Flag::CARRY));
    guiFlagBox(termX + 40, 200, "AC:", cpu->getRegisters().getFlag(Registers8080::Flag::AUX_CARRY));
}

void EmulatorWindow8080::renderDisassemblyBox() {
        GuiGroupBox(Rectangle{termX - 10, 235, panelWidth, 100}, "ASM");
        for(float i=4; i>=0; i--) {
            if(i == 4) {
                DrawRectangleRec(Rectangle{termX, 245 + i*15 + 5, panelWidth - 20, 10}, DARKGREEN);
            }
            std::string label = " ";
            if(cpu->lastInstructions.size() > i) label = cpu->lastInstructions[i];
            GuiLabel(Rectangle{termX + 15, 245 + i*15, panelWidth - 30, 20}, label.c_str());
        }
}

void EmulatorWindow8080::guiRegisterBox(float x, float y, const char* label, uint8_t regValue, bool editable) {
    int tempValue = static_cast<int>(regValue);
    char registerValue[4];
    snprintf(registerValue, sizeof(registerValue), "%02Xh", tempValue);
    GuiLabel(Rectangle{(float)x, (float)y, 20, 15}, label);
    GuiTextBox(Rectangle{(float)x + 15, (float)y, 40, 15}, registerValue, sizeof(registerValue), editable);
}

void EmulatorWindow8080::guiRegisterBox(float x, float y, const char* label, uint16_t regValue, bool editable) {
    int tempValue = static_cast<int>(regValue);
    char registerValue[6];
    snprintf(registerValue, sizeof(registerValue), "%04Xh", tempValue);
    GuiLabel(Rectangle{(float)x, (float)y, 20, 15}, label);
    GuiTextBox(Rectangle{(float)x + 20, (float)y, 40, 15}, registerValue, sizeof(registerValue), editable);
}

void EmulatorWindow8080::guiFlagBox(float x, float y, const char* label, int flagValue, bool editable) {
    char fValue[2];
    snprintf(fValue, sizeof(flagValue), "%d", flagValue);
    GuiLabel(Rectangle{(float)x, (float)y, 20, 15}, label);
    GuiTextBox(Rectangle{(float)x + 20, (float)y, 40, 15}, fValue, sizeof(fValue), editable);
}

void EmulatorWindow8080::renderMemoryBox(uint16_t startAddress) {
    float posY = buttonsY + 40;
    GuiGroupBox(Rectangle{20, posY, WINDOW_WIDTH - 40, 350}, "Memory Dump");

    // every single byte
    for(int i=0; i<=0xF; i++) {
        char colLabel[3];
        snprintf(colLabel, sizeof(colLabel), "%02Xh", i);
        float posX = 100 + i*50;
        DrawTextEx(font, colLabel, Vector2{posX, posY + 10}, 18, 1, WHITE);
    }

    // every 16 bytes
    for(int i=0; i<=0xF; i++) {
        char rowLabel[5];
        snprintf(rowLabel, sizeof(rowLabel), "%04Xh", startAddress + i*16);
        float posX = 30;
        float posY = buttonsY + 80 + i*20;
        DrawTextEx(font, rowLabel, Vector2{posX, posY}, 18, 1, WHITE);
    }

    // memory dump
    for(int i=0; i<=0xF; i++) {
        for(int j=0; j<=0xF; j++) {
            uint16_t pc = startAddress + i*16 + j;
            if(pc == cpu->getRegisters().PC) {
                DrawRectangle(100 + j*50, buttonsY + 80 + i*20, 30, 15, RED);
            }
            auto color = WHITE;
            if(j % 2 == 0) color = LIGHTGRAY;
            char memLabel[3];
            snprintf(memLabel, sizeof(memLabel), "%02Xh", cpu->getMemory().read(startAddress + i*16 + j));
            float posX = 100 + j*50;
            float posY = buttonsY + 80 + i*20;
            DrawTextEx(font, memLabel, Vector2{posX, posY}, 16, 1, color);
        }
    }
}

void EmulatorWindow8080::renderAndHandleButtons() {
        if(GuiButton(Rectangle{20, buttonsY, 100, 30}, cpu->isRunning() ? "Stop" : "Start")) {
            cpu->setRunning(!cpu->isRunning());
        }
        if(GuiButton(Rectangle{130, buttonsY, 100, 30}, "Load PRG")) {
            QString fileName = QFileDialog::getOpenFileName(nullptr, "Load Program", ".", "Binary files (*.bin)");
            if(!fileName.isEmpty()) {
                std::ifstream file(fileName.toStdString(), std::ios::in | std::ios::binary);
                if(!file) {
                    QMessageBox::critical(nullptr, "Load Error", QString("Can not load program from file %1").arg(QString::fromStdString(fileName.toStdString())));
                    return;
                }

                StartAddressDialog dialog(nullptr, 0x0100);
                if(dialog.exec() == QDialog::Accepted) {
                    startAddressForLoadedProgram = dialog.getStartAddress();
                }

                file.seekg(0, std::ios::end);
                size_t fileSize = file.tellg();
                file.seekg(0, std::ios::beg);

                if(fileSize > Memory8080::RAM_SIZE - 0x0100) {
                    QMessageBox::critical(nullptr, "Load Error", QString("Program is too large to fit in memory"));
                    return;
                }

                std::vector<uint8_t> buffer(fileSize);
                file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
                if(cpu->loadProgram(startAddressForLoadedProgram, buffer)) {
                    cpu->getIO().terminal->printString("Loaded program from %s to address: %04Xh", fileName.toStdString().c_str(), startAddressForLoadedProgram);
                    cpu->getIO().terminal->printNewLine();
                } else {
                    QMessageBox::critical(nullptr, "Load Error", QString("Can not load program from file %1").arg(QString::fromStdString(fileName.toStdString())));
                }
            }
        }
        if(GuiButton(Rectangle{240, buttonsY, 100, 30}, "Load DSK")) {
            QString fileName = QFileDialog::getOpenFileName(nullptr, "Load Disk", ".", "Disk files (*.dsk)");
            if(!fileName.isEmpty()) {
                //cpu.disk.loadDiskFromFile(fileName.toStdString(), cpu.memory, 0x0100);
                //cpu->get
                cpu->getIO().terminal->printString("Loaded disk from %s\n", fileName.toStdString().c_str());
            }
        }
        if(GuiButton(Rectangle{350, buttonsY, 100, 30}, "Step >")) {
            cpu->step();
        }
        if(GuiButton(Rectangle{460, buttonsY, 100, 30}, "Reset")) {
            cpu->reset();
        }
        if(GuiButton(Rectangle{570, buttonsY, 100, 30}, "Dump CPU")) {
            cpu->consoleDump();
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
}