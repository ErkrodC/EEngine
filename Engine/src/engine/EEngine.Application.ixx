module;
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include <Profiling/Profiling.hpp>

export module EEngine.Application;
import EEngine.Core;
import EEngine.Event;
import EEngine.Math;
import EEngine.Profiling;
import EEngine.Rendering;
import EEngine.Standard;

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// Window Interface & Props
	// ============================================================================

	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		explicit WindowProps(
			const std::string& title = "EEngine",
			unsigned int width = 1280,
			unsigned int height = 720
		) : Title(title), Width(width), Height(height) {
		}
	};

	class IWindow {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~IWindow() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
	};

	// ============================================================================
	// Input Interface
	// ============================================================================

	class IInput {
	public:
		virtual bool IsKeyPressedImpl(KeyCode keyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseButtonCode button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	};

	// ============================================================================
	// Layer & LayerStack
	// ============================================================================

	class Layer {
	public:
		Layer(const std::string& name = "Layer") : m_DebugName(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep) {}
		virtual void OnIMGUIRender() {}
		virtual void OnEvent(Event&) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

	class LayerStack {
	public:
		LayerStack() = default;
		~LayerStack() = default;

		LayerStack(const LayerStack&) = delete;
		LayerStack& operator=(const LayerStack&) = delete;
		LayerStack(LayerStack&&) noexcept = default;
		LayerStack& operator=(LayerStack&&) noexcept = default;

		void PushLayer(Ref<Layer> layer) {
			m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
			++m_LayerInsertIndex;
		}

		void PushOverlay(Ref<Layer> overlay) {
			m_Layers.emplace_back(std::move(overlay));
		}

		void PopLayer(Layer* layer) {
			const auto it = std::ranges::find_if(
				m_Layers,
				[layer](const Ref<Layer>& ptr) { return ptr.get() == layer; }
			);

			if (it != m_Layers.end()) {
				m_Layers.erase(it);
				--m_LayerInsertIndex;
			}
		}

		void PopOverlay(Layer* overlay) {
			const auto it = std::ranges::find_if(
				m_Layers,
				[overlay](const Ref<Layer>& ptr) { return ptr.get() == overlay; }
			);

			if (it != m_Layers.end()) {
				m_Layers.erase(it);
			}
		}

		class Iterator {
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = Layer*;
			using difference_type = std::ptrdiff_t;
			using pointer = Layer**;
			using reference = Layer*&;

			explicit Iterator(std::vector<Ref<Layer>>::iterator it) : m_It(it) {}

			Layer* operator*() const { return m_It->get(); }
			Layer* operator->() const { return m_It->get(); }
			Iterator& operator++() { ++m_It; return *this; }
			Iterator operator++(int) { Iterator tmp = *this; ++m_It; return tmp; }
			Iterator& operator--() { --m_It; return *this; }
			Iterator operator--(int) { Iterator tmp = *this; --m_It; return tmp; }
			bool operator==(const Iterator& other) const { return m_It == other.m_It; }
			bool operator!=(const Iterator& other) const { return m_It != other.m_It; }

		private:
			std::vector<Ref<Layer>>::iterator m_It;
		};

		Iterator begin() { return Iterator(m_Layers.begin()); }
		Iterator end() { return Iterator(m_Layers.end()); }

	private:
		std::vector<Ref<Layer>> m_Layers;
		uint32_t m_LayerInsertIndex = 0;
	};

	// ============================================================================
	// ImGui Layer
	// ============================================================================

	class IMGUILayer : public Layer {
	public:
		IMGUILayer(Shared<IWindow> window) : Layer("IMGUILayer"), m_Window(window) {}
		~IMGUILayer() = default;

		void OnAttach() override {
#if defined(IMGUI_IMPL_OPENGL_ES2)
			const char* glsl_version = "#version 100";
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
			const char* glsl_version = "#version 150";
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
			const char* glsl_version = "#version 130";
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			ImGui::StyleColorsDark();

			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			auto window = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init(glsl_version);
		}

		void OnDetach() override {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void OnIMGUIRender() override {
		}

		void Begin() {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void End() {
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2((float)m_Window->GetWidth(), (float)m_Window->GetHeight());

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}
	private:
		float m_Time = 0.0f;
		Shared<IWindow> m_Window;
	};

	// ============================================================================
	// Camera Controller
	// ============================================================================

	class CameraController {
	public:
		explicit CameraController(float aspectRatio);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& event);

		const Camera& GetCamera() { return m_Camera; }
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		float m_MoveSpeed = 1.0f;
		float m_ZoomSpeed = 0.05f;
		Camera m_Camera;

		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
	};
}

// ============================================================================
// Platform-specific Implementations (Windows)
// ============================================================================

namespace EEngine {
#if WIN32
	inline int EngineToGLFWKeyCode(KeyCode engineKeyCode) { return (int)engineKeyCode; }
	inline int EngineToGLFWMouseButtonCode(MouseButtonCode engineMouseButtonCode) { return (int)engineMouseButtonCode; }
	inline KeyCode GLFWToEngineKeyCode(int glfwKeyCode) { return (KeyCode)glfwKeyCode; }
	inline MouseButtonCode GLFWToEngineMouseButtonCode(int glfwMouseButtonCode) { return (MouseButtonCode)glfwMouseButtonCode; }
#endif
}

export namespace EEngine {
	class WindowsInput : public IInput {
	public:
		void SetWindow(void* window) {
			m_Window = static_cast<IWindow*>(window);
		}

	protected:
		bool IsKeyPressedImpl(KeyCode keyCode) override  {
			if (!m_Window) { return false; }
			auto glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());
			auto state = glfwGetKey(glfwWindow, EngineToGLFWKeyCode(keyCode));

			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}

		bool IsMouseButtonPressedImpl(MouseButtonCode mouseButtonCode) override {
			if (!m_Window) { return false; }
			auto glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());
			auto state = glfwGetMouseButton(glfwWindow, EngineToGLFWMouseButtonCode(mouseButtonCode));

			return state == GLFW_PRESS;
		}

		std::pair<float, float> GetMousePositionImpl() override {
			if (!m_Window) return { 0.0f, 0.0f };
			auto glfwWindow = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());
			double x, y;
			glfwGetCursorPos(glfwWindow, &x, &y);

			return { (float)x, (float)y };
		}

		float GetMouseXImpl() override {
			auto[x, y] = GetMousePositionImpl();
			return x;
		}

		float GetMouseYImpl() override {
			auto[x, y] = GetMousePositionImpl();
			return y;
		}

	private:
		IWindow* m_Window = nullptr;
	};

	class WindowsWindow : public IWindow {
	public:
		explicit WindowsWindow(const WindowProps& props) {
			Initialize(props);
		}

		~WindowsWindow() override {
			Shutdown();
		}

		void OnUpdate() override {
			glfwPollEvents();
			if (m_Context) { m_Context->SwapBuffers(); }
		}

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		void* GetNativeWindow() const override {
			return m_Window;
		}

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

		void SetVSync(bool enabled) override {
			if (enabled) {
				glfwSwapInterval(1);
			} else {
				glfwSwapInterval(0);
			}

			m_Data.VSync = enabled;
		}

		bool IsVSync() const override {
			return m_Data.VSync;
		}
	private:
		inline static bool s_GLFWInitialized = false;

		GLFWwindow* m_Window{};
		IGraphicsContext* m_Context{};

		virtual void Initialize(const WindowProps& props) {
			m_Data.Title = props.Title;
			m_Data.Width = props.Width;
			m_Data.Height = props.Height;

			if (!s_GLFWInitialized) {
				int success = glfwInit();
				Log::CoreAssert(success, "Could not initialize GLFW!");
				glfwSetErrorCallback([](int error, const char* description) {
					Log::CoreError("GLFW Error {0}: {1}", error, description);
				});

				s_GLFWInitialized = true;
			}

			m_Window = glfwCreateWindow(
				(int)props.Width,
				(int)props.Height,
				m_Data.Title.c_str(),
				nullptr,
				nullptr
			);

			m_Context = new OpenGLContext(m_Window);
			m_Context->Initialize();
			Log::CoreInfo("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

			glfwSetWindowUserPointer(m_Window, &m_Data);
			SetVSync(true);

			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;
				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int glfwKeyCode, int, int action, int) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyCode engineKeyCode = GLFWToEngineKeyCode(glfwKeyCode);
				switch (action) {
					case GLFW_PRESS: {
						KeyPressedEvent event(engineKeyCode, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE: {
						KeyReleasedEvent event(engineKeyCode);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT: {
						KeyPressedEvent event(engineKeyCode, 1);
						data.EventCallback(event);
						break;
					}
					default: break;
				}
			});

			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int glfwKeyCode) {
				WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
				KeyTypedEvent event(GLFWToEngineKeyCode((int)glfwKeyCode));
				data.EventCallback(event);
			});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int glfwMouseButtonCode, int action, int ) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseButtonCode engineMouseButtonCode = GLFWToEngineMouseButtonCode(glfwMouseButtonCode);
				switch (action) {
					case GLFW_PRESS: {
						MouseButtonPressedEvent event(engineMouseButtonCode);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE: {
						MouseButtonReleasedEvent event(engineMouseButtonCode);
						data.EventCallback(event);
						break;
					}
					default: break;
				}
			});

			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
		}

		virtual void Shutdown() {
			glfwDestroyWindow(m_Window);
		}

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

// ============================================================================
// Input Facade
// ============================================================================

namespace EEngine::Input {
	inline std::unique_ptr<IInput>& GetInstance() {
		static std::unique_ptr<IInput> instance = nullptr;

		if (!instance) {
#ifdef EE_PLATFORM_WINDOWS
			instance = std::make_unique<WindowsInput>();
#endif
		}

		return instance;
	}

	export inline void SetInputInstance(std::unique_ptr<IInput> instance) { GetInstance() = std::move(instance); }
	export inline void SetWindow(void* window) {
#ifdef EE_PLATFORM_WINDOWS
		if (auto* windowsInput = dynamic_cast<WindowsInput*>(GetInstance().get())) {
			windowsInput->SetWindow(window);
		}
#endif
	}
	export inline bool IsKeyPressed(KeyCode keyCode) { return GetInstance()->IsKeyPressedImpl(keyCode); }
	export inline bool IsMouseButtonPressed(MouseButtonCode mouseButtonCode) { return GetInstance()->IsMouseButtonPressedImpl(mouseButtonCode); }
	export inline std::pair<float, float> GetMousePosition() { return GetInstance()->GetMousePositionImpl(); }
	export inline float GetMouseX() { return GetInstance()->GetMouseXImpl(); }
	export inline float GetMouseY() { return GetInstance()->GetMouseYImpl(); }
}

// ============================================================================
// Window Creation (TBD namespace for platform abstraction)
// ============================================================================

export namespace EEngine::TBD {
	IWindow* CreateWindow(const WindowProps& props = WindowProps()) {
		return new WindowsWindow(props);
	}
}

// ============================================================================
// Application
// ============================================================================

export namespace EEngine {
	class Application {
	public:
		Application() {
			Log::CoreInfo("Selected Renderer API: {}", Renderer::GetRendererAPIString(Renderer::GetSelectedAPI()));
			Log::CoreAssert(!s_Instance, "Multiple applications created.");

			s_Instance = this;

			m_Window.reset(TBD::CreateWindow());
			m_Window->SetEventCallback([this](auto& event) -> void { OnEvent(event); });

			Input::SetWindow(static_cast<void*>(m_Window.get()));

			Renderer::Initialize();
			Renderer2D::Initialize();

			auto imguiLayer = MakeRef<IMGUILayer>(m_Window);
			m_IMGUILayer = imguiLayer.get();
			PushOverlay(std::move(imguiLayer));
		}

		virtual ~Application() = default;

		void OnEvent(Event& event) {
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<WindowCloseEvent>([this](auto& event) -> bool { return OnWindowClose(event); });
			dispatcher.Dispatch<WindowResizeEvent>([this](auto& event) -> bool { return OnWindowResized(event); });

			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
				(*--it)->OnEvent(event);
				if (event.Handled) { break; }
			}
		}

		void Run() {
			while (m_Running) {
				Profiling::Profiler::Get().BeginFrame();

				auto time = (float)glfwGetTime();
				Timestep timestep {
					time - m_LastFrameTime
				};
				m_LastFrameTime = time;

				if (!m_Minimized) {
					for (Layer* layer: m_LayerStack) {
						layer->OnUpdate(timestep);
					}
				}

				{
					m_IMGUILayer->Begin();
					for (Layer* layer: m_LayerStack) {
						layer->OnIMGUIRender();
					}
					m_IMGUILayer->End();
				}

				m_Window->OnUpdate();

				Profiling::Profiler::Get().EndFrame();
			}
		}

		void PushLayer(Ref<Layer> layer) {
			layer->OnAttach();
			m_LayerStack.PushLayer(std::move(layer));
		}

		void PushOverlay(Ref<Layer> overlay) {
			overlay->OnAttach();
			m_LayerStack.PushOverlay(std::move(overlay));
		}

		static inline Application& Get() { return *s_Instance; }
		inline IWindow& GetWindow() const { return *m_Window; }
	private:
		static inline Application* s_Instance = nullptr;

		Shared<IWindow> m_Window;
		IMGUILayer* m_IMGUILayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float  m_LastFrameTime;

		bool OnWindowClose(WindowCloseEvent& event) {
			m_Running = false;
			return true;
		}

		bool OnWindowResized(WindowResizeEvent& event) {
			if (event.GetWidth() == 0 || event.GetHeight() == 0) {
				m_Minimized = true;
				return false;
			}

			m_Minimized = false;
			Renderer::OnWindowResized(event.GetWidth(), event.GetHeight());

			return true;
		}
	};

	// To be defined by consumer
	Application* CreateApplication();
}

// ============================================================================
// CameraController Implementation
// ============================================================================

namespace EEngine {
	CameraController::CameraController(float aspectRatio)
		: m_AspectRatio(aspectRatio)
		, m_Camera(Math::ortho(
			-m_AspectRatio * m_ZoomLevel,
			m_AspectRatio * m_ZoomLevel,
			-m_ZoomLevel,
			m_ZoomLevel)
	) {}

	void CameraController::OnUpdate(Timestep timestep) {
		{
			EE_PROFILE_SCOPE("Camera Movement");
			float deltaDist = m_MoveSpeed * timestep.GetSeconds();

			if (Input::IsKeyPressed(KeyCode::Up) || Input::IsKeyPressed(KeyCode::W)) {
				const auto& cameraPos = m_Camera.GetPosition();
				m_Camera.SetPosition({ cameraPos.x, cameraPos.y + deltaDist, cameraPos.z });
			}

			if (Input::IsKeyPressed(KeyCode::Left) || Input::IsKeyPressed(KeyCode::A)) {
				const auto& cameraPos = m_Camera.GetPosition();
				m_Camera.SetPosition({ cameraPos.x - deltaDist, cameraPos.y, cameraPos.z });
			}

			if (Input::IsKeyPressed(KeyCode::Down) || Input::IsKeyPressed(KeyCode::S)) {
				const auto& cameraPos = m_Camera.GetPosition();
				m_Camera.SetPosition({ cameraPos.x, cameraPos.y - deltaDist, cameraPos.z });
			}

			if (Input::IsKeyPressed(KeyCode::Right) || Input::IsKeyPressed(KeyCode::D)) {
				const auto& cameraPos = m_Camera.GetPosition();
				m_Camera.SetPosition({ cameraPos.x + deltaDist, cameraPos.y, cameraPos.z });
			}
		}
	}

	void CameraController::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>([this](auto& event) -> bool { return OnMouseScrolled(event); });
		dispatcher.Dispatch<WindowResizeEvent>([this](auto& event) -> bool { return OnWindowResized(event); });
		dispatcher.Dispatch<MouseMovedEvent>([this](auto& event) -> bool { return OnMouseMoved(event); });
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& event) {
		m_ZoomLevel -= event.GetYOffset() * m_ZoomSpeed;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.01f);

		m_Camera.SetProjection(Math::ortho(
			-m_AspectRatio * m_ZoomLevel,
			m_AspectRatio * m_ZoomLevel,
			-m_ZoomLevel,
			m_ZoomLevel
		));

		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& event) {
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(Math::ortho(
			-m_AspectRatio * m_ZoomLevel,
			m_AspectRatio * m_ZoomLevel,
			-m_ZoomLevel,
			m_ZoomLevel
		));

		return false;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& event) {
		bool isCtrlPressed = Input::IsKeyPressed(KeyCode::LeftControl)
			|| Input::IsKeyPressed(KeyCode::RightControl);

		if (isCtrlPressed && Input::IsMouseButtonPressed(MouseButtonCode::Mouse2)) {
			// ER TODO camera rotation still broken
		}

		return false;
	}
}
