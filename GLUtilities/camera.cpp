#include "camera.h"
#include "transformation_3d.h"
#include "quaternion.h"

Camera::Camera(const Eigen::Vector3f& aCamPos,
	const Eigen::Vector3f& aLookAtPos,
	const Eigen::Vector3f& aUpDir) : m_cam_pos(aCamPos),
m_cam_front_dir(aLookAtPos-aCamPos),
m_cam_up_dir(aUpDir),
m_cam_move_speed(0.2f),
m_cam_rotate_speed(0.005f),
m_pitch_angle(0.f),
m_yaw_angle(0.f)
{
	update_camera_system();
}

void Camera::update_camera_system() {
	m_cam_front_dir.normalize();

	// Computer the right direction
	m_cam_right_dir = m_cam_front_dir.cross(m_cam_up_dir);
	m_cam_right_dir.normalize();

	// Update the up direction
	m_cam_up_dir = m_cam_right_dir.cross(m_cam_front_dir);
	m_cam_up_dir.normalize();
}

void Camera::update_orientation(float aPitchUnits, float aYawUnits) {
	// Rotate the front direction around the y axis
	Quaternion quat(aYawUnits * m_cam_rotate_speed, m_cam_up_dir);
	Eigen::Matrix4f rotation_mat = quat.toMatrix();
    m_cam_front_dir = rotation_mat.block(0, 0, 3, 3) * m_cam_front_dir;
	
    // Rotate the front direction around the x axis
	quat.reset(aPitchUnits * m_cam_rotate_speed, m_cam_right_dir);
	rotation_mat = quat.toMatrix();
	m_cam_front_dir = rotation_mat.block(0, 0, 3, 3) * m_cam_front_dir;

    // Update the camera coordinate system accroding to the new fornt direction
	update_camera_system();
}

Eigen::Matrix4f Camera::get_view_mat() {
    // A viweing matrix is a 4x4 matrix composed of a rotation and a translation
	Eigen::Matrix4f view_mat = Eigen::Matrix4f::Zero();
	view_mat.block(0, 0, 1, 3) = m_cam_right_dir.transpose();
	view_mat.block(1, 0, 1, 3) = m_cam_up_dir.transpose();
	view_mat.block(2, 0, 1, 3) = -m_cam_front_dir.transpose();
	view_mat(3, 3) = 1.f;
	view_mat = view_mat * translate(-m_cam_pos);
	return view_mat;
}
