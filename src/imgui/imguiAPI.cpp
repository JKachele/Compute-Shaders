/*************************************************
 *File----------src/gfx/imgui_backend.cpp
 *Project-------Compute-Shaders
 *Author--------Justin Kachele
 *Created-------Wednesday Nov 23, 2026
 *License-------GNU GPL-3.0
 ************************************************/

#include <glad/glad.h>

#include "imguiAPI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void imgui_init(GLFWwindow *window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.Fonts->AddFontDefaultVector(); // Always loads ProggyForever
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");
}

void imgui_setMouse(bool capture) {
        ImGuiIO &io = ImGui::GetIO();
        if (capture) {
                io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        } else {
                io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
        }
}

void imgui_newFrame(void) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
}

void imgui_startRender(void) {
        ImGui::Begin("Debug Panel");
}

void imgui_finishRender(void) {
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_shutdown(void) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
}

// Wigdets
void imgui_seperator(void) {
        ImGui::Separator();
}

void imgui_text(const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ImGui::TextV(fmt, args);
        va_end(args);
}

void imgui_dragFloat(const char *label, float *v, float speed, float min, float max) {
        ImGui::DragFloat(label, v, speed, min, max);
}

