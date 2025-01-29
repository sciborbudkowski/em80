#include "EmulatorWindow8080.h"
#include "AssetsLoader.h"

#include <iostream>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>

EmulatorWindow8080::~EmulatorWindow8080() {
    if(font.texture.id != 0) UnloadFont(font);
}

void EmulatorWindow8080::start() { cpu.reset(); }

void EmulatorWindow8080::update() { cpu.step(); }

void EmulatorWindow8080::start() {
    cpu.reset();

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

    cpu.setRunning(true);
    char prgPath[512] = "\0";

    while(!WindowShouldClose()) {
        if(IsKeyPressed(KEY_BACKSPACE)) {
            // TODO: Remove last character from terminal
        } else if(IsKeyPressed(KEY_ENTER)) {
            // TODO: Process command
        } else if(IsKeyPressed(KEY_SPACE)) {
            // TODO: Print space
        } else {
            std::string ch = "";
            for(int key=KEY_A; key<=KEY_Z; key++) {
                if(IsKeyPressed(key)) {
                    char chKey = (char)(key - KEY_A + 'a');
                    // TODO: Print character
                    ch = chKey;
                }
            }
            for(int key=KEY_ZERO; key<=KEY_NINE; key++) {
                if(IsKeyPressed(key)) {
                    char chKey = (char)(key - KEY_ZERO + '0');
                    // TODO: Print character
                    ch = chKey;
                }
            }
            // TODO: Set last command
        }

        BeginDrawing();
        ClearBackground(BLACK);

        float winWidth = GetScreenWidth();
        float termX = 20 + 80*9 + 20;
        float panelWidth = winWidth - termX;
        float pX = termX + panelWidth - 80;

        EndDrawing();
    }

    cpu.setRunning(false);
    UnloadFont(font);
    CloseWindow();
}