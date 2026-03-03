#pragma once

#include <Interface/ISerialisable.h>
#include <Interface/ITimestampProvider.h>
#include <Base/Object.h>
#include <CCCommon.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/common.hpp>
#include <memory>
#include <vector>
#include <Events/Event.h>

namespace CitrusCore
{
	class CITRUS_CORE_API Transform : public CitrusCore::Object, public ISerialisable {
	public:
		Transform();
		Transform(glm::mat4 mat);
		virtual ~Transform();

		/// SetTimestampProvider used by transforms to prevent unecessary updates per frame.
		static void SetTimestampProvider(std::shared_ptr<ITimestampProvider> provider) {
			m_timestampProvider = provider;
		}

		/// Sets the transforms position.
		void SetPosition(glm::vec3 Position);

		/// Sets the transforms position.
		void SetPosition(float x, float y, float z);

		/// Sets the transforms position.
		void SetPosition(float x, float y);

		/// Sets the rotation of the transform using euler angles(radians)
		void SetRotationFromEuler(glm::vec3 Rotation);

		/// Sets the rotation of the transform using euler angles(radians)
		void SetRotationFromEuler(double x, double y, double z);

		/// Sets the rotation using a glm quaternion.
		void SetRotation(glm::quat);

		/// Sets the scale of the transform
		void SetScale(glm::vec3 Scale);

		/// Moves the transform by the specified amount 
		void Move(glm::vec3 MoveVector);

		/// Roates the transform by the specified amount in euler angles
		void Rotate(glm::vec3 eulerAngle);

		/// Get the position of the transform relative to its parent, in local space
		glm::vec3 GetPosition();

		/// Get the position of the transform in 2d space relative to its parent
		glm::vec2 GetPosition2d();

		/// Get the position of the transform relative to its parent, in local space
		glm::vec3 GetLocalPosition();

		/// Get the world position of the transform
		glm::vec3 GetWorldPosition();

		/// Get the world rotation of the transform
		glm::quat GetWorldRotation();

		/// Get the forward vector for this transformation 
		glm::vec3 GetForward();

		/// Get the right vector for this transformation 
		glm::vec3 GetRight();

		/// Get the up vector for this transformation 
		glm::vec3 GetUp();

		/// Get the rotation in euler angles for this transformation, returns angle as radians.
		glm::vec3 GetEulerRotation();

		/// Get the rotation 
		glm::quat GetRotation();

		/// Get the world matrix read only
		const glm::mat4& GetWorldMatrix();

		void SetLocal(glm::mat4 mat);

		glm::mat4 GetLocal();

		static void MakeParent(std::shared_ptr<Transform> transform, std::shared_ptr<Transform> parent);

		/// Unparent child from transform
		std::shared_ptr<Transform> RemoveChild(Transform* child);

		/// Returns the parent transform of this transform, or nullptr if not set
		Transform* GetParentTransform() const;

		/// Does this transform have any children 
		const bool HasChildren() const { return !m_children.empty(); }

		/// Get a const reference to the children of this transform. 
		const std::vector<std::shared_ptr<Transform>>& GetChildren() const { return m_children; }

		/// Return the last modified datestamp 
		double GetDatestamp() const { return m_datestamp; }

		/// Event broadcaster, listeners called when transform is updated.
		Event<Transform*> OnTransformChanged;

		virtual void Serialise(StreamWriter& byteStream) override;
        virtual void Deserialise(StreamReader& byteStream) override;

	protected:
		/// Calculates the local transformation matrix from the position, rotation and scale
		void UpdateLocalTransform();

		/// Called to recalculate the absolute transformation within the world
		void UpdateGlobalTransform();

		std::shared_ptr<ITimestampProvider> GetTimestampProvider() const { return m_timestampProvider; }

	private:
		/// Vec3 position 
		glm::vec3 m_position = glm::vec3(0.0f);

		/// World position 
		glm::vec3 m_worldPosition = glm::vec3(0.0f);

		/// The transforms rotation as a quaternion
		glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		/// The transforms scale 
		glm::vec3 m_scale;

		/// The local transformation matrix, relative to parent
		glm::mat4 m_localTransform = glm::mat4(1.0f);

		/// The global transformation matrix, absolute position in world ( World Matrix ) 
		glm::mat4 m_globalTransform = glm::mat4(1.0f);;

		/// Datestamp when this transform was last calulcated, using the time this frame was started
		/// Used to determine if the trasform has already been calculated for the current frame
		double m_datestamp = 0;

		/// Datestamp last time was called.
		double m_lastTick = 0;

		/// Is the transform dirty and needs to be recalculated
		bool m_bDirty;

		/// The parent transform, nullptr if a parent is not present
		Transform* m_parent = nullptr;

		static std::shared_ptr<ITimestampProvider> m_timestampProvider;

		/// A list of all the children attached to this transform
		std::vector<std::shared_ptr<Transform>> m_children;
	};
}
