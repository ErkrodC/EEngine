export module EEngine.Core:Memory;
import EEngine.Standard;

export namespace EEngine {
	// ============================================================================
	// Smart Pointer Aliases
	// ============================================================================

	// Ref<T> = Reference with exclusive ownership (wraps std::unique_ptr)
	// Use for single-owner resources (most common case)
	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename TType, typename TDeleter>
	using UniqueD = std::unique_ptr<TType, TDeleter>;

	// Shared<T> = Reference with shared ownership (wraps std::shared_ptr)
	// Use when multiple owners need to keep resource alive
	template<typename T>
	using Shared = std::shared_ptr<T>;

	// MakeRef - construct a new object with exclusive ownership
	template<typename T, typename... Args>
	constexpr Unique<T> MakeUnique(Args &&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	// wrap an existing raw pointer
	template<typename T, typename Deleter>
	constexpr UniqueD<T, Deleter> MakeUniqueFromRaw(T* ptr, Deleter deleter) {
		return std::unique_ptr<T, Deleter>(ptr, deleter);
	}

	// MakeShared - construct a new object with shared ownership
	template<typename T, typename... Args>
	constexpr Shared<T> MakeShared(Args &&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// wrap an existing raw pointer
	template<typename T, typename Deleter>
	constexpr Shared<T> MakeSharedFromRaw(T* ptr, Deleter deleter) {
		return std::shared_ptr<T>(ptr, deleter);
	}
}
