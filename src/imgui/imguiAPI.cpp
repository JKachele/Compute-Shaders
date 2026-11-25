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

void imgui_render(void) {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_shutdown(void) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
}

void imgui_debugPanel(struct imgui_data *data) {
        ImGui::Begin("Debug Panel");
        ImGui::Text("FPS: %.1f", data->fps);
        ImGui::Separator();
        ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", data->camX, data->camY, data->camZ);
        ImGui::Text("Yaw: %.1f  Pitch: %.1f", data->yaw, data->pitch);
        ImGui::Separator();
        ImGui::Text("Items: %d", data->numItems);
        ImGui::Text("Window: %.0fx%.0f", data->winWidth, data->winHeight);
        ImGui::End();
}
