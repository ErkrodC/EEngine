#include "Layer.hpp"

namespace EEngine {
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName) {}

	Layer::~Layer() = default;
} // EEngine