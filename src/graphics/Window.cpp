
#include <iostream>
#include "Window.h"


Window::Window()
	:m_Width{ 800 }, m_Height{ 600 },
	m_MouseFirstMoved{ true }, m_Title{ "title" }
{
	for (int i = 0; i < 1024; i++)
	{
		m_Keys[i] = false;
	}
}

Window::Window(int windowWidth, int windowHeight, const char* title)
	:m_Width{ windowWidth }, m_Height{ windowHeight }, 
	m_MouseFirstMoved{ true }, m_Title{ title }
{
	for (int i = 0; i < 1024; i++) 
	{
		m_Keys[i] = false;
	}
}

Window::~Window()
{
	glfwDestroyWindow(m_MainWindow);
	glfwTerminate();
}

int Window::Initialize()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Using openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_MainWindow = glfwCreateWindow(m_Width, m_Height, "Hello World", NULL, NULL);
	if (!m_MainWindow)
	{
		glfwTerminate();
		return -1;
	}

	glfwGetFramebufferSize(m_MainWindow, &m_BufferWidth, &m_BufferHeight);

	/* Make the window's context current */
	glfwMakeContextCurrent(m_MainWindow);

	glfwSwapInterval(1); // 1-> refresh rate = vsync rate

	// after glfwMakeContextCurrent is been called, glewInit should call
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error\n";
	}

	glViewport(0, 0, m_BufferWidth, m_BufferHeight);

	std::cout << glGetString(GL_VERSION) << std::endl; // Print the current version of platform

	// Create the callback
	CreateCallback();
	glfwSetInputMode(m_MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Set the invisible mouse cursor
	glfwSetWindowUserPointer(m_MainWindow, this);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return 0;
}

float Window::GetXChange()
{
	float theChange = m_XChange;
	m_XChange = 0.0f;
	return theChange;
}

float Window::GetYChange()
{
	float theChange = m_YChange;
	m_YChange = 0.0f;
	return theChange;
}

void Window::CreateCallback()
{
	glfwSetKeyCallback(m_MainWindow, HandleKeys);
	glfwSetCursorPosCallback(m_MainWindow, HandleMouse);
}

void Window::WindowLoop()
{
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{// case the pressing the ESC key -> close the window
		glfwSetWindowShouldClose(window, true);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{ // case certain key pressed->set that key as true
			windowObject->m_Keys[key] = true;
			//std::cout << "Pressed: " << key << std::endl;
		}
		else if (action == GLFW_RELEASE)
		{ // case certain key released-> set that key as false
			windowObject->m_Keys[key] = false;
			//std::cout<< "Release: " << key << std::endl;
		}
	}
}

void Window::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* windowObject = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (windowObject->m_MouseFirstMoved)// First time start the moving the mouse
	{
		// save the coordinate of the mouse
		windowObject->m_LastX = xPos;
		windowObject->m_LastY = yPos;
		windowObject->m_MouseFirstMoved = false;
	}
	else // keep moving the mouse
	{
		// change = mouse coordinate of the last frame - mouse coordinate of  the current frame
		windowObject->m_XChange = xPos - windowObject->m_LastX;
		windowObject->m_YChange = windowObject->m_LastY - yPos;

		// Update the current loaction with last location of the mouse
		windowObject->m_LastX = xPos;
		windowObject->m_LastY = yPos;

		//std::cout << "XChange : " << windowObject->m_XChange <<
		//	" YChange : " << windowObject->m_YChange << std::endl;
	}
}
