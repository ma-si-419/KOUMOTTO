#include "TitleCamera.h"
#include <cmath>
namespace
{
	//‰ñ“]‚·‚é‰~‚Ì‘å‚«‚³
	constexpr float kLoopRadius = 2000.0f;
	//ƒJƒƒ‰‚Ì‰ñ“]‘¬“x
	constexpr float kCameraSpeed = 0.01f;
}
TitleCamera::TitleCamera():
	m_loopCount(0)
{
	SetCameraNearFar(15.0f, 100000.0f);
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Init()
{
	m_cameraPos = MyEngine::Vector3(kLoopRadius, 0.0f, kLoopRadius);
}

void TitleCamera::Update()
{
	m_loopCount += kCameraSpeed;
	m_cameraPos = MyEngine::Vector3(sinf(m_loopCount) * kLoopRadius, 0.0f, cosf(m_loopCount) * kLoopRadius);
	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(),VGet(0,0,0));
}
