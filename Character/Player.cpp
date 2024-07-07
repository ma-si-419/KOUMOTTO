#include "Player.h"
#include "Game.h"
#include "SceneGame.h"
namespace
{
	constexpr float kMoveSpeed = 0.1f;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
	m_targetPos(),
	m_rota(0)
{
}

Player::~Player()
{
}

void Player::Init(Physics* physics)
{
	m_pPhysics = physics;
	Collidable::Init(physics);
}

void Player::Update(std::shared_ptr<SceneGame> scene,MyEngine::Input input)
{

	//移動ベクトル
	MyEngine::Vector3 velo;

	//移動処理の計算
	velo = Move(velo, input);

	if (input.IsTrigger("X"))
	{
	}

	//リギットボディにベロシティを設定する
	m_rigidbody.SetVelo(velo);

	//ハンドルの座標を設定する
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
	DrawSphere3D(VGet(0, 0, 0), 50, 5, GetColor(255, 255, 255), GetColor(255, 0, 0), false);
}

MyEngine::Vector3 Player::Move(MyEngine::Vector3 velo, MyEngine::Input input)
{
	//スティックでの移動
	{
		//スティックの入力情報
		MyEngine::Input::StickInfo stick = input.GetStickInfo();
		//プレイヤーの移動方向ベクトル
		MyEngine::Vector3 stickDir(stick.leftStickX, 0, -stick.leftStickY);

		//プレイヤーが上下移動するかするかどうか
		bool isMoveVertical = input.IsPress("LB");

		if (stickDir.sqLength() != 0)
		{

			MyEngine::Vector3 toTargetVec = m_targetPos - m_rigidbody.GetPos();

			//回転速度(横移動の速さ)
			float HMoveSpeed = 0;

			if (stickDir.x != 0)
			{
				HMoveSpeed = (stickDir.x * kMoveSpeed) / toTargetVec.Length();
			}

			DrawFormatString(200, 0, GetColor(255, 255, 255), "%f", HMoveSpeed);

			m_rota += HMoveSpeed;

			//左右移動は敵の周囲を回る
			velo.x += (m_targetPos.x + cosf(m_rota) * toTargetVec.Length()) - m_rigidbody.GetPos().x;
			velo.z += (m_targetPos.z + sinf(m_rota) * toTargetVec.Length()) - m_rigidbody.GetPos().z;

			/*if (isMoveVertical)
			{
				velo.y += -stickDir.z * kMoveSpeed;
			}
			else
			{
			}*/

			velo += toTargetVec.Normalize() * (stickDir.z * kMoveSpeed);
			DrawFormatString(400, 0, GetColor(255, 255, 255), "%f", velo.Length());;

			//MATRIX mat = MGetRotY(toTargetVec.y);

			//stickDir = stickDir.Normalize();


			//velo = stickDir * kHSpeed + toTargetVec * kHSpeed;

			//velo = velo.MatTransform(mat);

		}
	}

	//キーボードでの移動

	//スティックでの入力が行われていなかったら
	//if (velo.sqLength() == 0)
	//{
	//	//プレイヤーの移動方向ベクトル
	//	MyEngine::Vector3 stickDir;
	//	//上入力
	//	if (input.IsPress(Game::InputId::kUp))
	//	{
	//		stickDir.z = 1;
	//	}
	//	//下入力
	//	else if (input.IsPress(Game::InputId::kDown))
	//	{
	//		stickDir.z = -1;
	//	}
	//	//右入力
	//	if (input.IsPress(Game::InputId::kRight))
	//	{
	//		stickDir.x = 1;
	//	}
	//	//左入力
	//	else if (input.IsPress(Game::InputId::kLeft))
	//	{
	//		stickDir.x = -1;
	//	}
	//	//J入力
	//	if (input.IsPress(Game::InputId::kLb))
	//	{
	//		stickDir.y = stickDir.z;
	//		stickDir.z = 0;
	//	}
	//	if (stickDir.sqLength() != 0)
	//	{
	//		MATRIX mat = MGetRotY(m_targetPos.y);

	//		stickDir = stickDir.Normalize();

	//		velo = stickDir * kHSpeed;

	//		velo = velo.MatTransform(mat);

	//	}
	//}

	return velo;
}
