#pragma once

/// <summary>
/// シーンの基底クラス
/// </summary>
class BaseScene abstract
{
public:
	//純粋仮想関数
	virtual void Update() abstract;
	virtual void Draw() abstract;
	virtual void DrawOffScreen() abstract;

	/// <summary>
	/// シーンが遷移していいかどうか取得
	/// </summary>
	bool GetIsChangeScene()const { return m_isChangeScene; }
protected:
	bool m_isChangeScene = false;		//シーンが遷移していいかどうか
};