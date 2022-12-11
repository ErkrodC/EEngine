module;
#include <memory>

export module EEngine.Core:Core;

export namespace EEngine {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}