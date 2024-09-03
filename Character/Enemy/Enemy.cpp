#include "Enemy.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CapsuleColliderData.h"
#include "Ui.h"
#include "EnemyStateIdle.h"
namespace
{
	//当たり判定の大きさ
	constexpr float kColScale = 100.0f;
	//初期位置
	const MyEngine::Vector3 kInitPos(0, 0, 1000);
	//スタンゲージのマックス
	constexpr float kMaxStanPoint = 500;
	//スタンゲージが回復するまでの時間
	constexpr int kHealStanPointTime = 200;
	//コンボがつながらなくなるまでの時間
	constexpr int kComboTime = 180;
}
Enemy::Enemy() :
	CharacterBase("data/model/Enemy.mv1", ObjectTag::kEnemy),
	m_stanPoint(kMaxStanPoint),
	m_lastHitDamageTime(0),
	m_comboCount(0)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(std::shared_ptr<Physics> physics)
{

	MV1SetScale(m_modelHandle, VGet(300, 300, 300));
	Collidable::Init(physics);
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	colData->m_radius = kColScale;
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_rigidbody.SetPos(kInitPos);
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());


}

void Enemy::RetryInit()
{
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	MyEngine::Vector3 pos = kInitPos;

	m_rigidbody.SetPos(pos);
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	//当たり判定の縦幅
	pos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = pos;
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);

}

void Enemy::Update(std::shared_ptr<SceneGame> scene)
{
	//StateにAiの情報をわたす
	m_pState->m_aiData = m_aiStateData;

	//ダメージを受けてからの時間をカウントする
	m_lastHitDamageTime++;

	//ダメージを受けてから一定時間たったらスタン値を回復していく
	if (m_lastHitDamageTime > kHealStanPointTime)
	{
		m_stanPoint++;
		//上限値を超えないように
		if (m_stanPoint > kMaxStanPoint)
		{
			m_stanPoint = kMaxStanPoint;
		}
	}
	//ダメージを受けてから一定時間たったらコンボ数を減らす
	if (m_lastHitDamageTime > kComboTime)
	{
		m_comboCount = 0;
	}
	//コンボ数をUIに渡し続ける
	m_pUi->SetComboCount(m_comboCount);

	//State変更フラグが立っていたらStateを変更する
	if (m_pState->m_isChangeState)
	{
		m_pState = m_pState->m_nextState;
	}

	m_pState->Update();
	

	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());

	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	
	//当たり判定の縦幅
	colPos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = colPos;

	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);

}

void Enemy::Draw()
{
	DrawFormatString(1300, 300, GetColor(0, 0, 0), "HP:%f\nMP:%f", m_nowHp, m_nowMp);

	MV1DrawModel(m_modelHandle);
}

void Enemy::OnCollide(std::shared_ptr<Collidable> collider)
{
	//プレイヤーの攻撃に当たった時
	if (collider->GetTag() == ObjectTag::kPlayerAttack)
	{
		auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
		int damage = attack->GetDamage() - GetRand(static_cast<int>(m_status.def));
		if (damage < 0)
		{
			damage = 2;
		}
		m_nowHp -= damage;
		if (m_nowHp < 0)
		{
			m_nowHp = 0;
		}
		//スタンダメージを受ける
		m_stanPoint -= attack->GetStanDamage();
		//下がりすぎないように
		if (m_stanPoint < 0)
		{
			m_stanPoint = 0;
		}
		//コンボをカウントするタイミングだったら
		if (m_pUi->GetIsCountCombo())
		{
			//コンボ数を増やす
			m_comboCount++;
		}
		//ダメージを受けてからの時間を数える
		m_lastHitDamageTime = 0;
		//UIに受けたダメージを送る
		m_pUi->AddShowDamage(m_rigidbody.GetPos(), damage);
	}
}

void Enemy::SetUi(std::shared_ptr<Ui> pUi)
{
	m_pUi = pUi;
}

void Enemy::InitAiState(std::shared_ptr<SceneGame> scene)
{
	m_pState = std::make_shared<EnemyStateIdle>(std::dynamic_pointer_cast<Enemy>(shared_from_this()), scene);
}

float Enemy::GetStanPointRate()
{
	return m_stanPoint / kMaxStanPoint;
}

void Enemy::StateUpdate(std::shared_ptr<Player> player)
{
	m_pState->CheckSituation(player);
}
