export module EEngine.Profiling;
import EEngine.Standard;

export namespace EEngine::Profiling {
	struct ProfileResult {
		const char* Name;
		float DurationMs;
		uint32_t CallCount;
	};

	struct FrameData {
		std::vector<ProfileResult> Results;
		float TotalFrameTime;
	};

	// Profiler - Unity-style in-app profiler
	export class Profiler {
	public:
		static Profiler& Get() {
			static Profiler instance;
			return instance;
		}

		void BeginFrame() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_CurrentFrame.Results.clear();
			m_CurrentFrame.TotalFrameTime = 0.0f;
			m_FrameStartTime = std::chrono::high_resolution_clock::now();
		}

		void EndFrame() {
			std::lock_guard<std::mutex> lock(m_Mutex);

			auto frameEnd = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - m_FrameStartTime);
			m_CurrentFrame.TotalFrameTime = duration.count() * 0.001f;

			// Store frame in history (circular buffer)
			m_FrameHistory[m_FrameHistoryIndex] = m_CurrentFrame;
			m_FrameHistoryIndex = (m_FrameHistoryIndex + 1) % m_FrameHistorySize;

			if (m_FrameCount < m_FrameHistorySize) {
				m_FrameCount++;
			}
		}

		void RecordProfile(const char* name, float durationMs) {
			std::lock_guard<std::mutex> lock(m_Mutex);

			// Find existing entry or create new one
			for (auto& result : m_CurrentFrame.Results) {
				if (std::strcmp(result.Name, name) == 0) {
					result.DurationMs += durationMs;
					result.CallCount++;
					return;
				}
			}

			// New entry
			m_CurrentFrame.Results.push_back({ name, durationMs, 1 });
		}

		const FrameData& GetCurrentFrame() const {
			return m_CurrentFrame;
		}

		const FrameData* GetFrameHistory() const {
			return m_FrameHistory;
		}

		uint32_t GetFrameHistorySize() const {
			return m_FrameHistorySize;
		}

		uint32_t GetFrameCount() const {
			return m_FrameCount;
		}

		uint32_t GetFrameHistoryIndex() const {
			return m_FrameHistoryIndex;
		}

		// Calculate averages over last N frames
		ProfileResult GetAverageResult(const char* name, uint32_t frameCount = 60) const {
			std::lock_guard<std::mutex> lock(m_Mutex);

			float totalDuration = 0.0f;
			uint32_t totalCalls = 0;
			uint32_t framesChecked = std::min(frameCount, m_FrameCount);

			for (uint32_t i = 0; i < framesChecked; i++) {
				uint32_t index = (m_FrameHistoryIndex - 1 - i + m_FrameHistorySize) % m_FrameHistorySize;
				const auto& frame = m_FrameHistory[index];

				for (const auto& result : frame.Results) {
					if (std::strcmp(result.Name, name) == 0) {
						totalDuration += result.DurationMs;
						totalCalls += result.CallCount;
						break;
					}
				}
			}

			return { name, totalDuration / framesChecked, totalCalls / framesChecked };
		}

		float GetAverageFPS(uint32_t frameCount = 60) const {
			std::lock_guard<std::mutex> lock(m_Mutex);

			float totalFrameTime = 0.0f;
			uint32_t framesChecked = std::min(frameCount, m_FrameCount);

			for (uint32_t i = 0; i < framesChecked; i++) {
				uint32_t index = (m_FrameHistoryIndex - 1 - i + m_FrameHistorySize) % m_FrameHistorySize;
				totalFrameTime += m_FrameHistory[index].TotalFrameTime;
			}

			float avgFrameTime = totalFrameTime / framesChecked;
			return avgFrameTime > 0.0f ? 1000.0f / avgFrameTime : 0.0f;
		}

	private:
		Profiler() {
			m_FrameHistory = new FrameData[m_FrameHistorySize];
		}

		~Profiler() {
			delete[] m_FrameHistory;
		}

		FrameData m_CurrentFrame;
		FrameData* m_FrameHistory;
		uint32_t m_FrameHistorySize = 120; // Store last 2 seconds at 60fps
		uint32_t m_FrameHistoryIndex = 0;
		uint32_t m_FrameCount = 0;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_FrameStartTime;
		mutable std::mutex m_Mutex;
	};

	// RAII timer for automatic profiling
	export class Timer {
	public:
		Timer(const char* name)
			: m_Name(name)
			, m_Stopped(false)
			, m_StartTimepoint(std::chrono::high_resolution_clock::now()) {
		}

		~Timer() {
			if (!m_Stopped) {
				Stop();
			}
		}

		void Stop() {
			auto endTimepoint = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimepoint - m_StartTimepoint);

			m_Stopped = true;

			float durationMs = duration.count() * 0.001f;
			Profiler::Get().RecordProfile(m_Name, durationMs);
		}

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	// Legacy compatibility
	using InstrumentationSession = Profiler;
}
