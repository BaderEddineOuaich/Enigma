#include <Epch.hpp>
#include "Window.hpp"


NS_ENIGMA_BEGIN
/* Static Functions Begin */
static void GLFWErrorCallback(int error, const char* message)
{
	ENIGMA_CORE_ERROR("GFLW ERROR #{}: {}", error, message);
}

static void GLAPIENTRY OpenGLDebugCallBack(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	///----- Filter debug msg -----///
	
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	String output("");

	output += "OpenGL Debug Message:\n";
	output += "Debug message (" + std::to_string(id) + "): " + message + "\n";

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:				output += "Source: API";				break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		output += "Source: Window System";		break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	output += "Source: Shader Compiler";	break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		output += "Source: Third Party";		break;
		case GL_DEBUG_SOURCE_APPLICATION:		output += "Source: Application";		break;
		case GL_DEBUG_SOURCE_OTHER:				output += "Source: Other";				break;
	}

	output += "\n";

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               output += "Type: Error";				break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: output += "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  output += "Type: Undefined Behaviour";	break;
		case GL_DEBUG_TYPE_PORTABILITY:         output += "Type: Portability";			break;
		case GL_DEBUG_TYPE_PERFORMANCE:         output += "Type: Performance";			break;
		case GL_DEBUG_TYPE_MARKER:              output += "Type: Marker";				break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          output += "Type: Push Group";			break;
		case GL_DEBUG_TYPE_POP_GROUP:           output += "Type: Pop Group";			break;
		case GL_DEBUG_TYPE_OTHER:               output += "Type: Other";				break;
	}

	output += "\n";

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:			
		{
			output += "Severity: High";
			ENIGMA_CORE_ERROR(output);
			break;
		}
		case GL_DEBUG_SEVERITY_MEDIUM:
		{
			output += "Severity: Medium";
			ENIGMA_CORE_WARN(output);
			break;
		}
		case GL_DEBUG_SEVERITY_LOW:				
		{
			output += "Severity: Low";	
			ENIGMA_CORE_INFO(output);
			break;
		}
		case GL_DEBUG_SEVERITY_NOTIFICATION:
		{
			output += "Severity: Notification";	
			ENIGMA_CORE_INFO(output);
			break;
		}
	}

	
	/*
	if (severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH)
	ENIGMA_CORE_WARN
	(
		"OpenGL Debug\n"
		"\t type: {}\n"
		"\t id: {}\n"
		"\t severity: {}\n"
		"\t length: {}\n"
		"\t message: {}\n",
		 type, id, severity, length, message);
	*/
}
/* Static Functions End */

Window::Window(const WindowSettings& window_settings)
	:
	m_title(window_settings.title),
	m_position(0, 0),
	m_size(window_settings.width, window_settings.height),
	m_minimum_size(window_settings.minimum_width, window_settings.minimum_height),
	m_maximum_size(window_settings.maximum_width, window_settings.maximum_height),
	m_frame_buffer_size(0, 0),
	m_vsync(window_settings.is_vsync),
	m_GLFWwindow(nullptr),
	m_monitor(nullptr),
	m_video_mode(nullptr),
	m_cursor(nullptr),
	m_OpenGL_version(window_settings.gl_major_version, window_settings.gl_minor_version)
{
	if (!InitGLFW(window_settings))
		throw std::exception("Failed to Initialize GLFW");
	if (!InitGLFWCallbacks())
		throw std::exception("Failed to Initialize GLFW Callbacks");
	if (!InitGLAD())
		throw std::exception("Failed to Initialize GLAD");
	if (!InitOpenGLOptions())
		throw std::exception("Failed to Initialize OpenGL Options");
}


bool Window::InitGLFW(const WindowSettings& window_settings)
{
	//INIT GLFW
	if (glfwInit() != GLFW_TRUE)
		return false;

	/// Set error callback
	glfwSetErrorCallback(GLFWErrorCallback);

	/// Set modern opengl core profile (which enables us to use latest funcs like debugging...)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/// Set window hints
	//OpenGL version
	const auto& [major, minor] = m_OpenGL_version;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(major));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(minor));

	glfwWindowHint(GLFW_RESIZABLE, window_settings.is_resizable);
	glfwWindowHint(GLFW_DECORATED, window_settings.is_decorated);
	glfwWindowHint(GLFW_FOCUSED, window_settings.is_focused);
	glfwWindowHint(GLFW_MAXIMIZED, window_settings.is_maximized);
	glfwWindowHint(GLFW_FLOATING, window_settings.is_floating);
	glfwWindowHint(GLFW_VISIBLE, window_settings.is_visible);
	glfwWindowHint(GLFW_AUTO_ICONIFY, window_settings.is_auto_iconify);
	glfwWindowHint(GLFW_REFRESH_RATE, window_settings.refresh_rate);
	glfwWindowHint(GLFW_SAMPLES, window_settings.samples);


#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#endif
#ifdef ENIGMA_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif


	//Create window
	m_GLFWwindow = glfwCreateWindow(window_settings.width, window_settings.height, window_settings.title.c_str(), nullptr, nullptr);
	if (!m_GLFWwindow)
	{
		this->Destroy();
		return false;
	} 
	//Set min & max size
	this->SetMinimumSize(window_settings.minimum_width, window_settings.minimum_height);
	this->SetMaximumSize(window_settings.maximum_width, window_settings.maximum_height);
	//Set fullscreen mode if enabled
	this->SetFullscreen(window_settings.is_fullscreen);

	//MUST BE RIGHT AFTER WINDOW CREATED
	glfwMakeContextCurrent(m_GLFWwindow); // !! IMPORTANT FOR GLAD TO WORK !!

	//Enable/Disable vsync
	glfwSwapInterval(m_vsync);

	//Set this window as data ptr to access in callbacks to avoid static funcs headaches
	glfwSetWindowUserPointer(m_GLFWwindow, this);

	//Set position & size & framebuffersize after window created
	glfwGetWindowSize(m_GLFWwindow, &m_size.first, &m_size.second);
	glfwGetWindowPos(m_GLFWwindow, &m_position.first, &m_position.second);
	glfwGetFramebufferSize(m_GLFWwindow, &m_frame_buffer_size.first, &m_frame_buffer_size.second);

	return true;
}

bool Window::InitGLFWCallbacks()
{
	/// Set GLFW Callbacks ///
	if (!m_GLFWwindow)
		return false;

	
	//Window ReSize
	glfwSetWindowSizeCallback(m_GLFWwindow, [](GLFWwindow* window, int width, int height)
		{
			Window& this_window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
			if (!this_window.IsFullscreen())
			{
				auto& [w, h] = this_window.m_size;
				w = width;
				h = height;

				WindowResizeEvent event(width, height);
				this_window.m_event_callback(event);

			}
		});

	//Framebuffer ReSize
	glfwSetFramebufferSizeCallback(m_GLFWwindow, [](GLFWwindow* window, int fbw, int fbh)
		{
			Window& this_window = *static_cast<Window*>(glfwGetWindowUserPointer(window));

			auto& [w, h] = this_window.m_frame_buffer_size;
			w = fbw;
			h = fbh;

			FrameBufferResizeEvent event(fbw, fbh);
			this_window.m_event_callback(event);
		});

	//Window Move
	glfwSetWindowPosCallback(m_GLFWwindow, [](GLFWwindow* window, int x, int y)
		{
			Window& this_window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
			if (! this_window.IsFullscreen()) // fullscreen triggers a move event, ignore it.
			{
				auto& [win_x, win_y] = this_window.m_position;
				win_x = x;
				win_y = y;

				WindowMoveEvent event(x, y);
				this_window.m_event_callback(event);
			}

		});

	//Window Maximized
	glfwSetWindowMaximizeCallback(m_GLFWwindow, [](GLFWwindow* window, int maximized)
		{
			Window& this_window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
			WindowMaximizedEvent event(maximized == GLFW_TRUE);
			this_window.m_event_callback(event);
		});


	//Window Focus
	glfwSetWindowFocusCallback(m_GLFWwindow, [](GLFWwindow* window, int focused)
		{
			Window& this_window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
			if (focused)
				this_window.m_event_callback(WindowFocusEvent());
			else
				this_window.m_event_callback(WindowFocusLostEvent());
		});

	//Window Close
	glfwSetWindowCloseCallback(m_GLFWwindow, [](GLFWwindow* window)
		{
			Window& this_window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
			this_window.m_event_callback(WindowCloseEvent());
		});

	//Key
	glfwSetKeyCallback(m_GLFWwindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window& this_window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(static_cast<KeyCode>(key), 0);
					this_window.m_event_callback(event); 
				}
				break;
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(static_cast<KeyCode>(key));
					this_window.m_event_callback(event);
				}
				break;
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(static_cast<KeyCode>(key), 1);
					this_window.m_event_callback(event); 
				}
				break;
				default:
					ENIGMA_CORE_WARN("Unsupported Key Event {}", key);
					break;
			}
		});

	//Key Typed
	glfwSetCharCallback(m_GLFWwindow, [](GLFWwindow* window, unsigned int keycode)
		{
			Window& this_window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
			KeyTypedEvent event(static_cast<KeyCode>(keycode));
			this_window.m_event_callback(event);
		});


	//Mouse button 
	glfwSetMouseButtonCallback(m_GLFWwindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			Window& this_window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(static_cast<MouseCode>(button));
				this_window.m_event_callback(event); //dispatch event
			}
			break;
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
				this_window.m_event_callback(event); //dispatch event
			}
			break;
			default:
				ENIGMA_CORE_WARN("Unsupported Mouse Button Event {}", button);
				break;
			}
		});

	//Mouse Wheel Scroll
	glfwSetScrollCallback(m_GLFWwindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			Window& this_window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
			MouseWheelEvent event(static_cast<f32>(xOffset), static_cast<f32>(yOffset));
			this_window.m_event_callback(event); 
		});

	//Mouse Move
	glfwSetCursorPosCallback(m_GLFWwindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			Window& this_window = *static_cast<Window*>(glfwGetWindowUserPointer(window));
			MouseMotionEvent event(static_cast<f32>(xPos), static_cast<f32>(yPos));
			this_window.m_event_callback(event); //dispatch event
		});
	return true;
}

bool Window::InitGLAD()
{
	/* Init Glad OpenGL */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return false;
	return true;
}

bool Window::InitOpenGLOptions()
{
	//TODO: Require GL 4.3+

	glEnable(GL_DEPTH_TEST); // TO USE THE Z COORDINATE

	//DONT DRAW BACK FACES OF DRAWINGS, tell gl to cull back face
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glFrontFace(GL_CCW); // DRAW FRONT FACE DRAWINGS INDICES CLOCK WISE 

	glEnable(GL_BLEND); // ENABLE BLENDING

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // TEXTURES ALPHA BLENDING

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DRAW BACK AND FRONT OF DRAWINGS FILLED (USE GL_LINE OR LINES or other opts)

#if ENIGMA_DEBUG
	//Set OpenGL Debug CALLBACK for gl >= 4.3 http://docs.gl/gl4/glDebugMessageCallback
	const auto& [major, minor] = m_OpenGL_version;
	if (major >= 4 && minor >= 3)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLDebugCallBack, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	}
	else
	{
		ENIGMA_CORE_WARN("Ignoring glDebugMessageCallback() as it is not supported in this GL version, use gl >= 4.3");
		if (major < 4)
			ENIGMA_CORE_WARN("Enigma requires at least OpenGL version >= 4.+ to work properly");
	}

	/// Log OpenGL Info
	ENIGMA_CORE_INFO(
		"| OpenGL Info |\n"
		"\t\tManufacturer: {}\n"
		"\t\tRenderer: {}\n"
		"\t\tVersion: {}\n",
		glGetString(GL_VENDOR),
		glGetString(GL_RENDERER),
		glGetString(GL_VERSION)
	);
#endif


	return true;
}

void Window::PollEvents() const noexcept
{
	glfwPollEvents();
}

void Window::SwapBuffers() const noexcept
{
	glfwSwapBuffers(m_GLFWwindow);
}


const std::pair<i32, i32>& Window::GetSize() const noexcept
{
	return m_size;
}

const i32& Window::GetWidth() const noexcept
{
	return m_size.first;
}

const i32& Window::GetHeight() const noexcept
{
	return m_size.second;
}

const std::pair<i32, i32>& Window::GetFrameBufferSize() noexcept
{
	auto& [fbw, fbh] = m_frame_buffer_size;
	glfwGetFramebufferSize(m_GLFWwindow, &fbw, &fbh);
	return m_frame_buffer_size;
}

const i32& Window::GetFrameBufferWidth() noexcept
{
	const auto& [w, h] = GetFrameBufferSize();
	return w;
}

const i32& Window::GetFrameBufferHeight() noexcept
{
	const auto& [w, h] = GetFrameBufferSize(); 
	return h; 
}

f32 Window::GetAspectRatio() const noexcept
{
	const auto& [width, height] = m_size;
	if (width > 0 && height > 0)
		return static_cast<f32>(width) / static_cast<f32>(height);
	return 1.0f;
}

const GLFWwindow* Window::GetGLFWwindow() const noexcept
{
	return m_GLFWwindow;
}

GLFWwindow* Window::GetGLFWwindow() noexcept
{
	return m_GLFWwindow;
}

const String& Window::GetTitle() const noexcept
{
	return m_title;
}

const std::pair<i32, i32>& Window::GetMinimumSize() const noexcept
{
	return m_minimum_size;
}

const std::pair<i32, i32>& Window::GetMaximumSize() const noexcept
{
	return m_maximum_size;
}

const std::pair<i32, i32>& Window::GetPosition() const noexcept
{
	return m_position;
}

bool Window::ShouldClose() const noexcept
{
	return glfwWindowShouldClose(m_GLFWwindow) == GLFW_TRUE;
}

bool Window::IsMinimized() const noexcept
{
	return  glfwGetWindowAttrib(m_GLFWwindow, GLFW_MAXIMIZED) == GLFW_FALSE;
}

bool Window::IsFocused() const noexcept
{
	return glfwGetWindowAttrib(m_GLFWwindow, GLFW_FOCUSED) == GLFW_TRUE;
}

bool Window::IsResizable() const noexcept
{
	return glfwGetWindowAttrib(m_GLFWwindow, GLFW_RESIZABLE) == GLFW_TRUE;
}

bool Window::IsDecorated() const noexcept
{
	return glfwGetWindowAttrib(m_GLFWwindow, GLFW_DECORATED) == GLFW_TRUE;;
}

bool Window::IsHidden() const noexcept
{
	return glfwGetWindowAttrib(m_GLFWwindow, GLFW_VISIBLE) == GLFW_FALSE;
}

bool Window::IsVisible() const noexcept
{
	return glfwGetWindowAttrib(m_GLFWwindow, GLFW_VISIBLE) == GLFW_TRUE;
}

bool Window::IsMaximized() const noexcept
{
	return glfwGetWindowAttrib(m_GLFWwindow, GLFW_MAXIMIZED) == GLFW_TRUE;
}

const bool& Window::IsVSync() const noexcept
{
	return m_vsync;
}

bool Window::IsFullscreen() const noexcept
{
	return glfwGetWindowMonitor(m_GLFWwindow) != nullptr;
}

const i32& Window::GetRefreshRate() noexcept
{
	// Get resolution of monitor
	m_monitor = glfwGetPrimaryMonitor();
	m_video_mode = glfwGetVideoMode(m_monitor);
	return m_video_mode->refreshRate;
}

const std::pair<ui16, ui16>& Window::GetOpenGLVersion() const noexcept
{
	return m_OpenGL_version;
}


void Window::SetFullscreen(bool full_screen) noexcept
{
	if (this->IsFullscreen() == full_screen) return;

	// Get resolution of monitor
	m_monitor = glfwGetPrimaryMonitor();
	m_video_mode = glfwGetVideoMode(m_monitor);

	if (full_screen)
	{
		// switch to full screen
		glfwSetWindowMonitor(m_GLFWwindow, m_monitor, 0, 0, m_video_mode->width, m_video_mode->height, m_video_mode->refreshRate);
	}
	else
	{
		// backup window position and window size 
		// (Note: position and size are not updated by GLFW callback when fullscreen mode is on)
		const auto& [w, h] = this->GetSize();
		const auto& [x, y] = this->GetPosition();

		// restore last window size and position
		glfwSetWindowMonitor(m_GLFWwindow, nullptr, x, y, w, h, m_video_mode->refreshRate);
	}
}

void Window::SetRefreshRate(const i32& refresh_rate) noexcept
{
	glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
}

void Window::SetVsync(bool vsync) noexcept
{
	m_vsync = vsync;
	glfwSwapInterval(vsync);
}

void Window::SetEventCallback(const EventCallback& callback) noexcept
{
	m_event_callback = callback;
}

void Window::SetMinimumSize(const i32& minimum_width, const i32& minimum_height) noexcept
{
	auto& [min_w, min_h] = m_minimum_size;
	const auto& [max_w, max_h] = m_maximum_size;

	min_w = minimum_width;
	min_h = minimum_height;

	glfwSetWindowSizeLimits(m_GLFWwindow, min_w, min_h, max_w, max_h);
}

void Window::SetMaximumSize(const i32& maximum_width, const i32& maximum_height) noexcept
{
	const auto& [min_w, min_h] = m_minimum_size;
	auto& [max_w, max_h] = m_maximum_size;

	max_w = maximum_width;
	max_h = maximum_height;

	glfwSetWindowSizeLimits(m_GLFWwindow, min_w, min_h, max_w, max_h);
}

void Window::SetShouldClose(bool close) const noexcept
{
	glfwSetWindowShouldClose(m_GLFWwindow, close);
}

void Window::SetTitle(const String& title) noexcept
{
	// title - FPS: x
	const ui32& FPS = Application::GetInstance().GetFPS();
	glfwSetWindowTitle(m_GLFWwindow, (title + " - FPS: " + std::to_string(FPS)).c_str());
	m_title = title;
	
#if 0
	if (show_fps)
	{
		// title - FPS: x
		const ui32& FPS = Application::GetInstance().GetFPS();
		glfwSetWindowTitle(m_GLFWwindow, const_cast<const String&&>(title + " - FPS: " + std::to_string(FPS)).c_str());
	}
	else
	{
		// title
		glfwSetWindowTitle(m_GLFWwindow, title.c_str());
	}
	m_title = title;
#endif
}

void Window::SetPosition(const i32& x, const i32& y) const noexcept
{
	glfwSetWindowPos(m_GLFWwindow, x ,y);
}

void Window::SetIcon(const String& icon_path) noexcept
{
	i32 width, height, channels;
	stbi_set_flip_vertically_on_load(false);
	byte* pixels = stbi_load(icon_path.c_str(), &width, &height, &channels, 4);

	ENIGMA_CORE_ASSERT(pixels, "Failed to load image!");
	ENIGMA_CORE_ASSERT(channels == 4, "Icon must be RGBA");

	GLFWimage images[1];
	images[0].pixels = pixels;
	images[0].width = width;
	images[0].height = height;
	glfwSetWindowIcon(m_GLFWwindow, 1, images);

	stbi_image_free(pixels);
	pixels = nullptr;
}

void Window::SetCursor(CursorMode mode) noexcept
{
	m_cursor = Cursor::CreateUnique(mode);
	glfwSetCursor(m_GLFWwindow, m_cursor->GetGLFWCursor());
}

void Window::SetCursor(const String& image_path, const i32& xhot, const i32& yhot) noexcept
{
	m_cursor = Cursor::CreateUnique(image_path, xhot, yhot);
	glfwSetCursor(m_GLFWwindow, m_cursor->GetGLFWCursor());
}

void Window::DisableCursor() const noexcept
{
	glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::EnableCursor() const noexcept
{
	glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::HideCursor() const noexcept
{
	glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Window::SetCursorPosition(const f64& xPos, const f64& yPos) const noexcept
{
	glfwSetCursorPos(m_GLFWwindow, xPos, yPos);
}

void Window::Minimize() const noexcept
{
	glfwIconifyWindow(m_GLFWwindow);
}

void Window::Maximize() const noexcept
{
	glfwMaximizeWindow(m_GLFWwindow);
}

void Window::Restore() const noexcept
{
	glfwRestoreWindow(m_GLFWwindow);
}

void Window::Hide() const noexcept
{
	glfwHideWindow(m_GLFWwindow);
}

void Window::Show() const noexcept
{
	glfwShowWindow(m_GLFWwindow);
}

void Window::Focus() const noexcept
{
	glfwFocusWindow(m_GLFWwindow);
}


void Window::Destroy() noexcept
{
	if (m_cursor)
	{
		m_cursor->Destroy();
	}

	if (m_GLFWwindow)
	{
		glfwDestroyWindow(m_GLFWwindow);
		m_GLFWwindow = nullptr;
	}

	glfwTerminate();
}



Window::~Window()
{
	this->Destroy();
}


NS_ENIGMA_END