export module EEngine.Rendering:Camera;
import EEngine.Math;

export namespace EEngine {
	class Camera {
	public:
		explicit Camera(const Math::mat4& projection) : m_Projection(projection) { }

		void SetProjection(const Math::mat4& projection) { m_Projection = projection; }
		const Math::mat4& GetProjection() const { return m_Projection; }
	private:
		Math::mat4 m_Projection;
	};
}
