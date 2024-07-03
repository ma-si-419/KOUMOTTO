#include "Input.h"
using namespace MyEngine;

Input::Input()
{
	//ボタンの設定
	m_inputActionMap["OK"] = { {InputType::kKeyboard,KEY_INPUT_RETURN},{InputType::kPad,PAD_INPUT_A} };
	m_inputActionMap["PAUSE"] = { {InputType::kKeyboard,KEY_INPUT_P}, {InputType::kPad,PAD_INPUT_START} };
	m_inputActionMap["SELECT"] = { {InputType::kKeyboard,KEY_INPUT_TAB}, {InputType::kPad,PAD_INPUT_7} };
	m_inputActionMap["CANCEL"] = { {InputType::kKeyboard,KEY_INPUT_ESCAPE}, {InputType::kPad,PAD_INPUT_B} };
	m_inputActionMap["UP"] = { {InputType::kKeyboard,KEY_INPUT_UP}, {InputType::kPad,PAD_INPUT_UP} };
	m_inputActionMap["DOWN"] = { {InputType::kKeyboard,KEY_INPUT_DOWN}, {InputType::kPad,PAD_INPUT_DOWN} };
	m_inputActionMap["LEFT"] = { {InputType::kKeyboard,KEY_INPUT_LEFT}, {InputType::kPad,PAD_INPUT_LEFT} };
	m_inputActionMap["RIGHT"] = { {InputType::kKeyboard,KEY_INPUT_RIGHT}, {InputType::kPad,PAD_INPUT_RIGHT} };
	m_inputActionMap["LB"] = { {InputType::kKeyboard,KEY_INPUT_J}, {InputType::kPad,PAD_INPUT_5} };
	m_inputActionMap["RB"] = { {InputType::kKeyboard,KEY_INPUT_L}, {InputType::kPad,PAD_INPUT_6} };

}
void Input::Update()
{
	//前のフレームの入力情報を保存する
	m_lastInput = m_currentInput;

	//すべての入力を取得する
	char keyState[256] = {};
	int padState = {};
	int mouseState = {};
	GetHitKeyStateAll(keyState);
	padState = GetJoypadInputState(DX_INPUT_PAD1);
	mouseState = GetMouseInput();


	//アクション名に割り当てられているすべてのキーの入力をチェックする
	for (const auto& keyInfo : m_inputActionMap)
	{
		bool isPress = false;
		for (const auto& inputInfo : keyInfo.second)
		{
			//キーボードのチェック
			if (inputInfo.type == InputType::kKeyboard && keyState[inputInfo.buttonID])
			{
				isPress = true;
			}
			//パッドのチェック
			if (inputInfo.type == InputType::kPad && padState & inputInfo.buttonID)
			{
				isPress = true;
			}
			//マウスのチェック
			if (inputInfo.type == InputType::kMouse && mouseState & inputInfo.buttonID)
			{
				isPress = true;
			}

			//ボタンが押されていたらループを抜ける
			if (isPress)
			{
				break;
			}
		}
		//現在のフレームで押されていたかどうかを返す
		m_currentInput[keyInfo.first] = isPress;
	}

	//スティックの入力を初期化する
	m_stickInfo.leftStickX = 0;
	m_stickInfo.leftStickY = 0;
	m_stickInfo.rightStickX = 0;
	m_stickInfo.rightStickY = 0;

	//スティックの入力を取得する
	GetJoypadAnalogInput(&m_stickInfo.leftStickX, &m_stickInfo.leftStickY, DX_INPUT_PAD1);
	GetJoypadAnalogInputRight(&m_stickInfo.rightStickX, &m_stickInfo.rightStickY, DX_INPUT_PAD1);
}

bool Input::IsPress(const std::string& action) const
{
	auto keyInfo = m_currentInput.find(action);

	//ボタン名が定義されていなかったらfalseを返す
	if (keyInfo == m_currentInput.end())
	{
		return false;
	}
	else
	{
		//見つかったらboolの値を返す
		return keyInfo->second;
	}
}

bool Input::IsTrigger(const std::string& action) const
{
	//まず押されているかどうか判定
	if (IsPress(action))
	{
		//前のフレームを参照
		auto last = m_lastInput.find(action);
		//未定義のボタン名が来たらfalseを返す
		if (last == m_lastInput.end())
		{
			return true;
		}
		//前のフレームでも押されていたら
		else
		{
			return !last->second;
		}
	}
	else
	{
		return false;
	}
}