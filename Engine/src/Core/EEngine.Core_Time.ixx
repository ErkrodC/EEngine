export module EEngine.Core:Time;
import EEngine.Standard;

export namespace EEngine {
	// ============================================================================
	// Timestep
	// ============================================================================

	class Timestep {
	public:
		explicit Timestep(double_t time = 0.0f)
			: m_Time(time) {}

		inline double_t GetSeconds() const { return m_Time; }
		inline double_t GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		double_t m_Time;
	};
}
