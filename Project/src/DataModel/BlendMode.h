#pragma once
enum BlendMode {
	//!<ブレンドなし
	kBlendModeNone,
	//!<通常αブレンド。ディフォルト。Src*SrcA+Dest*(1-SrcA)
	kBlendModeNormal,
	//!<加算。src*SrcA+Dest*1
	kBlendModeAdd,
	//!<減算。Dest*1-Src*SrcA
	kBlendModeSubtract,
	//!<乗算。Src*0+Dest*Src
	kBlendModeMultiply,
	//!<スクリーン。Src*(1-Dest)+Dest*1
	kBlendModeScreen,
	//モードの個数の値
	kCountOfBlendMode
};