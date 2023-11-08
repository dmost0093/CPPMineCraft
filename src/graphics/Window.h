#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window 
{
private:
	GLFWwindow* m_MainWindow;
	int m_BufferWidth, m_BufferHeight;
	int m_Width, m_Height;
	const char* m_Title; // Title for the window

	bool m_Keys[1024];
	float m_LastX, m_LastY;
	float m_XChange, m_YChange;
	bool m_MouseFirstMoved;

	float m_DeltaTime;
	float m_LastTime;
	float m_Now;

	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);

public:
	Window();
	Window(int windowWidth, int windowHeight, const char* title);
	~Window();

	int Initialize();

	int GetBufferWidth() { return m_BufferWidth; }
	int GetBufferHeight() { return m_BufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(m_MainWindow); }
	void SwapBuffer() { glfwSwapBuffers(m_MainWindow); }

	bool* GetKeys() { return m_Keys; }
	float GetXChange();
	float GetYChange();
	void CreateCallBack();
	void WindowLoop();
};