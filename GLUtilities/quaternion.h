#ifndef _QUATERNION_H
#define _QUATERNION_H

/*
 * A Quaternion class to represent a rotation around an arbitary axis.
 * Author: Yinhui Yang
 * Zhejiang A&F University
 * yhyang@zafu.edu.cn
*/


#include "Eigen/Dense"

class Quaternion {
public:
	Quaternion();

	/* Construct a quaternion with the given four components
	*/
	Quaternion(float w, float x, float y, float z);

	/* Construct a unit quaternion with the given rotation angle and rotation direction
	*/
	Quaternion(float aRotRadians, Eigen::Vector3f& aRotDir);

    /*
     * The copy constructor
    */
	Quaternion(const Quaternion& aCopy);

    /*
     * The overloaded assignment operator
    */
	Quaternion& operator=(const Quaternion& aRhs);
public:
    /*
     * Reset the quaternion to a new rotating axis and the rotation angle.
    */
	void reset(float aRotRadians, Eigen::Vector3f& aRotDir);

    /*
     * Convert the quaternion to a 4x4 matrix.
    */
    Eigen::Matrix4f toMatrix() const;

    /*
     * Computer the norm of the quaternion
    */
    inline float norm() const {
		return m_quat_norm;
	}
public:
	/* The spherical linear interpolation between two quaternions.
	*/
	static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);

    /*
     * Multiply two quaternions by overloading the * operator.
    */
    friend Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
private:
	// a quaternion is represented by a 4d vector
	// m_quat(0) = cos(theta/2)
	// m_quat(1) = sin(theta/2)*x
	// m_quat(2) = sin(theta/2)*y
	// m_quat(3) = sin(theta/2)*z
	// the 3d vector (x, y, z) is the unit rotation direction
	// theta is the rotation angle in radians
	Eigen::Vector4f m_quat; 
	float m_quat_norm;
};



#endif
