#include "Player.h"
#include "Game.h"
namespace
{
	constexpr float kSpeed = 18.0f;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
	m_cameraDir()
{
}

Player::~Player()
{
}

void Player::Init(Physics* physics)
{
	Collidable::Init(physics);
}

void Player::Update(MyEngine::Input input)
{

	//移動ベクトル
	MyEngine::Vector3 velo;
	
	//移動処理の計算
	velo = Move(velo,input);

	//リギットボディにベロシティを設定する
	m_rigidbody.SetVelo(velo);

	//ハンドルの座標を設定する
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
}

MyEngine::Vector3 Player::Move(MyEngine::Vector3 velo,MyEngine::Input input)
{
	//スティックでの移動
	{
		//スティックの入力情報
		MyEngine::Input::StickInfo stick = input.GetStickInfo();
		//プレイヤーの移動方向ベクトル
		MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);
		if (input.IsPress("LB"))
		{
			dir.y = dir.z;
			dir.z = 0;
		}
		if (dir.sqLength() != 0)
		{
			MATRIX mat = MGetRotY(m_cameraDir.y);

			dir = dir.Normalize();

			velo = dir * kSpeed;

			velo = velo.MatTransform(mat);

		}
	}

	//キーボードでの移動

	//スティックでの入力が行われていなかったら
	if (velo.sqLength() == 0)
	{
		//プレイヤーの移動方向ベクトル
		MyEngine::Vector3 dir;
		//上入力
		if (input.IsPress(Game::InputId::kUp))
		{
			dir.z = 1;
		}
		//下入力
		else if (input.IsPress(Game::InputId::kDown))
		{
			dir.z = -1;
		}
		//右入力
		if (input.IsPress(Game::InputId::kRight))
		{
			dir.x = 1;
		}
		//左入力
		else if (input.IsPress(Game::InputId::kLeft))
		{
			dir.x = -1;
		}
		//J入力
		if (input.IsPress(Game::InputId::kLb))
		{
			dir.y = dir.z;
			dir.z = 0;
		}
		if (dir.sqLength() != 0)
		{
			MATRIX mat = MGetRotY(m_cameraDir.y);

			dir = dir.Normalize();

			velo = dir * kSpeed;

			velo = velo.MatTransform(mat);

		}
	}

	return velo;
}
