#include "Enemy.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CapsuleColliderData.h"
#include "Ui.h"
#include "EnemyStateIdle.h"
#include "EffekseerForDXLib.h"
namespace
{
	//当たり判定の大きさ
	constexpr float kColScale = 100.0f;
	//初期位置
	const MyEngine::Vector3 kInitPos(3000, 0, 3000);
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
	ChangeAnim("Idle");

	MV1SetScale(m_modelHandle, VGet(300, 300, 300));
	Collidable::Init(physics);
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	colData->m_radius = kColScale;
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_pPhysics = physics;

	m_rigidbody.SetPos(kInitPos);
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);

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

	//Stateの更新処理
	m_pState->Update();

	//エフェクトの再生
	PlayEffect();

	//座標の更新
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());

	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();

	//当たり判定の縦幅
	colPos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = colPos;
	//プレイヤーの方を向くStateかどうか判断する
	if (m_pState->m_isLookPlayer)
	{
		MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
	}

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
		int damage = m_pState->OnDamage(attack);
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

std::shared_ptr<AttackBase> Enemy::CreateAttack(std::string id)
{
	//エネミーの攻撃を作成
	std::shared_ptr<AttackBase> ans = std::make_shared<AttackBase>(ObjectTag::kEnemyAttack);

	//攻撃を出す座標を作成
	MyEngine::Vector3 toTargetVec = m_targetPos - m_rigidbody.GetPos();
	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	//ステータス設定
	ans->SetStatus(m_attackData[id], m_targetPos, m_rigidbody.GetPos(), m_status.atk);
	ans->Init(m_pPhysics, attackPos,m_effekseerHandle[m_attackData[id].effekseerName].first);

	return ans;
}

void Enemy::SetPlayEffect(std::pair<int, int> playHandleData)
{
	m_playEffectData = playHandleData;
}

void Enemy::StopEffect()
{
	StopEffekseer3DEffect(m_playEffectData.first);
	m_playEffectData.first = -1;
	m_playEffectData.second = 0;
	m_playEffectHandle = -1;
}

void Enemy::InitPos()
{
	m_rigidbody.SetPos(kInitPos);
}
