#include<DxLib.h>
#include"debugcamera.h"
#include<math.h>

#define ZERO (VGet(0.0f,0.0f,0.0f))
static const float ROT_SPEED = 0.05f;
static const float MOVE_SPEED = 20.0f;

DebugCamera::DebugCamera()
{
	m_pos = ZERO;
	m_rot = ZERO;
}

DebugCamera::~DebugCamera()
{

}

void DebugCamera::Init()
{
	m_pos = VGet(0.0f, 100.0f, 0.0f);

	m_rot = VGet(0.0f, 0.0f, 0.0f);
}

void DebugCamera::Init(VECTOR camPos, VECTOR targetPos, VECTOR upVec)
{
	m_pos = camPos;

}




void DebugCamera::Step(VECTOR focus, float rota)
{

	//ÉJÉÅÉâÇÃâÒì]èàóù
	//è„â∫âÒì]
	if (CheckHitKey(KEY_INPUT_UP))
	{
		m_rot.x -= ROT_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		m_rot.x += ROT_SPEED;
	}
	//ç∂âEâÒì]
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		m_rot.y -= ROT_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		m_rot.y += ROT_SPEED;
	}

	if (CheckHitKey(KEY_INPUT_LSHIFT))
	{
		m_pos.y -= MOVE_SPEED;
	}
	if (CheckHitKey(KEY_INPUT_RSHIFT))
	{
		m_pos.y += MOVE_SPEED;
	}

	//ÉJÉÅÉâÇÃà⁄ìÆèàóù
	//ëOêi
	if (CheckHitKey(KEY_INPUT_W))
	{
		MATRIX rotX = MGetRotX(m_rot.x);
		MATRIX rotY = MGetRotY(m_rot.y);
		VECTOR defaultDir = VGet(0.0f, 0.0f, MOVE_SPEED);
		MATRIX dir = MGetTranslate(defaultDir);
		MATRIX res = MMult(dir, rotX);
		res = MMult(dir, rotY);

		VECTOR move;

		move.x = res.m[3][0];
		move.y = res.m[3][1];
		move.z = res.m[3][2];

		m_pos = VAdd(m_pos, move);
	}
	//å„ëﬁ
	if (CheckHitKey(KEY_INPUT_S))
	{
		MATRIX rotX = MGetRotX(m_rot.x);
		MATRIX rotY = MGetRotY(m_rot.y);
		VECTOR defaultDir = VGet(0.0f, 0.0f, MOVE_SPEED);
		MATRIX dir = MGetTranslate(defaultDir);
		MATRIX res = MMult(dir, rotX);
		res = MMult(dir, rotY);

		VECTOR move;

		move.x = res.m[3][0] * -1;
		move.y = res.m[3][1] * -1;
		move.z = res.m[3][2] * -1;

		m_pos = VAdd(m_pos, move);
	}
	//å„ëﬁ
	if (CheckHitKey(KEY_INPUT_A))
	{
		MATRIX rotX = MGetRotX(m_rot.x + DX_PI_F * 0.5f);
		MATRIX rotY = MGetRotY(m_rot.y + DX_PI_F * 0.5f);
		VECTOR defaultDir = VGet(0.0f, 0.0f, MOVE_SPEED);
		MATRIX dir = MGetTranslate(defaultDir);
		MATRIX res = MMult(dir, rotX);
		res = MMult(dir, rotY);

		VECTOR move;

		move.x = res.m[3][0] * -1;
		move.y = res.m[3][1] * -1;
		move.z = res.m[3][2] * -1;

		m_pos = VAdd(m_pos, move);
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		MATRIX rotX = MGetRotX(m_rot.x - DX_PI_F * 0.5f);
		MATRIX rotY = MGetRotY(m_rot.y - DX_PI_F * 0.5f);
		VECTOR defaultDir = VGet(0.0f, 0.0f, MOVE_SPEED);
		MATRIX dir = MGetTranslate(defaultDir);
		MATRIX res = MMult(dir, rotX);
		res = MMult(dir, rotY);

		VECTOR move;

		move.x = res.m[3][0] * -1;
		move.y = res.m[3][1] * -1;
		move.z = res.m[3][2] * -1;

		m_pos = VAdd(m_pos, move);
	}


}

void DebugCamera::UpDate()
{
	SetCameraPositionAndAngle(m_pos, m_rot.x,m_rot.y,m_rot.z);
}

void DebugCamera::SetNearFar(float n, float f)
{
	SetCameraNearFar(n, f);
}