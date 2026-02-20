export module EEngine.Core:Core;
import EEngine.Standard;

export namespace EEngine {
	// Ref<T> = Reference with exclusive ownership (wraps std::unique_ptr)
	// Use for single-owner resources (most common case)
	template<typename T>
	using Ref = std::unique_ptr<T>;

	// SharedRef<T> = Reference with shared ownership (wraps std::shared_ptr)
	// Use when multiple owners need to keep resource alive
	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> MakeRef(Args &&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	constexpr Shared<T> MakeShared(Args &&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}