#pragma once
#include "GLFW/glfw3.h"
#include "string"
#include "lve_window.hpp"
namespace lve {
    class LveWindow {
    public:
        LveWindow(int w, int h, const std::string& name);
        LveWindow(LveWindow&)=delete;
        LveWindow& operator=(const LveWindow&) = delete;
        ~LveWindow();

        bool shouldClose() { return glfwWindowShouldClose(window); }
        GLFWwindow* getWindow() { return window; }
    private:
      void  initWindow();
    private:
        int width;
        int height;
        std::string windowName;

        GLFWwindow* window;
    };
    
}