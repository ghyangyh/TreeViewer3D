#ifndef _CAMERA_H
#define _CAMERA_H

/*
* This is an implementation of a FPS camera.
* Author: Yinhui Yang
* Zhejiang A&F University
*/

#include <Eigen/Dense>

class Camera {
public:
	Camera(const Eigen::Vector3f& aCamPos,
		const Eigen::Vector3f& aLookAtPos,
		const Eigen::Vector3f& aUpDir);
public:
    /*
     * Get the viewing matrix.
    */
	Eigen::Matrix4f get_view_mat();

	Eigen::Vector3f get_cam_position() const {
		return m_cam_pos;
	}

    /*
     * Camera positions movers.
    */
	void move_foward() {
		m_cam_pos = m_cam_pos + m_cam_move_speed * m_cam_front_dir;
	}
	void move_backward() {
		m_cam_pos = m_cam_pos - m_cam_move_speed * m_cam_front_dir;
	}
	void move_left() {
		m_cam_pos = m_cam_pos - m_cam_move_speed * m_cam_right_dir;
	}
	void move_right() {
		m_cam_pos = m_cam_pos + m_cam_move_speed * m_cam_right_dir;
	}

	// This function will only change the front direction of the camera
    // aPitchUnits: rotation units around the right direction
    // aYawUnits: rotation units around the up direction
	void update_orientation(float aPitchUnits, float aYawUnits);

	// Update the camera's front, right and up directions
	void update_camera_system();

    // Setters and Getters
    void set_camera_move_speed(float aSpeed) {
        m_cam_move_speed = aSpeed;
    }

    void set_camera_rotate_speed(float aSpeed) {
        m_cam_rotate_speed = aSpeed;
    }

    void set_camera_position(const Eigen::Vector3f& aCamPos) {
        m_cam_pos = aCamPos;
    }

    void set_camera_front_dir(const Eigen::Vector3f& aCamFrontDir) {
        m_cam_front_dir = aCamFrontDir;
    }

    void set_camera_right_dir(const Eigen::Vector3f& aCamRightDir) {
        m_cam_right_dir = aCamRightDir;
    }

    void set_camera_up_dir(const Eigen::Vector3f& aCamUpDir) {
        m_cam_up_dir = aCamUpDir;
    }

    const Eigen::Vector3f& get_cam_pos() const {
        return m_cam_pos;
    }
    const Eigen::Vector3f& get_cam_front_dir() const {
        return m_cam_front_dir;
    }
    const Eigen::Vector3f& get_cam_right_dir() const {
        return m_cam_right_dir;
    }
    const Eigen::Vector3f& get_cam_up_dir() const {
        return m_cam_up_dir;
    }

private:
    Eigen::Vector3f m_cam_pos;          // the camera's position
    Eigen::Vector3f m_cam_front_dir;    // the camera's looking direction
    Eigen::Vector3f m_cam_right_dir;    // the camera's right direction
    Eigen::Vector3f m_cam_up_dir;       // the camera's up direction
    float m_yaw_angle;                  // the rotation angle around the up direction
    float m_pitch_angle;                // the rotation angle around the right direction
    float m_cam_move_speed;             // the camera's moving speed
    float m_cam_rotate_speed;           // the cmaera's rotating speed
};


#endif
