#pragma once
#include "Collidable.h"
#include "Game.h"
#include "DataManager.h"
class AttackBase : public Collidable
{
public:
	AttackBase(ObjectTag tag);
	virtual ~AttackBase();

	void Init(std::shared_ptr<Physics> physics, MyEngine::Vector3 pos, int effekseerHandle);
	void SetStatus(DataManager::AttackInfo status, MyEngine::Vector3 target, MyEngine::Vector3 playerPos, float power);
	void Update(MyEngine::Vector3 targetPos);
	void Draw() {};

	//ダメージを返す
	int GetDamage() { return m_status.damage; }
	//スタンダメージを返す
	int GetStanDamage() { return m_status.stanDamage; }
	//座標を返す
	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }

	//何かに当たった時の処理
	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;

	//当たった時の処理を返す
	int GetHitEffect() { return m_status.hitEffect; }

	//ライフタイムを設定する
	void SetAttackTime(int time) { m_status.lifeTime = time; };

	void Final(std::shared_ptr<Physics> physics);
	bool GetIsExist() { return m_isExist; }

	void SetLeaveEffect() { m_isLeaveEffect = true; }
private:
	/*ステータス*/
	DataManager::AttackInfo m_status;
	//攻撃の方向
	MyEngine::Vector3 m_dir;
	//存在しているか
	bool m_isExist;
	//存在した時間
	int m_lifeTime;
	//初期化時の敵との距離
	float m_targetLength;
	//移動した距離
	float m_moveLength;
	//エフェクトのハンドル
	int m_effectHandle;
	//プレイ中のエフェクトのハンドル
	int m_playEffectHandle;
	//当たり判定が消えた後にエフェクトを残すかどうか
	bool m_isLeaveEffect;
};

