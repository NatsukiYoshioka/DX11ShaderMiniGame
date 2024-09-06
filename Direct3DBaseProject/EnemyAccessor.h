#pragma once

class Enemy;

/// <summary>
/// エネミーアクセサクラス
/// </summary>
class EnemyAccessor
{
private:
	/// <summary>
	/// モデルの生成
	/// </summary>
	EnemyAccessor();

	/// <summary>
	/// データ破棄
	/// </summary>
	~EnemyAccessor();
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// インスタンス破棄
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static EnemyAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// 敵のオブジェクト取得
	/// </summary>
	/// <returns>Enemyオブジェクト</returns>
	Enemy* GetEnemy() { return m_enemy; }
private:
	static EnemyAccessor* m_instance;	//アクセサのインスタンス
	Enemy* m_enemy;						//敵オブジェクトのインスタンス
};

