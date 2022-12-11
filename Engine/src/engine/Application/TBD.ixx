export module EEngine.Application:TBD;
import :IWindow;
import :WindowsWindow;

export namespace EEngine::TBD {
	IWindow* CreateWindow(const WindowProps& props = WindowProps()) {
		return new WindowsWindow(props);
	}
}