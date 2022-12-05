#pragma once

namespace EEngine {
	class Timestep {
	public:
		explicit Timestep(float time = 0.0f)
			: m_Time(time) {

		}

		inline float GetSeconds() const { return m_Time; }
		inline float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}