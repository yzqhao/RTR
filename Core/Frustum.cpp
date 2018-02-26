
#include "Frustum.h"

namespace RTR
{
	Frustum::Frustum(CAMERA_TYPE mode, const vector3df &pos, const vector3df &dir,
		const vector3df& target, f32 nearZ, f32 farZ, f32 ffov,
		f32 viewportWidth, f32 viewportHeight)
		: camMode(mode), position(pos), direction(dir), camTarget(target),
		clip_z_near(nearZ), clip_z_far(farZ), fov(ffov),
		viewport_width(viewportWidth), viewport_height(viewportHeight),
		camUp(vector3df(0, 1, 0)), camRight(vector3df(1, 0, 0)), camLook(vector3df(0, 0, 1)),
		mWorldToCamera(Matrix()), mCameraToPerspective(Matrix()),
		mPerspectiveToScreen(Matrix())
	{
		viewport_center_X = (viewport_width - 1) * 0.5f;
		viewprot_center_Y = (viewport_height - 1) * 0.5f;

		aspect_ratio = viewport_width / viewport_height;

		viewplane_width = 2.0f;
		viewplane_height = 2.0f / aspect_ratio;

		f32 tan_fov_div2 = tan(Degree_TO_Radian(fov  * 0.5f));
		view_dist = 0.5f * viewplane_width / tan_fov_div2;

		// ���òü�ƽ��
		// �ü�ƽ�������ǻ������������ϵ��
		vector3df	point;
		// �Ҳü���
		vector3df	normalR = vector3df(-view_dist, 0, viewplane_width  * 0.5f);
		clip_plane_R = Plane3D(point, normalR);

		// ��ü���
		vector3df	normalL = vector3df(view_dist, 0, viewplane_width  * 0.5f);
		clip_plane_L = Plane3D(point, normalL);

		// �ϲü���
		vector3df	normalT = vector3df(0, -view_dist, viewplane_height  * 0.5f);
		clip_plane_T = Plane3D(point, normalT);

		// �²ü���
		vector3df	normalB = vector3df(0, view_dist, viewplane_height  * 0.5f);
		clip_plane_B = Plane3D(point, normalB);

		// ���������ϵ��͸������ϵ�任����, ��������任֮����Ҫ����w
		mPerspectiveToScreen = Matrix(
			view_dist, 0, 0, 0,
			0, view_dist * aspect_ratio, 0, 0,
			0, 0, 1, 1,
			0, 0, 0, 0);
	}
}
