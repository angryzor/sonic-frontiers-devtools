#include "Translations.h"

Translations::Language Translations::currentLanguage{ Language::VAR_NAMES };

void Translations::Init(csl::fnd::IAllocator* allocator) {
	translationMap = new (allocator) csl::ut::PointerMap<const char*, const char*>{ allocator };
	for (size_t i = 0; i < TRANSLATION_COUNT; i++)
		translationMap->Insert(reinterpret_cast<const char*>(translations[i].originalStrAddr), reinterpret_cast<const char*>(translations[i].translation));
}

Translations::Language Translations::GetCurrentLanguage()
{
	return currentLanguage;
}

const char* Translations::GetTranslation(const char* str) {
	switch (currentLanguage) {
	case Language::VAR_NAMES:
		return nullptr;
	case Language::JAPANESE:
		return str;
	case Language::ENGLISH:
		return translationMap->GetValueOrFallback(str, nullptr);
	default:
		return nullptr;
	}
}

const char* Translations::GetLanguageName(Translations::Language language) {
	switch (language) {
	case Language::VAR_NAMES:
		return "Variable names (English)";
	case Language::JAPANESE:
		return "Japanese descriptions";
	case Language::ENGLISH:
		return "English descriptions (translated from Japanese)";
	default:
		return "<ERROR>";
	}
}

void Translations::SetCurrentLanguage(Translations::Language language)
{
	currentLanguage = language;
}

csl::ut::PointerMap<const char*, const char*>* Translations::translationMap = nullptr;

Translations::CaptionTranslation Translations::translations[] = {
	{ 0x1416c9af0, u8"Client background color" }, // クライアント背景色
	{ 0x1416c9b10, u8"Window transmittance" }, // ウィンドウ透過率
	{ 0x1416c9b30, u8"Gradation guide starting point color" }, // グラデーションガイド始点色
	{ 0x1416c9b58, u8"Gradation guide end color" }, // グラデーションガイド終点色
	{ 0x1413f552c, u8"Radius" }, // 半径
	{ 0x1415c3854, u8"Direction" }, // 方向
	{ 0x1416c9ce0, u8"Src coordinate X" }, // src座標X
	{ 0x1416c9cf0, u8"Src coordinate Y" }, // src座標Y
	{ 0x1416c9d00, u8"Dst coordinate X" }, // dst座標X
	{ 0x1416c9d10, u8"Dst coordinate Y" }, // dst座標Y
	{ 0x1416c9d20, u8"Src color" }, // srcカラー
	{ 0x1416c9d30, u8"Dst color" }, // dstカラー
	{ 0x1415e1080, u8"Color" }, // カラー
	{ 0x1413e86a0, u8"Size" }, // サイズ
	{ 0x1416c9bf8, u8"Spread" }, // スプレッド
	{ 0x1416c9c08, u8"Distance (X)" }, // 距離(X)
	{ 0x1416c9c18, u8"Distance (Y)" }, // 距離(Y)
	{ 0x1416c9da8, u8"Drop shadow" }, // ドロップシャドウ
	{ 0x1416c9e70, u8"The thickness" }, // 太さ
	{ 0x1416c9e78, u8"Offset" }, // オフセット X
	{ 0x1416c9e90, u8"Offset Y" }, // オフセット Y
	{ 0x1416c9dc8, u8"Edging" }, // 縁取り
	{ 0x1416c9dd8, u8"Gradation" }, // グラデーション
	{ 0x1416c9df0, u8"Blur" }, // ブラー
	{ 0x1416c9e00, u8"Avoid edge artifacts" }, // エッジのアーティファクトを回避する
	{ 0x1413a1ff0, u8"Name" }, // 名前
	{ 0x1416c9f38, u8"Font color" }, // フォントカラー
	{ 0x1416c9f50, u8"Font gradient" }, // フォントグラデーション
	{ 0x1416c9f78, u8"Layer composition method" }, // レイヤー合成方式
	{ 0x1416c9f98, u8"Number of passes" }, // パス数
	{ 0x1416c9fa8, u8"Path parameter" }, // パスパラメーター
	{ 0x1416ca028, u8"Technique presets" }, // テクニックプリセット
	{ 0x1415c36d0, u8"Radius [m]" }, // 半径[m]
	{ 0x1415c36e0, u8"Height [m]" }, // 高さ[m]
	{ 0x1415c36f0, u8"Climbing height [m]" }, // 登坂可能な高さ[m]
	{ 0x1415c3710, u8"Climbable angle [degree]" }, // 登坂可能な角度[degree]
	{ 0x1415c3748, u8"Sampling distance [m]" }, // サンプリング距離[m]
	{ 0x1415c3768, u8"Sampling tolerance [m]" }, // サンプリング許容誤差[m]
	{ 0x1415c3838, u8"Starting point" }, // 始点
	{ 0x1415c3840, u8"The last stop" }, // 終点
	{ 0x1413cfa48, u8"Flag" }, // フラグ
	{ 0x1415c3848, u8"Area" }, // エリア
	{ 0x1415c3860, u8"User ID" }, // ユーザーID
	{ 0x1415c38a0, u8"Elements" }, // elements
	{ 0x1415c38c8, u8"LowHanging Obstacles" }, // lowHangingObstacles
	{ 0x1415c38e0, u8"LedgeSpans" }, // ledgeSpans
	{ 0x1415c38f0, u8"WalkableLowHeightSpans" }, // walkableLowHeightSpans
	{ 0x1415c3c20, u8"MIN of construction range AABB" }, // 構築範囲AABBのMIN
	{ 0x1415c3c38, u8"Construction range AABB MAX" }, // 構築範囲AABBのMAX
	{ 0x1413e8fb0, u8"World" }, // World
	{ 0x1415c3ad0, u8"Voxel size [m]" }, // Voxelのサイズ[m]
	{ 0x1415c3ae8, u8"Voxel height [m]" }, // Voxelの高さ[m]
	{ 0x1415c3918, u8"Rasterization" }, // Rasterization
	{ 0x1415c373c, u8"Agent" }, // Agent
	{ 0x1415c3b20, u8"Minimum region area [voxelCount]" }, // 最小リージョンエリア[voxelCount]
	{ 0x1415c3b50, u8"Mergeable region area [voxelCount]" }, // マージ可能リージョンエリア[voxelCount]
	{ 0x1415c3928, u8"Region" }, // Region
	{ 0x1415c3a18, u8"Partition method" }, // パーティション方式
	{ 0x1415c3930, u8"Partitioning" }, // Partitioning
	{ 0x1415c3908, u8"Filtering" }, // Filtering
	{ 0x1415c3a38, u8"Maximum edge length [m]" }, // 最大エッジ長[m]
	{ 0x1415c3a50, u8"Vectorization simplified deviation value" }, // ベクトル化の簡略化偏差値
	{ 0x1415c3a78, u8"Number of vertices of polygon" }, // ポリゴンの頂点数
	{ 0x1415c3940, u8"Polygonization" }, // Polygonization
	{ 0x1415c37b0, u8"DetailMesh" }, // DetailMesh
	{ 0x1415c3ba0, u8"NavMesh tiling size [m]" }, // NavMeshのタイリングサイズ[m]
	{ 0x1415c3bc8, u8"Maximum number of tiles that can be read simultaneously" }, // 同時読み込み最大タイル数
	{ 0x1415c3bf0, u8"Is it a cache tile?" }, // キャッシュタイルか
	{ 0x1415c3950, u8"Tiling" }, // Tiling
	{ 0x1416b19b0, u8"Mold type" }, // 型タイプ
	{ 0x1413ed1e0, u8"Value" }, // 値
	{ 0x1416b18e0, u8"Statistics list" }, // 統計値リスト
	{ 0x1415e6410, u8"Enable real-time celestial sphere" }, // リアルタイム天球を有効
	{ 0x1415e7770, u8"Size of the sun" }, // 太陽の大きさ
	{ 0x1415e7788, u8"How to specify the sun position" }, // 太陽位置の指定方法
	{ 0x1413b9010, u8"Azimuth" }, // 方位角
	{ 0x1413b9168, u8"Elevation angle" }, // 仰角
	{ 0x1415e77a8, u8"Set by angle" }, // 角度で設定
	{ 0x1415e7848, u8"Azimuth correction" }, // 方位角補正
	{ 0x1415e7858, u8"Latitude" }, // 緯度
	{ 0x1415e7860, u8"Longitude" }, // 経度
	{ 0x1415e643c, u8"Month" }, // 月
	{ 0x1413d7ec4, u8"Day" }, // 日
	{ 0x1413d0fd4, u8"Time" }, // 時
	{ 0x1415e6b60, u8"Enable animation" }, // アニメーション有効化
	{ 0x1415e7868, u8"Time change speed" }, // 時勢変化速度
	{ 0x1415e77b8, u8"Set by date and time" }, // 日時で設定
	{ 0x1415e6434, u8"Sun" }, // 太陽
	{ 0x1415e74e0, u8"How to specify the position" }, // 位置の指定方法
	{ 0x1415e76d0, u8"Type of celestial sphere" }, // 天球のタイプ
	{ 0x1415e6550, u8"Illumination Scale" }, // illuminanceScale
	{ 0x1415e66c8, u8"Scattering enabled" }, // スキャッタリング有効化
	{ 0x1415e66f0, u8"Scattering level" }, // スキャッタリング具合
	{ 0x1415e6440, u8"Common to the celestial sphere" }, // 天球共通
	{ 0x1415e6568, u8"RayleighColor" }, // rayleighColor
	{ 0x1415e6578, u8"Parallel light color correction" }, // 平行光の色味補正
	{ 0x1415e6598, u8"Atmospheric turbidity Directivity" }, // 大気混濁度 指向性
	{ 0x1415e65b8, u8"Mie scattering color" }, // mie scattering color
	{ 0x1415e65d0, u8"Mie scattering scale" }, // mie scattering scale
	{ 0x1415e65e8, u8"Mie absorption color" }, // mie absorption color
	{ 0x1415e6600, u8"Mie absorption scale" }, // mie absorption scale
	{ 0x1415e6618, u8"Rayleigh scattering color" }, // rayleigh scattering color
	{ 0x1415e6638, u8"Rayleigh scattering scale" }, // rayleigh scattering scale
	{ 0x1415e6658, u8"Ground albedo" }, // 地面のアルベド
	{ 0x1415e6670, u8"Ground lighting correction" }, // 地面ライティング補正
	{ 0x1415e6690, u8"Multiply value to IBL" }, // IBLへの乗算値
	{ 0x1415e66a8, u8"How to apply to IBL multiplication value" }, // IBL乗算値への適用具合
	{ 0x1415e68d8, u8"Brightness of the moon on the celestial sphere" }, // 天球上の月の明るさ
	{ 0x1415e68f8, u8"Brightness of stars on the celestial sphere" }, // 天球上の星の明るさ
	{ 0x1415e6918, u8"The strength of the light that illuminates the clouds" }, // 雲を照らすライトの強さ
	{ 0x1415e6940, u8"Sky and cloud brightness scale" }, // 空と雲の輝度スケール
	{ 0x1415e6960, u8"Brightness of the sky" }, // 空の明るさ
	{ 0x1415e6970, u8"[Offset] Color correction for parallel light" }, // [オフセット]平行光の色味補正
	{ 0x1415e69a0, u8"Parallel light intensity correction" }, // 平行光の強さ補正
	{ 0x1415e6710, u8"Night sky" }, // 夜の天球
	{ 0x1415e6720, u8"Tracking limit in Y direction" }, // Y方向の追従制限
	{ 0x1415e6738, u8"[debug] GBufferCube brightness scale" }, // [debug] GBufferCubeの輝度スケール
	{ 0x1415e6450, u8"Bruneton Sky" }, // BrunetonSky
	{ 0x1415e7510, u8"[offset] Mie scattering color" }, // [オフセット]mie scattering color
	{ 0x1415e7538, u8"[offset] Mie absorption color" }, // [オフセット]mie absorption color
	{ 0x1415e7560, u8"[offset] Rayleigh scattering color" }, // [オフセット]rayleigh scattering color
	{ 0x1415e7590, u8"[Offset] Ground albedo" }, // [オフセット]地面のアルベド
	{ 0x1415e75b8, u8"Virtual ground enable" }, // 仮想地面有効化
	{ 0x1415e75d0, u8"NumScatteringOrder" }, // numScatteringOrder
	{ 0x1415e6460, u8"Sebastian Sky" }, // SebastienSky
	{ 0x1415e6b48, u8"Enable cloud drawing" }, // 雲描画有効化
	{ 0x1415e6b80, u8"Current number of frames" }, // 現在のフレーム数
	{ 0x1415e6ba0, u8"Low cloud animation speed" }, // 低層雲アニメーション速度
	{ 0x1415e6bc8, u8"Direction of flow of low clouds" }, // 低層雲の流れる向き
	{ 0x1415e6be8, u8"High altitude cloud animation speed" }, // 高高度雲アニメーション速度
	{ 0x1415e6c10, u8"Direction of flow of high-altitude clouds" }, // 高高度雲の流れる向き
	{ 0x1415e6c30, u8"Horizontal line position" }, // 水平線の位置
	{ 0x1415e6c48, u8"Cloud height" }, // 雲の高さ
	{ 0x1415e6c58, u8"R: Cloud range change rate" }, // R:雲範囲変化速度
	{ 0x1415e6c70, u8"G: Cloud thickness change rate" }, // G:雲厚み変化速度
	{ 0x1415e6c88, u8"B: Cloud darkness change speed" }, // B:雲の暗さ変化速度
	{ 0x1415e6ca8, u8"Mie phase g" }, // mie phase g
	{ 0x1415e6cb8, u8"Silver intensity" }, // silver intensity
	{ 0x1415e6cd0, u8"Silver spread" }, // silver spread
	{ 0x1415e6ce0, u8"UVScale" }, // UVScale
	{ 0x1415e6ce8, u8"Cloud density" }, // 雲の密度
	{ 0x1415e6cf8, u8"Cloud density threshold" }, // 雲の密度閾値
	{ 0x1415e6d10, u8"Disappearing color during the day" }, // 日中の消失カラー
	{ 0x1415e6d30, u8"[Offset] Disappearing color during the day" }, // [オフセット]日中の消失カラー
	{ 0x1415e6d60, u8"Vanishing color of the evening" }, // 夕方の消失カラー
	{ 0x1415e6d80, u8"[Offset] Vanishing color in the evening" }, // [オフセット]夕方の消失カラー
	{ 0x1415e6db0, u8"Disappearing color at night" }, // 夜中の消失カラー
	{ 0x1415e6dd0, u8"[Offset] Disappearing color at night" }, // [オフセット]夜中の消失カラー
	{ 0x1415e6e00, u8"Blending ratio of clouds and sky" }, // 雲と空のブレンド率
	{ 0x1415e6e20, u8"Enable cloud shadow" }, // 雲影有効化
	{ 0x1415e6e30, u8"Shadow casting range [m]" }, // 影を落とす範囲[m]
	{ 0x1415e6e50, u8"Minimum cloud occlusion" }, // 雲の遮蔽最低値
	{ 0x1415e6a78, u8"Toggle cloud texture" }, // 雲テクスチャの切り替え
	{ 0x1415e6aa0, u8"Probability of switching" }, // 切り替わる確率
	{ 0x1415e6ab8, u8"Cloudiness [Forcibly cloudy]" }, // 曇り具合[強制的に曇らせる]
	{ 0x1415e70a0, u8"UV scale" }, // UVスケール
	{ 0x1415e70b0, u8"Gamma" }, // ガンマ
	{ 0x1415e70c0, u8"Color scale" }, // カラースケール
	{ 0x1415e70d8, u8"Color offset" }, // カラーオフセット
	{ 0x1415e6ae0, u8"Cloudy clouds [procedural]" }, // 曇りの雲[procedural]
	{ 0x1415e6e68, u8"Cloud switching" }, // 雲の切り替え
	{ 0x1415e6470, u8"Cloud" }, // 雲
	{ 0x1415e7160, u8"Enable God Ray" }, // ゴッドレイ有効化
	{ 0x1415e717c, u8"Density" }, // 密度
	{ 0x1415e7188, u8"Attenuation rate" }, // 減衰率
	{ 0x1415e7194, u8"Weight" }, // 重み
	{ 0x1415e6478, u8"Godley" }, // ゴッドレイ
	{ 0x1415e71c0, u8"Height fog enabled" }, // ハイトフォグ有効化
	{ 0x1415e71e0, u8"Using fog maps" }, // フォグマップの利用
	{ 0x1415e7200, u8"Height scale" }, // 高さスケール
	{ 0x1415e7218, u8"Distance offset" }, // 距離オフセット
	{ 0x1415e7230, u8"Distance scale" }, // 距離スケール
	{ 0x1415e7248, u8"Rayleigh correction" }, // レイリー補正
	{ 0x1415e7260, u8"Me correction" }, // ミー補正
	{ 0x1415e7270, u8"Noise multiplication (-1) / addition selection (1)" }, // ノイズ乗算(-1)/加算選択(1)
	{ 0x1415e7298, u8"Noise scale" }, // ノイズスケール
	{ 0x1415e72b0, u8"Noise maximum distance" }, // ノイズ最大距離
	{ 0x1415e72c8, u8"Noise flow speed" }, // ノイズの流れる速度
	{ 0x1415e72e8, u8"Direction of noise flow" }, // ノイズの流れる向き
	{ 0x1415e7308, u8"Link with the atmosphere" }, // 大気と連動させる
	{ 0x1415e73e0, u8"Sun color" }, // 太陽カラー
	{ 0x1415e73f0, u8"Ambient color" }, // アンビエントカラー
	{ 0x1415e7328, u8"If not linked" }, // 連動しない場合
	{ 0x1415e6488, u8"Height fog" }, // ハイトフォグ
	{ 0x1415e78b8, u8"Debugging 1" }, // デバッグ１
	{ 0x1415e78c8, u8"Factor" }, // Factor
	{ 0x1415e78d0, u8"Display interval adjustment" }, // 表示間隔調整
	{ 0x1415e78e8, u8"Adjust number of sections" }, // 区間数調整
	{ 0x1415e78f8, u8"LOD distance addition" }, // LOD距離加算
	{ 0x1415e7908, u8"Add display section number" }, // 表示区間数加算
	{ 0x1415e7920, u8"Display distance limit" }, // 表示距離限界
	{ 0x1415e7938, u8"DrawCall reduction" }, // DrawCall削減
	{ 0x1415e7948, u8"Thinning function" }, // 間引き機能
	{ 0x1415e7958, u8"Grass thinning" }, // 草の間引き
	{ 0x1415e7968, u8"Thinning of non-grass" }, // 草以外間引き
	{ 0x1415e7980, u8"Resource placement grass thinning" }, // リソース配置の草の間引き
	{ 0x1415e79a8, u8"Thinning out non-grass resources placement" }, // リソース配置の草以外間引き
	{ 0x1415e79d0, u8"Debugging for load measurement yesterday" }, // 負荷計測用のデバック昨日
	{ 0x1415e7af0, u8"Lod switching rate" }, // Lod切り替え割合
	{ 0x1415e7b08, u8"Dither increase rate" }, // Dither上昇率
	{ 0x1415e7b18, u8"Dither reduction rate" }, // Dither減少率
	{ 0x1415e7b28, u8"Dither threshold (camera movement speed)" }, // Dither閾値(カメラ移動速度)
	{ 0x1415e7b50, u8"Enable setting" }, // 有効設定
	{ 0x1415e7c68, u8"Dynamic placement enabled" }, // 動的配置有効
	{ 0x1415e7c80, u8"Resource placement enabled" }, // リソース配置有効
	{ 0x1415e7ca0, u8"Camera enabled" }, // カメラ有効
	{ 0x1415e7cb0, u8"Basic type of placement" }, // 配置の基本種別
	{ 0x1415e7cc8, u8"Position adjustment at random" }, // random時の位置調整
	{ 0x1415e7ce8, u8"AlphaMap enabled" }, // AlphaMap有効
	{ 0x1415e7cf8, u8"AlphaMap threshold" }, // AlphaMapの閾値
	{ 0x1415e7d10, u8"AlphaMap scale sensitivity" }, // AlphaMapのスケール影響度
	{ 0x1415e7d38, u8"DensityMap completion enabled" }, // DensityMap補完有効
	{ 0x1415e7d50, u8"Mowing disabled" }, // 草刈り無効
	{ 0x1415e7d60, u8"Culling for shadows" }, // 影用のカリング処理
	{ 0x1415e7d80, u8"Occlusion culling process" }, // オクルージョンカリング処理
	{ 0x1415e7da8, u8"Culling by display size" }, // 表示サイズによるカリング
	{ 0x1415e7dd0, u8"Culling by display size of shadow model" }, // 影モデルの表示サイズによるカリング
	{ 0x1415e7e08, u8"ZBias for occlusion culling" }, // オクルージョンカリングのZBias
	{ 0x1415e7e38, u8"LodDither settings" }, // LodDither設定
	{ 0x1415e7e48, u8"Angle culling" }, // 角度カリング
	{ 0x1415e7e60, u8"Culling angle" }, // カリング角度
	{ 0x1415e7e78, u8"Debug function" }, // デバッグ機能
	{ 0x1415e7fd0, u8"Wind direction [0 = 0,0,1]" }, // "風向き[0 = 0,0,1]"
	{ 0x1415e7fe8, u8"Maximum angle" }, // 最大角度
	{ 0x1415e7ff8, u8"Wind wavelength" }, // 風の波長
	{ 0x1415e8008, u8"Wind speed" }, // 風の速度
	{ 0x1415e8018, u8"Wavelength of wind deviation" }, // 風のずれの波長
	{ 0x1415e8030, u8"Strength of wind shift" }, // 風のずれの強さ
	{ 0x1415e8048, u8"Wind shift stage" }, // 風のずれの段階
	{ 0x1415e8060, u8"Percentage of return width" }, // 戻り幅の割合
	{ 0x1413bc3a0, u8"Valid" }, // 有効
	{ 0x1415e8108, u8"Simulation radius" }, // シミュレーション半径
	{ 0x1415e8128, u8"Depth of recess" }, // 凹む深さ
	{ 0x1415e8138, u8"Normal strength" }, // 法線の強さ
	{ 0x1415e8148, u8"Swelling at the border" }, // 境界部の盛り上がり
	{ 0x1415e8168, u8"Border fade" }, // 境界フェード
	{ 0x1415e81d0, u8"Enable" }, // Enable
	{ 0x1415e81d8, u8"Parallel light color scale" }, // 平行光の色スケール
	{ 0x1415e81f8, u8"Strength of parallel light" }, // 平行光の強さ
	{ 0x1415e8210, u8"Rayleigh" }, // Rayleigh
	{ 0x1415e821c, u8"Mie" }, // Mie
	{ 0x1415e8220, u8"Mie Power" }, // Mie Power
	{ 0x141235924, u8"Near" }, // Near
	{ 0x14123592c, u8"Far" }, // Far
	{ 0x1415e8230, u8"Depth Scale" }, // Depth Scale
	{ 0x1415ee350, u8"Tile X" }, // タイルX
	{ 0x1415ee360, u8"Tile Y" }, // タイルY
	{ 0x1415ee370, u8"Noise/non-noise switching speed" }, // ノイズ有り無し切り替わり速度
	{ 0x1415ee3a0, u8"Noise switching speed" }, // ノイズの切り替わり速度
	{ 0x1415ee3c8, u8"Change speed of extraction threshold" }, // 抜きの閾値の変化速度
	{ 0x1415ee3e8, u8"Minimum extraction threshold" }, // 抜きの閾値最小値
	{ 0x1415ee408, u8"Maximum extraction threshold" }, // 抜きの閾値最大値
	{ 0x1415ee428, u8"Probability that a noise-free frame will be selected" }, // ノイズ無しフレームが選ばれる確率
	{ 0x1415ee460, u8"Scan line R speed" }, // 走査線R速度
	{ 0x1415ee478, u8"Scan line R blend rate" }, // 走査線Rブレンド率
	{ 0x1415ee498, u8"Scan line G speed" }, // 走査線G速度
	{ 0x1415ee4b0, u8"Scan line G blend rate" }, // 走査線Gブレンド率
	{ 0x1415ee4d0, u8"RGB line scale" }, // RGB線スケール
	{ 0x1415ee4e8, u8"RGB line blend rate" }, // RGB線ブレンド率
	{ 0x1415ee6a8, u8"DOF resolution scale" }, // DOF解像度スケール
	{ 0x1415e3ff0, u8"Shadow map width" }, // シャドウマップの幅
	{ 0x1415e4010, u8"Shadow map height" }, // シャドウマップの高さ
	{ 0x1413d1080, u8"Minutes" }, // 分
	{ 0x1413a5448, u8"Debug display" }, // デバッグ表示
	{ 0x1415e28f0, u8"Clear RenderTarget" }, // RenderTargetをクリア
	{ 0x1415e2910, u8"Display IBLProbe Box" }, // IBLProbeのBoxを表示する
	{ 0x1415e5cc8, u8"Parallel light source enabled" }, // 平行光源有効
	{ 0x1415e5c68, u8"Restriction type" }, // 制限タイプ
	{ 0x1415e5c78, u8"Minimum value of light source elevation angle [deg]" }, // 光源仰角の最小値[deg]
	{ 0x1415e5ce0, u8"Parallel light angle limit" }, // 平行光角度制限
	{ 0x1415e2930, u8"Parallel light source settings" }, // 平行光源設定
	{ 0x1415e2948, u8"Point light source enabled" }, // 点光源有効
	{ 0x1415e2958, u8"Do you use distortion effects?" }, // 歪みエフェクトを使うか
	{ 0x1415e2980, u8"Use pre-synthesized IBL" }, // 合成済みのIBLを使用する
	{ 0x1415e29a8, u8"Use lightweight post effects" }, // 軽量ポストエフェクトを使うか
	{ 0x1415e28a0, u8"LocalLightCullingType" }, // LocalLightCullingType
	{ 0x1415e29d8, u8"LocalLight brightness scale" }, // LocalLightの輝度スケール
	{ 0x1415e29f8, u8"Intrashadow IBL attenuation degree" }, // 影内IBL減衰度
	{ 0x1415e2a10, u8"Maximum number of IBLProbes used for drawing" }, // 描画に使うIBLProbeの最大数
	{ 0x1415e2a38, u8"[DEBUG] LocalLight display" }, // [DEBUG] LocalLight表示
	{ 0x1415e2a58, u8"[DEBUG] Buffer display" }, // [DEBUG] バッファ表示
	{ 0x1415e2a78, u8"[DEBUG] Output buffer display" }, // [DEBUG] バッファ表示を出力
	{ 0x1415e2aa0, u8"DebugScreenshotResolutionHeight" }, // debugScreenshotResolutionHeight
	{ 0x1415e2ac0, u8"[DEBUG] Near when outputting depth" }, // [DEBUG]デプス出力時の near
	{ 0x1415e2ae8, u8"[DEBUG] Far when outputting depth" }, // [DEBUG]デプス出力時の far
	{ 0x1415e2b10, u8"[DEBUG] Shadow specular type" }, // [DEBUG] 影内スペキュラタイプ
	{ 0x1415e2b38, u8"[DEBUG] Use SG GITexture V2" }, // [DEBUG] SG GITexture V2を使う
	{ 0x1415e2b60, u8"[DEBUG] AO GITexture test" }, // [DEBUG] AO GITexture テスト
	{ 0x1413bf748, u8"Enable" }, // enable
	{ 0x1415dfc08, u8"Full screen" }, // フルスクリーン
	{ 0x1415dfc20, u8"Color channel type" }, // カラーチャンネルタイプ
	{ 0x1413fc788, u8"Minimum" }, // 最小
	{ 0x1413fc868, u8"Maximum" }, // 最大
	{ 0x1415dfc48, u8"RenderTargetType" }, // renderTargetType
	{ 0x1415dfc60, u8"DepthTargetType" }, // depthTargetType
	{ 0x1415dfc70, u8"Screen type" }, // スクリーンタイプ
	{ 0x1415dfc90, u8"Error checking" }, // エラーチェック
	{ 0x1415dfca8, u8"Visualization mode" }, // 可視化モード
	{ 0x1415e2b80, u8"[DEBUG] DebugScreen" }, // [DEBUG] DebugScreen
	{ 0x1415e2b98, u8"[DEBUG] Debug Screen display" }, // [DEBUG] DebugScreen表示
	{ 0x1415e2bb8, u8"EnableMSAA" }, // enableMSAA
	{ 0x1415e2bc8, u8"[FrustumCulling] Frustum display" }, // [FrustumCulling] Frustum表示
	{ 0x1415e2be8, u8"[FrustumCulling] Frustum position fixed" }, // [FrustumCulling] Frustum位置固定
	{ 0x1415e2c10, u8"[FrustumCulling] Display group index" }, // [FrustumCulling] 表示グループインデックス
	{ 0x1415e2c48, u8"[OcclusionCulling] Occluder display" }, // [OcclusionCulling] Occluder表示
	{ 0x1415e2c70, u8"[OcclusionCulling] Maximum number of vertices in Occluder" }, // [OcclusionCulling] Occluderの最大頂点数
	{ 0x1413f4d6c, u8"Value" }, // value
	{ 0x1415e2ca0, u8"[DEBUG]User parameters" }, // [DEBUG]ユーザーパラメータ
	{ 0x1415ee728, u8"Drawing settings" }, // 描画設定
	{ 0x1415e3808, u8"SG Blend Start Smoothness" }, // SGブレンド開始Smoothness
	{ 0x1415e3828, u8"SG Blend End Smoothness" }, // SGブレンド終了Smoothness
	{ 0x1415e3848, u8"DO blend start Smoothness" }, // DOブレンド開始Smoothness
	{ 0x1415e3868, u8"DO blend end Smoothness" }, // DOブレンド終了Smoothness
	{ 0x1415e3888, u8"DO offset" }, // DOオフセット
	{ 0x1415e38a0, u8"AO offset" }, // AOオフセット
	{ 0x1415ee738, u8"GI" }, // GI
	{ 0x1415e2f98, u8"High precision settings" }, // 高精度設定
	{ 0x1415e2fa8, u8"Applies to semitransparent" }, // 半透明に適用
	{ 0x1415e2fbc, u8"Penetration" }, // 貫通
	{ 0x1415e2fc8, u8"1/4 size" }, // 1/4サイズ
	{ 0x1415e2fd8, u8"Use G buffer normal" }, // Gバッファノーマルを使用する
	{ 0x1415e1390, u8"Ray march number" }, // レイマーチ数
	{ 0x1415e3008, u8"Surface normal distance" }, // 面法線距離
	{ 0x1415e3018, u8"Trace threshold" }, // トレース閾値
	{ 0x1415e3030, u8"Reprojection strength" }, // リプロジェクション強度
	{ 0x1415e3058, u8"Blending ratio with IBL" }, // IBLとのブレンド率
	{ 0x1415e3078, u8"Maximum Roughness at which RLR is displayed" }, // RLRが表示される最大Roughness
	{ 0x1415e30a0, u8"Bokeh level at Roughness" }, // Roughnessでのボケ具合
	{ 0x1415e30c0, u8"Final UV offset strength" }, // 最終UVのオフセット強度
	{ 0x1415ee740, u8"Local reflection" }, // ローカルリフレクション
	{ 0x1415e36f4, u8"Strength" }, // 強さ
	{ 0x1415e4d68, u8"Using abd for lighting" }, // ライティングにabdを使用
	{ 0x1415e4d90, u8"Use prm for lighting" }, // ライティングにprmを使用
	{ 0x1415e4d20, u8"Noise removal" }, // ノイズ除去
	{ 0x1415e4d30, u8"Ray length" }, // Rayの長さ
	{ 0x1415ee768, u8"Screen space GI" }, // スクリーンスペースGI
	{ 0x1413bdbc4, u8"Plane" }, // 平面
	{ 0x1415e1e70, u8"Resolution: Width" }, // 解像度:幅
	{ 0x1415e1e80, u8"Resolution: height" }, // 解像度:高さ
	{ 0x1415ee788, u8"Planar reflection" }, // プラナーリフレクション
	{ 0x1415e0130, u8"Enable bloom" }, // ブルームを有効
	{ 0x1415e0148, u8"Fast mode" }, // 高速モード
	{ 0x1415e0158, u8"Bloom strength" }, // ブルームの強さ
	{ 0x1415e0170, u8"Sample radius scale" }, // サンプル半径スケール
	{ 0x1415e0190, u8"Blur quality" }, // ブラー品質
	{ 0x1415df8d8, u8"Bloom" }, // ブルーム
	{ 0x1415e0240, u8"Exposure method" }, // 露出方法
	{ 0x1415e19d8, u8"Exposure value" }, // 露光値
	{ 0x1415e0200, u8"Manual exposure" }, // 手動露出
	{ 0x1415dffc0, u8"Middle gray" }, // ミドルグレイ
	{ 0x1415dffd8, u8"Maximum brightness" }, // 最大輝度
	{ 0x1415dffe8, u8"Minimum brightness" }, // 最小輝度
	{ 0x1415dfff8, u8"Maximum cutoff brightness" }, // 最大カットオフ輝度
	{ 0x1415e0018, u8"Minimum cutoff brightness" }, // 最小カットオフ輝度
	{ 0x1415e0038, u8"Maximum cutoff percentage" }, // 最大カットオフパーセント
	{ 0x1415e0060, u8"Minimum cutoff percentage" }, // 最小カットオフパーセント
	{ 0x1415e0088, u8"Adaptation time percentage" }, // 順応時間割合
	{ 0x1415e00a0, u8"Brightness range" }, // 輝度レンジ
	{ 0x1415e0220, u8"Automatic exposure" }, // 自動露出
	{ 0x1415ee7b0, u8"Camera exposure" }, // カメラ露出
	{ 0x1415e5380, u8"Tone map type" }, // トーンマップタイプ
	{ 0x1415e5420, u8"White point" }, // ホワイトポイント
	{ 0x1415e5440, u8"Strength of dark areas" }, // 暗い部分の強さ
	{ 0x1415e5458, u8"Angle of intermediate interval" }, // 中間区間の角度
	{ 0x1415e5470, u8"Strength in the middle section" }, // 中間区間の強さ
	{ 0x1415e5488, u8"Strength of bright areas" }, // 明るい部分の強さ
	{ 0x1415e539c, u8"Filmic" }, // Filmic
	{ 0x1415e5520, u8"Monitor maximum brightness" }, // モニター最大輝度
	{ 0x1415e0488, u8"Contrast" }, // コントラスト
	{ 0x1415e5540, u8"Start of linear section" }, // 線形区間の始まり
	{ 0x1415e5560, u8"Length of linear section" }, // 線形区間の長さ
	{ 0x1415e5578, u8"Black tightness" }, // 黒の締り
	{ 0x1415e5374, u8"GT" }, // GT
	{ 0x1415e53a8, u8"Update brightness" }, // 輝度を更新する
	{ 0x1415ee7c0, u8"Tone map" }, // トーンマップ
	{ 0x1415e0680, u8"Enable color correction" }, // 色補正を有効
	{ 0x1415e0698, u8"Dynamic range" }, // ダイナミックレンジ
	{ 0x1415e06b8, u8"Dark contrast" }, // 暗部コントラスト
	{ 0x1415e06d8, u8"Bright contrast" }, // 明部コントラスト
	{ 0x1415e06f8, u8"Enable LUT" }, // LUTを有効
	{ 0x1415e0708, u8"LUT0" }, // LUT0
	{ 0x1415e0710, u8"LUT1" }, // LUT1
	{ 0x1415e0718, u8"Blending rate" }, // ブレンド率
	{ 0x1415e0728, u8"LUT reflection rate" }, // LUT反映率
	{ 0x1415e0738, u8"Enable HLS" }, // HLSを有効
	{ 0x1415e0748, u8"[HLS] Hue offset value [°]" }, // [HLS]色相オフセット値[°]
	{ 0x1415e0770, u8"[HLS] Brightness offset value [%]" }, // [HLS]明度オフセット値[%]
	{ 0x1415e0798, u8"[HLS] Saturation offset value [%]" }, // [HLS]彩度オフセット値[%]
	{ 0x1415e07c0, u8"[HLS] RGB offset value" }, // [HLS]RGBオフセット値
	{ 0x1415e07e0, u8"[HLS] Color uniformity reflection rate [%]" }, // [HLS]色彩の統一反映率[%]
	{ 0x1415e0808, u8"[HLS] Unification of colors: Hue [°]" }, // [HLS]色彩の統一：色相[°]
	{ 0x1415ee7d8, u8"Color correction" }, // 色補正
	{ 0x1415ee7e8, u8"Light scattering" }, // ライトスキャッタリング
	{ 0x1415e0c80, u8"Enable DOF" }, // DOFを有効
	{ 0x1415e0c90, u8"Focus at LookAt position" }, // LookAt位置でピント
	{ 0x1415e0cb0, u8"Maximum front blur depth (near)" }, // 前ボケ最大深度(近)
	{ 0x1415e0cd0, u8"Front blur minimum depth" }, // 前ボケ最小深度
	{ 0x1415e0ce8, u8"Minimum back blur depth" }, // 後ボケ最小深度
	{ 0x1415e0d00, u8"Maximum back blur depth (far)" }, // 後ボケ最大深度(遠)
	{ 0x1415e0d20, u8"Should I use the circle of confusion?" }, // 錯乱円を使うか
	{ 0x1415e0d38, u8"Maximum radius of the circle of confusion" }, // 錯乱円の最大半径
	{ 0x1415e0d58, u8"Blurred circle of confusion scale" }, // ボケの錯乱円のスケール
	{ 0x1415e0d80, u8"Number of bokeh samples" }, // ボケのサンプル数
	{ 0x1415e0da0, u8"Focus distance of the celestial sphere [0.0 will be blurry]" }, // 天球のピント距離[0.0だとボケる]
	{ 0x1415e0dd0, u8"MAX blur ratio [0.0 means no blur]" }, // MAXボケの比率[0.0だとボケない]
	{ 0x1415e0e00, u8"Indication of focus position" }, // ピントが合う位置の表示
	{ 0x1415e0e28, u8"Resolution scale" }, // 解像度スケール
	{ 0x1415e0e40, u8"EnableSWA" }, // enableSWA
	{ 0x1415e0e50, u8"SwaFocus" }, // swaFocus
	{ 0x1415e0e60, u8"SwaFocusRange" }, // swaFocusRange
	{ 0x1415e0e70, u8"SwaNear" }, // swaNear
	{ 0x1415e0e78, u8"SwaFar" }, // swaFar
	{ 0x1415e0e80, u8"Emphasize front blur" }, // 前ボケを強調する
	{ 0x1415e0ea0, u8"Front blur scale" }, // 前ボケスケール
	{ 0x1415e0eb8, u8"Maximum brightness when enhancing front blur" }, // 前ボケ強調時の最大輝度
	{ 0x1413e8ba0, u8"DOF" }, // DOF
	{ 0x1415e3e58, u8"RenderingType" }, // renderingType
	{ 0x1415e3e68, u8"Enable shadow map" }, // シャドウマップを有効にする
	{ 0x1415e3e90, u8"Shadow map filter" }, // シャドウマップフィルター
	{ 0x1415e3eb8, u8"ShadowRangeType" }, // shadowRangeType
	{ 0x1415e3ec8, u8"FitProjection" }, // fitProjection
	{ 0x1415e3ed8, u8"FitNearFar" }, // fitNearFar
	{ 0x1415e3ee8, u8"Cascade split" }, // カスケード分割
	{ 0x1415e3f00, u8"Scene range" }, // シーンレンジ
	{ 0x1415e3f18, u8"Scene center" }, // シーン中心
	{ 0x1415e1c90, u8"ManualLightPos" }, // manualLightPos
	{ 0x1415e3f28, u8"PssmLambda" }, // pssmLambda
	{ 0x1415e3f38, u8"Offset from NearPlane in cascade" }, // カスケードのNearPlaneからのオフセット
	{ 0x1415e3f70, u8"Cascade level" }, // カスケードレベル
	{ 0x1415e3f90, u8"Cascade split ratio" }, // カスケード分割割合
	{ 0x1415e3fb0, u8"Bias value for each cascade" }, // カスケードごとのバイアス値
	{ 0x1415e3938, u8"Bias" }, // バイアス
	{ 0x1413bda38, u8"Offset" }, // オフセット
	{ 0x1415e3fd8, u8"Normal bias" }, // 法線バイアス
	{ 0x1415e4030, u8"BlurSize" }, // blurSize
	{ 0x1415e4040, u8"Fade out distance" }, // フェードアウト距離
	{ 0x1415e4060, u8"Transition distance between cascades" }, // カスケード間の遷移距離
	{ 0x1415e4088, u8"EnableCSMCache" }, // enableCSMCache
	{ 0x1415e4098, u8"CsmCacheMaxHeight" }, // csmCacheMaxHeight
	{ 0x1415e40b0, u8"CsmCacheMinHeight" }, // csmCacheMinHeight
	{ 0x1415e40c8, u8"Maximum number of update passes per CSM cache frame" }, // CSMキャッシュ フレーム毎の最大更新パス数
	{ 0x1415e4108, u8"CSM cache fixed angle of view" }, // CSMキャッシュ 固定画角
	{ 0x1415e4128, u8"CSM cache update threshold [deg]" }, // CSMキャッシュ 更新しきい値[deg]
	{ 0x1415e4158, u8"CSM cache parallax correction" }, // CSMキャッシュ 視差補正
	{ 0x1415e4178, u8"CSM cache parallax correction horizontal bias" }, // CSMキャッシュ 視差補正水平バイアス
	{ 0x1415e41b0, u8"CSM cache parallax correction vertical bias" }, // CSMキャッシュ 視差補正垂直バイアス
	{ 0x1415e41f0, u8"Number of split update frames for CSM cache cascade" }, // CSMキャッシュ カスケードの分割更新フレーム数
	{ 0x1415e4240, u8"CSM cache fade range by light source elevation angle [deg]" }, // CSMキャッシュ 光源仰角によるフェード範囲[deg]
	{ 0x1415e4280, u8"CSM cache minimum value of light source elevation angle [deg]" }, // CSMキャッシュ 光源仰角の最小値[deg]
	{ 0x1415e42b8, u8"Shadow camera ViewMatrix" }, // シャドウカメラViewMatrix
	{ 0x1415e42d8, u8"Shadow camera ProjectionMatrix" }, // シャドウカメラProjectionMatrix
	{ 0x1415e4300, u8"Shadow camera NearDepth" }, // シャドウカメラNearDepth
	{ 0x1415e4320, u8"Shadow camera FarDepth" }, // シャドウカメラFarDepth
	{ 0x1415e4340, u8"Shadow camera LookAtDepth" }, // シャドウカメラLookAtDepth
	{ 0x1415e4368, u8"Back side shadow" }, // 裏面シャドウ
	{ 0x1415e4380, u8"Calculate frustum with shadow camera" }, // フラスタムの計算をシャドウカメラで行う
	{ 0x1415e43c0, u8"EnableDrawSceneAABB" }, // enableDrawSceneAABB
	{ 0x1415e43d8, u8"Show shadow frustum" }, // 影のフラスタムを表示する
	{ 0x1415e4400, u8"EnableDrawCascade" }, // enableDrawCascade
	{ 0x1415e4418, u8"EnableDrawCameraFrustum" }, // enableDrawCameraFrustum
	{ 0x1415e4430, u8"EnableDrawCSMCache" }, // enableDrawCSMCache
	{ 0x1415e4448, u8"EnableClearOnCSMCacheIsInvalidated" }, // enableClearOnCSMCacheIsInvalidated
	{ 0x1415e4470, u8"EnablePauseCamera" }, // enablePauseCamera
	{ 0x1415ee810, u8"Shadow map" }, // シャドウマップ
	{ 0x1415e3948, u8"Falloff by distance" }, // 距離によるフォールオフ
	{ 0x1415e3970, u8"Falloff due to height Minimum height" }, // 高さによるフォールオフ 最小高さ
	{ 0x1415e39a0, u8"Falloff due to height Blurred feet" }, // 高さによるフォールオフ ボケ足
	{ 0x1415ee828, u8"Height map shadow" }, // ハイトマップシャドウ
	{ 0x1415e5a70, u8"Force use of shadow maps" }, // シャドウマップの強制使用
	{ 0x1415e5a98, u8"Using cloud shadows" }, // クラウドシャドウの使用
	{ 0x1415e5ac0, u8"Using heightmap shadows" }, // ハイトマップシャドウの使用
	{ 0x1415ee848, u8"Volumetric shadow" }, // ボリューメトリックシャドウ
	{ 0x1415e4c20, u8"Enable SSAO" }, // SSAOを有効
	{ 0x1415e4c30, u8"Types of SSAO" }, // SSAOの種類
	{ 0x1415e4c40, u8"Blur size" }, // Blurサイズ
	{ 0x1415e4c50, u8"Bilateral threshold" }, // Bilateral閾値
	{ 0x1415e49c8, u8"Fade out end distance" }, // フェードアウト終了距離
	{ 0x1415e49f0, u8"Fade out start distance" }, // フェードアウト開始距離
	{ 0x1415e4a18, u8"Calculated intensity of darkening effect" }, // 計算された暗色化効果の強度
	{ 0x1415e4a40, u8"Distance between objects where AO occurs" }, // AOが発生するオブジェクト間の距離
	{ 0x1415e4a70, u8"Effect on direct light" }, // 直接光への影響度
	{ 0x1415e4c60, u8"SSAO settings" }, // SSAO設定
	{ 0x1415e15f8, u8"HBAO_Intensity" }, // HBAO_強度
	{ 0x1415e1608, u8"HBAO_Angle Bias" }, // HBAO_角度バイアス
	{ 0x1415e1620, u8"HBAO_sampling radius" }, // HBAO_サンプリング半径
	{ 0x1415e1640, u8"HBAO_distance attenuation" }, // HBAO_距離減衰
	{ 0x1415e1658, u8"HBAO_step count" }, // HBAO_ステップ数
	{ 0x1415e1670, u8"HBAO_Gitter" }, // HBAO_Gitter
	{ 0x1415e1680, u8"HBAO_Ray count" }, // HBAO_Ray数
	{ 0x1415e1690, u8"Enable HBAO_gi_mask" }, // HBAO_gi_maskを有効
	{ 0x1415e4c70, u8"HBAO settings" }, // HBAO設定
	{ 0x1415e4de8, u8"SSS_1 time distance" }, // SSS_1回の距離
	{ 0x1415e4e00, u8"SSS_calculation count" }, // SSS_計算回数
	{ 0x1415e4e18, u8"SSS_Shadow sharpness" }, // SSS_影の鮮明度
	{ 0x1415e4e30, u8"SSS_Strength" }, // SSS_強度
	{ 0x1415e4e40, u8"SSS_attenuation start distance" }, // SSS_減衰開始距離
	{ 0x1415e4e58, u8"SSS_decay end distance" }, // SSS_減衰終了距離
	{ 0x1415e4e70, u8"SSS_light distance" }, // SSS_ライト距離
	{ 0x1415e4e88, u8"SSS_Thickness" }, // SSS_厚み
	{ 0x1415e4e98, u8"Enable SSS_gi_mask" }, // SSS_gi_maskを有効
	{ 0x1415e4eb0, u8"SSS_distance limit" }, // SSS_距離制限
	{ 0x1415e4ec8, u8"SSS_dither enabled" }, // SSS_ディザ有効
	{ 0x1415e4c80, u8"SSS settings" }, // SSS設定
	{ 0x1415df92c, u8"SSAO" }, // SSAO
	{ 0x1415e47b0, u8"Debug drawing type" }, // デバッグ描画タイプ
	{ 0x1415e47d0, u8"Debug drawing SkyVisibility display" }, // デバッグ描画SkyVisibility表示
	{ 0x1415e47f8, u8"Debug drawing probe size" }, // デバッグ描画プローブサイズ
	{ 0x1415e4820, u8"Multiply color: top" }, // 乗算カラー:上
	{ 0x1415e4838, u8"Multiply color: bottom" }, // 乗算カラー:下
	{ 0x1415e4850, u8"Normal offset" }, // 法線方向オフセット
	{ 0x1415e1980, u8"Merge AOLF" }, // AOLFをマージする
	{ 0x1415e1998, u8"Blending ratio between LFs" }, // LF同士のブレンド率
	{ 0x1415e4870, u8"AOLF merge settings" }, // AOLFのマージ設定
	{ 0x1415ee870, u8"Light field" }, // ライトフィールド
	{ 0x1415e3598, u8"Enable screen blur" }, // 画面ブラーを有効
	{ 0x1415e35b8, u8"Blur type" }, // ブラータイプ
	{ 0x1415e35d0, u8"Blur strength" }, // ブラーの強さ
	{ 0x1415e35e8, u8"Focus position type" }, // フォーカス位置タイプ
	{ 0x1415e3608, u8"User-defined focus position" }, // ユーザー設定のフォーカス位置
	{ 0x1415e0b28, u8"Range without blur" }, // ブラーがかからない範囲
	{ 0x1415e0b50, u8"Blur alpha scale" }, // ブラーのα値スケール
	{ 0x1415e0b70, u8"The number of samples" }, // サンプル数
	{ 0x1415e3638, u8"Lightweight camera blur" }, // 軽量カメラブラー
	{ 0x1415ee890, u8"Screen blur" }, // 画面ブラー
	{ 0x1415e1a18, u8"Occlusion enabled" }, // Occlusion有効
	{ 0x1415e1a28, u8"Occlusion color" }, // Occlusionカラー
	{ 0x1415e1a40, u8"Occlusion density" }, // Occlusion濃さ
	{ 0x1415e1a50, u8"Specular Occlusion enabled" }, // SpecularOcclusion有効
	{ 0x1415e1a68, u8"SpecularOcclusionTransparency" }, // SpecularOcclusion透明度
	{ 0x1415e1a88, u8"Specular Occlusion Density" }, // SpecularOcclusion濃さ
	{ 0x1415e1aa0, u8"SpecularOcclusion cone angle" }, // SpecularOcclusionコーン角度
	{ 0x1415e1ac8, u8"Shadow enabled" }, // Shadow有効
	{ 0x1415e1ad8, u8"Shadow color" }, // Shadowカラー
	{ 0x1415e1ae8, u8"Shadow density" }, // Shadow濃さ
	{ 0x1415e1af8, u8"Shadow cone angle" }, // Shadowコーン角度
	{ 0x1415e1380, u8"Effective range" }, // 有効範囲
	{ 0x1415e1b10, u8"Enable manual light settings" }, // 手動でのライト設定を有効にする
	{ 0x1415e1b40, u8"Number of manually set lights" }, // 手動設定ライトの数
	{ 0x1415e1b60, u8"Manual setting light position" }, // 手動設定ライト位置
	{ 0x1415e1b80, u8"Debug drawing" }, // デバッグ描画
	{ 0x1415ee8a0, u8"Occlusion capsule" }, // オクルージョンカプセル
	{ 0x1415dedd8, u8"LightFieldColor coefficient" }, // LightFieldColor係数
	{ 0x1415dedf0, u8"Intensity offset factor" }, // 強度オフセット係数
	{ 0x1415dee0c, u8"Shadow color" }, // 影色
	{ 0x1415dee18, u8"Parallel light color" }, // 並行光色
	{ 0x1415dee28, u8"Parallel light intensity" }, // 平行光Intensity
	{ 0x1415dee40, u8"Override parallel light color" }, // 平行光色を上書きする
	{ 0x1415dee60, u8"Point light intensity scale" }, // ポイントライトの強度スケール
	{ 0x1413baa08, u8"LOD distance" }, // LOD距離
	{ 0x1415dee90, u8"Output overdraw to debug screen" }, // オーバードローをデバッグスクリーンに出力する
	{ 0x1415deed8, u8"Wireframe display" }, // ワイヤーフレーム表示
	{ 0x1415def00, u8"Bilateral upsampling of reduced buffers" }, // 縮小バッファをバイラテラルアップサンプリング
	{ 0x1413d31f0, u8"Effect" }, // エフェクト
	{ 0x1415e36c8, u8"Length of light beam" }, // 光筋の長さ
	{ 0x1415e36d8, u8"Threshold" }, // しきい値
	{ 0x1415e36e8, u8"Maximum value" }, // 最大値
	{ 0x1415e3700, u8"Dither enabled" }, // ディザ有効
	{ 0x1415e3710, u8"LightPos" }, // lightPos
	{ 0x1415e3720, u8"LightDir" }, // lightDir
	{ 0x1415ee8c8, u8"Screen space godray" }, // スクリーンスペースゴッドレイ
	{ 0x1415e12f8, u8"Using shadow maps" }, // シャドウマップの使用
	{ 0x1415e1318, u8"Variable ray march" }, // 可変レイマーチ
	{ 0x1413ea8a8, u8"Shadow" }, // shadow
	{ 0x1415e1330, u8"Display range" }, // 表示範囲
	{ 0x1415e1340, u8"Color scale" }, // 色スケール
	{ 0x1415e1350, u8"Fog density" }, // フォグの濃さ
	{ 0x1415e1368, u8"Diffusion of light" }, // 光の拡散具合
	{ 0x1415e13a8, u8"RayMarchingStep" }, // rayMarchingStep
	{ 0x1415e13b8, u8"Emphasize shadow edges" }, // 影のエッヂを強調
	{ 0x1415e13d8, u8"IsScanFromBack" }, // isScanFromBack
	{ 0x1415e13e8, u8"BoxCount" }, // boxCount
	{ 0x1415e13f8, u8"New method (1/4)" }, // 新方式(1/4)
	{ 0x1415e1568, u8"EnableVolumeTexture" }, // enableVolumeTexture
	{ 0x1415e1580, u8"Noise texture scale" }, // ノイズテクスチャのスケール
	{ 0x1415e15a8, u8"Noise texture scroll speed" }, // ノイズテクスチャのスクロール速度
	{ 0x1415e1408, u8"Volume texture" }, // ボリュームテクスチャ
	{ 0x1415e1428, u8"Transparency degree" }, // 透過度合
	{ 0x1415e1438, u8"3D texture volumetric light" }, // 3Dテクスチャボリューメトリックライト
	{ 0x1415e1470, u8"3D reprojection strength" }, // 3Dリプロジェクション強度
	{ 0x1415e1498, u8"3D logarithmic coefficient" }, // 3D対数係数
	{ 0x1413e9a80, u8"Volumetric light" }, // ボリューメトリックライト
	{ 0x1413c13a0, u8"Speed" }, // スピード
	{ 0x1413b6078, u8"Scale" }, // スケール
	{ 0x1413c10d8, u8"Period" }, // 周期
	{ 0x1415e1748, u8"Distance it starts to take" }, // かかり始める距離
	{ 0x1415e1768, u8"Maximum distance" }, // 最大になる距離
	{ 0x1415e1780, u8"Altitude limit" }, // 高度制限
	{ 0x1415e1790, u8"Parallax correction coefficient" }, // 視差補正係数
	{ 0x1415ee8f4, u8"Heat haze" }, // 陽炎
	{ 0x1415ee900, u8"Atmospheric optics" }, // 大気光学系
	{ 0x1415ee2e8, u8"Moisture level" }, // 湿り具合
	{ 0x1415ede28, u8"Heightmap threshold" }, // ハイトマップ閾値
	{ 0x1415ede48, u8"Ground slope threshold" }, // 地面の傾き閾値
	{ 0x1415ede60, u8"XZ scale of puddle texture" }, // 水たまりテクスチャのXZスケール
	{ 0x1415ede90, u8"Y scale of puddle texture" }, // 水たまりテクスチャのYスケール
	{ 0x1415edec0, u8"Puddle texture threshold" }, // 水たまりテクスチャの閾値
	{ 0x1415edee8, u8"Falloff width of puddle texture" }, // 水たまりテクスチャの減衰幅
	{ 0x1415ee2f8, u8"Puddle" }, // 水たまり
	{ 0x1415edf90, u8"Enable raindrops" }, // 雨粒有効化
	{ 0x1415edfa0, u8"Enable droplet filter" }, // 飛沫フィルタ有効化
	{ 0x1415edfc0, u8"Ripple enable" }, // 波紋有効化
	{ 0x1415edfd0, u8"Intensity of rain" }, // 雨の強さ
	{ 0x1415edfe0, u8"Color of rain" }, // 雨の色
	{ 0x1415edff0, u8"Rotate the raindrop camera at the point of interest" }, // 雨粒のカメラ回転を注視点位置で行う
	{ 0x1415ee028, u8"Shielding raindrops" }, // 雨粒を遮蔽
	{ 0x1415ee038, u8"Raindrop camera interpolation coefficient" }, // 雨粒のカメラ補完係数
	{ 0x1415ee058, u8"Raindrop camera influence coefficient" }, // 雨粒のカメラ影響係数
	{ 0x1415ee078, u8"Raindrop camera rotation" }, // 雨粒のカメラ回転
	{ 0x1415ee098, u8"Thickness of raindrops" }, // 雨粒の太さ
	{ 0x1415ee0a8, u8"Raindrop length" }, // 雨粒の長さ
	{ 0x1415ee0b8, u8"Wind influence coefficient of raindrops" }, // 雨粒の風影響係数
	{ 0x1415ee0d8, u8"Range of raindrops" }, // 雨粒の範囲
	{ 0x1415ee0e8, u8"Filter color" }, // フィルタの色
	{ 0x1415ee100, u8"Filter range" }, // フィルタの範囲
	{ 0x1415ee118, u8"Filter depth difference strength" }, // フィルタの深度差分強度
	{ 0x1415ee140, u8"Filter normal strength" }, // フィルタの法線方向強度
	{ 0x1415ee2c0, u8"Strength of ripples" }, // 波紋の強さ
	{ 0x1415ede00, u8"Time scale" }, // 時間スケール
	{ 0x1415ee168, u8"Ripple settings" }, // 波紋設定
	{ 0x1415ee178, u8"Raindrop settings" }, // 雨粒設定
	{ 0x1413cfd1c, u8"Rain" }, // 雨
	{ 0x1415ee910, u8"Weather" }, // 天気
	{ 0x1415e31b0, u8"Wind direction: Z axis" }, // 風の方向:Z軸
	{ 0x1415e31c8, u8"Wind strength" }, // 風の強さ
	{ 0x1415e31d8, u8"Wind complexity" }, // 風の複雑度
	{ 0x1415e31e8, u8"Wind amplitude" }, // 風の振幅
	{ 0x1415e31f8, u8"Wind cycle" }, // 風の周期
	{ 0x1415e3208, u8"Effective for pushing down grass" }, // 草の押し倒し有効
	{ 0x1415e3228, u8"Grass LOD distance" }, // 草のLOD距離
	{ 0x1415e3238, u8"Effective for standing out in the dark" }, // 暗い時の目立たせ有効
	{ 0x1415e3258, u8"Make it stand out when it is darker than this value" }, // この値より暗い時に目立たせる
	{ 0x1415e3288, u8"Object: Dodge" }, // オブジェクト:覆い焼き
	{ 0x1415e32a8, u8"Object: brightness" }, // オブジェクト:明るさ
	{ 0x1415e32c8, u8"Character: Dodge" }, // キャラ:覆い焼き
	{ 0x1415e32e0, u8"Character: Brightness" }, // キャラ:明るさ
	{ 0x1415e32f8, u8"Character: Falloff Strength" }, // キャラ:Falloff強さ
	{ 0x1415ee918, u8"Scene environment" }, // シーン環境
	{ 0x1415e4fa0, u8"Enable upscaling" }, // アップスケーリングを有効にする
	{ 0x1415e4fd0, u8"Jitter scale" }, // ジッタースケール
	{ 0x1415e4ff0, u8"Mip bias" }, // ミップバイアス
	{ 0x1415e5008, u8"Sharpness filter strength" }, // シャープネスフィルタの強さ
	{ 0x1415e5030, u8"Reference weight of latest frame" }, // 最新フレームの基準ウェイト
	{ 0x1415e5060, u8"Latest frame weight bias according to velocity variance" }, // 速度分散に応じた最新フレームのウェイトバイアス
	{ 0x1415e50a8, u8"Color space scale" }, // 色空間のスケール
	{ 0x1415e50c8, u8"Color space scale [stationary objects]" }, // 色空間のスケール[静止物]
	{ 0x1415e50f0, u8"Minimum velocity dispersion to improve edges" }, // エッジを改善する最小速度分散
	{ 0x1415e5120, u8"Maximum velocity dispersion to improve edges" }, // エッジを改善する最大速度分散
	{ 0x1415e5150, u8"Character stencil mask" }, // キャラクターステンシルマスク
	{ 0x1415e5180, u8"Lightweight mode" }, // 軽量モード
	{ 0x1415e5190, u8"FXAA settings for NX handheld" }, // NXハンドヘルド用FXAA設定
	{ 0x1415e51b8, u8"Visualize velocity dispersion" }, // 速度分散を可視化する
	{ 0x14140df54, u8"Debug" }, // debug
	{ 0x1413e866c, u8"TAA" }, // TAA
	{ 0x1415ee928, u8"Vegetation map" }, // 植生マップ
	{ 0x1415e02a0, u8"Displacement R" }, // 変位R
	{ 0x1415e02a8, u8"Displacement G" }, // 変位G
	{ 0x1415e02b0, u8"Displacement B" }, // 変位B
	{ 0x1415e02b8, u8"Displacement gamma" }, // 変位ガンマ
	{ 0x1415e02c8, u8"Displacement scale" }, // 変位スケールX
	{ 0x1415e02e0, u8"Displacement scale Y" }, // 変位スケールY
	{ 0x1415e02f8, u8"Center position" }, // 中心位置X
	{ 0x1415e0308, u8"Center position Y" }, // 中心位置Y
	{ 0x1415ee938, u8"Chromatic aberration" }, // 色収差
	{ 0x1415e5990, u8"Gradient type" }, // グラデーションタイプ
	{ 0x1415e59b0, u8"Blend mode" }, // ブレンドモード
	{ 0x1415e59c8, u8"Opacity" }, // 不透明度
	{ 0x1415e57c8, u8"Center point" }, // 中心点X
	{ 0x1415e57d8, u8"Center point Y" }, // 中心点Y
	{ 0x1415e57e8, u8"Scale x" }, // スケールX
	{ 0x1415e57f8, u8"Scale Y" }, // スケールY
	{ 0x1415e5808, u8"Whole scale" }, // スケール全体
	{ 0x1413f3c30, u8"Rotate" }, // 回転
	{ 0x1415e59d8, u8"Image circle" }, // イメージサークル
	{ 0x1415e5840, u8"Direction x" }, // 方向X
	{ 0x1415e5848, u8"Direction Y" }, // 方向Y
	{ 0x1413e0bdc, u8"Straight line" }, // 直線
	{ 0x1415e59f8, u8"Penumbra scale" }, // 半影スケール
	{ 0x1413ba40c, u8"Strength" }, // 強度
	{ 0x1415e55f0, u8"Minimum penumbra scale" }, // 最小半影スケール
	{ 0x1415e5610, u8"Maximum penumbra scale" }, // 最大半影スケール
	{ 0x1415e5630, u8"Bokeh area scale" }, // ボケ領域スケール
	{ 0x1415e5650, u8"Minimum DOF transparency scale" }, // 最小DOF透明度スケール
	{ 0x1415e5670, u8"Max DOF transparency scale" }, // 最大DOF透明度スケール
	{ 0x1415e5690, u8"Minimum transparency scale" }, // 最小透明度スケール
	{ 0x1415e56b0, u8"Maximum transparency scale" }, // 最大透明度スケール
	{ 0x1415e56d0, u8"Minimum transparency distance" }, // 最小透明度距離
	{ 0x1415e56e8, u8"Maximum transparency distance" }, // 最大透明度距離
	{ 0x1413cb8a8, u8"Depth" }, // 奥行き
	{ 0x1415ee948, u8"Vignette" }, // ビネット
	{ 0x1415e6068, u8"Global activation" }, // 全体の有効化
	{ 0x1415e6080, u8"Dynamicwind/enabled (GlobalWindMap random number interlocking)" }, // Dynamicwind・有効 (GlobalWindMap乱数連動)
	{ 0x1415e60b8, u8"Globalwind・Enabled (GlobalWindMap random number link)" }, // Globalwind・有効 (GlobalWindMap乱数連動)
	{ 0x1415e60e8, u8"DynamicWind・Time coefficient" }, // DynamicWind・時間係数
	{ 0x1415e6108, u8"Decrease rate" }, // 　　　　　　 減少率
	{ 0x1415e6128, u8"Straight line wind A/wind direction (vector notation)" }, // 直線風A・風向き(ベクトル表記)
	{ 0x1415e6158, u8"Wind strength" }, // 　　　 　風の強さ
	{ 0x1415e6178, u8"Speed of wind sway" }, // 　　　 　風揺れの速さ
	{ 0x1415e6198, u8"Wind sway return amount" }, // 　　　 　風揺れ戻り量
	{ 0x1415e61b8, u8"Straight line wind B/wind direction (vector notation)" }, // 直線風B・風向き(ベクトル表記)
	{ 0x1415e61e8, u8"GlobalWindMap・Size" }, // GlobalWindMap・サイズ
	{ 0x1415e6208, u8"Center" }, // 　　　　　　　 中心
	{ 0x1415e6228, u8"Offset" }, // 　　　　　　　 オフセット
	{ 0x1415e6250, u8"Strength" }, // 　　　　　　　 強さ
	{ 0x1415e6270, u8"Speed of shaking" }, // 　　　　　　　 揺れの速さ
	{ 0x1415e5f18, u8"Buffer display" }, // バッファ表示
	{ 0x1415e5f30, u8"Spatial display" }, // 空間表示
	{ 0x1415e5f40, u8"Displaying global wind in space" }, // グローバル風を空間に表示
	{ 0x1415e5f68, u8"Display GlobalWindMap in space" }, // GlobalWindMapを空間に表示
	{ 0x1415e5f88, u8"Full space display" }, // 全空間表示
	{ 0x1415e5f98, u8"Space number to display" }, // 表示する空間の番号
	{ 0x1415e5fb8, u8"Wind threshold for spatial display" }, // 空間表示する風の閾値
	{ 0x1413d1bb0, u8"DebugParam" }, // debugParam
	{ 0x1415ee958, u8"GPU_Kaze" }, // GPU_風
	{ 0x1415e5e50, u8"Enable blending" }, // ブレンドを有効
	{ 0x1415e5e68, u8"Blend contrast" }, // ブレンドのコントラスト
	{ 0x1415e5e90, u8"Height blend influence rate" }, // ハイトブレンドの影響率
	{ 0x1415e5eb8, u8"Culling distance" }, // カリング距離
	{ 0x1415ee960, u8"Terrain material blend" }, // 地形マテリアルブレンド
	{ 0x1415df0a8, u8"Grass leaf dither depth start" }, // 草葉のディザ深度開始
	{ 0x1415df0c8, u8"Grass leaf dither depth end" }, // 草葉のディザ深度終了
	{ 0x1415df260, u8"Collision enabled" }, // 衝突有効
	{ 0x1415df270, u8"Strength factor" }, // 強度係数
	{ 0x1415df280, u8"Decrease value" }, // 減少値
	{ 0x1415de7d0, u8"Time factor" }, // 時間係数
	{ 0x1415df1c0, u8"Collision buffer display enabled" }, // 衝突バッファ表示有効
	{ 0x1415df1e0, u8"Collision space display enabled" }, // 衝突空間表示有効
	{ 0x1415df200, u8"Threshold for spatial display" }, // 空間表示する閾値
	{ 0x1415df290, u8"Shaking intensity coefficient" }, // 揺れの強さ係数
	{ 0x1415df2a8, u8"Shaking speed coefficient" }, // 揺れの速さ係数
	{ 0x1415df2c0, u8"Reducing the amplitude of shaking due to the size of the object" }, // 物体の大きさによる揺れ幅軽減
	{ 0x1415df2f0, u8"Reducing speed due to object size" }, // 物体の大きさによる速さの軽減
	{ 0x1415df0e8, u8"Collision settings" }, // 衝突設定
	{ 0x1415df0f8, u8"Lod correction when zooming" }, // Zoom時のLod補正
	{ 0x1415ee988, u8"GPU environment settings" }, // GPU環境設定
	{ 0x1415de7e0, u8"Distance at which decay begins" }, // 減衰が始まる距離
	{ 0x1415de800, u8"Wave speed" }, // 波の速度
	{ 0x1415de810, u8"Simulation range" }, // シミュレーション範囲
	{ 0x1415de830, u8"Player's maximum squared velocity (m/frame)" }, // プレイヤーの最大2乗速度(m/フレーム)
	{ 0x1415de868, u8"[Currently unused] Water entry judgment dummy" }, // [現在未使用]入水判定ダミー
	{ 0x1415ee998, u8"Interactive waves" }, // インタラクティブ波
	{ 0x1415ee9b8, u8"Cyber space noise effect" }, // 電脳空間ノイズエフェクト
	{ 0x1415df468, u8"Low frequency block noise size" }, // 低周波ブロックノイズサイズX
	{ 0x1415df498, u8"Low frequency block noise size Y" }, // 低周波ブロックノイズサイズY
	{ 0x1415df4c8, u8"High frequency block noise size" }, // 高周波ブロックノイズサイズX
	{ 0x1415df4f8, u8"High frequency block noise size Y" }, // 高周波ブロックノイズサイズY
	{ 0x1415df528, u8"Ease of generating high frequencies" }, // 高周波の出やすさ
	{ 0x1415e5de8, u8"UV shift strength" }, // UVシフトの強さ
	{ 0x1415e5e00, u8"UV shift strength per pixel" }, // ピクセル単位のUVシフトの強さ
	{ 0x1415e0950, u8"UV shift" }, // UVシフト
	{ 0x1415df5e0, u8"Low frequency block noise size" }, // 低周波ブロックノイズサイズ
	{ 0x1415df608, u8"High frequency block noise size" }, // 高周波ブロックノイズサイズ
	{ 0x1415df630, u8"Strength of color shift" }, // カラーシフトの強さ
	{ 0x1415e0960, u8"RGB shift" }, // RGBずれ
	{ 0x1415e5d28, u8"Interlace appearance rate" }, // インターレース出現率
	{ 0x1415e5d48, u8"Ease of interlace line breakage" }, // インターレース線の途切れやすさ
	{ 0x1415e0970, u8"Interlace noise" }, // インターレースノイズ
	{ 0x1415df548, u8"Color subtraction strength" }, // 色減算の強さ
	{ 0x1415e0990, u8"Color fading" }, // 色落ち
	{ 0x1415e5d90, u8"Probability of color inversion" }, // 色が反転する確率
	{ 0x1415e5db0, u8"Probability that colors will be reversed across the screen" }, // 画面全体で色が反転する確率
	{ 0x1415e09a0, u8"Color inversion" }, // 色反転
	{ 0x1415e5b50, u8"Probability of becoming monochrome at block level" }, // ブロックレベルでモノクロになる確率
	{ 0x1415e5b88, u8"Probability that the entire screen will become monochrome" }, // 画面全体でモノクロ化する確率
	{ 0x1415e09b0, u8"Monochrome" }, // モノクロ化
	{ 0x1415e09c0, u8"Noise update rate" }, // ノイズの更新速度
	{ 0x1415e09e0, u8"Abundance bias" }, // 出現量バイアス
	{ 0x1415e09f8, u8"Amplitude of change in appearance amount" }, // 出現量の変化の振れ幅
	{ 0x1415e0a18, u8"Period of change in appearance amount" }, // 出現量変化の周期
	{ 0x1415ee9e0, u8"Cyber space start noise effect" }, // 電脳空間開始ノイズエフェクト
	{ 0x1415eea10, u8"NPC cyber expression SS effect" }, // NPCサイバー表現SSエフェクト
	{ 0x1415e1040, u8"Scan display enabled" }, // スキャン表示有効
	{ 0x1415e1060, u8"Scan center coordinates" }, // スキャン中心座標
	{ 0x1415e1090, u8"Scan circle 1 radius" }, // スキャン円1 半径
	{ 0x1415e10a8, u8"Scan circle 2 radius" }, // スキャン円2 半径
	{ 0x1415e10c0, u8"Scan circle 3 radius" }, // スキャン円3 半径
	{ 0x1415e10d8, u8"Scan circle 1 brightness" }, // スキャン円1 明るさ
	{ 0x1415e10f8, u8"Scan circle 2 brightness" }, // スキャン円2 明るさ
	{ 0x1415e1118, u8"Scan circle 3 brightness" }, // スキャン円3 明るさ
	{ 0x1415e1138, u8"Grid brightness" }, // グリッドの明るさ
	{ 0x1415e1158, u8"Gradient width within the circle" }, // 円内のグラデーションの幅
	{ 0x1415e1180, u8"Grid line thinness (the higher the value, the thinner it is)" }, // グリッドの線細さ（値が大きいほど細い）
	{ 0x1415e11c0, u8"Grid spacing" }, // グリッドの間隔
	{ 0x1415eea38, u8"Map scan effect" }, // 地図スキャンエフェクト
	{ 0x1415e0468, u8"Filter strength" }, // フィルタ強度
	{ 0x1415e03b0, u8"Type 1 color vision [red]" }, // 1型色覚[赤]
	{ 0x1415e03e0, u8"Type 2 color vision [green]" }, // 2型色覚[緑]
	{ 0x1415e0410, u8"Type 3 color vision [blue]" }, // 3型色覚[青]
	{ 0x1415e047c, u8"Luminance" }, // 輝度
	{ 0x1415e04a0, u8"Influence of mask texture" }, // マスクテクスチャの影響度
	{ 0x1415e04c8, u8"[For development] Color vision simulation" }, // [開発用]色覚シミュレーション
	{ 0x1415eea60, u8"Color vision filter" }, // 色覚フィルタ
	{ 0x1415eea74, u8"Footprints" }, // 足跡
	{ 0x1415e3750, u8"Enable SeparableSSS" }, // SeparableSSSを有効
	{ 0x1413ad200, u8"Width" }, // 幅
	{ 0x1415e3768, u8"Normal extrusion" }, // 法線押し出し
	{ 0x1415e3780, u8"Blur threshold" }, // ブラー閾値
	{ 0x1415e3790, u8"Color for each CDRF index" }, // CDRF各インデックスごとのカラー
	{ 0x1415eea7c, u8"SSSS" }, // SSSS
	{ 0x1415eeaa0, u8"RenderTarget size" }, // RenderTargetの大きさ
	{ 0x1415dfe40, u8"Antialias type" }, // アンチエイリアスタイプ
	{ 0x1415dfe68, u8"Upscale type" }, // アップスケールタイプ
	{ 0x1415dfe88, u8"FSR sharpness" }, // FSRシャープネス
	{ 0x1415eeac0, u8"Antialiasing" }, // アンチエイリアス
	{ 0x1415eee18, u8"Brightness scale" }, // 輝度スケール
	{ 0x1415eeae0, u8"Atmospheric optics settings" }, // 大気光学設定
	{ 0x1415df388, u8"Layer range [m]" }, // レイヤー範囲[m]
	{ 0x1415eeaf8, u8"LOD settings" }, // LOD設定
	{ 0x1415de9e8, u8"Distance at which Detailmap disappears [m]" }, // Detailmapが消える距離[m]
	{ 0x1415dea08, u8"Blur strength when switching" }, // 切り替え時のぼかしの強さ
	{ 0x1415eeb08, u8"DetailMap settings" }, // DetailMap設定
	{ 0x1415deb58, u8"Dynamic resolution mode" }, // 動的解像度モード
	{ 0x1415deb78, u8"Resolution: Only valid in fixed resolution mode" }, // 解像度：固定解像度モードのみ有効
	{ 0x1415debb0, u8"Minimum resolution ratio" }, // 最小解像度比
	{ 0x1415debc8, u8"Difference from target time for expansion" }, // 拡大用の目標時間との差
	{ 0x1415debf0, u8"Difference from target time for reduction" }, // 縮小用の目標時間との差
	{ 0x1415dec18, u8"Resolution magnification" }, // 解像度の拡大率
	{ 0x1415dec30, u8"Resolution reduction rate" }, // 解像度の縮小率
	{ 0x1415dec48, u8"Maximum change rate of resolution expansion" }, // 解像度の拡大最大変化率
	{ 0x1415dec70, u8"Maximum change rate of resolution reduction" }, // 解像度の縮小最大変化率
	{ 0x1415deca0, u8"For checking: Force resolution to change over time" }, // チェック用:強制的に時間経過に応じて解像度変化させる
	{ 0x1415eeb18, u8"Dynamic resolution settings" }, // 動的解像度設定
	{ 0x1415eef18, u8"Basic time progress speed/seconds per hour" }, // 基本時刻進行速度/1時間あたりの秒数
	{ 0x1415edd10, u8"Start time" }, // 開始時刻
	{ 0x1415edd20, u8"End time" }, // 終了時刻
	{ 0x1415eeda8, u8"Seconds per hour" }, // 1時間あたりの秒数
	{ 0x1415eef50, u8"Overwrite time progress speed" }, // 上書き時刻進行速度
	{ 0x1415eef70, u8"Night time section" }, // 夜時間区間
	{ 0x1415eeb30, u8"Tense change settings" }, // 時制変化設定
	{ 0x1415eeb48, u8"Weather change settings" }, // 天候変化設定
	{ 0x1415edcc8, u8"Terrain grid drawing" }, // 地形グリッド描画
	{ 0x1415eeb60, u8"Heightmap terrain settings" }, // ハイトマップ地形設定
	{ 0x1415df3e0, u8"ZPrePass Enable" }, // ZPrePass Enable
	{ 0x1415df3f0, u8"BlueNoise Dither" }, // BlueNoiseDither
	{ 0x1415eeb80, u8"Model settings" }, // モデル設定
	{ 0x1415eee50, u8"Culling range" }, // カリングレンジ
	{ 0x1415eeb90, u8"Decal model settings" }, // デカールモデル設定
	{ 0x1415eed50, u8"Small stuff culling threshold" }, // 小さいものカリング閾値
	{ 0x1415eebb0, u8"Performance settings" }, // パフォーマンス設定
	{ 0x1413db448, u8"Common setting" }, // 共通設定
	{ 0x1415eec80, u8"Parameter list" }, // パラメータリスト
	{ 0x1415edda8, u8"Camera exposure [event]" }, // カメラ露出[イベント]
	{ 0x1415edd58, u8"Celestial sphere [bruneton]" }, // 天球[bruneton]
	{ 0x1415edd70, u8"Celestial sphere [sebastien]" }, // 天球[sebastien]
	{ 0x1415eeca0, u8"Time-varying parameter list" }, // 時間変化パラメーターリスト
	{ 0x1415eecc8, u8"Weather change parameter list" }, // 天候変化パラメーターリスト
	{ 0x1415eee98, u8"Death altitude" }, // 死亡高度
	{ 0x1415eeea8, u8"Sea level altitude" }, // 海面高度
	{ 0x1415eeeb8, u8"Death fall time" }, // 死亡落下時間
	{ 0x1413a8878, u8"Basic configuration" }, // 基本設定
	{ 0x1413bf2d4, u8"Fovy" }, // Fovy
	{ 0x141392dd8, u8"Camera settings" }, // カメラ設定
	{ 0x1415eda08, u8"Heightmap terrain" }, // ハイトマップ地形
	{ 0x1415eda28, u8"Heightmap accuracy 16bit" }, // ハイトマップ精度16bit
	{ 0x1415edc78, u8"Height map range" }, // ハイトマップレンジ
	{ 0x1415eda48, u8"Parameters at 16bit" }, // 16bit時のパラメータ
	{ 0x1415eda68, u8"Terrain size [meters]" }, // 地形の大きさ[メートル]
	{ 0x1415eda90, u8"Heightmap density [pixel/m]" }, // ハイトマップ密度[pixel/m]
	{ 0x1415edab8, u8"Heightmap height scale" }, // ハイトマップ高さスケール
	{ 0x1415edae0, u8"Mesh minimum division unit" }, // メッシュ最小分割単位
	{ 0x1415edb00, u8"Slope Clip threshold" }, // 斜面Clip閾値
	{ 0x1415edb18, u8"Range Minimum value of xz plane coordinates" }, // 範囲 xz平面座標の最小値
	{ 0x1415edb40, u8"Maximum value of range xz plane coordinates" }, // 範囲 xz平面座標の最大値
	{ 0x1415eeff0, u8"UV scale of detail texture" }, // DetailテクスチャのUVスケール
	{ 0x1415ef018, u8"Base texture UV scale" }, // BaseテクスチャのUVスケール
	{ 0x1415ef040, u8"Distance at which Detail texture disappears [m]" }, // Detailテクスチャが消える距離[m]
	{ 0x1415edb68, u8"Material settings" }, // 素材設定
	{ 0x1415edb78, u8"Blending Placements and Heightmap Terrain" }, // 配置物とハイトマップ地形のブレンド
	{ 0x1415eefe0, u8"Terrain settings" }, // 地形設定
	{ 0x1415eecf0, u8"Stage setting" }, // ステージ設定
	{ 0x1413a4868, u8"Node" }, // ノード
	{ 0x1413cb674, u8"Shape" }, // 形状
	{ 0x1413e4af8, u8"Priority" }, // プライオリティ
	{ 0x1413e2568, u8"Position offset" }, // 位置オフセット
	{ 0x1413f2bd0, u8"Rotational offset" }, // 回転オフセット
	{ 0x1415de998, u8"Rate of various occlusions" }, // 各種オクルージョンの割合
	{ 0x1415de9c0, u8"Effective radius" }, // 有効半径
	{ 0x1415de9d0, u8"LOD" }, // LOD
	{ 0x1415dea68, u8"Fog color" }, // フォグカラー
	{ 0x1415dea80, u8"Fog color intensity" }, // フォグカラー強度
	{ 0x1415deaa0, u8"Nearest distance" }, // 最近距離
	{ 0x1415deab0, u8"Farthest distance" }, // 最遠距離
	{ 0x1415deac0, u8"Impact" }, // 影響度
	{ 0x1415df060, u8"Distance fog" }, // 距離フォグ
	{ 0x1415df188, u8"Minimum height" }, // 最小の高さ
	{ 0x1415df198, u8"Maximum height" }, // 最大の高さ
	{ 0x1415df070, u8"Height fog" }, // 高さフォグ
	{ 0x1413f6ea8, u8"Parameters" }, // パラメータ
	{ 0x1413de144, u8"Center" }, // 中心
	{ 0x1415e0ae8, u8"Blur strength maximum" }, // ブラーの強さ最大
	{ 0x1415e0b08, u8"Blur strength minimum" }, // ブラーの強さ最小
	{ 0x1415e12cc, u8"Quality" }, // 品質
	{ 0x1415e17f8, u8"Make it independent when splitting the screen" }, // 画面分割時に独立させる
	{ 0x1415e1820, u8"Frustrum drawing enabled" }, // フラスタム描画有効
	{ 0x1415e1840, u8"Reprojection" }, // reprojection
	{ 0x1415e1850, u8"Render target width" }, // レンダーターゲット幅
	{ 0x1415e1870, u8"Render target height" }, // レンダーターゲット高さ
	{ 0x1415e1898, u8"Clear value" }, // クリア値
	{ 0x1413e75a0, u8"Disappearance time" }, // 消失時間
	{ 0x1415e18a8, u8"Color influence due to displacement" }, // 変位による色影響度
	{ 0x1415e18c8, u8"ViewMatrix" }, // viewMatrix
	{ 0x1415e18d8, u8"ProjMatrix" }, // projMatrix
	{ 0x1415e1cc0, u8"Plane to project on" }, // 投影する平面
	{ 0x1415e1cd8, u8"Planar projection light position" }, // 平面投影のライト位置
	{ 0x1415e1cf8, u8"Number of lights in planar projection" }, // 平面投影のライト数
	{ 0x1415e1d18, u8"Distance at which the shadow begins to disappear" }, // 影が消え始める距離
	{ 0x1415e1d38, u8"Distance at which the shadow disappears" }, // 影が消える距離
	{ 0x1415e1d50, u8"Bias to crush during projection" }, // 投影時の潰すバイアス
	{ 0x1415e1d70, u8"Box size of drawing area" }, // 描画範囲のボックスサイズ
	{ 0x1415e1d98, u8"Draw range box offset" }, // 描画範囲のボックスオフセット
	{ 0x1413c2b30, u8"Preset" }, // プリセット
	{ 0x1415e4998, u8"TestMode" }, // TestMode
	{ 0x1413b0110, u8"Gaze offset" }, // 注視点オフセット
	{ 0x1415831b0, u8"Viewpoint offset" }, // 視点オフセット
	{ 0x141583220, u8"Collision type" }, // コリジョンタイプ
	{ 0x141583240, u8"Scale offset" }, // スケールオフセット
	{ 0x141583260, u8"Collision connection destination model ID" }, // コリジョン接続先モデルＩＤ
	{ 0x141583288, u8"Collision connection destination model node name" }, // コリジョン接続先モデルノード名
	{ 0x1413adf00, u8"Number of parts" }, // パーツ数
	{ 0x1413ae070, u8"Homing attack possible" }, // ホーミングアタック可能
	{ 0x1413ae60c, u8"Amount" }, // no
	{ 0x1413ae750, u8"Event activation" }, // イベント起動
	{ 0x1413ae8b0, u8"Lighting maintenance time" }, // 点灯維持時間
	{ 0x1413aea14, u8"Phase" }, // 位相
	{ 0x1413aeb48, u8"Stop time" }, // 停止時間
	{ 0x1413aec30, u8"Time stop group ID (-1 means unaffiliated)" }, // 時間停止グループID（-1は未所属）
	{ 0x1413aecf0, u8"Is homing attack possible?" }, // ホーミングアタック可能か
	{ 0x1413aed60, u8"Follow parent object?" }, // 親オブジェクトに追従するか
	{ 0x1413f8348, u8"Action" }, // アクション
	{ 0x1413aa3f0, u8"Target object" }, // 対象オブジェクト
	{ 0x1413aa4f0, u8"Delay time" }, // 遅延時間
	{ 0x1413aee08, u8"Action" }, // action
	{ 0x1413af5e4, u8"Dummy" }, // dummy
	{ 0x1413afae8, u8"Initial velocity" }, // 初速度
	{ 0x1413afc20, u8"Operation unavailable time" }, // 操作不可時間
	{ 0x1413afd50, u8"The speed is constant over this distance" }, // この距離は速度が一定
	{ 0x1413b0390, u8"Island aerial patrol path name" }, // 島上空巡回パス名
	{ 0x1413b0550, u8"Boss nest tour path name" }, // ボスの巣巡回パス名
	{ 0x1413b0658, u8"Approach path information" }, // 接近パス情報
	{ 0x1413b07a0, u8"Climb path name" }, // 登頂パス名
	{ 0x1413b0850, u8"Climb end path name" }, // 登頂終了パス名
	{ 0x1413b08f0, u8"Aura road path name" }, // オーラロードパス名
	{ 0x1413b0a00, u8"Battle path name" }, // 戦闘パス名
	{ 0x1413b0ab8, u8"Boss pass name" }, // ボスパス名
	{ 0x1413b0b80, u8"Player pass name" }, // プレイヤーパス名
	{ 0x1413b0ce0, u8"Map release location upon defeat" }, // 撃破時マップ解放位置
	{ 0x1413d7e40, u8"Boss path name" }, // ボスのパス名
	{ 0x1413d7ff8, u8"Player pathname" }, // プレイヤーのパス名
	{ 0x1413b0de8, u8"Chase pass information" }, // チェイスパス情報
	{ 0x1413b1770, u8"Path name" }, // パス名
	{ 0x1413b18b0, u8"Boss related objects" }, // ボス関連オブジェクト
	{ 0x1413b1970, u8"Ring supply object generation position" }, // リング供給オブジェクト生成位置
	{ 0x1413b1b98, u8"Event trigger flag" }, // イベント起動フラグ
	{ 0x1413b1ca0, u8"Motion test flag [For debugging only]" }, // モーションテストフラグ[デバック時専用]
	{ 0x1413b24d0, u8"Dummy" }, // ダミー
	{ 0x1413b28e8, u8"Lap pass" }, // 周回パス
	{ 0x1413b29d8, u8"Climb waiting position" }, // 登頂 待機位置
	{ 0x141399760, u8"Use" }, // 使用する
	{ 0x1413a4300, u8"Lane" }, // レーン
	{ 0x1413a4370, u8"Position on path" }, // パス上の位置
	{ 0x1413b2ac8, u8"[Not used] Climb Grind Rail Trap" }, // [不使用] 登頂 グラインドレールトラップ
	{ 0x1413b2c08, u8"Battle 2 driving path" }, // 戦闘2 走行パス
	{ 0x1413b2d48, u8"Battle 2 starting position" }, // 戦闘2 開始位置
	{ 0x1413b2de8, u8"Battle 2 Aerial Warp Location" }, // 戦闘2 エリアルワープ位置
	{ 0x1413b2f68, u8"[For debugging] Hit the top with the On signal" }, // [デバッグ用] 登頂 On信号で叩きつけ移行
	{ 0x1413b38c8, u8"Child machine object wave list [small]" }, // 子機オブジェクトWaveリスト[小型]
	{ 0x1413b3a48, u8"Child machine object wave list [large]" }, // 子機オブジェクトWaveリスト[大型]
	{ 0x1413b3d08, u8"Beast ID" }, // BeastのID
	{ 0x1413b4348, u8"Barrier object" }, // バリアのオブジェクト
	{ 0x1413b49f8, u8"Size" }, // 大きさ
	{ 0x1413b4ad0, u8"Friends objects" }, // フレンズのオブジェクト
	{ 0x1413b4bb0, u8"Boss object" }, // ボスのオブジェクト
	{ 0x1413b53f0, u8"NPC type" }, // NPCのタイプ
	{ 0x1413b54c8, u8"Rifle spawn location offset" }, // ライフルの出現場所オフセット
	{ 0x1413b5cc8, u8"Position adjustment per block" }, // ブロック当たりの位置調整
	{ 0x1413b5e70, u8"Adjusting the depth of model display: Towards the back of the camera" }, // モデル表示の深さ調整:カメラ奥方向
	{ 0x1413b6250, u8"Size per block" }, // ブロック当たりサイズ
	{ 0x1413b63f0, u8"Size per attack" }, // 攻撃当たりサイズ
	{ 0x1413b6518, u8"Laser target" }, // レーザー用ターゲット
	{ 0x1413b7408, u8"Child objects" }, // 子オブジェクト群
	{ 0x1413b7500, u8"Keep updating Transform" }, // Transformを更新し続ける
	{ 0x1413f4e88, u8"Camera view" }, // カメラビュー
	{ 0x1413b7b78, u8"Upward offset" }, // 上方向オフセット
	{ 0x1413b7d40, u8"Minimum camera distance" }, // 最小カメラ距離
	{ 0x1413b7e28, u8"Maximum camera distance" }, // 最大カメラ距離
	{ 0x1413b7ef0, u8"Elevation angle [at low speed]" }, // 仰角[低速時]
	{ 0x1413b7fc8, u8"Elevation angle [at high speed]" }, // 仰角[高速時]
	{ 0x1413b80dc, u8"Angle of view" }, // 画角
	{ 0x1413b88cc, u8"Distance" }, // 距離
	{ 0x1413b8a08, u8"Screen upper limit" }, // スクリーン上限
	{ 0x1413b8b50, u8"Screen lower limit" }, // スクリーン下限
	{ 0x1413b8cc8, u8"Limited visibility" }, // 視界制限
	{ 0x1413b8e70, u8"Visibility limit [lower limit]" }, // 視界制限[下限]
	{ 0x1413b92a0, u8"Gaze point Offset" }, // 注視点Offset
	{ 0x1413b9920, u8"Associated camera" }, // 関連付けるカメラ
	{ 0x1413b9988, u8"Priority" }, // 優先度
	{ 0x1413b6928, u8"Base point" }, // 基点
	{ 0x1413aae18, u8"Filter" }, // フィルター
	{ 0x1413cc048, u8"Width" }, // width
	{ 0x1413cc098, u8"Height" }, // height
	{ 0x1413cc100, u8"Depth" }, // depth
	{ 0x1413bdd44, u8"Offset" }, // offset
	{ 0x1413b9ad0, u8"Volume" }, // ボリューム
	{ 0x1413ba040, u8"Camera movement limit 1" }, // カメラ移動制限１
	{ 0x1413ba108, u8"Camera movement restriction 2" }, // カメラ移動制限２
	{ 0x1413bae9c, u8"Roll" }, // Roll
	{ 0x1413baf90, u8"Offset on path" }, // パス上のOffset
	{ 0x1413bb000, u8"Reversal of path direction" }, // パス方向の逆転
	{ 0x1413bb088, u8"Azimuth sensitivity" }, // 方位角の感度
	{ 0x1413bb100, u8"Elevation elevation sensitivity" }, // 仰角上昇の感度
	{ 0x1413bb1a0, u8"Sensitivity of elevation descent" }, // 仰角下降の感度
	{ 0x1413bb2b8, u8"Azimuth offset" }, // 方位角オフセット
	{ 0x1413bb3a8, u8"Elevation offset" }, // 仰角オフセット
	{ 0x1413bb428, u8"Upward offset of the fixation point" }, // 注視点の上方オフセット
	{ 0x1413bc800, u8"Azimuth Offset" }, // 方位角Offset
	{ 0x1413bc908, u8"Elevation angle Offset" }, // 仰角Offset
	{ 0x1413bca28, u8"Offset in gravity direction" }, // 重力方向のOffset
	{ 0x1413bcb90, u8"Interpolation start angle" }, // 補間開始角度
	{ 0x1413bccb8, u8"Interpolation sensitivity" }, // 補間感度
	{ 0x1413bd748, u8"Pitch angle" }, // ピッチ角
	{ 0x1413be628, u8"Rotation around the Z axis" }, // Z軸周りの回転
	{ 0x1413be758, u8"Gaze type" }, // 注視点タイプ
	{ 0x1413be8b8, u8"Gaze coordinates" }, // 注視座標
	{ 0x1413bec48, u8"Gaze object" }, // 注視オブジェクト
	{ 0x1413bed88, u8"Distance to point of interest" }, // 注視点までの距離
	{ 0x1413bef10, u8"Offset from gaze object" }, // 注視オブジェクトからのオフセット
	{ 0x1413bfbb0, u8"Player coordinates Offset" }, // プレイヤー座標Offset
	{ 0x1413b690c, u8"Target" }, // target
	{ 0x1413c0230, u8"Use angle limits" }, // 角度制限を使用する
	{ 0x1413c0338, u8"Azimuth limit" }, // 方位角制限
	{ 0x1413c0410, u8"Elevation angle limit" }, // 仰角制限
	{ 0x1413c0510, u8"World coordinate Offset" }, // ワールド座標Offset
	{ 0x1413c0658, u8"Distance to point of interest when zooming" }, // ズーム時の注視点までの距離
	{ 0x1413c0720, u8"Set the distance to the point of interest" }, // 注視点までの距離を設定する
	{ 0x1413c1690, u8"Camera position" }, // カメラ位置
	{ 0x1413c3568, u8"Upward direction of camera" }, // カメラの上方向
	{ 0x1413c3820, u8"Viewpoint path name" }, // 視点パス名
	{ 0x1413c39b0, u8"Offset on the viewpoint path" }, // 視点パス上のOffset
	{ 0x1413c3b68, u8"Point of interest path name" }, // 注視点パス名
	{ 0x1413c3d10, u8"Offset on the gaze point path" }, // 注視点パス上のOffset
	{ 0x1413c3e80, u8"Type" }, // タイプ
	{ 0x1413c3ff0, u8"Play time" }, // 再生時間
	{ 0x1413c4188, u8"Reverse direction on path" }, // パス上の進行方向を逆転
	{ 0x1413c4310, u8"Gaze correction for players" }, // プレイヤーに対する注視点補正
	{ 0x1413c5670, u8"Distance while boosting" }, // ブースト中の距離
	{ 0x1413c5838, u8"Horizontal tracking rate" }, // 水平追従率
	{ 0x1413c6358, u8"Use the vertical component of the path direction" }, // パス方向の垂直成分を使用する
	{ 0x1413c6478, u8"Camera Y-Up tilt" }, // カメラY-Upの傾き
	{ 0x1413c6568, u8"Smoothing in path direction" }, // パス方向のスムージング
	{ 0x1413c66b0, u8"View direction sensitivity" }, // 視点方向の感度
	{ 0x1413c6808, u8"View direction sensitivity (during boost)" }, // 視点方向の感度（ブースト中）
	{ 0x1413c6d78, u8"Type of player coordinate Offset" }, // プレイヤー座標Offsetのタイプ
	{ 0x1413c9178, u8"Left/right movement lower limit" }, // 左右移動下限
	{ 0x1413c92e0, u8"Left/right movement upper limit" }, // 左右移動上限
	{ 0x1413c9458, u8"Up scroll start position" }, // 上スクロール開始位置
	{ 0x1413c9600, u8"Upper scroll limit" }, // 上スクロール限界
	{ 0x1413c9790, u8"Down scroll start position" }, // 下スクロール開始位置
	{ 0x1413c98e8, u8"Down scroll limit" }, // 下スクロール限界
	{ 0x1413c9a18, u8"Path movement range limit" }, // パス移動範囲制限
	{ 0x1413c9ad8, u8"Path movement range lower limit" }, // パス移動範囲下限
	{ 0x1413c9b48, u8"Pass movement range upper limit" }, // パス移動範囲上限
	{ 0x1413c9bd8, u8"Posture standards" }, // 姿勢の基準
	{ 0x1413cac30, u8"Vibration time" }, // 振動時間
	{ 0x1413cad28, u8"Vibration distance" }, // 振動距離
	{ 0x1413cae48, u8"Vibration only happens once" }, // 振動は一度きり
	{ 0x1413caf58, u8"Vibration type" }, // 振動タイプ
	{ 0x1413cb040, u8"Amplitude size" }, // 振幅の大きさ
	{ 0x1413cb178, u8"Frequency. How many times it vibrates per second" }, // 周波数.1秒間に何回振動するか
	{ 0x1413cb2b0, u8"Z rotation.Depending on line of sight direction and upward direction.Top is 0 degree" }, // Z回転.視線方向と上方向による.上が0度
	{ 0x1413cb3a0, u8"Amplitude attenuation rate" }, // 振幅の減衰率
	{ 0x1413cb520, u8"Do distance attenuation?" }, // 距離減衰を行うか
	{ 0x1413cb6f0, u8"Width/diameter" }, // 幅/直径
	{ 0x1413cb7dc, u8"Height" }, // 高さ
	{ 0x1413cc688, u8"[When the player is in the air] Distance" }, // [プレイヤーが空中にいる時]距離
	{ 0x1413cc790, u8"[When the player is in the air] Elevation angle" }, // [プレイヤーが空中にいる時]仰角
	{ 0x1413cca98, u8"Path offset" }, // パスオフセット
	{ 0x1413ccb70, u8"Target coordinate offset Y" }, // ターゲット座標オフセットY
	{ 0x14139aed0, u8"Azimuth tracking sensitivity" }, // 方位角の追従感度
	{ 0x14139c380, u8"Elevation angle tracking sensitivity" }, // 仰角の上昇追従感度
	{ 0x14139c4e0, u8"Elevation angle downward tracking sensitivity" }, // 仰角の下降追従感度
	{ 0x1413cd120, u8"Common" }, // 共通
	{ 0x14139f3c0, u8"Minimum distance" }, // 最小距離
	{ 0x14139f4e0, u8"Maximum distance" }, // 最大距離
	{ 0x1413cd228, u8"Dash" }, // ダッシュ
	{ 0x1413cd330, u8"Lock-on" }, // ロックオン
	{ 0x1413cd430, u8"Disable transition to combat state" }, // 戦闘中ステートへの遷移無効化
	{ 0x1413cda10, u8"Parent volume" }, // 親ボリューム
	{ 0x1413ce0a8, u8"Use azimuth constraints" }, // 方位角制限を使用する
	{ 0x1413ce438, u8"Camera position offset" }, // カメラ位置オフセット
	{ 0x1413cfcb8, u8"Prioritize action cameras" }, // アクションカメラより優先する
	{ 0x1413cfdd8, u8"Start transition time" }, // 開始移行時間
	{ 0x1413cfed8, u8"End transition time" }, // 終了移行時間
	{ 0x1413d0070, u8"Starting migration type" }, // 開始移行タイプ
	{ 0x1413d01a0, u8"End migration type" }, // 終了移行タイプ
	{ 0x1413d0260, u8"Initial state" }, // 初期状態
	{ 0x1413d0338, u8"Motion" }, // 動作
	{ 0x1413d03e0, u8"Disable free camera mode" }, // フリーカメラモードを無効化
	{ 0x1413d1a18, u8"Default camera" }, // デフォルトカメラ
	{ 0x1413d1e40, u8"Far" }, // far
	{ 0x1413d3168, u8"Model" }, // モデル
	{ 0x1413d3210, u8"Model position offset (production model only)" }, // モデル位置オフセット（本番モデルのみ）
	{ 0x1413d3318, u8"Bottom center position locator on the board (answer board)" }, // 盤面下中心位置ロケーター（回答用盤面）
	{ 0x1413d33e8, u8"Correct answer order for parts" }, // パーツの正しい回答順
	{ 0x1413f6e0c, u8"Number of lines" }, // 行数
	{ 0x1413f6f44, u8"Number of columns" }, // 列数
	{ 0x1413f6d3c, u8"Number of layers" }, // 層数
	{ 0x1413d34b8, u8"Puzzle size (used for collision determination)" }, // パズルのサイズ（コリジョン判定に使用）
	{ 0x1413d3558, u8"Total number of blocks available" }, // 使用できるブロックの総数
	{ 0x1413d3620, u8"Number of vertical squares (number of layers) of the correct shape" }, // 正解形状の縦のマス数（層数）
	{ 0x1413d3770, u8"Warp hole height from block position (in squares)" }, // ブロック位置からのワープホール高度（マス単位）
	{ 0x1413d38a0, u8"Copy block position from the top of the board (in square units)" }, // 盤の上端からのコピーブロック位置（マス単位）
	{ 0x1413d39c8, u8"Number of squares to float" }, // 浮遊させるマス数
	{ 0x1413d3af0, u8"Effect scale during reverse playback (for answer board)" }, // 逆再生時のエフェクトスケール（回答盤面用）
	{ 0x1413d3c20, u8"Effect playback speed when playing in reverse (for answer board)" }, // 逆再生時のエフェクト再生速度（回答盤面用）
	{ 0x1413f696c, u8"Column" }, // 列
	{ 0x1413f63cc, u8"Line" }, // 行
	{ 0x1413f5f9c, u8"Layer" }, // 層
	{ 0x1413d3d70, u8"Correct shape (when using debug drawing)" }, // 正解の形状（デバグ描画使用時）
	{ 0x1413d3e68, u8"3DPuzzleBlock" }, // 3DPuzzleBlock
	{ 0x1413d5478, u8"Shape number" }, // 形状番号
	{ 0x1413d5550, u8"Camera activator for rising blocks" }, // 上昇ブロック用カメラアクティベーター
	{ 0x1413d5658, u8"Camera activator for lowering block" }, // 下降ブロック用カメラアクティベーター
	{ 0x1413d5730, u8"Distance of rise of block to switch camera" }, // カメラを切り替えるブロックの上昇距離
	{ 0x1413d5848, u8"Warp effect scale" }, // ワープエフェクトスケール
	{ 0x1413d5990, u8"Number of squares from the origin to the top of the block after applying rotation (in 0.5 increments)" }, // 回転適用後のブロックの原点から上端へのマス数（0.5刻み）
	{ 0x1413f8760, u8"Line number" }, // 行番号
	{ 0x1413f8858, u8"Column index" }, // 列番号
	{ 0x1413d5b20, u8"Warp location" }, // ワープ場所
	{ 0x1413d5c40, u8"Color" }, // 色
	{ 0x1413d5df0, u8"Preset shapes" }, // プリセット形状
	{ 0x1413d5ec0, u8"Use presets" }, // プリセットを使用する
	{ 0x1413f8400, u8"Rotation about Y axis" }, // Y軸に対する回転
	{ 0x1413d6020, u8"Rotation settings (only presets supported, reflected in falling shape)" }, // 回転設定（プリセットのみ対応、落下形状に反映）
	{ 0x1413d68e0, u8"Camera during clear performance" }, // クリア演出中カメラ
	{ 0x1413d6998, u8"Camera interpolation start time" }, // カメラ補間開始時間
	{ 0x1413d6a60, u8"Camera interpolation end time" }, // カメラ補間終了時間
	{ 0x1413d6b30, u8"Clear performance start time" }, // クリア演出開始時間
	{ 0x1413d6c00, u8"Camera switching time" }, // カメラ切り替え時間
	{ 0x1413d6da0, u8"Serial number" }, // 通し番号
	{ 0x1413d6e80, u8"Display number" }, // 表示番号
	{ 0x1413d8ca0, u8"Move" }, // 移動
	{ 0x1413d8dfc, u8"Patrol" }, // 巡回
	{ 0x1413d8f68, u8"Update time" }, // 更新時間
	{ 0x1413d9160, u8"Will it fall if I ride it?" }, // 乗ったら落下するか
	{ 0x1413d92a8, u8"Match path and Y rotation" }, // パスとY回転を合わせる
	{ 0x1413d9450, u8"Acceleration/deceleration of movement" }, // 動きの加減速
	{ 0x1413d9600, u8"Follow the reverse path" }, // パスの逆を辿る
	{ 0x1413d9748, u8"Will you be crushed to death?" }, // 圧死するか
	{ 0x1413d9920, u8"[Move between two points] Move destination" }, // [二点間移動]移動先
	{ 0x1413d9a68, u8"[Path movement] path name" }, // [パス移動]パス名
	{ 0x1413d9b68, u8"[Path movement] Stop point list" }, // [パス移動]停止点リスト
	{ 0x1413d9ca0, u8"Waiting time after moving" }, // 移動後待機時間
	{ 0x1413d9ee8, u8"Moving Speed" }, // 移動速度
	{ 0x1413d9fd0, u8"Play SE while on the move" }, // 移動中にSEを再生する
	{ 0x1413da098, u8"Compatible with time stop" }, // 時間停止対応
	{ 0x1413da300, u8"Floor type" }, // 床の種類
	{ 0x1413db7a8, u8"Start panel" }, // スタートパネル
	{ 0x141396850, u8"Does not turn off after startup" }, // 起動後消灯しない
	{ 0x1413db8a0, u8"Start panel settings" }, // スタートパネル設定
	{ 0x1413dba28, u8"Start moving" }, // 移動開始
	{ 0x1413dcbe8, u8"One stroke panel" }, // 一筆書きパネル
	{ 0x1413dccb0, u8"First start panel" }, // 最初のスタートパネル
	{ 0x1413dcdb8, u8"There is a time limit" }, // 制限時間あり
	{ 0x1413dcec0, u8"Time limit" }, // 制限時間
	{ 0x1413dcf70, u8"Camera Activator" }, // CameraActivator
	{ 0x1413dcfa8, u8"Wait until count starts" }, // カウント開始までのウエイト
	{ 0x1413dd110, u8"Player distance range for start judgment" }, // スタート判定のプレイヤー距離範囲
	{ 0x1413de0f8, u8"Wall type" }, // 壁の種類
	{ 0x1413de780, u8"Use Convex collision" }, // Convexコリジョンを使う
	{ 0x1413de880, u8"Cast a shadow" }, // 影を投射する
	{ 0x1413df680, u8"Sensing distance" }, // 感知距離
	{ 0x1413df7c0, u8"Gravity" }, // 重力
	{ 0x1413df8e8, u8"Turning speed (deg/s)" }, // 旋回速度(deg/s)
	{ 0x1413dfb60, u8"Travel time" }, // 移動時間
	{ 0x1413dfce8, u8"Waiting time before rush" }, // 突進前待ち時間
	{ 0x1413dfe38, u8"Waiting time after rush" }, // 突進後待ち時間
	{ 0x1413dff60, u8"Blow away power" }, // 吹っ飛びパワー
	{ 0x1413e0078, u8"Blown away float" }, // 吹っ飛び浮き
	{ 0x1413e0ed0, u8"Operation type" }, // 動作タイプ
	{ 0x1413e10f0, u8"Maximum speed" }, // 最高速度
	{ 0x1413e1218, u8"Minimum speed" }, // 最低速度
	{ 0x1413e1318, u8"Maximum speed/Boost" }, // 最高速度/Boost
	{ 0x1413e1430, u8"Minimum speed/Boost" }, // 最低速度/Boost
	{ 0x1413e15f8, u8"Maximum speed/BoostLvMax" }, // 最高速度/BoostLvMax
	{ 0x1413e1780, u8"Minimum speed/BoostLvMax" }, // 最低速度/BoostLvMax
	{ 0x1413e19dc, u8"Initial velocity" }, // 初速
	{ 0x1413e1b80, u8"Operation prohibited time" }, // 操作禁止時間
	{ 0x1413e1cd8, u8"Movable width" }, // 移動可能横幅
	{ 0x1413e1e80, u8"Time limit (0 means no limit)" }, // 制限時間（０で制限なし）
	{ 0x1413e2098, u8"Jump mode" }, // ジャンプモード
	{ 0x1413e2218, u8"Options/Correction for falling trajectory" }, // オプション/落下軌道に補正
	{ 0x1413e2348, u8"Optional/End in non-grounded state" }, // オプション/非接地状態で終了
	{ 0x1413e2478, u8"Collision" }, // コリジョン
	{ 0x1413e4128, u8"Movement mode" }, // 移動モード
	{ 0x1413e4ac8, u8"Minimum injection speed" }, // 射出最低速度
	{ 0x1413e4d80, u8"Are you running in the opposite direction?" }, // 逆走か
	{ 0x1413e4eb8, u8"Plane" }, // プレーン
	{ 0x1413e5f40, u8"Number of bounces" }, // 跳ね返す回数
	{ 0x1413e6068, u8"Batter box" }, // バッターボックス
	{ 0x1413e6110, u8"Pitching machine" }, // ピッチングマシン
	{ 0x14139d088, u8"Use camera parameters" }, // カメラパラメータ使用する
	{ 0x1413afde8, u8"Interpolation time at camera startup" }, // カメラ起動時補間時間
	{ 0x1413aff40, u8"Interpolation time at camera end" }, // カメラ終了時補間時間
	{ 0x1413b01f8, u8"Camera offset" }, // カメラオフセット
	{ 0x1413e6228, u8"Camera parameters" }, // カメラパラメータ
	{ 0x1413e6870, u8"Cool down time" }, // クールダウン時間
	{ 0x1413e6978, u8"Cooldown random width" }, // クールダウンランダム幅
	{ 0x1413e6aa0, u8"Rate of fire" }, // 発射速度
	{ 0x1413e7020, u8"FV?" }, // FVか
	{ 0x1413b9158, u8"Movement type" }, // 移動タイプ
	{ 0x141397ce8, u8"[Move between two points] Destination offset" }, // [二点間移動]移動先オフセット
	{ 0x1413d9330, u8"Do you want to attack?" }, // 攻撃するか
	{ 0x141396f90, u8"Bullet velocity" }, // 弾速度
	{ 0x1413b9b50, u8"Storage time" }, // 溜め時間
	{ 0x1413971a8, u8"Number of consecutive shots" }, // 連射回数
	{ 0x141397280, u8"Rate of fire" }, // 連射速度
	{ 0x1413f1530, u8"Fire interval" }, // 連射間隔
	{ 0x1413fce10, u8"Search distance" }, // 索敵距離
	{ 0x1413b99f8, u8"Enemy search angle of view" }, // 索敵画角
	{ 0x1413e89c0, u8"Bullet survival time" }, // 弾の生存時間
	{ 0x1413e71b8, u8"Attack" }, // 攻撃
	{ 0x1413e7b18, u8"Length of each side (cuboid)" }, // 各辺の長さ（直方体）
	{ 0x1413e7c48, u8"Radius (sphere, capsule)" }, // 半径（球、カプセル）
	{ 0x1413e7d80, u8"Height (capsule)" }, // 高さ（カプセル）
	{ 0x1413e7ee0, u8"Collide with camera" }, // カメラと衝突する
	{ 0x1413e88f8, u8"Non-support side" }, // 非サポート面
	{ 0x1413e909c, u8"Kinds" }, // 種類
	{ 0x1413e9180, u8"Time until explosion" }, // 爆発までの時間
	{ 0x1413e9280, u8"Elapsed time to speed up anime" }, // アニメを早くする経過時間
	{ 0x1413e9420, u8"Blinking animation pause time" }, // 明滅アニメ休止時間
	{ 0x1413e9518, u8"Explosion damage determination duration" }, // 爆発ダメージ判定継続時間
	{ 0x1413e9650, u8"Detection radius" }, // 検知半径
	{ 0x1413e96c8, u8"Explosion radius" }, // 爆発半径
	{ 0x1413e9790, u8"1st stage blinking speed" }, // 1段階目明滅速度
	{ 0x1413e98d0, u8"2nd stage blinking speed" }, // 2段階目明滅速度
	{ 0x1413e9a20, u8"Timed explosion damage speed multiplier" }, // 時限爆発ダメージ時速度倍率
	{ 0x1413e9bc8, u8"Amount of rise when stumbling" }, // 躓き時上昇量
	{ 0x1413e9d58, u8"Amount of rise when blowing" }, // 吹き飛ばし時上昇量
	{ 0x1413e9f00, u8"Blow Sonic Speed Threshold" }, // 吹き飛ばすソニック速度閾値
	{ 0x1413eaa58, u8"Presence of timer" }, // タイマーの有無
	{ 0x1413eab90, u8"Detection range" }, // 検知範囲
	{ 0x1413ead28, u8"Time to make blinking faster" }, // 明滅を速くするまでの時間
	{ 0x1413eadf8, u8"Time stop period" }, // 時間停止期間
	{ 0x1413eae90, u8"Projection angle" }, // 投射角度
	{ 0x1413eaf88, u8"Projection speed" }, // 投射速さ
	{ 0x1413eb0f0, u8"Gravitational acceleration" }, // 重力加速度
	{ 0x1413eb2a8, u8"Offset for CyAction" }, // CyAction用のOffset
	{ 0x1413eb368, u8"Do you want to respawn?" }, // リスポーンするか
	{ 0x1413eb408, u8"Time until reappearance" }, // 再び現れるまでの時間
	{ 0x1413ec9e0, u8"Flag used" }, // 使用フラグ
	{ 0x1413eca98, u8"Child machine object ID list" }, // 子機オブジェクトIDリスト
	{ 0x1413ecb70, u8"WAVE name" }, // WAVE名
	{ 0x1413ecc18, u8"Follow the direction of the target" }, // 目標のいる方向に追従
	{ 0x1413ecf78, u8"Height adjustment" }, // 高さ調整
	{ 0x1413ed280, u8"Pattern [-1 and below are treated as unset]" }, // パターン[-1以下は未設定扱い]
	{ 0x1413ed418, u8"Fix the initial posture outward from the center" }, // 初期姿勢を中心から外向きに固定
	{ 0x1413ed818, u8"Survival time" }, // 生存時間
	{ 0x1413edc58, u8"Starts in a broken state" }, // 壊れ状態で開始
	{ 0x1413edd78, u8"Time until collapse" }, // 崩れるまでの時間
	{ 0x1413edee8, u8"Cyloop extension time" }, // サイループ延長時間
	{ 0x1413ee4f8, u8"Homing judgment" }, // ホーミング判定
	{ 0x1413ee620, u8"Number of attacks before breaking" }, // 壊れるまでの攻撃回数
	{ 0x1413efbf8, u8"Switching type" }, // 切り替えタイプ
	{ 0x1413efd88, u8"Start delay time" }, // 開始遅延時間
	{ 0x1413efea0, u8"Does it turn off after a certain amount of time?" }, // 一定時間でオフになるか
	{ 0x1413eff68, u8"Time to turn off" }, // オフになるまでの時間
	{ 0x1413f0028, u8"Make inoperable" }, // 操作不可能にする
	{ 0x1413f0148, u8"Inoperable type" }, // 操作不可能タイプ
	{ 0x1413f02b0, u8"Pause the game" }, // ゲームをポーズする
	{ 0x14139fcc8, u8"Use intermediate camera when unlocking" }, // 解除時に中間カメラを使用
	{ 0x14139fda8, u8"Intermediate camera gaze point" }, // 中間カメラの注視点
	{ 0x14139fe58, u8"Intermediate camera overhead passing height" }, // 中間カメラの頭上通過高さ
	{ 0x14139ff18, u8"Interpolation time percentage to intermediate camera" }, // 中間カメラへの補間時間割合
	{ 0x1413f0388, u8"Settings during interpolation" }, // 補間時の設定
	{ 0x14139ede0, u8"Fade out time at startup" }, // 起動時のフェードアウト時間
	{ 0x14139ef08, u8"Fade stop time at startup" }, // 起動時のフェード停止時間
	{ 0x14139efd8, u8"Fade-in time at startup" }, // 起動時のフェードイン時間
	{ 0x14139f140, u8"Fade out time when canceling" }, // 解除時のフェードアウト時間
	{ 0x14139f248, u8"Fade stop time when canceling" }, // 解除時のフェード停止時間
	{ 0x14139f348, u8"Fade-in time when canceling" }, // 解除時のフェードイン時間
	{ 0x14139f3e8, u8"Does the camera movement stop during a fade?" }, // フェード時カメラ移動は停止か
	{ 0x1413b0a90, u8"Playback delay time" }, // 再生遅延時間
	{ 0x1413f0488, u8"Settings for fade" }, // フェード時の設定
	{ 0x1413f057c, u8"Purpose" }, // 用途
	{ 0x1413f06a0, u8"Quest type" }, // クエストタイプ
	{ 0x1413f1298, u8"Shooting type" }, // 射撃タイプ
	{ 0x1413f1368, u8"Shooting direction" }, // 射撃方向
	{ 0x1413f1470, u8"Number of consecutive shots" }, // 連射数
	{ 0x1413f15a8, u8"Firing interval" }, // 発射間隔
	{ 0x1413f1670, u8"Bullet speed" }, // 弾の速度
	{ 0x1413f16f0, u8"Velocity of the bullet that bounces back" }, // 弾き返した弾の速度
	{ 0x1413f17f0, u8"Enemy search range" }, // 索敵範囲
	{ 0x1413f1830, u8"Bullet lifespan" }, // 弾の生存期間
	{ 0x1413f18e0, u8"Bullet survival range" }, // 弾の生存範囲
	{ 0x1413f1960, u8"Homing possible" }, // ホーミング可能
	{ 0x1413f2c88, u8"Does the navigation disappear when the input is established?" }, // 入力成立でナビを消すか
	{ 0x1413f2d10, u8"Warning type" }, // 警告タイプ
	{ 0x1413f2d78, u8"Navigation type" }, // ナビタイプ
	{ 0x1413f2e48, u8"Is the switch activated?" }, // スイッチ起動か
	{ 0x1413f3398, u8"Chaos emerald type" }, // カオスエメラルドタイプ
	{ 0x1413f34b0, u8"Attenuation coefficient of horizontal component with contact surface" }, // 接触面との水平成分の減衰係数
	{ 0x1413f3548, u8"Reflection coefficient of normal component with contact surface" }, // 接触面との垂直成分の反射係数
	{ 0x1413f35e8, u8"Settings on Gedit" }, // Gedit上設定
	{ 0x1413f3dc0, u8"Save data ID (match with ChaosEmeraldStorage)" }, // セーブデータID(ChaosEmeraldStorageと一致させる)
	{ 0x1413f3f48, u8"Effect override" }, // エフェクト上書き
	{ 0x1413f4660, u8"Used for sequence progression" }, // シーケンス進行に使用
	{ 0x1413f4758, u8"Enable guide circles" }, // ガイドサークルを有効にするか
	{ 0x1413f4830, u8"Guide circle position offset" }, // ガイドサークル位置オフセット
	{ 0x1413f4900, u8"Color [map icon]" }, // 色[マップアイコン]
	{ 0x1413f5058, u8"Save data ID (match with ChaosEmeraldPillar)" }, // セーブデータID(ChaosEmeraldPillarと一致させる)
	{ 0x1413f51f0, u8"Release order" }, // 解放順
	{ 0x1413f55f8, u8"Is it the end point?" }, // 終点かどうか
	{ 0x1413f5b28, u8"Appearance" }, // 見た目
	{ 0x1413f5be0, u8"Emmy launch gimmick" }, // エミー起動ギミック
	{ 0x1413f5c88, u8"Knuckles activation gimmick" }, // ナックルズ起動ギミック
	{ 0x1413f5d40, u8"Tails startup gimmick" }, // テイルス起動ギミック
	{ 0x1413f5dc0, u8"Direction of hole appearance" }, // 穴出現の穴の向き
	{ 0x1413f65a8, u8"Cyloop trace collision" }, // サイループ・トレースコリジョン
	{ 0x1413f66f0, u8"Sample point" }, // サンプルポイント
	{ 0x1413f71c8, u8"Radius (cylinder)" }, // 半径（シリンダー）
	{ 0x1413f72b8, u8"Height (cylinder)" }, // 高さ（シリンダー）
	{ 0x1413f78a0, u8"Occurrence conditions" }, // 発生条件
	{ 0x1413be1d0, u8"Object" }, // オブジェクト
	{ 0x1413afb70, u8"Damage value" }, // ダメージ値
	{ 0x1413f79f8, u8"Send target" }, // 送信対象
	{ 0x1413f8110, u8"Speed" }, // 速度
	{ 0x1413f82a8, u8"Model availability" }, // モデルの有無
	{ 0x1413a0ae8, u8"Correction speed" }, // 補正速度
	{ 0x1413a0c18, u8"Correction time" }, // 補正時間
	{ 0x1413f83f0, u8"Path correction" }, // パス補正
	{ 0x1413f84d0, u8"Cyloop activated type?" }, // サイループ起動型か
	{ 0x1413f8578, u8"Cyloop launch billboard display distance" }, // サイループ起動ビルボード表示距離
	{ 0x1413f8640, u8"Cyloop launch billboard display/hide distance difference" }, // サイループ起動ビルボード表示非表示距離差
	{ 0x1413f8700, u8"Cyknuckle activated type?" }, // サイナックル起動型か
	{ 0x1413f87d0, u8"Change behavior to boarding" }, // ボーディングへの変更挙動
	{ 0x1413f9fd0, u8"Speed maintenance time" }, // 速度維持時間
	{ 0x1413fa220, u8"Fixed starting position" }, // 開始位置固定
	{ 0x1413fa2f0, u8"Fixed initial speed" }, // 初期速度固定
	{ 0x1413fa3a8, u8"Injection in a spin state" }, // スピン状態で射出
	{ 0x1413fa430, u8"Is it possible to do an air trick?" }, // エアトリック可能か
	{ 0x14139e690, u8"Stop point list" }, // 停止点リスト
	{ 0x14139e748, u8"2-point movement offset" }, // 2点間移動オフセット
	{ 0x14139eac0, u8"Waiting time at the end of one-way (disappearance)" }, // 片道(消滅)の終点での待機時間
	{ 0x1413fa598, u8"Collision scale" }, // コリジョンスケール
	{ 0x1413faf18, u8"Text" }, // テキスト
	{ 0x1413faf90, u8"Letter color" }, // 文字色
	{ 0x1413fb570, u8"Enable side view" }, // サイドビュー有効化
	{ 0x1413fbf60, u8"Movement restrictions" }, // 移動制限
	{ 0x1413fc200, u8"Inoperable time during transition" }, // 遷移時操作不能時間
	{ 0x1413fc340, u8"Re-entry possible time" }, // 再突入可能時間
	{ 0x1413fc458, u8"Movement speed reset at transition" }, // 遷移時移動速度リセット
	{ 0x1413fc680, u8"Enable homing attack" }, // ホーミングアタック有効化
	{ 0x1413fcd30, u8"Should I approach and run away?" }, // 近づいて逃げるか
	{ 0x1413fcea0, u8"The time from when it takes off until it disappears" }, // 飛び立ってから消えるまでの時間
	{ 0x1413fcf98, u8"Flying speed" }, // 飛ぶ速さ
	{ 0x1413fd068, u8"Offset of flying power" }, // 飛ぶ力のオフセット
	{ 0x1413fd700, u8"Item type" }, // アイテムタイプ
	{ 0x1413fd810, u8"Number of items on display" }, // 表示上のアイテム数
	{ 0x1413fd910, u8"Number of items that can be obtained with one displayed item (rings, skill pieces only)" }, // 表示上のアイテム1つで取得できる数(リング、スキルピースのみ)
	{ 0x1413fda48, u8"Cyloop activation" }, // サイループ起動
	{ 0x1413bc7a0, u8"Time until ring collection" }, // リング回収までの時間
	{ 0x14139f770, u8"Spout angle" }, // 噴出角度
	{ 0x14139f870, u8"Maximum jetting speed" }, // 噴出最高速度
	{ 0x14139f968, u8"Minimum jetting speed" }, // 噴出最低速度
	{ 0x1413bcbd8, u8"Width of spawn point (minimum)" }, // 出現地点の幅(最小)
	{ 0x1413bcd08, u8"Width of spawn point (maximum)" }, // 出現地点の幅(最大)
	{ 0x1413bcdb0, u8"Time to disappear (default with negative number)" }, // 消えるまでの時間(負数でデフォルト)
	{ 0x1413fdc00, u8"RingParam" }, // ringParam
	{ 0x1413fe040, u8"Affected by the wind" }, // 風の影響を受ける
	{ 0x1413fe0d0, u8"Release interval" }, // 放出間隔
	{ 0x1413fe418, u8"Number" }, // 番号
	{ 0x1413fe800, u8"Speed m/s" }, // 速度 m/s
	{ 0x1413fe880, u8"Time required for capture" }, // 捕獲に必要な時間
	{ 0x1413fe8e8, u8"Capture distance" }, // 捕獲できる距離
	{ 0x1413fe988, u8"Distance at which the marker is displayed" }, // マーカーが表示される距離
	{ 0x1413fea78, u8"Capture progress reduction rate (per second)" }, // 捕獲進捗減少率(毎秒)
	{ 0x1413feff0, u8"Event name" }, // イベント名
	{ 0x1413928a0, u8"Page name [priority to number]" }, // ページ名[番号より優先]
	{ 0x141392920, u8"Page number [unused at -1]" }, // ページ番号[-1で不使用]
	{ 0x1413929e8, u8"Use default path" }, // デフォルトパスを使う
	{ 0x141392af8, u8"Show Enemies" }, // エネミーを表示
	{ 0x141392b88, u8"Play when player is idle" }, // プレイヤーがアイドル状態時再生
	{ 0x141392c38, u8"Can't be skipped" }, // スキップ不可
	{ 0x141392ce0, u8"Player time stop" }, // プレイヤーの時間停止
	{ 0x1413a3e20, u8"Start camera interpolation time" }, // 開始カメラ補間時間
	{ 0x1413a3f10, u8"End camera interpolation time" }, // 終了カメラ補間時間
	{ 0x1413a3ff0, u8"Use exit camera" }, // 終了カメラを使用する
	{ 0x1413a4100, u8"Gaze point position specification [priority]" }, // 注視点位置指定[優先]
	{ 0x1413a41e8, u8"Gaze point coordinates" }, // 注視点座標
	{ 0x141392ef8, u8"Specify player posture at end" }, // 終了時プレイヤー姿勢指定
	{ 0x141392f70, u8"FPS30 fixed" }, // FPS30固定
	{ 0x141393668, u8"Puzzle ID" }, // 謎解きID
	{ 0x141394468, u8"Trigger target" }, // トリガーターゲット
	{ 0x141394668, u8"The conditions are" }, // 条件が
	{ 0x1413936cc, u8"Orders" }, // orders
	{ 0x141393718, u8"Script file name upon success" }, // 成功時のスクリプトファイル名
	{ 0x1413937d8, u8"Script file name in case of failure" }, // 失敗時のスクリプトファイル名
	{ 0x1413938a0, u8"Action when correct answer" }, // 正解時のアクション
	{ 0x141393988, u8"Actions for incorrect answers" }, // 不正解時のアクション
	{ 0x141394560, u8"Trigger condition" }, // トリガー条件
	{ 0x141394708, u8"Number of reactions" }, // 反応回数
	{ 0x1413947b0, u8"Execution script file name" }, // 実行スクリプトファイル名
	{ 0x141394f98, u8"Event file name at the time of collision" }, // 衝突時のイベントファイル名
	{ 0x141395060, u8"Event file name when leaving" }, // 離脱時のイベントファイル名
	{ 0x141395b98, u8"Column height (before stretching)" }, // 柱の高さ(伸びる前)
	{ 0x141395c78, u8"Column height (after stretching)" }, // 柱の高さ(伸びた後)
	{ 0x141395d90, u8"Specify the time in the fully extended state" }, // 伸びきった状態の時間を指定する
	{ 0x141395e38, u8"Time in a fully extended state" }, // 伸びきった状態の時間
	{ 0x141395fb0, u8"Should I perform a process to stop the character's movement for the update?" }, // アップデート用のキャラクターの動きを止める処理を行うか
	{ 0x141396890, u8"Type of behavior" }, // 挙動の種類
	{ 0x141396950, u8"Distance traveled: Do not judge if negative" }, // 移動距離 負だと判定しない
	{ 0x141396ea0, u8"Return position after falling to death from within the area" }, // 領域内からの落下死亡の復帰位置
	{ 0x141397700, u8"Installation location" }, // 設置場所
	{ 0x141397780, u8"Length where force is applied" }, // 力が加わる長さ
	{ 0x141397930, u8"Time out" }, // 出ている時間
	{ 0x141397a58, u8"Disappearing time" }, // 消えている時間
	{ 0x141397b80, u8"Phase (for synchronization)" }, // 位相(同期用)
	{ 0x141398268, u8"Managed object" }, // 管理オブジェクト
	{ 0x141398320, u8"Respawn in a meteor shower" }, // 流星雨でリスポーンする
	{ 0x141398af0, u8"Camera position type" }, // カメラ位置タイプ
	{ 0x141398d50, u8"Camera position locator" }, // カメラ位置ロケーター
	{ 0x141398e68, u8"Fireworks camera parameters" }, // 花火カメラパラメータ
	{ 0x141398f60, u8"Number of drop rings" }, // ドロップリング数
	{ 0x1413990e8, u8"Drop ring height" }, // ドロップリング高さ
	{ 0x141399210, u8"Drop ring parameters" }, // ドロップリングパラメータ
	{ 0x141399fd8, u8"Fall start altitude (Y coordinate offset)" }, // 落下開始高度(Y座標オフセット)
	{ 0x14139a5d8, u8"Fishing spot ID" }, // 釣り場ID
	{ 0x14139a6b8, u8"Fast travel locator" }, // ファストトラベル用ロケーター
	{ 0x14139b028, u8"Occurrence time" }, // 発生時間
	{ 0x14139bd88, u8"Driving time" }, // 駆動時間
	{ 0x14139be60, u8"Lateral movement limit value" }, // 横移動限界値
	{ 0x14139bf20, u8"Vertical movement limit value" }, // 縦移動限界値
	{ 0x14139cbe0, u8"Timer time" }, // タイマー時間
	{ 0x14139ccd0, u8"Working weight" }, // 動作する重量
	{ 0x14139ce18, u8"Timer display" }, // タイマー表示
	{ 0x14139d7d8, u8"Save data ID" }, // セーブデータID
	{ 0x14139d9a8, u8"Characters to obtain" }, // 入手対象キャラ
	{ 0x14139dc18, u8"Operation from script" }, // スクリプトから操作
	{ 0x14139e288, u8"Valid only for release version" }, // リリース版のみ有効
	{ 0x14139e678, u8"Using external camera" }, // 外部カメラ使用
	{ 0x14139ec40, u8"Time required to rise (s)" }, // 上昇所要時間(s)
	{ 0x14139ed40, u8"Descending time (s)" }, // 下降所要時間(s)
	{ 0x1413a5810, u8"Offset standard" }, // オフセット基準
	{ 0x1413a5a88, u8"Target position offset" }, // ターゲット位置オフセット
	{ 0x1413a5bc0, u8"Vertical angle of view" }, // 縦画角
	{ 0x1413e8390, u8"Interpolation time" }, // 補間時間
	{ 0x14139f318, u8"Loaded camera" }, // 装填カメラ
	{ 0x14139f3b0, u8"Aiming camera" }, // 照準カメラ
	{ 0x14139f490, u8"Shooting camera" }, // 射撃カメラ
	{ 0x14139f578, u8"Radius of loading judgment range" }, // 装填判定範囲の半径
	{ 0x14139f608, u8"Player position object when loading" }, // 装填時のプレイヤー位置オブジェクト
	{ 0x14139f6b0, u8"Loading operation waiting time" }, // 装填動作の待ち時間
	{ 0x14139f7b8, u8"Turret rotation speed" }, // 砲塔の回転スピード
	{ 0x14139f8a8, u8"Gun barrel rotation speed" }, // 砲身の回転スピード
	{ 0x14139f9dc, u8"Bullet speed" }, // 弾速
	{ 0x14139fa70, u8"Maximum time to wait until impact" }, // 着弾まで待つ最大時間
	{ 0x1413a0038, u8"Waiting time until event start" }, // イベント開始までの待ち時間
	{ 0x1413a0130, u8"Object to hide during event" }, // イベント中に非表示にするオブジェクト
	{ 0x1413a0228, u8"Post-event position object" }, // イベント後位置オブジェクト
	{ 0x1413a02d0, u8"Post-event fixation object" }, // イベント後注視点オブジェクト
	{ 0x1413a0f40, u8"GimmickInfo" }, // gimmickInfo
	{ 0x1413a1028, u8"Condition/hub" }, // condition/hub
	{ 0x1413a10e8, u8"Event" }, // event
	{ 0x1413a1300, u8"Don't show on map" }, // マップ上に表示しない
	{ 0x1413a13e8, u8"Is it a riddle?" }, // 謎解きか
	{ 0x1413a14c8, u8"Operational time" }, // 操作可能時間
	{ 0x1413a1be0, u8"Script file name" }, // スクリプトファイル名
	{ 0x1413a21f8, u8"Instance drawable" }, // インスタンス描画可能
	{ 0x1413a22b8, u8"GI texture name" }, // GIテクスチャ名
	{ 0x1413a2338, u8"GIOcclusion texture name" }, // GIOcclusionテクスチャ名
	{ 0x1413a6358, u8"Number of drops/Number of rings" }, // ドロップ数/リング数
	{ 0x1413a6430, u8"[RING only] Number of super rings" }, // [RING専用]スーパーリング数
	{ 0x1413a6518, u8"Scattering" }, // ばらつき
	{ 0x1413a2490, u8"Drop items" }, // ドロップアイテム
	{ 0x1413a25a8, u8"Should I move?" }, // 移動するか
	{ 0x1413a3118, u8"The speed is constant during this time" }, // この時間は速度が一定
	{ 0x1413a323c, u8"Inversion" }, // 反転
	{ 0x1413a36c0, u8"Height of the stand" }, // 台の高さ
	{ 0x1413a37b0, u8"End point coordinates" }, // 終点座標
	{ 0x1413a38b0, u8"Laser extinction conditions" }, // レーザーの消滅条件
	{ 0x1413a39f0, u8"Extinction time" }, // 消滅時間
	{ 0x1413a40b0, u8"Start in flower state" }, // 花状態で開始
	{ 0x1413a4c10, u8"Installation method" }, // 設置方式
	{ 0x1413a4d70, u8"Number of lights (evenly spaced only)" }, // 光の数(等間隔配置のみ)
	{ 0x1413a4ef0, u8"Light placement node (node placement only)" }, // 光配置ノード(ノード配置のみ)
	{ 0x1413a5030, u8"Valid only at certain times" }, // 特定時間のみ有効
	{ 0x1413a5138, u8"Start time (hour)" }, // 開始時刻(時)
	{ 0x1413a5300, u8"Start time (minutes)" }, // 開始時刻(分)
	{ 0x1413a53c8, u8"End time (hour)" }, // 終了時刻(時)
	{ 0x1413a54b8, u8"End time (minutes)" }, // 終了時刻(分)
	{ 0x1413a5558, u8"Appearance interval" }, // 出現間隔
	{ 0x1413a5cf8, u8"Stage name" }, // ステージ名
	{ 0x1413a5fc0, u8"Lateral movement right limit value" }, // 横移動右限界値
	{ 0x1413a60e0, u8"Lateral movement left limit value" }, // 横移動左限界値
	{ 0x1413a6218, u8"Limit value before vertical movement" }, // 縦移動前限界値
	{ 0x1413a62f8, u8"Limit value after vertical movement" }, // 縦移動後限界値
	{ 0x1413a63c0, u8"Vertical effective" }, // 縦有効
	{ 0x1413a64a0, u8"TargetManager" }, // targetManager
	{ 0x1413a6fa8, u8"Accessible distance" }, // アクセス可能距離
	{ 0x1413a70d8, u8"Positioning" }, // 位置調整
	{ 0x1413a71e0, u8"Orientation adjustment" }, // 向き調整
	{ 0x1413a7648, u8"Model type" }, // モデル種類
	{ 0x1413a76f8, u8"Title label" }, // タイトルラベル
	{ 0x1413a77d8, u8"Body label" }, // 本文ラベル
	{ 0x1413a78a0, u8"Animation speed" }, // アニメーション速度
	{ 0x1413a7a18, u8"Respawn time" }, // リスポーン時間
	{ 0x1413a7b30, u8"Cuboid collision" }, // 直方体コリジョン
	{ 0x1413a7bd0, u8"Collision offset" }, // コリジョンオフセット
	{ 0x1413a8178, u8"With pedestal" }, // 台座あり
	{ 0x1413a81f8, u8"Start floating" }, // 浮遊状態で開始
	{ 0x1413a8270, u8"Altitude (m)" }, // 高度(m)
	{ 0x1413a8338, u8"Time from impact to start of fall (s)" }, // 衝撃から落下開始までの時間(s)
	{ 0x1413a8418, u8"Limit floating time" }, // 浮遊時間を制限
	{ 0x1413a84c8, u8"Floating time (s)" }, // 浮遊時間(s)
	{ 0x1413a8620, u8"Fly backwards with HA when floating" }, // 浮かせる時のHAで後ろに飛ばす
	{ 0x1413a8760, u8"Backward speed (m/s)" }, // 後ろに飛ばす速度(m/s)
	{ 0x1413a9240, u8"OFF when attacked again" }, // 再度攻撃でOFF
	{ 0x1413a92e0, u8"Bound strength after HA (Y)" }, // HA後のバウンド強度(Y)
	{ 0x1413a93b8, u8"Bound strength after HA (Z)" }, // HA後のバウンド強度(Z)
	{ 0x1413a9b30, u8"Play area range" }, // プレイエリアの範囲
	{ 0x1413a9c18, u8"Impact value" }, // インパクトの値
	{ 0x1413a9db0, u8"Damage correction multiplier when difficulty level is Very Hard" }, // 難易度VeryHard時のダメージ補正倍率
	{ 0x1413aa2e8, u8"Number of boxes (X axis)" }, // 箱の数(X軸)
	{ 0x1413aa410, u8"Number of boxes (Y axis)" }, // 箱の数(Y軸)
	{ 0x1413aa530, u8"Number of boxes (Z axis)" }, // 箱の数(Z軸)
	{ 0x1413aa680, u8"Cast a shadow" }, // 影を落とすか
	{ 0x1413aafb0, u8"Model type" }, // モデルタイプ
	{ 0x1413ab008, u8"Direction of movement" }, // 移動方向
	{ 0x1413ab070, u8"Movement range: start" }, // 移動範囲：開始
	{ 0x1413ab168, u8"Movement range: End" }, // 移動範囲：終了
	{ 0x1413ab298, u8"Gun barrel angle" }, // 砲身の角度
	{ 0x1413ab350, u8"Number of injection directions" }, // 射出方向の数
	{ 0x1413b0688, u8"Direction of injection" }, // 射出する方向
	{ 0x1413ab410, u8"Injection settings" }, // 射出設定
	{ 0x1413abd28, u8"Target distance" }, // 目標距離
	{ 0x1413abe08, u8"Target distance (left and right)" }, // 目標距離（左右）
	{ 0x1413abf48, u8"Target altitude" }, // 目標高度
	{ 0x1413ac0b8, u8"Injection strength" }, // 射出強度
	{ 0x1413ac1f0, u8"Ending strength" }, // 終了強度
	{ 0x1413ac328, u8"End angle" }, // 終了角度
	{ 0x1413ac478, u8"Injection speed" }, // 射出速度
	{ 0x1413ac588, u8"Uncontrollable time" }, // 制御不可時間
	{ 0x1413ac6b8, u8"Motion playback time" }, // モーション再生時間
	{ 0x1413ac7e8, u8"Is it event-driven?" }, // イベント起動型か
	{ 0x1413aec60, u8"Is it horizontal?" }, // 横置きか
	{ 0x1413af440, u8"Maximum rope length" }, // ロープの最大長さ
	{ 0x1413af4e8, u8"Maximum winding speed" }, // 巻き取り速度の最大値
	{ 0x1413af660, u8"Inoperable time during injection" }, // 射出時の操作不可時間
	{ 0x1413af7c0, u8"Can only be used once" }, // 一回だけ使用可能
	{ 0x1413b2ca0, u8"Light color" }, // ライトの色
	{ 0x1413b34a8, u8"Number of gates (evenly spaced only)" }, // ゲート数(等間隔配置のみ)
	{ 0x1413b35a8, u8"Gate placement node (node placement only)" }, // ゲート配置ノード(ノード配置のみ)
	{ 0x1413b36d8, u8"Link timer switch" }, // リンクタイマースイッチ
	{ 0x1413b37a0, u8"Collision size" }, // コリジョンの大きさ
	{ 0x1413b3948, u8"Respawn on failure elsewhere" }, // 他で失敗時にリスポーンする
	{ 0x1413b3ae8, u8"Timed gate to respawn on failure" }, // 失敗時にリスポーンさせる時限ゲート
	{ 0x1413b44a0, u8"Number of flowers (evenly spaced only)" }, // 花の数(等間隔配置のみ)
	{ 0x1413b4558, u8"Flower placement node (node placement only)" }, // 花配置ノード(ノード配置のみ)
	{ 0x1413b6710, u8"Type of seeds" }, // タネの種類
	{ 0x1413b7478, u8"Label name" }, // ラベル名
	{ 0x1413b7920, u8"The camera ends when you exit the collision." }, // コリジョンから出たらカメラ終了
	{ 0x1413b7a60, u8"Camera closes after time passes" }, // 時間経過でカメラ終了
	{ 0x1413b7b58, u8"Time until camera ends" }, // カメラ終了までの時間
	{ 0x1413b7d20, u8"The camera ends when the subtitles end" }, // 字幕終了でカメラ終了
	{ 0x1413b7dc8, u8"Movement control" }, // 移動制御
	{ 0x1413b7e70, u8"Is it letterbox?" }, // レターボックスか
	{ 0x1413b7fb0, u8"Exit margin" }, // 離脱マージン
	{ 0x1413b9098, u8"Installation type" }, // 設置タイプ
	{ 0x1413b9260, u8"Movement width" }, // 移動幅
	{ 0x1413b9430, u8"Rush speed" }, // 突進速度
	{ 0x1413b9550, u8"Crazy speed" }, // ぶっ飛び速度
	{ 0x1413b9610, u8"Tire rotation angle per second (during normal movement)" }, // タイヤの毎秒回転角度（通常移動時）
	{ 0x1413b9758, u8"Tire rotation angle per second (when charging)" }, // タイヤの毎秒回転角度（溜め時）
	{ 0x1413b9868, u8"Tire rotation angle per second (during rush)" }, // タイヤの毎秒回転角度（突進時）
	{ 0x1413b9aa0, u8"Reappearance time" }, // 再出現時間
	{ 0x1413b9bf8, u8"Time until lost during rush" }, // 突進中に見失うまでの時間
	{ 0x1413b9cf8, u8"Height recognized as a cliff" }, // 崖と認識する高さ
	{ 0x1413b9d98, u8"Obstacle recognition distance" }, // 障害物を認識する距離
	{ 0x1413b9f20, u8"Rotation angle per second" }, // 毎秒回転角度
	{ 0x1413ba030, u8"Reappear" }, // 再出現する
	{ 0x1413ba0f8, u8"Attack" }, // 攻撃する
	{ 0x1413ba178, u8"Take a discovery motion" }, // 発見モーションを取る
	{ 0x1413bafe8, u8"Music No." }, // MusicNo
	{ 0x1413bb530, u8"Number" }, // 数
	{ 0x1413bbbe0, u8"Number of placements" }, // 配置数X
	{ 0x1413bbc68, u8"Number of placements Y" }, // 配置数Y
	{ 0x1413bbce0, u8"Number of placements Z" }, // 配置数Z
	{ 0x1413bc240, u8"Layer name" }, // レイヤー名
	{ 0x1413b7680, u8"Panel list" }, // パネルリスト
	{ 0x1413b7738, u8"Signal transmission target" }, // 信号送信対象
	{ 0x1413bce10, u8"Dummy panel" }, // ダミーパネル
	{ 0x1413bd980, u8"Inoperable time" }, // 操作不能時間
	{ 0x1413bdb48, u8"Unable to operate upon grounding" }, // 接地で操作不能解除
	{ 0x1413bf808, u8"Effect name" }, // エフェクト名
	{ 0x1413bf9f8, u8"Color [red]" }, // カラー[赤]
	{ 0x1413bfa98, u8"Color [green]" }, // カラー[緑]
	{ 0x1413bfb90, u8"Color [blue]" }, // カラー[青]
	{ 0x1413bfc78, u8"Color [alpha]" }, // カラー[アルファ]
	{ 0x1413b08e0, u8"Queue name" }, // キュー名
	{ 0x1413b7ecc, u8"3D?" }, // 3Dか
	{ 0x1413b3348, u8"Volume" }, // 音量
	{ 0x1413b8968, u8"Sound parameters" }, // サウンドパラメータ
	{ 0x1413b79c8, u8"Vibration name" }, // 振動名
	{ 0x1413b8a48, u8"Controller vibration parameters" }, // コントローラ振動パラメータ
	{ 0x1413be8a8, u8"Launch trigger" }, // 起動トリガ
	{ 0x1413be970, u8"Sector" }, // セクター
	{ 0x1413bfe30, u8"Range" }, // 範囲
	{ 0x1413bff10, u8"Location of occurrence" }, // 発生位置
	{ 0x1413bff68, u8"Suitable for playback" }, // 再生向き
	{ 0x1413c0060, u8"Regeneration cycle" }, // 再生サイクル
	{ 0x1413c01e0, u8"Do not generate on the bottom of the rectangle" }, // 四角形の底面には発生させない
	{ 0x1413c02a0, u8"Occurs only in the upper half" }, // 上半分のみに発生させる
	{ 0x1413c0380, u8"Default active state" }, // デフォルトのアクティブ状態
	{ 0x1413c1518, u8"Collision: Width (diameter)" }, // コリジョン:幅(直径)
	{ 0x1413c15f8, u8"Collision: Height" }, // コリジョン:高さ
	{ 0x1413c16c0, u8"Collision: Depth" }, // コリジョン:奥行き
	{ 0x1413c1760, u8"Front camera Z offset" }, // カメラ前Zオフセット
	{ 0x1413c1880, u8"Play only while in contact" }, // 接触中のみ再生
	{ 0x1413c1a88, u8"Gravity position" }, // 重力位置
	{ 0x1413c23e0, u8"How to specify speed" }, // 速度指定方法
	{ 0x1413c2550, u8"Operation prohibition time after injection" }, // 射出後操作禁止時間
	{ 0x1413c27a8, u8"Attack time when firing" }, // 射出時の攻撃時間
	{ 0x1413c28d8, u8"Orient yourself in the opposite direction of gravity?" }, // 重力方向の逆に姿勢を向けるか
	{ 0x1413c3288, u8"Maximum travel time" }, // 最大移動時間
	{ 0x1413c36b0, u8"Color:R" }, // 色:R
	{ 0x1413c385c, u8"Color:G" }, // 色:G
	{ 0x1413c3a1c, u8"Color:B" }, // 色:B
	{ 0x1413c3bc0, u8"Light source radius" }, // 光源半径
	{ 0x1413c3d70, u8"Light source length" }, // 光源長さ
	{ 0x1413c3ed8, u8"Attenuation radius" }, // 減衰半径
	{ 0x1413c4078, u8"Shadow" }, // シャドウ
	{ 0x1413ea768, u8"Moving distance" }, // 移動距離
	{ 0x1413c54e8, u8"Behavior on restart" }, // 再開時挙動
	{ 0x1413c5648, u8"Camera volume when returning" }, // 復帰時カメラボリューム
	{ 0x1413c5780, u8"Camera valid time when returning (permanent at 0)" }, // 復帰時カメラ有効時間(0で永続)
	{ 0x1413c60e8, u8"Drawing time" }, // 描画時間
	{ 0x1413c67e0, u8"Number of portal fragments required for activation" }, // 起動に必要なポータル欠片数
	{ 0x1413b99d0, u8"Time until portal collection" }, // ポータル回収までの時間
	{ 0x1413b9a68, u8"PortalbitType" }, // portalbitType
	{ 0x1413c6fe0, u8"Portal parameters" }, // ポータルのパラメータ
	{ 0x1413c70f8, u8"Number of bounces" }, // バウンス回数
	{ 0x1413c7858, u8"Can be moved again after stopping" }, // 停止後再度移動可能
	{ 0x1413c84c8, u8"Stage Flag No." }, // ステージFlagNo
	{ 0x1413c8600, u8"Does it work as a goal?" }, // ゴールとして機能するか
	{ 0x1413c9990, u8"Use camera" }, // カメラを使用する
	{ 0x141395628, u8"Roll" }, // ロール
	{ 0x1413ca668, u8"Gimmick number" }, // ギミック番号
	{ 0x1413ca788, u8"Practice number" }, // 練習番号
	{ 0x1413ca848, u8"Tips number" }, // Tips番号
	{ 0x1413ca9f8, u8"Hidden margin" }, // 非表示マージン
	{ 0x1413cab18, u8"Lock settings" }, // ロック設定
	{ 0x1413cb368, u8"Effect time" }, // 効果時間
	{ 0x1413cc270, u8"(HA) Initial velocity" }, // (HA)初速度
	{ 0x1413cc388, u8"(HA) Operation unavailable time" }, // (HA)操作不可時間
	{ 0x1413cc488, u8"(HA) Speed is constant over this distance" }, // (HA)この距離は速度が一定
	{ 0x1413cc908, u8"Climbing speed (m/s)" }, // 上昇速度(m/s)
	{ 0x1413cca60, u8"Climb altitude (m)" }, // 上昇高度(m)
	{ 0x1413ccb40, u8"Limit altitude (m)" }, // 限界高度(m)
	{ 0x1413ccc20, u8"Descending speed (m/s)" }, // 下降速度(m/s)
	{ 0x1413ccce0, u8"Time until start of descent" }, // 下降開始までの時間
	{ 0x1413cd6c8, u8"Starting position" }, // 開始位置
	{ 0x1413cd7d8, u8"How many meters before do you want to drop the player?" }, // 何ｍ手前でプレイヤーを落とすか
	{ 0x1413cd900, u8"Is there jump cancellation?" }, // ジャンプキャンセルあり？
	{ 0x1413cdb18, u8"Maximum speed (speed at which friction is applied)" }, // 最高速度(摩擦がかかる速度)
	{ 0x1413cdbd0, u8"Time until starting return (waiting time at arrival point)" }, // 戻りを開始するまでの時間(到着地点での待機時間)
	{ 0x1413cdde8, u8"Return speed" }, // 戻りの速度
	{ 0x1413ce5d0, u8"Height extension amount (m)" }, // 高さ延長量(m)
	{ 0x1413cebc0, u8"Unique pass ID" }, // ユニークパスID
	{ 0x1413ced20, u8"Node list" }, // Nodeリスト
	{ 0x1413cf1a0, u8"Column length" }, // 柱の長さ
	{ 0x1413d0c50, u8"Barrier radius" }, // バリアの半径
	{ 0x1413d0e40, u8"Barrier aspect ratio" }, // バリアの縦横比
	{ 0x1413d0f38, u8"Barrier height" }, // バリアの高さ
	{ 0x1413d1010, u8"Number of sides" }, // 側面の面数
	{ 0x1413d1130, u8"Enable collision" }, // コリジョンを有効にする
	{ 0x1413d11d8, u8"Enable ceiling" }, // 天井を有効にする
	{ 0x1413d12b8, u8"Launch CameraActivator on appearance" }, // 出現時にCameraActivatorを起動
	{ 0x1413d1408, u8"CameraActivator when appearing" }, // 出現時のCameraActivator
	{ 0x1413d1558, u8"Wait until appearance" }, // 出現までのウエイト
	{ 0x1413d1670, u8"Start CameraActivator on death" }, // 消滅時にCameraActivatorを起動
	{ 0x1413d1770, u8"CameraActivator when disappearing" }, // 消滅時のCameraActivator
	{ 0x1413d17e8, u8"Wait until extinction" }, // 消滅までのウエイト
	{ 0x1413d20d8, u8"Contained object" }, // 内包オブジェクト
	{ 0x1413ddec0, u8"Line" }, // ライン
	{ 0x1413bb298, u8"Step width" }, // ステップ幅
	{ 0x1413d3740, u8"Initial appearance" }, // 初期の見た目
	{ 0x1413d3860, u8"Item number" }, // アイテム番号
	{ 0x1413d3958, u8"Event response" }, // イベントの対応
	{ 0x1413d3a20, u8"Direction of rotation" }, // 回転方向
	{ 0x1413d3b40, u8"Draw separated semi-transparently [SV only]" }, // 分離半透明に描画[SV限定]
	{ 0x1413d3d48, u8"Acquisition range collision radius" }, // 取得範囲コリジョン半径
	{ 0x1413d4630, u8"Reflex panel ID list" }, // 反射神経パネルIDリスト
	{ 0x1413d4730, u8"Boot panel ID" }, // 起動用パネルID
	{ 0x1413d48a0, u8"Number of panels to turn off" }, // 消灯させるパネル数
	{ 0x1413d4a30, u8"Minimum lighting interval time" }, // 最小点灯間隔時間
	{ 0x1413d4b48, u8"Maximum lighting interval time" }, // 最大点灯間隔時間
	{ 0x1413d4c30, u8"Minimum lighting time" }, // 最小点灯時間
	{ 0x1413d4d50, u8"Maximum lighting time" }, // 最大点灯時間
	{ 0x1413bb898, u8"Target offset" }, // ターゲットオフセット
	{ 0x1413eecb0, u8"Angle" }, // 角度
	{ 0x1413d4e38, u8"Camera" }, // カメラ
	{ 0x1413d55e0, u8"Related GimmickInfo ID" }, // 関連GimmickInfoのID
	{ 0x1413d5708, u8"Map scanning camera" }, // マップスキャン中カメラ
	{ 0x1413d5800, u8"Scan camera interpolation start time" }, // スキャンカメラ補間開始時間
	{ 0x1413d5950, u8"Scan camera interpolation end time" }, // スキャンカメラ補間終了時間
	{ 0x1413d5aa0, u8"Map sensor display distance" }, // マップセンサー表示距離
	{ 0x1413d68c8, u8"Play phrase" }, // 再生フレーズ
	{ 0x1413d6f00, u8"Display state" }, // 表示状態
	{ 0x1413d7780, u8"Axis of rotation" }, // 回転軸
	{ 0x1413d8150, u8"Maximum speed" }, // 最大速度
	{ 0x1413d8300, u8"Minimum speed" }, // 最小速度
	{ 0x1413d8498, u8"Maximum stopping time directly above" }, // 真上での最大停止時間
	{ 0x1413d8628, u8"Minimum stopping time directly above" }, // 真上での最小停止時間
	{ 0x1413d8820, u8"Double jump occurrence rate" }, // 二重跳び発生率
	{ 0x1413d8930, u8"Double jump random" }, // 二重跳びランダム
	{ 0x1413d8a98, u8"Rate of double jump immobilization: numerator" }, // 二重跳び固定発生の割合：分子
	{ 0x1413d8b90, u8"Percentage of double jump fixed occurrences: denominator" }, // 二重跳び固定発生の割合：分母
	{ 0x1413d9d60, u8"Rotation method" }, // 回転方式
	{ 0x1413d9e58, u8"Angular velocity (deg/s)" }, // 角速度(deg/s)
	{ 0x1413d9f80, u8"[Vertical/Fixed] Vertical rotation initial angle ID" }, // [縦/固定]縦回転初期角度ID
	{ 0x1413daff8, u8"Statue type" }, // 像のタイプ
	{ 0x1413db100, u8"Rotatable" }, // 回転可能
	{ 0x1413db1d8, u8"Rotation type" }, // 回転タイプ
	{ 0x1413db288, u8"Correct angle" }, // 正解の角度
	{ 0x1413db480, u8"Number of rotations required for forced clearing at random" }, // ランダム時の強制クリアに必要な回転数
	{ 0x1413db620, u8"Minimum number to rotate as a performance at random time" }, // ランダム時に演出として回転する最低数
	{ 0x1413db730, u8"Maximum number to rotate as a performance at random time" }, // ランダム時に演出として回転する最高数
	{ 0x1413db878, u8"Laser irradiation target" }, // レーザー照射ターゲット
	{ 0x1413db970, u8"Use gaze target" }, // 視線ターゲットを使用
	{ 0x1413dba60, u8"Gaze target" }, // 視線ターゲット
	{ 0x1413dc4e8, u8"Initial lock state" }, // 初期ロック状態
	{ 0x1413dc6f0, u8"Objects that rotate together" }, // 一緒に回転するオブジェクト
	{ 0x1413dc7a0, u8"Object for managing rotating tombstones" }, // 回転墓石の管理用オブジェクト
	{ 0x1413dc890, u8"Galle Falls Pebble Statue Object" }, // ゴールの滝の小石像オブジェクト
	{ 0x1413dc970, u8"Camera offset when selected" }, // 選択時のカメラのオフセット
	{ 0x1413dd3b0, u8"Clear conditions" }, // クリア条件
	{ 0x1413c13d8, u8"Mileage" }, // 走行距離
	{ 0x1413dd030, u8"Distance type" }, // 距離タイプ
	{ 0x1413bfa40, u8"Target speed change" }, // 目標速度変化
	{ 0x1413c03a8, u8"Target speed [m/s]" }, // 目標速度[m/s]
	{ 0x1413c0f40, u8"Fixed" }, // 固定
	{ 0x1413c0de8, u8"Median [m/s]" }, // 中央値[m/s]
	{ 0x1413c0f58, u8"Amplitude [m/s]" }, // 振幅[m/s]
	{ 0x1413bf900, u8"Simple harmonic motion" }, // 単振動
	{ 0x1413c0680, u8"Minimum speed [m/s]" }, // 最小速度[m/s]
	{ 0x1413c0768, u8"Maximum speed [m/s]" }, // 最大速度[m/s]
	{ 0x1413c0880, u8"Update cycle" }, // 更新周期
	{ 0x1413bf440, u8"Random" }, // ランダム
	{ 0x1413bfd28, u8"Tolerance [m/s]" }, // 許容誤差[m/s]
	{ 0x1413bfdf8, u8"Regulation time" }, // 規定時間
	{ 0x1413dd0d0, u8"Speed adjustment type" }, // 速度合わせタイプ
	{ 0x1413dd8a8, u8"Sonic fixed" }, // ソニック固定
	{ 0x1413dd9a8, u8"Speed to reach the forefront position [m/s]" }, // 最前位置になる速度[m/s]
	{ 0x1413dda78, u8"Speed at final position [m/s]" }, // 最後位置になる速度[m/s]
	{ 0x1413de548, u8"Is there a time limit?" }, // 制限時間の有無
	{ 0x1413de660, u8"Number of directions of injection" }, // 射出する方向の数
	{ 0x1413e0570, u8"Prohibit menu operations while in contact" }, // 接触中のメニュー操作を禁止
	{ 0x1413e0e68, u8"Node number" }, // Node番号
	{ 0x1413e0fc8, u8"Line type" }, // ラインタイプ
	{ 0x1413e34a0, u8"Pass type" }, // パスの種類
	{ 0x1413dfab8, u8"Grind cap" }, // グラインドのキャップ
	{ 0x1413dfc00, u8"Grind model unit distance" }, // グラインドのモデル単位距離
	{ 0x1413dfd58, u8"ParamType" }, // paramType
	{ 0x1413e3930, u8"Is it possible to control movement after creation?" }, // 生成後に移動制御可能か
	{ 0x1413e14b8, u8"Is it a loop path? [Loop does not support concatenation]" }, // ループパスか[Loopすると連結非対応]
	{ 0x1413e1638, u8"Starting point line type" }, // 始点のラインタイプ
	{ 0x1413e3758, u8"Division distance" }, // 分割距離
	{ 0x1413dc960, u8"Is it a connected path?" }, // 連結パスか
	{ 0x1413dca48, u8"Next path connection method" }, // 次パス連結方法
	{ 0x1413dcb48, u8"Curve strength when connecting the next pass" }, // 次パス連結時のカーブの強さ
	{ 0x1413dd6e8, u8"Connection settings" }, // 接続設定
	{ 0x1413e1570, u8"Set parameters" }, // セットパラメータ
	{ 0x1413de150, u8"Starting loop radius" }, // 開始ループ半径
	{ 0x1413de210, u8"End loop radius" }, // 終了ループ半径
	{ 0x1413de2e8, u8"Loop angle" }, // ループ角度
	{ 0x1413de388, u8"Horizontal misalignment" }, // 横方向のズレ
	{ 0x1413de490, u8"Straight line distance before loop" }, // ループ前の直線距離
	{ 0x1413de570, u8"Straight line distance after loop" }, // ループ後の直線距離
	{ 0x1413de5f8, u8"Interpolation strength" }, // 補間強度
	{ 0x1413de6d8, u8"Interpolation point" }, // 補間箇所
	{ 0x1413e2850, u8"Vertical misalignment" }, // 縦方向のズレ
	{ 0x1413ddce8, u8"Twist angle" }, // ねじれ角度
	{ 0x1413dd2d8, u8"Normal direction type" }, // 法線方向種類
	{ 0x1413dd4d8, u8"Adjust the number of divisions" }, // 分割数調整
	{ 0x1413dd610, u8"Reverse path direction" }, // パス方向反転
	{ 0x1413e06f8, u8"Shape type" }, // 形状種類
	{ 0x1413acf24, u8"Width" }, // 横幅
	{ 0x1413e0b20, u8"Position offset [height]" }, // 位置オフセット[高さ]
	{ 0x1413e3588, u8"Path list" }, // パスリスト
	{ 0x1413e3640, u8"Is it a loop path?" }, // ループパスか
	{ 0x1413e4100, u8"Target generation minimum radius" }, // ターゲット生成最小半径
	{ 0x1413e41a8, u8"Target generation maximum radius" }, // ターゲット生成最大半径
	{ 0x1413e42d8, u8"Target generation altitude" }, // ターゲット生成高度
	{ 0x1413e43d0, u8"Target falling speed" }, // ターゲット落下速度
	{ 0x1413e4498, u8"Launch switch ID" }, // 起動スイッチID
	{ 0x1413e4d18, u8"Shutter size" }, // シャッターサイズ
	{ 0x1413e4e98, u8"Height when shutter opens" }, // シャッター開く時高さ
	{ 0x1413e5088, u8"Shutter opening speed" }, // シャッター開くスピード
	{ 0x1413e5268, u8"Set up in a vacant state" }, // 空いてる状態で設置
	{ 0x1413e5430, u8"Interlocking target" }, // 連動するターゲット
	{ 0x1413e5fc0, u8"Time until it disappears" }, // 消えるまでの時間
	{ 0x1413e6d18, u8"Type of appearance" }, // 外見の種類
	{ 0x1413e7330, u8"Support for status change timing bugs enabled" }, // 状態変更のタイミングバグの対応有効
	{ 0x1413e7780, u8"Slider speed" }, // スライダー速度
	{ 0x1413e87c8, u8"Level" }, // レベル
	{ 0x1413e8a70, u8"2-point offset" }, // 2点間オフセット
	{ 0x1413e8b70, u8"Offset phase" }, // オフセット位相
	{ 0x1413e8d38, u8"Waiting time" }, // 待機時間
	{ 0x1413e8e30, u8"Attack time" }, // 攻撃時間
	{ 0x1413e8f88, u8"Rotation phase" }, // 回転位相
	{ 0x1413e9038, u8"With rigid body" }, // 剛体あり
	{ 0x1413e9200, u8"Rigid body collision radius" }, // 剛体コリジョン半径
	{ 0x1413e9358, u8"Damage collision radius" }, // ダメージコリジョン半径
	{ 0x1413e94a8, u8"Battle starting distance" }, // 戦闘開始距離
	{ 0x1413e95a0, u8"Rotation speed when facing the target" }, // ターゲットを向くときの回転速度
	{ 0x1413ea2d8, u8"Inner cone angle" }, // 内側のコーン角度
	{ 0x1413ea428, u8"Outer cone angle" }, // 外側のコーン角度
	{ 0x1413eae78, u8"Should HA be enabled?" }, // HAを有効にするか
	{ 0x1413eb4b0, u8"Fly the pass smoothly" }, // パスをなめらかに飛ぶか
	{ 0x1413ede20, u8"Rotational speed" }, // 回転速度
	{ 0x1413eb538, u8"Rotation parameters for Spider" }, // Spider用回転パラメータ
	{ 0x1413eb608, u8"Contact collision position offset" }, // 接触コリジョンの位置オフセット
	{ 0x1413eb760, u8"Contact collision scale factor" }, // 接触コリジョンのスケール倍率
	{ 0x1413ec238, u8"Minimum addition speed" }, // 加算最低速度
	{ 0x1413ec300, u8"Maximum addition speed" }, // 加算最高速度
	{ 0x1413ec898, u8"Start performance type" }, // 開始演出タイプ
	{ 0x1413ec908, u8"Running/moving speed" }, // 走り/移動速度
	{ 0x1413ec9c0, u8"Running/movement fixed time" }, // 走り/移動固定時間
	{ 0x1413ecad8, u8"Drop/no operation time" }, // 落下/操作禁止時間
	{ 0x1413ee568, u8"Block type" }, // ブロックの種類
	{ 0x1413ee658, u8"Starting road width" }, // 開始道幅
	{ 0x1413ee7c0, u8"End road width" }, // 終了道幅
	{ 0x1413ee878, u8"Ground thickness" }, // 地面の厚み
	{ 0x1413eea18, u8"Depth offset" }, // 奥行きオフセット
	{ 0x1413eeb68, u8"Curve radius" }, // カーブ半径
	{ 0x1413eed68, u8"Number of curve divisions" }, // カーブ分割数
	{ 0x1413eee20, u8"Slope height" }, // 勾配の高さ
	{ 0x1413eef00, u8"Rigid type" }, // 剛体タイプ
	{ 0x1413eefe0, u8"Attribute: Parkour possible" }, // 属性：パルクール可
	{ 0x1413ef0d0, u8"Attribute: broken" }, // 属性：壊れる
	{ 0x1413ef190, u8"Attribute: Slope" }, // 属性：斜面化
	{ 0x1413ef2a0, u8"Attribute: Wall surface" }, // 属性：壁面化
	{ 0x1413ef380, u8"Attribute: loop terrain" }, // 属性：ループ地形
	{ 0x1413ef440, u8"Attribute: Slide terrain" }, // 属性：スライド地形
	{ 0x1413ef520, u8"Wall [left]" }, // 壁[左]
	{ 0x1413ef608, u8"Wall [right]" }, // 壁[右]
	{ 0x1413ef768, u8"Wall [front]" }, // 壁[前]
	{ 0x1413ef828, u8"Wall [back]" }, // 壁[後]
	{ 0x1413ef918, u8"Wall height" }, // 壁の高さ
	{ 0x1413efa08, u8"Color type" }, // 色タイプ
	{ 0x1413efaa8, u8"Compartment width" }, // 区画幅
	{ 0x1413f0dbc, u8"Thickness" }, // 厚み
	{ 0x1413f0f10, u8"Point distance" }, // ポイント距離
	{ 0x1413f1e20, u8"Division number" }, // 分割数
	{ 0x1413f2138, u8"Normal direction" }, // 法線方向
	{ 0x1413f2c18, u8"Number of cylinder faces" }, // シリンダー面数
	{ 0x1413f2cc8, u8"Distance to bend the cylinder" }, // シリンダーを曲げる距離
	{ 0x1413f32d0, u8"Thorn placement interval" }, // トゲ配置間隔
	{ 0x1413f3368, u8"Thorn sinking time" }, // トゲ沈下時間
	{ 0x1413f3430, u8"Thorn protrusion time" }, // トゲ突出時間
	{ 0x1413f3530, u8"Initial operation type" }, // 初期動作タイプ
	{ 0x1413f3658, u8"Does it have a rigid body?" }, // 剛体を持つか
	{ 0x1413f3e98, u8"Model type" }, // モデルの種類
	{ 0x1413f3f18, u8"Will it also damage the enemy?" }, // エネミーにもダメージを与えるか
	{ 0x1413c5558, u8"Round trip time" }, // 往復にかかる時間
	{ 0x1413c5718, u8"Distance traveled back and forth" }, // 往復移動する距離
	{ 0x1413b89f8, u8"Update type" }, // 更新タイプ
	{ 0x1413f4050, u8"Movement parameters" }, // 移動パラメータ
	{ 0x1413c6040, u8"Time taken for one round" }, // 一周にかかる時間
	{ 0x1413f4150, u8"Rotation parameters" }, // 回転パラメータ
	{ 0x1413f42e8, u8"Collision magnification" }, // コリジョン倍率
	{ 0x1413f504c, u8"Rigid body" }, // 剛体
	{ 0x1413f5158, u8"Can you tie it to a moving floor?" }, // 動く床と紐付けるか
	{ 0x1413f5420, u8"Height of the ball" }, // ボールが浮く高さ
	{ 0x1413f54f0, u8"Correspondence to change the behavior of the ball to that for upd3" }, // ボールの挙動をupd3用の挙動に変更する対応
	{ 0x1413f5598, u8"The value of gravity acting on the ball" }, // ボールにかかる重力の値
	{ 0x1413f6080, u8"Goal" }, // ゴール
	{ 0x1413f61a8, u8"Ball" }, // ボール
	{ 0x1413f6290, u8"Generator" }, // 生成装置
	{ 0x1413f64c0, u8"Disappearance distance of the ball" }, // ボールの消滅距離
	{ 0x1413f6640, u8"Launch CameraActivator on start" }, // 開始時にCameraActivatorを起動
	{ 0x1413f6790, u8"CameraActivator at start" }, // 開始時のCameraActivator
	{ 0x1413bb188, u8"End interpolation time" }, // 終了補間時間
	{ 0x1413f6e30, u8"Acquisition production camera" }, // 取得演出カメラ
	{ 0x1413f6ef0, u8"Stop player input during acquisition performance" }, // 取得演出中のプレイヤー入力を止める
	{ 0x1413f7010, u8"Height of mountain movement during acquisition performance" }, // 取得演出時の山なり移動の高さ
	{ 0x1413f7188, u8"Offset during transportation" }, // 運搬時オフセット
	{ 0x1413f7280, u8"Y-axis rotation angle during transportation" }, // 運搬時Y軸回転角度
	{ 0x1413f7400, u8"Tracking coefficient during transportation" }, // 運搬時追従係数
	{ 0x1413f7490, u8"Transport camera distance" }, // 運搬カメラ距離
	{ 0x1413f7518, u8"Initial downward angle of transport camera" }, // 運搬カメラ初期見下げ角度
	{ 0x1413f75e8, u8"Offset during installation" }, // 設置時オフセット
	{ 0x1413f7760, u8"Explosion effective time" }, // 爆発有効時間
	{ 0x1413f7870, u8"Disappearance waiting time" }, // 消滅待機時間
	{ 0x1413f7f28, u8"Time bomb" }, // 時限爆弾
	{ 0x1413f8fb0, u8"Number of rings (equally spaced only)" }, // リングの数(等間隔配置のみ)
	{ 0x1413f9100, u8"Ring placement node (node placement only)" }, // リング配置ノード(ノード配置のみ)
	{ 0x1413f9ac0, u8"Whether to play the first SE" }, // 最初のSEを再生するかどうか
	{ 0x1413f9bd8, u8"Count timer and weight separately" }, // タイマーとウエイトを個別カウント
	{ 0x1413fa6a0, u8"Rotating object" }, // 回転するオブジェクト
	{ 0x1413fab98, u8"Control console" }, // 操作台
	{ 0x1413fadb0, u8"Link object" }, // リンクオブジェクト
	{ 0x1413fb020, u8"Number of connection points recorded" }, // 接続点記録個数
	{ 0x1413fb070, u8"Control panel switch" }, // 操作台スイッチ
	{ 0x1413fb128, u8"Grind anchor" }, // グラインドアンカー
	{ 0x1413fb1d8, u8"[For development] Input order" }, // [開発用]入力順番
	{ 0x1413fb7b8, u8"Number obtained" }, // 入手数
	{ 0x1413fb8c0, u8"Use sub camera when appearing" }, // 出現時サブカメラ使用
	{ 0x1413fb940, u8"Sub camera position" }, // サブカメラ位置
	{ 0x1413fba70, u8"Sub camera target position" }, // サブカメラ対象位置
	{ 0x1413fbbe0, u8"Sub camera startup time" }, // サブカメラ起動時間
	{ 0x1413fcff8, u8"Action details" }, // 行動内容
	{ 0x1413fd090, u8"When there is a collision" }, // 衝突した際
	{ 0x1413fd0e0, u8"When moving away from a collision" }, // 衝突から離れた際
	{ 0x1413fd180, u8"Goal achievement UI display when automatically clearing" }, // 自動クリア時目標達成UI表示
	{ 0x1413fd670, u8"From back to front" }, // 後ろから前へ
	{ 0x1413fd6e0, u8"Front to back" }, // 前から後ろへ
	{ 0x1413fe140, u8"Warp target" }, // ワープ対象
	{ 0x1413fe200, u8"Warp position locator" }, // ワープ位置のLocator
	{ 0x1413fe298, u8"Do you want to change visibility after warping?" }, // ワープ後、表示・非表示を変更するか
	{ 0x1413fe370, u8"Display/hide settings after warp" }, // ワープ後の表示・非表示設定
	{ 0x1413fe788, u8"Warp destination" }, // ワープ先
	{ 0x1413fe820, u8"Status" }, // ステータス
	{ 0x141393708, u8"Startup type" }, // 起動タイプ
	{ 0x141393780, u8"Startup time type" }, // 起動時間タイプ
	{ 0x141393908, u8"Psyhammer effect time" }, // サイハンマー効果時間
	{ 0x141393ef0, u8"Water surface collider radius" }, // 水面コライダー半径
	{ 0x141394ca0, u8"Type of animal" }, // 動物の種類
	{ 0x141394dd0, u8"Rescue location object" }, // 救出場所オブジェクト
	{ 0x141394ef8, u8"Movement radius" }, // 移動範囲半径
	{ 0x141394f78, u8"Jump height: 0-default" }, // ジャンプ高さ: 0-default
	{ 0x1413950b8, u8"Jump time: 0-default" }, // ジャンプの時間: 0-default
	{ 0x141395200, u8"UI display order" }, // UIの表示順
	{ 0x1413958b0, u8"Number of blocks" }, // ブロック数
	{ 0x141395e70, u8"Balloon color" }, // 風船の色
	{ 0x141395f60, u8"Installation dimension" }, // 設置次元
	{ 0x141396038, u8"Upward force upon contact" }, // 接触時に上向きにかかる力
	{ 0x141396100, u8"Minimum speed during injection (if 0.0, only the maximum speed is treated)" }, // 射出時の最低速度(0.0の場合は最高速度のみを扱う)
	{ 0x141396238, u8"Maximum speed during injection" }, // 射出時の最高速度
	{ 0x141396438, u8"Time when no operations are accepted" }, // 操作を受けつけない時間
	{ 0x1413965a0, u8"Time to maintain initial velocity" }, // 初速を維持する時間
	{ 0x141396740, u8"No swinging time" }, // 空振り禁止時間
	{ 0x141396808, u8"Will it return to the set position?" }, // セット位置に復活する？
	{ 0x141396910, u8"Is it a group set?" }, // グループセットか？
	{ 0x141397e90, u8"Do tricks" }, // トリックを行うか
	{ 0x1413988c8, u8"Boarding ON/OFF when riding" }, // 乗ったらボーディングON/OFF
	{ 0x1413999a8, u8"Time until sign" }, // 予兆までの時間
	{ 0x141399b00, u8"Time of omens" }, // 予兆の時間
	{ 0x141399c98, u8"Will it be revived?" }, // 復活するか
	{ 0x141399da8, u8"Time until recovery" }, // 復活までの時間
	{ 0x141399ed8, u8"Number (length)" }, // 数(長さ)
	{ 0x14139a050, u8"End position" }, // 終端位置
	{ 0x14139a138, u8"Will it collapse?" }, // 崩れるか
	{ 0x14139a208, u8"Is it a lightweight version?" }, // 軽量版かどうか
	{ 0x14139aa98, u8"How to specify destination" }, // 移動先指定方法
	{ 0x14139ac70, u8"Destination object" }, // 移動先オブジェクト
	{ 0x14139adf0, u8"Destination coordinates" }, // 移動先座標
	{ 0x14139af18, u8"Basic speed" }, // 基本速度
	{ 0x14139afe8, u8"Initial speed magnification" }, // 初速倍率
	{ 0x14139b0f0, u8"Percentage of uniform movement (%)" }, // 等速移動の割合(%)
	{ 0x14139b188, u8"Automatic injection" }, // 自動射出
	{ 0x14139b218, u8"Should I move the gun barrel?" }, // 砲身を動かすか
	{ 0x14139bfd8, u8"Behavior type" }, // 行動タイプ
	{ 0x14139c050, u8"Acceleration when jumping" }, // 跳ねたときの加速度
	{ 0x14139c87c, u8"Car model" }, // 車種
	{ 0x14139c9a0, u8"Car color" }, // 車の色
	{ 0x14139caa8, u8"Additional time for successful jump" }, // ジャンプ成功時の加算時間
	{ 0x14139d670, u8"Make it a lighter version" }, // 軽量化版にする
	{ 0x14139e7c8, u8"Injection angle" }, // 射出角度
	{ 0x14139e9f8, u8"Does it require button input?" }, // ボタンの入力を必要とするか
	{ 0x14139ea58, u8"Additional time for successful trick" }, // トリック成功時の加算時間
	{ 0x1413a0890, u8"Actions at the event" }, // イベント時のアクション
	{ 0x1413a0960, u8"Time until extinction" }, // 消滅までの時間
	{ 0x1413a0a30, u8"Upward speed" }, // 上方へ加える速度
	{ 0x1413a0fd8, u8"Volume that disappears on contact" }, // 接触時に消えるボリューム
	{ 0x1413a10c0, u8"Start wait time" }, // 開始の待機時間
	{ 0x1413a1200, u8"Maximum speed during normal fall during first stage slipstream" }, // 第1段階のスリップストリーム中の通常落下時の最高速度
	{ 0x1413a1370, u8"Maximum speed during high-speed fall during first stage slipstream" }, // 第1段階のスリップストリーム中の高速落下時の最高速度
	{ 0x1413a1460, u8"Maximum speed during normal fall during second stage slipstream" }, // 第2段階のスリップストリーム中の通常落下時の最高速度
	{ 0x1413a1580, u8"Maximum speed during high-speed fall during second stage slipstream" }, // 第2段階のスリップストリーム中の高速落下時の最高速度
	{ 0x1413a1660, u8"Vertical movement speed" }, // 垂直方向の移動速度
	{ 0x1413ce298, u8"Maximum movement speed" }, // 移動最高速度
	{ 0x1413ce3d0, u8"Minimum movement speed" }, // 移動最低速度
	{ 0x1413ce500, u8"Distance from center to edge" }, // 中心から端までの距離
	{ 0x1413ce620, u8"Acceleration rate" }, // 加速率
	{ 0x1413ce6f8, u8"Deceleration rate" }, // 減速率
	{ 0x1413ce7d8, u8"Angle until start of deceleration" }, // 減速開始までの角度
	{ 0x1413a1720, u8"Figure 8 movement" }, // 8の字移動
	{ 0x1413cc2a0, u8"Maximum movement speed during circular motion" }, // 円運動時の移動最高速度
	{ 0x1413cc3d0, u8"Minimum movement speed during circular motion" }, // 円運動時の移動最低速度
	{ 0x1413cc520, u8"Traveling speed when going straight" }, // 直進時の移動速度
	{ 0x1413cc608, u8"Radius of orbit" }, // 軌道の半径
	{ 0x1413cc668, u8"Acceleration rate during circular motion" }, // 円運動時の加速率
	{ 0x1413cc770, u8"Deceleration rate during circular motion" }, // 円運動時の減速率
	{ 0x1413cc898, u8"Deceleration rate when traveling straight" }, // 直進時の減速率
	{ 0x1413cc9b8, u8"Angle at which deceleration begins during circular motion" }, // 円運動時の減速を始めるまでの角度
	{ 0x1413ccad0, u8"Waiting time at the start and end points of circular motion" }, // 円運動を開始点と終始点での待機時間
	{ 0x1413a17e0, u8"Circle movement" }, // 円移動
	{ 0x1413a1908, u8"Waiting time at the center" }, // 中心での待機時間
	{ 0x1413a19a8, u8"Homing position height offset" }, // ホーミング位置の高さのオフセット
	{ 0x1413a1ad8, u8"Time required to prepare to release weapon" }, // 武器を出す準備に入るまでの時間
	{ 0x1413a1bc0, u8"Time until weapon is released" }, // 武器を出すまでの時間
	{ 0x1413a1ce0, u8"Number of circular weapons" }, // 円状の武器の数
	{ 0x1413a1e08, u8"Circular weapon range" }, // 円状の武器の範囲
	{ 0x1413a1ef8, u8"Number of crossed weapons per direction" }, // クロス状の武器の方向あたりの数
	{ 0x1413a1fc8, u8"Interval between cross-shaped weapon rows" }, // クロス状の武器の並びの間隔
	{ 0x1413a20d8, u8"Time to move on to next attack pattern" }, // 次の攻撃パターンに移るまでの時間
	{ 0x1413a21c0, u8"Distance at which the bullet starts aiming directly at the player" }, // 弾丸がプレイヤーを直接狙い始める距離
	{ 0x1413a2280, u8"Y-axis distance from player to GameOver" }, // GameOverまでのプレイヤーとのY軸の距離
	{ 0x1413a2380, u8"Player's Y-axis coordinate for GameOver" }, // GameOverするプレイヤーのY軸座標
	{ 0x1413a2540, u8"Position of the first injection port" }, // 第1射出口の位置
	{ 0x1413a2688, u8"Location of second injection port" }, // 第2射出口の位置
	{ 0x1413cb7b0, u8"Bullet speed" }, // 弾丸の速度
	{ 0x1413cb850, u8"Time until bullet starts firing" }, // 弾丸を打ち始めるまでの時間
	{ 0x1413cb968, u8"Number of bullets fired" }, // 弾丸の連射数
	{ 0x1413cba80, u8"Number of seconds to fire bullets" }, // 弾丸を連射する秒数
	{ 0x1413cbbc8, u8"Spreading bullet range" }, // 拡散する弾丸の範囲
	{ 0x1413cbd10, u8"Y-axis offset of the bullet's target coordinates" }, // 弾丸が狙う座標のY軸のオフセット
	{ 0x1413a27a0, u8"Pattern A" }, // パターンA
	{ 0x1413a28a8, u8"Pattern B" }, // パターンB
	{ 0x1413cd8b8, u8"Use dash ring" }, // ダッシュリングを使用する
	{ 0x1413f9c88, u8"Dash ring" }, // ダッシュリング
	{ 0x1413a2b80, u8"Visualize collisions while playing" }, // コリジョンをプレイ中も可視化する
	{ 0x1413a2d18, u8"Display debug drawing of attack prediction" }, // 攻撃予知のデバッグ描画を表示する
	{ 0x1413cd208, u8"Short distance" }, // 近距離
	{ 0x1413cd378, u8"Long distance" }, // 遠距離
	{ 0x1413cd4a0, u8"Y-axis offset" }, // Y軸のオフセット
	{ 0x1413a2eb0, u8"First stage slipstream collision" }, // 第1段階のスリップストリームのコリジョン
	{ 0x1413a2fb0, u8"Second stage slipstream collision" }, // 第2段階のスリップストリームのコリジョン
	{ 0x1413a3098, u8"Collision that changes to the attacking state" }, // 攻撃を行うステートに変わるコリジョン
	{ 0x1413a4140, u8"Number of legs to reinforce the height (100cm per leg)" }, // 高いを補強する脚の本数（一本あたり100cm）
	{ 0x1413a4220, u8"Number of rods to reinforce the width (100cm per rod)" }, // 横幅を補強する棒の本数（一本あたり100cm）
	{ 0x1413a4388, u8"Height to rope injection" }, // ロープ射出までの高さ
	{ 0x1413a45f0, u8"Maximum rope contraction speed" }, // ロープ収縮速度の最大値
	{ 0x1413a49f0, u8"Will it grow beyond the maximum value?" }, // 最大値を越えても伸びるか？
	{ 0x1413a5b00, u8"Collapse interval time" }, // 崩落間隔時間
	{ 0x1413a5c38, u8"Collapse animation playback speed" }, // 崩落アニメーション再生速度
	{ 0x1413a68d0, u8"Number of constituent pieces" }, // 構成ピース数
	{ 0x1413a6a00, u8"PiecesType" }, // piecesType
	{ 0x1413a7010, u8"Time to goal" }, // ゴールまでの時間
	{ 0x1413a7148, u8"Type of player movement after goal" }, // ゴール後のPlayerの動きのタイプ
	{ 0x1413d05f0, u8"Coming on Switch" }, // スイッチで登場
	{ 0x1413d088c, u8"Appearance" }, // 出現
	{ 0x1413d1e60, u8"Maintenance distance (m)" }, // 維持距離(m)
	{ 0x1413d1f10, u8"Point-to-point movement destination" }, // 2点間移動目的地
	{ 0x1413d0948, u8"Parameter change with On signal" }, // On信号でパラメータ変更
	{ 0x1413d0a80, u8"Turret rotation start angle (°)" }, // 砲塔旋回開始角度(°)
	{ 0x1413d0b78, u8"Shooting duration (s)" }, // 射撃継続時間(s)
	{ 0x1413d0ce8, u8"Turret rotation speed (°/s)" }, // 砲塔旋回速度(°/s)
	{ 0x1413d0e78, u8"Bullet radius (m)" }, // 弾の半径(m)
	{ 0x1413d15c8, u8"Death control" }, // 死亡制御
	{ 0x1413d16b0, u8"Death distance (m)" }, // 死亡距離(m)
	{ 0x1413d17a0, u8"Death time (s)" }, // 死亡時間(s)
	{ 0x141393e08, u8"Death" }, // 死亡
	{ 0x1413da998, u8"MoveType" }, // moveType
	{ 0x1413a80c8, u8"Injection standby time" }, // 射出待機時間
	{ 0x1413a8210, u8"[Path movement] Distance to drop" }, // [パス移動]落とすまでの距離
	{ 0x1413d2670, u8"[Path movement] Multiplier of distance to drop (limited to when Sonic is holding it)" }, // [二点間移動]落とすまでの距離の倍率(ソニックが掴んでいる場合限定)
	{ 0x1413d2a20, u8"[Path movement] Distance to drop (limited to when Sonic is holding it)" }, // [パス移動]落とすまでの距離(ソニックが掴んでいる場合限定)
	{ 0x1413a8b40, u8"First move" }, // 第一移動
	{ 0x1413a8cb8, u8"Second move" }, // 第二移動
	{ 0x1413a8d80, u8"Grabbing direction" }, // 掴む方向
	{ 0x1413a9390, u8"Injection information below a certain speed" }, // 一定速度未満の射出情報
	{ 0x1413a9478, u8"Injection information above a certain speed" }, // 一定速度以上の射出情報
	{ 0x1413a9560, u8"Sonic's movement speed when the direction of ejection changes" }, // 射出方向が変わるソニックの移動速度
	{ 0x1413a9d98, u8"Input reception hours" }, // 入力受付時間
	{ 0x1413a9e58, u8"Jump injection setting" }, // ジャンプ射出設定
	{ 0x1413a9f10, u8"Attack launch settings" }, // 攻撃射出設定
	{ 0x1413aa008, u8"Operation unavailable time in case of failure" }, // 失敗時操作不可時間
	{ 0x1413aa138, u8"Collision radius" }, // コリジョン半径
	{ 0x1413aa288, u8"Return to initial state after use" }, // 使用後初期状態に戻す
	{ 0x1413aad20, u8"Update time type" }, // 更新時間タイプ
	{ 0x1413aadc8, u8"Laser length" }, // レーザーの長さ
	{ 0x1413aaeb8, u8"Waiting time" }, // 待ち時間
	{ 0x1413ab118, u8"SE regeneration flag" }, // SE再生フラグ
	{ 0x1413ab528, u8"Switch activation" }, // スイッチ起動
	{ 0x1413ab580, u8"Appearance range" }, // 出現範囲
	{ 0x1413ab658, u8"Minimum appearance interval" }, // 出現間隔最小値
	{ 0x1413ab790, u8"Maximum appearance interval" }, // 出現間隔最大値
	{ 0x1413ab8f8, u8"Basic traveling direction distance" }, // 基本進行方向距離
	{ 0x1413ab9d8, u8"Movement range (x,y)" }, // "移動範囲(x,y)"
	{ 0x1413abf88, u8"Air boost possible" }, // エアブースト可能
	{ 0x1413ace48, u8"Time to fall" }, // 落下までの時間
	{ 0x1413adfa8, u8"Can pass through from below" }, // 下から通り抜けれる
	{ 0x1413ae060, u8"Revive" }, // 復活する
	{ 0x1413ae1e8, u8"Collision height" }, // コリジョンの高さ
	{ 0x1413af880, u8"Display duration after leaving" }, // 離れてからの表示継続時間
	{ 0x1413afa50, u8"Types of operation guides" }, // 操作ガイドの種類
	{ 0x1413b0038, u8"Exit spring object" }, // 出口のバネオブジェクト
	{ 0x1413b0218, u8"It only breaks when you ride it" }, // 乗ったときのみ壊れる
	{ 0x1413b02c0, u8"Movement speed in the pipe" }, // パイプ内の移動速度
	{ 0x1413b03f8, u8"Upward launch speed at destruction" }, // 破壊時の上への打ち上げ速度
	{ 0x1413b0600, u8"Collision radius" }, // コリジョンの半径
	{ 0x1413b0760, u8"How many seconds before the signal arrives" }, // シグナルを到着何秒前に飛ばすか
	{ 0x1413b0c38, u8"Spring injection speed" }, // バネの射出速度
	{ 0x1413b0d28, u8"Uncontrollable seconds after spring" }, // バネ後のコントロール不可秒数
	{ 0x1413b0e60, u8"Pipe escape velocity (velocity in the pipe with negative value)" }, // パイプ脱出速度(負の値でパイプ中の速度)
	{ 0x1413b0fc8, u8"Uncontrollable seconds after exiting the pipe" }, // パイプ脱出後のコントロール不可秒数
	{ 0x1413b10e8, u8"Number of seconds until the lid closes completely" }, // 蓋が閉じきるまでの秒数
	{ 0x1413b1240, u8"Closed from the beginning" }, // 最初から閉じてる
	{ 0x1413b14b8, u8"Time until the lid fully opens" }, // 蓋が完全に開くまでの時間
	{ 0x1413b1eb8, u8"Whether to display columns (for SV)" }, // 柱を表示するか（SV用）
	{ 0x1413b2040, u8"Number of rods for correction in FV (1m per rod)" }, // FVでの補正用の棒の本数（1本あたり1m）
	{ 0x1413b21a0, u8"Number of columns for correction in SV (2m per column)" }, // SVでの補正用の柱の本数（1本あたり2m）
	{ 0x1413b22c8, u8"Distance offset to allow homing" }, // ホーミングを許可する距離のオフセット
	{ 0x1413b2408, u8"Collision Offset" }, // コリジョンのオフセット
	{ 0x1413b2550, u8"Collision size" }, // コリジョンのサイズ
	{ 0x1413b2678, u8"Successful injection vector" }, // 成功時の射出ベクトル
	{ 0x1413b27d8, u8"Ejection vector at failure" }, // 失敗時の射出ベクトル
	{ 0x1413b32e0, u8"View type" }, // ビュータイプ
	{ 0x1413b3b68, u8"Distance interval to generate" }, // 生成する距離の間隔
	{ 0x1413b46e8, u8"Floor type" }, // 床タイプ
	{ 0x1413b4768, u8"Pillar type" }, // 柱タイプ
	{ 0x1413b4808, u8"Floor Y coordinate offset" }, // 床Y座標オフセット
	{ 0x1413b4990, u8"Rotation angle/sec" }, // 回転角度／秒
	{ 0x1413b4ac0, u8"Initial angle" }, // 初期角度
	{ 0x1413b4b98, u8"Rotating range" }, // 回転するレンジ
	{ 0x1413b4c90, u8"Clockwise" }, // 時計回り
	{ 0x1413b4de8, u8"Play SE" }, // SEを再生する
	{ 0x1413b6698, u8"Length of one side of block" }, // ブロックの一辺の長さ
	{ 0x1413b67c8, u8"Number of blocks" }, // ブロックの数
	{ 0x1413b6b38, u8"Clockwise?" }, // 時計回りか
	{ 0x1413b6c50, u8"Animation curve" }, // アニメーションカーブ
	{ 0x1413b6da0, u8"Rotation time" }, // 回転時間
	{ 0x1413b6ea0, u8"Waiting time after rotation" }, // 回転後の待機時間
	{ 0x1413b70c0, u8"Offset distance in non-moving direction (%)" }, // 非移動方向のオフセット距離(%)
	{ 0x1413b71e8, u8"Moving distance (%)" }, // 移動距離(%)
	{ 0x1413b7360, u8"Is the event triggered?" }, // イベント起動か
	{ 0x1413b7458, u8"Does it start when you ride it?" }, // 乗ったら起動するか
	{ 0x1413b7540, u8"Behavior when caught between terrain" }, // 地形と挟まったときの挙動
	{ 0x1413d4888, u8"Do you want to jump?" }, // ジャンプするか
	{ 0x1413d49b0, u8"Do you want to spin?" }, // スピンするか
	{ 0x1413d4a98, u8"Maximum jump speed" }, // 最大ジャンプ速度
	{ 0x1413d4ba8, u8"Minimum jump speed" }, // 最小ジャンプ速度
	{ 0x1413d4ca0, u8"Height percentage for jump judgment" }, // ジャンプ判定をする高さの割合
	{ 0x1413f24b0, u8"Jump" }, // ジャンプ
	{ 0x1413b8748, u8"Angular velocity" }, // 角速度
	{ 0x1413b8868, u8"Initial behavior" }, // 初期挙動
	{ 0x1413b8bb0, u8"Axis model display" }, // 軸モデル表示
	{ 0x1413b8d20, u8"Number of axes" }, // 軸数
	{ 0x1413d5288, u8"Shaft length" }, // 軸の長さ
	{ 0x1413d53c0, u8"Number of iron balls" }, // 鉄球数
	{ 0x1413d5448, u8"Iron ball spacing" }, // 鉄球間隔
	{ 0x1413b8f10, u8"AxisParam" }, // axisParam
	{ 0x1413b98c8, u8"Model size" }, // モデルサイズ
	{ 0x1413b9978, u8"Trajectory length" }, // 軌跡の長さ
	{ 0x1413b9c60, u8"Pass label table for player progress comparison" }, // プレイヤー進行度比較用パスラベルテーブル
	{ 0x1413b9fc8, u8"Do you want to display the model of the feet that fix the handle?" }, // 取っ手を固定する足のモデルを表示するか
	{ 0x1413bb8f0, u8"Whether to fix the player's Yaw to the Obj's Front direction" }, // プレイヤーのYawをObjのFront方向に固定するか
	{ 0x1413bbea0, u8"Additional time" }, // 追加時間
	{ 0x1413bc538, u8"Rope length" }, // ロープの長さ
	{ 0x1413bc6d0, u8"Handle descent time" }, // ハンドル降下時間
	{ 0x1413d6488, u8"Acceleration amount (m/s)" }, // 加速量(m/s)
	{ 0x1413bc7d0, u8"Injection parameters" }, // 射出時パラメータ
	{ 0x1413d68b8, u8"Destination Obj" }, // 目的地Obj
	{ 0x1413d6950, u8"Offset (world coordinates)" }, // オフセット(ワールド座標)
	{ 0x1413d6b80, u8"Upward force applied during injection" }, // 射出時に加える上方向の力
	{ 0x1413e6ce8, u8"Angle of incidence at end (°)" }, // 終了時入射角(°)
	{ 0x1413d6e00, u8"Curved surface slope (°)" }, // 曲線面傾き(°)
	{ 0x1413bc8b8, u8"Ray parameters" }, // 射線パラメータ
	{ 0x1413d72a0, u8"Movable angle (°)" }, // 可動角度(°)
	{ 0x1413d5d30, u8"Injection timing (°)" }, // 射出タイミング(°)
	{ 0x1413d7490, u8"Shaking period" }, // 揺れ周期
	{ 0x1413bc980, u8"Portrait parameters" }, // 縦向きパラメータ
	{ 0x1413d5a28, u8"Tilt during rotation (°)" }, // 回転時の傾き(°)
	{ 0x1413d5b78, u8"Time to maximum slope" }, // 傾き最大までの時間
	{ 0x1413d5eb0, u8"Rotation period" }, // 回転周期
	{ 0x1413bca90, u8"Landscape parameters" }, // 横向きパラメータ
	{ 0x1413bd848, u8"Switch type" }, // スイッチタイプ
	{ 0x1413bdab8, u8"Should I use physical collision?" }, // 物理コリジョンを使うか
	{ 0x1413bf878, u8"Time until automatic injection" }, // 自動で射出されるまでの時間
	{ 0x1413bf990, u8"First stage injection" }, // 第1段階の射出
	{ 0x1413bfa28, u8"Second stage injection" }, // 第2段階の射出
	{ 0x1413bfae0, u8"Third stage injection" }, // 第3段階の射出
	{ 0x1413c0550, u8"Pitch count type" }, // 球数タイプ
	{ 0x1413c0610, u8"Distance (straight line only)" }, // 距離(直線のみ)
	{ 0x1413c06b0, u8"Distance (bending adjustment)" }, // 距離(曲がり調整)
	{ 0x1413c07a8, u8"Bullet speed (straight line only)" }, // 弾速(直線のみ)
	{ 0x1413c08a8, u8"Bullet speed when playing anime" }, // アニメ再生時の弾速
	{ 0x1413c0980, u8"Number of seconds to wait" }, // 待機秒数
	{ 0x1413d7ce0, u8"Round trip" }, // 往復
	{ 0x1413c0c10, u8"End model A" }, // 端モデルA
	{ 0x1413c0d10, u8"End model B" }, // 端モデルB
	{ 0x1413c1570, u8"Length of rod [m]" }, // 棒の長さ[m]
	{ 0x1413c16f0, u8"Travel distance (height)" }, // 移動距離（高さ）
	{ 0x1413c1780, u8"Offset of starting point (Phase)" }, // 開始地点のオフセット(Phase)
	{ 0x1413c2070, u8"Column height [m]" }, // 柱の高さ[m]
	{ 0x1413c2128, u8"Type of scaffolding" }, // 足場のタイプ
	{ 0x1413c3670, u8"Ascent distance" }, // 上昇距離
	{ 0x1413c4760, u8"Depth of deflection" }, // たわむ深さ
	{ 0x1413c48e8, u8"Number of logs" }, // 丸太の数
	{ 0x1413c5128, u8"Time until vibration" }, // 振動までの時間
	{ 0x1413c52e0, u8"Vibrating time" }, // 振動する時間
	{ 0x1413c5c58, u8"Swing angle" }, // 振れ幅角度
	{ 0x1413c5db8, u8"Stopping time after movement" }, // 移動後停止時間
	{ 0x1413af228, u8"Phase" }, // phase
	{ 0x1413c60c8, u8"Should propellers be displayed?" }, // プロペラを表示するか
	{ 0x1413c6168, u8"Floor start position" }, // 床開始位置
	{ 0x1413c6a88, u8"Tunnel type" }, // トンネルのタイプ
	{ 0x1413c71f8, u8"Territory object" }, // 縄張りオブジェクト
	{ 0x1413c7418, u8"Sonic set" }, // ソニックセット
	{ 0x1413f1a34, u8"Path" }, // パス
	{ 0x1413c7768, u8"Time offset until oxygen gauge runs out" }, // 酸素ゲージが尽きるまでの時間オフセット
	{ 0x1413c7930, u8"Travel speed offset" }, // 移動速度オフセット
	{ 0x1413bb030, u8"Do you use practice notifications?" }, // 練習通知を使用するか
	{ 0x1413c7a40, u8"Practice notification settings" }, // 練習通知設定
	{ 0x14139b518, u8"TargetPos" }, // targetPos
	{ 0x1413c8990, u8"Will it reappear?" }, // 再出現するか
	{ 0x1413c8b78, u8"Time until reappearance" }, // 再出現までの時間
	{ 0x1413c8d38, u8"Waiting time until appearance" }, // 出現までの待ち時間
	{ 0x1413c8ed8, u8"Orbit height" }, // 軌道の高さ
	{ 0x1413ca948, u8"Territory ID" }, // 縄張りID
	{ 0x1413caac0, u8"Play spawn effect" }, // 出現エフェクトを再生する
	{ 0x1413eddc0, u8"For sonic set" }, // ソニックセット用
	{ 0x1413edec8, u8"Search radius offset" }, // 索敵半径オフセット
	{ 0x1413ee000, u8"Rush speed offset" }, // 突進速度オフセット
	{ 0x1413ee0c8, u8"Meandering special attack speed offset" }, // 蛇行特攻速度オフセット
	{ 0x1413ee1f0, u8"Moving distance [forward]" }, // 移動可能距離[前方]
	{ 0x1413ee340, u8"Travelable distance [backward]" }, // 移動可能距離[後方]
	{ 0x1413cab88, u8"Parameters for Sonic Set" }, // ソニックセット用パラメータ
	{ 0x1413cb480, u8"Is it for Sonic set?" }, // ソニックセット用か
	{ 0x1413cb5a8, u8"Enemy search range for Sonic set" }, // 索敵範囲 ソニックセット用
	{ 0x1413cbfc8, u8"Guard starting distance [for Sonic set]" }, // ガード開始距離[ソニックセット用]
	{ 0x1413cc0c8, u8"Boomerang starting distance [for Sonic set]" }, // ブーメラン開始距離[ソニックセット用]
	{ 0x1413cc2c8, u8"Is it for trial?" }, // 試練用か
	{ 0x1413f5d30, u8"Appearance type" }, // 出現タイプ
	{ 0x1413f5f38, u8"Point-to-point start offset" }, // 二点間開始オフセット
	{ 0x1413f6050, u8"Seconds it takes to appear" }, // 出現に掛かる秒
	{ 0x1413f61b8, u8"EasingTypePath" }, // easingTypePath
	{ 0x1413f62b8, u8"Do you land after appearing?" }, // 出現後に着陸するか
	{ 0x1413f63b0, u8"Appearance effect playback" }, // 出現エフェクト再生
	{ 0x1413cc9a0, u8"Appearance parameters" }, // 出現パラメータ
	{ 0x1413fa028, u8"2-point movement offset" }, // ２点間移動オフセット
	{ 0x1413fa378, u8"Enemy search angle" }, // 索敵角度
	{ 0x1413fa3f8, u8"Will it take off and land after being discovered?" }, // 発見後に離着陸するか
	{ 0x1413fa480, u8"Takeoff and landing point offset" }, // 離着陸地点オフセット
	{ 0x1413fa548, u8"Time taken for takeoff and landing" }, // 離着陸にかける時間
	{ 0x1413fa610, u8"LandEasingType" }, // landEasingType
	{ 0x1413fa6c0, u8"EyesightOffsetDirection" }, // eyesightOffsetDirection
	{ 0x1413fa770, u8"EyesightRotateAxis" }, // eyesightRotateAxis
	{ 0x1413fa800, u8"EyesightRotateAngle" }, // eyesightRotateAngle
	{ 0x1413cca80, u8"Enemy search parameters" }, // 索敵パラメータ
	{ 0x1413f87c0, u8"Weapon type" }, // 武器タイプ
	{ 0x1413f8890, u8"Face the player" }, // プレイヤーの方を向く
	{ 0x1413f8938, u8"Tracking angular velocity to player" }, // プレイヤーへの追尾角速度
	{ 0x1413f8a28, u8"Time until lost" }, // 見失うまでの時間
	{ 0x1413f6ae8, u8"Launch collision length" }, // 発射コリジョンの長さ
	{ 0x1413f6bd0, u8"Launch collision height" }, // 発射コリジョンの高さ
	{ 0x1413f6ce0, u8"Lateral angle of launch collision" }, // 発射コリジョンの横角度
	{ 0x1413f7000, u8"For time" }, // ため時間
	{ 0x1413f70f0, u8"Number of consecutive shots" }, // 連続発射数
	{ 0x1413f7170, u8"Continuous firing interval" }, // 連続発射間隔
	{ 0x1413f7258, u8"Cooling time" }, // 冷却時間
	{ 0x1413f73a8, u8"Opening of the fan when firing all at once (FV only)" }, // 一斉発射時の扇の開き(FVのみ)
	{ 0x1413f7468, u8"Number of shots when firing simultaneously (FV only)" }, // 一斉発射時の発射数(FVのみ)
	{ 0x1413f74d8, u8"Should I add correction in the direction of gravity?" }, // 重力方向の補正を加えるか
	{ 0x1413f8b30, u8"Beam parameters" }, // ビームパラメータ
	{ 0x1413f7a58, u8"Launch collision cone length" }, // 発射コリジョンのコーンの長さ
	{ 0x1413f7b08, u8"Launch collision cone angle" }, // 発射コリジョンのコーンの角度
	{ 0x1413f7cf0, u8"Tracking time" }, // 追尾時間
	{ 0x1413f7d98, u8"Time until explosion after tracking" }, // 追尾後に爆発するまでの時間
	{ 0x1413f7eb0, u8"Tracking rate" }, // 追尾率
	{ 0x1413f7fd0, u8"Time until tracking starts" }, // 追尾開始までの時間
	{ 0x1413f8c90, u8"Missile parameters" }, // ミサイルパラメータ
	{ 0x1413ccb58, u8"Attack parameters" }, // 攻撃パラメータ
	{ 0x1413f9690, u8"Revival type" }, // 復活タイプ
	{ 0x1413f9830, u8"Resurrection locator (for reviving specified position)" }, // 復活ロケータ(指定位置復活用)
	{ 0x1413ccc08, u8"Resurrection parameters" }, // 復活パラメータ
	{ 0x1413cd568, u8"Do you take a discovery motion?" }, // 発見モーションを取るか
	{ 0x1413fccb8, u8"Target position" }, // 目標位置
	{ 0x1413fcdd8, u8"Initial position (ratio)" }, // 初期位置(比率)
	{ 0x1413fcf58, u8"Stopping time at the edge" }, // 端での停止時間
	{ 0x1413fcfe0, u8"Close range in combat" }, // 戦闘時接近距離
	{ 0x1413b8c68, u8"Move between two points" }, // 2点間移動
	{ 0x1413fbaf8, u8"Wandering interval" }, // キョロキョロ間隔
	{ 0x1413fbf20, u8"Enemy search radius" }, // 索敵半径
	{ 0x1413fc040, u8"Enemy search angle (deg)" }, // 索敵角度(deg)
	{ 0x1413fc150, u8"Enemy search range height" }, // 索敵範囲高さ
	{ 0x1413fc2a8, u8"Search range offset (Y axis)" }, // 索敵範囲オフセット(Y軸)
	{ 0x1413d8848, u8"Attack interval" }, // 攻撃間隔
	{ 0x1413fc510, u8"Impact distance" }, // 着弾距離
	{ 0x1413fc5b0, u8"Orbit height" }, // 軌道高さ
	{ 0x1413ce498, u8"Destination" }, // 移動先
	{ 0x1413ce650, u8"Will it be revived? (Yes=true)" }, // 復活するか（する=true）
	{ 0x1413ce728, u8"Yarn start point offset" }, // 糸の開始地点オフセット
	{ 0x1413cea28, u8"Rotation angle per second (when moving between two points)" }, // 毎秒回転角度（2点間移動時）
	{ 0x1413fdbd0, u8"Descending distance" }, // 下降距離
	{ 0x1413fdc28, u8"Descent speed" }, // 降下速度
	{ 0x1413fdcb8, u8"Climb speed" }, // 上昇速度
	{ 0x1413fdd80, u8"Flashing speed when rising" }, // 上昇時の明滅速度
	{ 0x1413fdea0, u8"Blinking speed after rising" }, // 上昇後の明滅速度
	{ 0x1413fdf50, u8"Time until self-destruction" }, // 自爆までの時間
	{ 0x1413fdff8, u8"Lebagacha time required for escape" }, // 脱出に必要なレバガチャ時間
	{ 0x1413ced50, u8"SV parameters" }, // SV用パラメータ
	{ 0x1413fd308, u8"Rotation angle per second (when showing butt)" }, // 毎秒回転角度（尻見せ時）
	{ 0x1413fd3d8, u8"Bullet height" }, // 弾の高さ
	{ 0x1413fd510, u8"Bullet direction magnification" }, // 弾の方向倍率
	{ 0x1413e2f18, u8"Bullet arrival time" }, // 弾の到達時間
	{ 0x1413fd718, u8"Attack interval (wait time after attack)" }, // 攻撃間隔（攻撃後待機時間）
	{ 0x1413cee58, u8"Parameters for FV" }, // FV用パラメータ
	{ 0x1413944c0, u8"Moving destination local coordinates" }, // 移動目的地ローカル座標
	{ 0x141394690, u8"Waiting time at starting point" }, // 始点での待ち時間
	{ 0x141394730, u8"Waiting time at the end" }, // 終点での待ち時間
	{ 0x1413cf960, u8"Point-to-point movement parameters" }, // 二点間移動パラメータ
	{ 0x141394000, u8"Movement method" }, // 移動方式
	{ 0x1413941d0, u8"Distance from player" }, // プレイヤーとの距離
	{ 0x1413cfa28, u8"Path movement parameters" }, // パス移動パラメータ
	{ 0x1413fe7c8, u8"Occurrence position local offset" }, // 出現位置ローカルオフセット
	{ 0x1413fe868, u8"Movement speed when appearing" }, // 登場時移動速度
	{ 0x1413fe9d0, u8"Resurrection interval" }, // 復活間隔
	{ 0x1413cfb58, u8"Appearance setting" }, // 登場設定
	{ 0x1413cfbe0, u8"Continuous machine gun attacks" }, // マシンガン連続攻撃するか
	{ 0x1413b9598, u8"Distance" }, // distance
	{ 0x1413d0a60, u8"Tolerance" }, // tolerance
	{ 0x1413d0b58, u8"Initial mode" }, // 初期モード
	{ 0x1413d0d88, u8"Do not change mode" }, // モード変化させない
	{ 0x1413d0ea8, u8"Do you want to make an appearance?" }, // 出現演出するか
	{ 0x1413d0f90, u8"Do you start from egg form?" }, // 卵形態からスタートするか
	{ 0x1413d1088, u8"Search distance offset" }, // 索敵距離オフセット
	{ 0x1413d1230, u8"Action probability [for Sonic set]" }, // 行動確率[ソニックセット用]
	{ 0x1413d1448, u8"Cooldown time by action [for Sonic set]" }, // 行動別クールタイム[ソニックセット用]
	{ 0x1413d1750, u8"Collisions only with HeightField" }, // HeightFieldのみと衝突
	{ 0x1413d17b0, u8"Wave attack prohibited [effective except Sonic set]" }, // Wave攻撃禁止[ソニックセット以外有効]
	{ 0x1413d22f8, u8"Alter ego object" }, // 分身オブジェクト
	{ 0x1413d23b0, u8"Camera volume to turn on when HA to clone" }, // 分身にHAした際にONにするカメラボリューム
	{ 0x1413d2490, u8"Alter ego survival time" }, // 分身の生存時間
	{ 0x1413d2820, u8"Limited to collisions with HeightField" }, // HeightFieldとの衝突に限定
	{ 0x1413d3108, u8"Will it be treated as a main unit?" }, // 本体扱いされるか
	{ 0x1413d3188, u8"Are you in the air?" }, // 空中にいるか
	{ 0x1413d3288, u8"Direction of flight" }, // 飛んでいく方向
	{ 0x1413a7040, u8"Escape distance in combat" }, // 戦闘時逃走距離
	{ 0x1413a6278, u8"Ray type" }, // 射線タイプ
	{ 0x1413a6310, u8"Enemy search range type" }, // 索敵範囲タイプ
	{ 0x1413a6478, u8"Additional enemy search angle (lower elevation angle) [°]" }, // 索敵追加角度(仰角下)[°]
	{ 0x1413a656c, u8"Number of bullets" }, // 弾数
	{ 0x1413a6658, u8"Diffusion width [°]" }, // 拡散幅[°]
	{ 0x1413d6078, u8"Chaos emerald drop object" }, // カオスエメラルドドロップオブジェクト
	{ 0x1413a87e0, u8"Path candidate 1" }, // パス候補1
	{ 0x1413a88b0, u8"Path candidate 2" }, // パス候補2
	{ 0x1413a89a8, u8"Path candidate 3" }, // パス候補3
	{ 0x1413a8b10, u8"Path candidate 4" }, // パス候補4
	{ 0x1413a8ce8, u8"Path candidate 5" }, // パス候補5
	{ 0x1413d6178, u8"Movement path candidate" }, // 移動パス候補
	{ 0x1413d6b50, u8"Combat distance offset" }, // 戦闘距離オフセット
	{ 0x1413d6c88, u8"Disable parry throw" }, // パリィスロー無効化
	{ 0x1413d6ea0, u8"Physical strength offset (Sonic set)" }, // 体力オフセット(ソニックセット)
	{ 0x1413d6f78, u8"Defense physical strength offset (Sonic set)" }, // 防御体力オフセット(ソニックセット)
	{ 0x1413d70a8, u8"Snipe bullet velocity offset (Sonic set)" }, // スナイプ弾の弾速オフセット(ソニックセット)
	{ 0x1413d71b0, u8"Distance to hide weaknesses (Sonic set)" }, // 弱点を隠す距離(ソニックセット)
	{ 0x1413d72c8, u8"Distance to expose weak point (Sonic set)" }, // 弱点を出す距離(ソニックセット)
	{ 0x1413d7c00, u8"Attack control with switch" }, // スイッチで攻撃制御
	{ 0x1413ad2d0, u8"Resurrection interval (s)" }, // 復活間隔(s)
	{ 0x1413ad710, u8"Shooting interval (s)" }, // 射撃間隔(s)
	{ 0x1413ad828, u8"Impact/attack judgment off" }, // 着弾・攻撃判定オフ
	{ 0x1413ad938, u8"Impact position" }, // 着弾位置
	{ 0x1413ada20, u8"Attack judgment radius (m)" }, // 攻撃判定半径(m)
	{ 0x1413adbd0, u8"Delay (s)" }, // 遅延(s)
	{ 0x1413d7d70, u8"Main gun settings" }, // 主砲設定
	{ 0x1413d7ec0, u8"HP" }, // HP
	{ 0x1413d8080, u8"Actions upon death" }, // 死亡時のアクション
	{ 0x1413d8fa0, u8"HP type" }, // HPタイプ
	{ 0x1413d90e0, u8"Is it possible to target?" }, // ターゲット可能か
	{ 0x1413d9210, u8"Is it possible to defeat even if the target is not possible?" }, // ターゲット不可でも撃破は可能か
	{ 0x1413d94e0, u8"Is it possible to parry?" }, // パリィ可能か
	{ 0x1413d9698, u8"Special move possible" }, // 必殺技可能
	{ 0x1413d9830, u8"Do you want to display the life gauge?" }, // ライフゲージ表示するか
	{ 0x1413d9970, u8"Forced focus" }, // 強制フォーカス
	{ 0x1413d9a80, u8"Time before attack begins" }, // 攻撃開始前の時間
	{ 0x1413d9ba0, u8"Attack interval time" }, // 攻撃間隔の時間
	{ 0x1413d9cf8, u8"Attack stop time after attack hit" }, // 攻撃ヒット後の攻撃停止時間
	{ 0x1413d9e40, u8"Shot direction" }, // ショット方向
	{ 0x1413d9f28, u8"Bullet movement speed" }, // 弾の移動速度
	{ 0x1413da000, u8"Bullet size" }, // 弾のサイズ
	{ 0x1413da108, u8"Respawn wait time" }, // リスポーン待ち時間
	{ 0x1413da230, u8"Respawn movement distance" }, // リスポーン移動距離
	{ 0x1413da370, u8"Respawn travel time" }, // リスポーン移動時間
	{ 0x1413db8e8, u8"Rotation speed switching time" }, // 回転速度切り替え時間
	{ 0x1413dbba8, u8"Is it for a quest?" }, // クエスト用か
	{ 0x1413b1730, u8"Number of stages" }, // 段数
	{ 0x1413b1888, u8"Number of pieces per row" }, // 1列当たりの個数
	{ 0x1413b19a0, u8"Distance between bullets" }, // 弾同士の距離
	{ 0x1413b1b10, u8"Range of fluctuation" }, // 揺らぎの範囲
	{ 0x1413b1c50, u8"Should I fly from the end?" }, // 端から順に飛ばすか
	{ 0x1413b1348, u8"ObjType" }, // objType
	{ 0x1413b1440, u8"OffSet" }, // offSet
	{ 0x1413b1da8, u8"Information per column" }, // 列毎の情報
	{ 0x1413dc6c0, u8"Barrage information" }, // 弾幕情報
	{ 0x1413b24a8, u8"Jump height" }, // ジャンプ高さ
	{ 0x1413afb98, u8"Approach detection distance" }, // 接近検知距離
	{ 0x1413b2700, u8"Firing distance" }, // 発射距離
	{ 0x1413b2878, u8"Blow-off speed" }, // 吹っ飛び速度
	{ 0x1413b2970, u8"Blow-off direction offset" }, // 吹っ飛び方向オフセット
	{ 0x1413dc848, u8"Sonic set information" }, // ソニックセット情報
	{ 0x1413dd4b0, u8"Management class ID" }, // 管理クラスID
	{ 0x1413ddc20, u8"Effect generation offset" }, // エフェクト生成オフセット
	{ 0x1413bb9a0, u8"Mode" }, // モード
	{ 0x1413bba40, u8"Number of summons" }, // 召喚数
	{ 0x1413bbb38, u8"Level random increase amount" }, // レベルランダム増加量
	{ 0x1413de4b0, u8"Summoning details settings list" }, // 召喚詳細設定リスト
	{ 0x1413de5b8, u8"Remember if event has been triggered" }, // イベント起動済みか記憶する
	{ 0x1413de650, u8"Appearance production" }, // 出現演出
	{ 0x1413de8d8, u8"Placement pattern" }, // 配置パターン
	{ 0x1413dea18, u8"Placement range shape" }, // 配置範囲形状
	{ 0x1413deb58, u8"Placement range size" }, // 配置範囲サイズ
	{ 0x1413deef8, u8"Should it be grounded to the ground?" }, // 地面に接地させるか
	{ 0x1413df1f0, u8"Use oneself as territory" }, // 自身を縄張りとして使用する
	{ 0x1413df348, u8"Range out is also treated as death." }, // レンジアウトも死亡として扱う
	{ 0x1413e02f0, u8"Territory shape" }, // 縄張り形状
	{ 0x1413e04b0, u8"Wandering path name [not implemented]" }, // 徘徊パス名[未実装]
	{ 0x1413e0660, u8"Wandering area size" }, // 徘徊エリアサイズ
	{ 0x1413e07a0, u8"Battle area size" }, // 戦闘エリアサイズ
	{ 0x1413e08a8, u8"Wandering area offset" }, // 徘徊エリアオフセット
	{ 0x1413e0a00, u8"Combat area offset" }, // 戦闘エリアオフセット
	{ 0x1413e0c28, u8"Treat as unsafe zone" }, // 非安全地帯として扱う
	{ 0x1413df480, u8"Territory information" }, // 縄張り情報
	{ 0x1413ad7c8, u8"Is gravity effective from the time of setting?" }, // セット時から重力有効か
	{ 0x1413ad8a0, u8"Is it a stationary gravitational field?" }, // 静止重力場か
	{ 0x1413aa924, u8"X" }, // X
	{ 0x1413aaae8, u8"Y" }, // Y
	{ 0x1413aac2c, u8"Z" }, // Z
	{ 0x1413aba30, u8"Inner Radius" }, // InnerRadius
	{ 0x1413ac230, u8"Surface of the earth" }, // 地表
	{ 0x1413ac368, u8"Split" }, // 分割
	{ 0x1413ad278, u8"Tolerance" }, // Tolerance
	{ 0x1413bf638, u8"Post-execution state" }, // 実行後状態
	{ 0x1413bf728, u8"Placed on the wall" }, // 壁に配置
	{ 0x1413e2f98, u8"Is it in the opposite direction?" }, // 逆方向か
	{ 0x1413e34c8, u8"Exit hole object" }, // 出口穴オブジェクト
	{ 0x1413e4168, u8"Item" }, // アイテム
	{ 0x1413e4320, u8"Experience points per piece" }, // 1ピースあたりの経験値
	{ 0x1413e4a30, u8"Exit camera" }, // 出口カメラ
	{ 0x1413e4b18, u8"Camera start interpolation time" }, // カメラ開始補間時間
	{ 0x1413e4c60, u8"Camera start interpolation algorithm" }, // カメラ開始補間アルゴリズム
	{ 0x1413e4dc0, u8"Camera end interpolation time" }, // カメラ終了補間時間
	{ 0x1413e4f60, u8"Camera end interpolation algorithm" }, // カメラ終了補間アルゴリズム
	{ 0x1413e8688, u8"Dash type" }, // ダッシュタイプ
	{ 0x1413e88e0, u8"Dash speed" }, // ダッシュ速度
	{ 0x1413e8aa0, u8"Angle adjustment" }, // 角度調整
	{ 0x1413e8c28, u8"Terrain check [height adjusted to ground when enabled]" }, // 地形チェック[有効時は地面に高さ調整]
	{ 0x1413e8cf0, u8"Adjust posture to the slope of the ground when terrain check is enabled." }, // 地形チェック有効時地面の傾斜に姿勢を合わせる
	{ 0x1413e8de8, u8"Position specification during injection" }, // 射出時の位置指定
	{ 0x1413e8ef8, u8"Height adjustment when specifying position" }, // 位置指定時の高さ調整
	{ 0x1413e97d8, u8"Death effect position object" }, // 死亡演出位置オブジェクト
	{ 0x1413e98a8, u8"Wait for generation save flag" }, // 生成用セーブフラグを待つ
	{ 0x1413e99e0, u8"Is it for a tutorial?" }, // チュートリアル用か
	{ 0x1413e9e08, u8"Dropped portal shards" }, // ドロップするポータルのかけら
	{ 0x1413e9f78, u8"Portal fragment dropped from left foot" }, // 左足からドロップするポータルのかけら
	{ 0x1413ea018, u8"Portal fragment dropped from right foot" }, // 右足からドロップするポータルのかけら
	{ 0x1413eacf8, u8"Use enemy search range settings when not in battle" }, // 非戦闘時の索敵範囲設定を使う
	{ 0x1413eae30, u8"Enemy search range when not in combat" }, // 非戦闘時の索敵範囲
	{ 0x1413eb280, u8"Will it attack outside the combat range?" }, // 戦闘範囲外も攻撃するか
	{ 0x1413eb2e8, u8"Should we adopt the parameters for the third edition?" }, // 第3弾用のパラメータを採用するか
	{ 0x1413eb3d0, u8"Turning movement speed on/off" }, // 移動高速化のON/OFF
	{ 0x1413eb470, u8"Acceleration rate when increasing speed (normal 1.0)" }, // 高速化時の加速割合(通常1.0)
	{ 0x1413d4318, u8"Number of shards to activate" }, // 起動する欠片の数
	{ 0x1413d4480, u8"Additional seconds of delay after the shards are depleted" }, // 欠片が減ってから更に遅延する秒数
	{ 0x1413ebe68, u8"Launch trigger" }, // 起動トリガー
	{ 0x1413ec000, u8"Type of bullet" }, // 弾の種類
	{ 0x1413ec0d8, u8"Source offset" }, // 移動元オフセット
	{ 0x1413ec278, u8"Target cursor maximum display distance" }, // ターゲットカーソルの最大表示距離
	{ 0x1413cf3e0, u8"Injection direction" }, // 射出方向
	{ 0x1413ec330, u8"Parameters for springs" }, // スプリング用パラメータ
	{ 0x1413ecb38, u8"Occurrence distance" }, // 発生距離
	{ 0x1413ecbd8, u8"Number of bullets generated at once" }, // 一度に生成する弾の数
	{ 0x1413ecc80, u8"Regeneration time" }, // 再生成時間
	{ 0x1413ecd70, u8"Number of times to regenerate" }, // 再生成する回数
	{ 0x1413ed2f0, u8"Number of lasers" }, // レーザーの本数
	{ 0x1413ed3e8, u8"Muzzle offset" }, // 銃口オフセット
	{ 0x1413ed4b8, u8"Length" }, // 長さ
	{ 0x1413ed540, u8"Movement type" }, // 移動種別
	{ 0x1413ed638, u8"Time taken for one round" }, // 1周にかかる時間
	{ 0x1413edb68, u8"Basic information" }, // 基本情報
	{ 0x1413edc88, u8"Tilt angle" }, // 傾き角度
	{ 0x1413ee6f8, u8"Object to generate next" }, // 次に生成するオブジェクト
	{ 0x1413d2fa8, u8"Count per piece" }, // 一つあたりのカウント
	{ 0x1413d3080, u8"Timer decrease" }, // タイマー減少
	{ 0x1413eeb00, u8"Timer information" }, // タイマー情報
	{ 0x1413eebe8, u8"Number of rails 2or3" }, // レールの数2or3
	{ 0x1413eed10, u8"Debug area radius" }, // デバッグエリア半径
	{ 0x1413ef740, u8"Grind rail layer ID" }, // グラインドレールレイヤーID
	{ 0x1413ef8f0, u8"Prohibit transition to rail state" }, // レールStateへの遷移を禁止に
	{ 0x1413f04a8, u8"Behavior mode" }, // 挙動モード
	{ 0x1413f0598, u8"Number of torsos" }, // 胴体の数
	{ 0x1413f0740, u8"Ring position" }, // リングの位置
	{ 0x1413f0a90, u8"Appearance production" }, // 登場演出
	{ 0x1413f0e68, u8"Additional stomp standby time [first time]" }, // 追加ストンプ待機時間[初回]
	{ 0x1413f0fc8, u8"Additional stomp wait time [every time]" }, // 追加ストンプ待機時間[毎回]
	{ 0x1413f1160, u8"Search distance camera offset" }, // 索敵距離カメラ外オフセット
	{ 0x1413f1320, u8"Should I show my thorns from the beginning?" }, // 最初からトゲを出すか
	{ 0x1413f13e0, u8"Enable visual occlusion judgment" }, // 視界の遮蔽判定を有効
	{ 0x1413ba6f0, u8"End position" }, // 終了位置
	{ 0x1413f1fc0, u8"Loop/spiral position information" }, // ループ・スパイラル位置情報
	{ 0x1413f2858, u8"[For verification] Launch direction correction rotation amount" }, // [検証用]発射方向補正回転量
	{ 0x1413f2df8, u8"Time taken to appear (seconds)" }, // 出現にかかる時間(秒)
	{ 0x1413f2f80, u8"Offset when hidden" }, // 隠れているときのオフセット
	{ 0x1413f35d8, u8"Move path name" }, // 移動パス名
	{ 0x1413f3680, u8"Velocity upper bound using path interpolation" }, // パス補間を使用する速度上限
	{ 0x1413f7164, u8"Attribute" }, // 属性
	{ 0x14139f7d8, u8"Pylon" }, // パイロン
	{ 0x1413fb498, u8"Bomb" }, // 爆弾
	{ 0x1413a6050, u8"[pylon] input end calculation merge offset" }, // [パイロン]入力終了計算マージオフセット
	{ 0x1413a6180, u8"[Missile] Input end calculation merge offset" }, // [ミサイル]入力終了計算マージオフセット
	{ 0x1413f3720, u8"Path attribute settings" }, // パス属性設定
	{ 0x1413a6f40, u8"Applicable type" }, // 適用タイプ
	{ 0x1413a6fd8, u8"Path position" }, // パス位置
	{ 0x1413a0610, u8"Meandering movement probability" }, // 蛇行移動確率
	{ 0x1413a0730, u8"Jump [high] probability" }, // ジャンプ[高]確率
	{ 0x1413a0810, u8"Jump [low] probability" }, // ジャンプ[低]確率
	{ 0x1413a0908, u8"Pylon probability" }, // パイロン確率
	{ 0x1413a09e0, u8"Bomb probability" }, // 爆弾確率
	{ 0x1413a70f8, u8"Action probability" }, // アクション確率
	{ 0x1413f38c8, u8"Action lottery" }, // アクション抽選
	{ 0x1413a5518, u8"Apply action" }, // 適用アクション
	{ 0x1413f3b20, u8"No jumping out area" }, // 飛び出し禁止エリア
	{ 0x1413f45d8, u8"Distance (specify interval), number of placements (specify number)" }, // 距離(間隔指定)、配置数(個数指定)
	{ 0x1413f4730, u8"Initial display number" }, // 初期表示個数
	{ 0x1413f47f8, u8"Camera collision that turns on only during operation" }, // 動作中のみONにするカメラコリジョン
	{ 0x1413a7560, u8"Camera that is turned on only during operation" }, // 動作中のみONにするカメラ
	{ 0x1413f48a0, u8"Root" }, // ルート
	{ 0x1413f5ba0, u8"Size adjustment per occurrence" }, // 発生当たりの大きさ調整
	{ 0x1413f5fb0, u8"Minimum scaling scale" }, // 拡縮最小スケール
	{ 0x1413f6120, u8"Maximum scaling scale" }, // 拡縮最大スケール
	{ 0x1413f6258, u8"Scaling cycle" }, // 拡縮周期
	{ 0x1413f64a8, u8"Path movement period" }, // パス移動周期
	{ 0x1413f6620, u8"Should I wrap the path movement?" }, // パス移動を折り返すか
	{ 0x1413f6770, u8"Movement scaling period delay" }, // 移動拡縮周期遅延
	{ 0x1413f68c0, u8"Collision thickness magnification" }, // コリジョンの厚さ倍率
	{ 0x1413a7ca0, u8"Beam firing point radius" }, // ビーム射撃地点半径
	{ 0x1413a7e38, u8"Initial phase" }, // 初期位相
	{ 0x1413f6fe0, u8"Number of laser row sides" }, // レーザー列辺の数
	{ 0x1413f8730, u8"Diving play position" }, // ダイビング遊び位置
	{ 0x1413f8808, u8"Shield durability count (normal)" }, // シールド耐久回数(通常)
	{ 0x1413f88d8, u8"Diving start height (normal)" }, // ダイビング開始高さ(通常)
	{ 0x1413f89c0, u8"Diving set name (normal)" }, // ダイビングセット名(通常)
	{ 0x1413f8ab8, u8"Effective layer when diving (normal)" }, // ダイビング時有効レイヤー(通常)
	{ 0x1413f8bb8, u8"Shield durability count (anger)" }, // シールド耐久回数(怒り)
	{ 0x1413f8d60, u8"Diving start height (anger)" }, // ダイビング開始高さ(怒り)
	{ 0x1413f8e10, u8"Diving set name (anger)" }, // ダイビングセット名(怒り)
	{ 0x1413f8f50, u8"Effective layer when diving (anger)" }, // ダイビング時有効レイヤー(怒り)
	{ 0x1413f9240, u8"Prohibit transition to air state" }, // 空中Stateへの遷移を禁止に
	{ 0x1413fa058, u8"Connection destination pole" }, // 接続先ポール
	{ 0x1413fb5c0, u8"Behavior type" }, // 挙動タイプ
	{ 0x1413fb6a0, u8"Survival seconds" }, // 生存秒数
	{ 0x1413fb730, u8"Deceleration start time" }, // 減速開始時間
	{ 0x1413fb838, u8"Deceleration" }, // 減速度
	{ 0x1413fc3c8, u8"Pillar object" }, // 柱オブジェクト
	{ 0x1413fcad8, u8"Foundation object" }, // 土台オブジェクト
	{ 0x1413fcbf8, u8"Attack target point for mines" }, // 攻撃目標地点 機雷用
	{ 0x1413fcc88, u8"Attack target point for ring hole" }, // 攻撃目標地点　リングホール用
	{ 0x1413fcd00, u8"Attack target point independent type" }, // 攻撃目標地点 自立型
	{ 0x1413fd9b8, u8"Type of attack" }, // 攻撃の種類
	{ 0x1413fda78, u8"Time until attack starts" }, // 攻撃開始までの時間
	{ 0x1413fdb38, u8"Time to repeat attack: 0 so it doesn't repeat" }, // 攻撃を繰り返す時間 0で繰り返さない
	{ 0x1413fde40, u8"Cooperation object" }, // 連携オブジェクト
	{ 0x1413fded8, u8"Radius to force alignment" }, // 強制位置合わせする半径
	{ 0x1413fdf90, u8"Display distance" }, // 表示距離
	{ 0x1413fef10, u8"Number of rails to be injected" }, // 射出されるレールの番号
	{ 0x1413e6c28, u8"Firing angle at injection (°)" }, // 射出時発射角(°)
	{ 0x141394050, u8"Movement speed minimum" }, // 移動速度最小
	{ 0x141394140, u8"Maximum movement speed" }, // 移動速度最大
	{ 0x141394248, u8"Travel speed minimum path length" }, // 移動速度最小パス長
	{ 0x1413942f8, u8"Movement speed maximum path length" }, // 移動速度最大パス長
	{ 0x141393848, u8"Injection information" }, // 射出情報
	{ 0x1413f4b88, u8"Type of function" }, // 機能の種類
	{ 0x1413f4c28, u8"Speed in high speed state" }, // 高速状態時の速度
	{ 0x1413f4df8, u8"Speed in low speed state" }, // 低速状態時の速度
	{ 0x1413f4f60, u8"Distance to change to slow speed" }, // 低速に変わる距離
	{ 0x1413f50b8, u8"Maximum speed during no-approach" }, // 接近禁止中の最高速度
	{ 0x1413f51a8, u8"No-access distance for relative speed" }, // 相対速度に入る接近禁止距離
	{ 0x1413f5280, u8"Speed within a certain distance and when Sonic is jumping" }, // 一定距離内且つソニックがジャンプ中時の速度
	{ 0x1413f5348, u8"Distance to jump and slow down" }, // ジャンプして速度が落ちる距離
	{ 0x141393f20, u8"Speed control" }, // 速度制御
	{ 0x1413f44c8, u8"Motion to use" }, // 使用するモーション
	{ 0x1413f45b8, u8"Motion pattern" }, // モーションパターン
	{ 0x141393ff0, u8"Motion" }, // モーション
	{ 0x1413948a8, u8"Pass for wandering" }, // 徘徊用のパス
	{ 0x1413fe150, u8"Path name from bombing to migration" }, // 爆撃から移行までのパス名
	{ 0x1413fe240, u8"Approach position" }, // 接近用位置
	{ 0x1413fe310, u8"Distance of the path that switches from bombing to migration route" }, // 爆撃から移行ルートに切り替わるパスの距離
	{ 0x1413fe3e8, u8"Path distance to end bombardment" }, // 爆撃を終了させるパスの距離
	{ 0x1413fe4a8, u8"Path name for grind during migration" }, // 移行時のグラインド用パス名
	{ 0x1413949c8, u8"Migration route information" }, // 移行ルート情報
	{ 0x141394a88, u8"ID for the escape pass used" }, // 使用される逃亡用のパス用のID
	{ 0x141394bb0, u8"Player warp position after victory" }, // 勝利後のプレイヤーのワープ位置
	{ 0x141394c78, u8"Body position during victory event" }, // 勝利イベント時の本体位置
	{ 0x141394d60, u8"Set camera information" }, // セットカメラ情報
	{ 0x141394e58, u8"Information on rotation speed change position" }, // 回転速度変更位置の情報
	{ 0x141394f10, u8"Section information of wandering path that cuts interpolation" }, // 補間を切る徘徊パスの区間情報
	{ 0x141394fe0, u8"Non-bombing path section information" }, // 爆撃をしないパスの区間情報
	{ 0x1413950a0, u8"Practice space information" }, // 練習空間情報
	{ 0x141395820, u8"Small boss ID that appears when the event is activated" }, // イベント起動で出現させる小ボスID
	{ 0x141396220, u8"Emerging from flower buds" }, // 花の芽から出現
	{ 0x1413966f8, u8"Center of throwing range" }, // 投げ入れ範囲中心
	{ 0x1413967e8, u8"Throwing range radius" }, // 投げ入れ範囲半径
	{ 0x1413968f0, u8"Final fishing position" }, // 最終釣り上げ位置
	{ 0x1413969e0, u8"Place for fish (large)" }, // 魚(大)の置き場所
	{ 0x14139d580, u8"Sonic" }, // ソニック
	{ 0x141397350, u8"Big" }, // ビッグ
	{ 0x141397490, u8"Conversation camera" }, // 会話カメラ
	{ 0x141397578, u8"Point selection camera" }, // ポイント選択カメラ
	{ 0x141397608, u8"Over shoulder camera" }, // 肩越しカメラ
	{ 0x1413976c0, u8"Sonic's fishing location" }, // ソニックの釣り位置
	{ 0x141397738, u8"Center of fish swimming range" }, // 魚泳ぎ範囲中心
	{ 0x1413977d8, u8"Fish swimming range radius" }, // 魚泳ぎ範囲半径
	{ 0x1413978d0, u8"Float camera transition time" }, // ウキカメラ遷移時間
	{ 0x1413979f0, u8"Float camera offset" }, // ウキカメラオフセット
	{ 0x141397ae8, u8"Battle camera offset" }, // バトルカメラオフセット
	{ 0x141397c10, u8"Fishing camera offset" }, // 釣り上げカメラオフセット
	{ 0x141397d18, u8"Fishing camera transition time" }, // 釣りカメラ遷移時間
	{ 0x141397d88, u8"Cyber noise effect position" }, // 電脳ノイズエフェクトの位置
	{ 0x141397e20, u8"Rotation of cyber noise effect" }, // 電脳ノイズエフェクトの回転
	{ 0x141397ec8, u8"Grilled meat sonic" }, // 肉焼きソニック
	{ 0x141397fe0, u8"Meat grilling machine" }, // 肉焼き機
	{ 0x1413980b8, u8"Meat grill camera" }, // 肉焼きカメラ
	{ 0x141399428, u8"DiEvent name" }, // DiEvent名
	{ 0x141399560, u8"Delete after conversation ends" }, // 会話終了後に消す
	{ 0x1413996e8, u8"Use default path when playing DiEvent" }, // DiEvent再生時デフォルトパスを使う
	{ 0x1413997c0, u8"Objects to hide during event playback" }, // イベント再生時に非表示にするオブジェクト
	{ 0x141399978, u8"Use sequence items" }, // シーケンスアイテムを使用する
	{ 0x141399a70, u8"Required number of sequence items" }, // シーケンスアイテム必要数
	{ 0x141399c00, u8"Script file name to play when sequence items are insufficient" }, // シーケンスアイテム不足時に再生するスクリプトファイル名
	{ 0x141399d68, u8"Sequence item injection DiEvent name" }, // シーケンスアイテム注入DiEvent名
	{ 0x141399e70, u8"Characters you can talk to" }, // 会話可能なキャラクター
	{ 0x141399fa0, u8"Use additional collider" }, // 追加のコライダーを使用する
	{ 0x14139a2f8, u8"Radius (cylinder, sphere, capsule)" }, // 半径（シリンダー、球、カプセル）
	{ 0x14139a470, u8"Height (cylinder, capsule)" }, // 高さ（シリンダー、カプセル）
	{ 0x14139d730, u8"Is this the second part of the Emmy collection?" }, // エミー後編ココ集め用か
	{ 0x14139e2a8, u8"Limit standby motion" }, // 待機モーションを制限
	{ 0x1413ef408, u8"Quest" }, // クエスト
	{ 0x14139ee78, u8"Seed of Power DiEvent name" }, // チカラのタネDiEvent名
	{ 0x14139ef48, u8"Mamori no Seed DiEvent name" }, // マモリのタネDiEvent名
	{ 0x14139f028, u8"Both seeds DiEvent name" }, // 両方のタネDiEvent名
	{ 0x14139f4b0, u8"Number of pop-outs" }, // 飛び出すココの数
	{ 0x14139f5b8, u8"Minimum radius of scattering" }, // 飛び散る最小半径
	{ 0x14139f680, u8"Maximum radius of scattering" }, // 飛び散る最大半径
	{ 0x14139ff00, u8"Character" }, // キャラクター
	{ 0x1413a0a50, u8"Trial type" }, // 試練タイプ
	{ 0x1413a0b38, u8"Condition type" }, // 状態タイプ
	{ 0x1413a0c28, u8"Object to reference location at the end of the trial" }, // 試練終了時に位置参照するオブジェクト
	{ 0x1413a1b40, u8"Distance to tracking location" }, // 追跡場所への距離
	{ 0x1413a1c00, u8"Distance to tracking location when homing cursor appears" }, // ホーミングカーソルが出ている時の追跡場所への距離
	{ 0x1413a1d50, u8"Coco's floating height setting" }, // ココの浮遊する高さ設定
	{ 0x1413a3338, u8"Target character" }, // 対象キャラ
	{ 0x1413a3458, u8"Amount of skill pieces acquired" }, // 獲得スキルピース量
	{ 0x1413a46c0, u8"Don't make it work" }, // 機能させない
	{ 0x1413a5c28, u8"Intended use" }, // 使用目的
	{ 0x1413a5ca0, u8"Model scale" }, // モデルスケール
	{ 0x1413aa3a8, u8"Number of vertical boxes" }, // 縦の箱の数
	{ 0x1413aa500, u8"Number of horizontal boxes" }, // 横の箱の数
	{ 0x1413aa620, u8"Number of boxes in the back" }, // 奥の箱の数
	{ 0x1413aa700, u8"Number of drops" }, // ドロップ数
	{ 0x1413acdc4, u8"Depth" }, // 奥行
	{ 0x1413aafd8, u8"Goal parameters" }, // ゴールのパラメータ
	{ 0x1413ab318, u8"Stomp wait time" }, // ストンプ待機時間
	{ 0x1413ab990, u8"Maximum number of bees" }, // ハチ最大数
	{ 0x1413aba98, u8"Bee spacing" }, // ハチ間隔
	{ 0x1413aca80, u8"Movement conditions" }, // 移動条件
	{ 0x1413ad510, u8"Enemy ID list" }, // 敵IDリスト
	{ 0x1413ad650, u8"Boss ID" }, // ボスID
	{ 0x1413b99a8, u8"Core enemy remaining HP" }, // コア敵残りHP
	{ 0x1413b9a18, u8"Core enemy remaining HP [%]" }, // コア敵残りHP[%]
	{ 0x1413b9ae0, u8"Time limit" }, // 時間制限
	{ 0x1413b9b78, u8"Time until transition [s]" }, // 遷移までの時間[s]
	{ 0x1413b9c30, u8"All enemies are annihilated" }, // ザコ敵全滅
	{ 0x1413ae048, u8"WAVE transition settings" }, // WAVE遷移設定
	{ 0x1413ae128, u8"Starts after dialogue plays" }, // セリフ再生後に開始
	{ 0x1413ae1b8, u8"Line name" }, // セリフ名
	{ 0x1413aea08, u8"HP settings" }, // HP設定
	{ 0x1413aef78, u8"Enemy generation management ID list" }, // 敵生成管理IDリスト
	{ 0x1413af558, u8"ID of the environmental sound to play" }, // 再生する環境音のID
	{ 0x1413af6f0, u8"Base position" }, // 基点位置
	{ 0x1413b00d0, u8"AISAC name" }, // AISAC名
	{ 0x1413b0178, u8"AISAC value" }, // AISAC値
	{ 0x1413b0268, u8"Fade in time" }, // フェードイン時間
	{ 0x1413b0350, u8"Fade out time" }, // フェードアウト時間
	{ 0x1413b12b8, u8"BGM number" }, // BGM番号
	{ 0x1413b1708, u8"Issue a stop request" }, // 停止リクエストを発行する
	{ 0x1413b1800, u8"Disappears once you leave" }, // １度離れたら消滅
	{ 0x1413b1900, u8"Do not lower the request until the background music changes" }, // BGMが変わるまでリクエストを下げない
	{ 0x1413b1a38, u8"Default BGM number" }, // デフォルトBGM番号
	{ 0x1413b30e0, u8"Playback method" }, // 再生方式
	{ 0x1413b3170, u8"Shift time (interval)" }, // ずらす時間(インターバル)
	{ 0x1413b3278, u8"Playback interval (interval)" }, // 再生間隔(インターバル)
	{ 0x1413b3450, u8"Hearing range" }, // 聞こえる範囲
	{ 0x1413b3550, u8"Range without attenuation" }, // 減衰しない範囲
	{ 0x1413b40f0, u8"Fade time" }, // フェード時間
	{ 0x1413b4ed0, u8"3D playback" }, // 3D再生
	{ 0x1413b4fe0, u8"3D playback position" }, // 3D再生位置
	{ 0x1413b4a48, u8"Specify coordinates" }, // 座標指定
	{ 0x1413bf9c0, u8"Hearing distance" }, // 聞こえる距離
	{ 0x1413bfa80, u8"Distance without attenuation" }, // 減衰しない距離
	{ 0x1413b5e40, u8"Path search division number" }, // パス検索分割数
	{ 0x1413b6690, u8"Cue name" }, // cue名
	{ 0x1413b6900, u8"3D playback?" }, // 3D再生か
	{ 0x1413b73e0, u8"Relative position" }, // 相対位置
	{ 0x1413b74e0, u8"Appearance straight forward threshold time" }, // 出現直進閾値時間
	{ 0x1413b7638, u8"Minimum tracking time" }, // 最小追従時間
	{ 0x1413b7750, u8"Maximum tracking time" }, // 最大追従時間
	{ 0x1413b7e40, u8"Number of placements" }, // 配置数
	{ 0x1413b7f28, u8"Appearance area" }, // 出現領域
	{ 0x1413b8050, u8"Area per body" }, // 1体あたり領域
	{ 0x1413b8418, u8"Rotation value phase" }, // 回転値位相
	{ 0x1413b8688, u8"Fixed flock direction" }, // 群れの向き固定
	{ 0x1413b8798, u8"Movement limit distance [0.0 is no limit]" }, // 移動限界距離[0.0は制限なし]
	{ 0x1413b9710, u8"Object to hide when playing spawn event" }, // 出現イベント再生時に非表示にするオブジェクト
	{ 0x1413ba5f8, u8"Starting position" }, // スタート位置
	{ 0x1413ba7f0, u8"End fixation point" }, // 終了注視点
	{ 0x1413ba95c, u8"Boss" }, // ボス
	{ 0x1413baa60, u8"Rock" }, // 岩
	{ 0x1413bb0e8, u8"Is it destructible?" }, // 破壊可能か
	{ 0x1413bba70, u8"Object to hide when playing clear event" }, // クリアイベント再生時に非表示にするオブジェクト
	{ 0x1413bbd10, u8"Reference position of gaze point" }, // 注視点の基準位置
	{ 0x1413bbe80, u8"Radius of movement of gaze point" }, // 注視点の移動半径
	{ 0x1413bbfb0, u8"Player movement radius" }, // プレイヤーの移動半径
	{ 0x1413bc100, u8"Y coordinate Offset of point of interest" }, // 注視点のY座標Offset
	{ 0x1413bcd90, u8"Impenetrable collision width" }, // 侵入不可コリジョン幅
	{ 0x1413bce78, u8"Impenetrable collision height" }, // 侵入不可コリジョン高さ
	{ 0x1413bd028, u8"Impenetrable collision depth" }, // 侵入不可コリジョン奥行き
	{ 0x1413bd220, u8"Impenetrable collision offset" }, // 侵入不可コリジョンオフセット
	{ 0x1413bd3c8, u8"Effect display volume (before cracking)" }, // エフェクト表示ボリューム(割れる前)
	{ 0x1413bd5b8, u8"Effect display volume (after cracking)" }, // エフェクト表示ボリューム(割れた後)
	{ 0x1413be338, u8"Number of bars" }, // バーの数
	{ 0x1413be5a8, u8"Bounce force" }, // バウンド力
	{ 0x1413bee78, u8"Action type" }, // アクション種別
	{ 0x1413bf770, u8"Approach radius" }, // 接近半径
	{ 0x1413bf858, u8"Dragon" }, // ドラゴン
	{ 0x1413bfec0, u8"Y-offset" }, // Y-オフセット
	{ 0x1413bff58, u8"Camera distance" }, // カメラ距離
	{ 0x1413c01d0, u8"Roll angle" }, // ロール角
	{ 0x1413c0318, u8"Focus setting" }, // フォーカス設定
	{ 0x1413c03f8, u8"Focus ratio" }, // フォーカス比率
	{ 0x1413c04e0, u8"Focus limit angle" }, // フォーカス制限角度
	{ 0x1413c1ff8, u8"Position object" }, // 位置オブジェクト
	{ 0x1413c2098, u8"Point of interest object" }, // 注視点オブジェクト
	{ 0x1413c0bf8, u8"Start event" }, // 開始イベント
	{ 0x1413c0cf8, u8"End event" }, // 終了イベント
	{ 0x1413c0e70, u8"Retry event" }, // リトライイベント
	{ 0x1413c12e8, u8"Auto run path name" }, // オートランパス名
	{ 0x1413c14e8, u8"Speed when boosting" }, // ブースト時のスピード
	{ 0x1413c15d0, u8"Speed at maximum boost" }, // 最大ブースト時のスピード
	{ 0x1413c16a0, u8"Movable width" }, // 移動可能幅
	{ 0x1413a1a80, u8"View direction sensitivity (during boost)" }, // 視点方向の感度(ブースト中)
	{ 0x1413c09b0, u8"Camera interpolation time" }, // カメラの補間時間
	{ 0x1413c1748, u8"Camera: Introduction" }, // カメラ:導入
	{ 0x1413c1800, u8"Camera: Main" }, // カメラ:メイン
	{ 0x1413c1930, u8"Set camera collision" }, // セットカメラコリジョン
	{ 0x1413c0fa0, u8"Autorun" }, // オートラン
	{ 0x1413c3ae0, u8"Altitude offset" }, // 高度のオフセット
	{ 0x1413c3c58, u8"Initial position path offset" }, // 初期位置のパスオフセット
	{ 0x1413c3e10, u8"Approach position path offset" }, // アプローチ位置のパスオフセット
	{ 0x1413c3fb8, u8"Approach speed" }, // アプローチ速度
	{ 0x1413c4148, u8"Attack speed" }, // アタック速度
	{ 0x1413c4340, u8"Aim time" }, // エイム時間
	{ 0x1413ee9c0, u8"Explosion radius" }, // 爆発の半径
	{ 0x1413c2bd0, u8"Explosion damage effective time" }, // 爆発のダメージ有効時間
	{ 0x1413c2dd0, u8"Number of rings of damage" }, // ダメージのリング数
	{ 0x1413c2fa0, u8"Minimum time interval for liveliness" }, // にぎやかしの最小時間間隔
	{ 0x1413c30f8, u8"Maximum time interval for liveliness" }, // にぎやかしの最大時間間隔
	{ 0x1413c3220, u8"Lively preset selection maximum value" }, // にぎやかしのプリセット選択最大値
	{ 0x1413c3330, u8"Noisy damage effective" }, // にぎやかしのダメージ有効
	{ 0x1413c10e0, u8"Missile common" }, // ミサイル共通
	{ 0x1413c1680, u8"Dragon Incident" }, // DragonIncident
	{ 0x1413c1b18, u8"Dragon Incident body" }, // DragonIncident本体
	{ 0x1413c1be8, u8"Height as reference for movement" }, // 移動基準となる高さ
	{ 0x1413c1d98, u8"Distance used as movement reference" }, // 移動基準となる距離
	{ 0x1413c2878, u8"Initial offset" }, // 初期オフセット
	{ 0x1413c2950, u8"Intermediate offset" }, // 中間オフセット
	{ 0x1413c2a50, u8"End offset" }, // 終端オフセット
	{ 0x1413c2cb0, u8"Aim at the player" }, // プレイヤーを狙う
	{ 0x1413c3518, u8"Missile launch prohibited" }, // ミサイル発射禁止
	{ 0x1413c3650, u8"Chase forced to continue" }, // チェイス強制継続
	{ 0x1413c37f8, u8"Motion switching" }, // モーション切り替え
	{ 0x1413c3e68, u8"Is it for an event?" }, // イベント用か
	{ 0x1413c53f0, u8"Number of vertical grids" }, // 縦のグリッド数
	{ 0x1413c55a8, u8"Number of horizontal grids" }, // 横のグリッド数
	{ 0x1413c5750, u8"Grid size" }, // グリッドの大きさ
	{ 0x1413c58f0, u8"Compensable grid" }, // 補正可能グリッド
	{ 0x1413c5b00, u8"Catch speed multiplier" }, // キャッチ速度倍率
	{ 0x1413c5b98, u8"Catch time" }, // キャッチ時間
	{ 0x1413c4aa0, u8"Vertical" }, // 縦
	{ 0x1413c4c88, u8"Beside" }, // 横
	{ 0x1413c5cb0, u8"Crane initial position" }, // クレーン初期位置
	{ 0x1413c5e10, u8"Jewel" }, // 宝玉
	{ 0x1413c51d4, u8"Position" }, // 位置
	{ 0x1413c5ec8, u8"Jewel pedestal" }, // 宝玉台座
	{ 0x1413c7a90, u8"Effect color" }, // エフェクト色
	{ 0x1413c7b90, u8"Range specification method" }, // 範囲指定方法
	{ 0x1413c7cb0, u8"Range size" }, // 範囲大きさ
	{ 0x1413c7e84, u8"Amount" }, // 量
	{ 0x1413c7ff0, u8"Effect interval" }, // エフェクト間隔
	{ 0x1413c81b8, u8"Duration" }, // 持続時間
	{ 0x1413c9978, u8"Bumper mark" }, // バンパーマーク
	{ 0x1413c9a38, u8"Earned score" }, // 獲得スコア
	{ 0x1413c9af8, u8"Power to play" }, // 弾く力
	{ 0x1413cac70, u8"Angle when flipping up" }, // 跳ね上げ時の角度
	{ 0x1413cb658, u8"Starting reflection angle" }, // 開始時の反射角度
	{ 0x1413cb760, u8"Score that gives energy" }, // エネルギーを付与するスコア
	{ 0x1413cbfa0, u8"Number of failures to lower difficulty" }, // 難易度を下げる失敗回数
	{ 0x1413cc210, u8"Speed decay rate" }, // 速度減衰率
	{ 0x1413cc370, u8"Minimum decay rate" }, // 最低減衰速度
	{ 0x1413cc450, u8"Maximum slope that can be climbed" }, // 登れる最大傾斜
	{ 0x1413cc568, u8"Distance from support plane considered to be supported" }, // 支えられているとみなす支持平面との距離
	{ 0x1413cc638, u8"Slide speed decay rate" }, // スライド速度減衰率
	{ 0x1413cc708, u8"Slide speed attenuation lower limit speed" }, // スライド速度減衰下限速度
	{ 0x1413cc820, u8"Limit number of pitches" }, // 球数制限数
	{ 0x1413cca28, u8"Score that gives ball energy" }, // ボールエネルギーを付与するスコア
	{ 0x1413ccb18, u8"Shutter opening score" }, // シャッターが開くスコア
	{ 0x1413ccbd8, u8"Rollover bonus score" }, // ロールオーバーのボーナススコア
	{ 0x1413cccb0, u8"Number of rings obtained to double the score" }, // スコア２倍になるリング取得数
	{ 0x1413ccd88, u8"Number of rings acquired that quadruples the score" }, // スコア４倍になるリング取得数
	{ 0x1413ccf18, u8"Number of rings acquired that increases the score by 8 times" }, // スコア８倍になるリング取得数
	{ 0x1413cd030, u8"Number of rings acquired that increases the score by 16 times" }, // スコア16倍になるリング取得数
	{ 0x1413cd128, u8"Number of rings acquired that increases the score by 32 times" }, // スコア32倍になるリング取得数
	{ 0x1413cd268, u8"Number of rings acquired that increases the score by 64 times" }, // スコア64倍になるリング取得数
	{ 0x1413cd400, u8"Number of rings acquired that increases the score by 128 times" }, // スコア128倍になるリング取得数
	{ 0x1413cd538, u8"Number of rings acquired that increases the score by 256 times" }, // スコア256倍になるリング取得数
	{ 0x1413cd648, u8"Time from game start to ball launch" }, // ゲーム開始からボール発射までの時間
	{ 0x1413cd770, u8"Time from ball loss to ball relaunch" }, // ボールロストからボール再発射までの時間
	{ 0x1413cd888, u8"Time from success to clear judgment" }, // 成功からクリア判定までの時間
	{ 0x1413cd9c0, u8"Time from failure/forced termination to failure determination" }, // 失敗・強制終了から失敗判定までの時間
	{ 0x1413cdaa0, u8"Red ring revival time [remaining ball]" }, // レッドリング復活時間[残ボール]
	{ 0x1413cdb60, u8"Red ring score" }, // レッドリングのスコア
	{ 0x1413cdc78, u8"Number of rings added when acquiring red rings" }, // レッドリング取得時のリング加算数
	{ 0x1413cde70, u8"Number of bumper reflections that cause fluctuations" }, // 揺らぎを発生させるバンパー反射回数
	{ 0x1413cdff0, u8"Fluctuation correction angle" }, // 揺らぎの補正角度
	{ 0x1413ce190, u8"Child objects after clearing" }, // クリア後の子オブジェクト群
	{ 0x1413ce1f0, u8"Red ring placement node (node placement only)" }, // レッドリングの配置ノード(ノード配置のみ)
	{ 0x1413c57f0, u8"Candidate or not?" }, // 候補かどうか
	{ 0x1413ce320, u8"Candidate patterns for red ring placement nodes" }, // レッドリングの配置ノードの候補パターン
	{ 0x1413cfa10, u8"Maximum movable angle" }, // 最大可動角度
	{ 0x1413cfad8, u8"Maximum angle arrival time" }, // 最大角度到達時間
	{ 0x1413cfbb8, u8"Tip firing angle" }, // 先端発射角度
	{ 0x1413cfca0, u8"Rotary axis firing angle" }, // 回転軸発射角度
	{ 0x1413cfe00, u8"Tip firing rate" }, // 先端発射速度
	{ 0x1413cfef0, u8"Rotary axis firing speed" }, // 回転軸発射速度
	{ 0x1413d00d0, u8"Minimum tip reflex speed" }, // 先端反射最低速度
	{ 0x1413cc220, u8"FV start position object" }, // FV開始位置オブジェクト
	{ 0x1413cc310, u8"FV shield movement path" }, // (FV盾移動パス
	{ 0x1413cc408, u8"CameraVolume" }, // CameraVolume
	{ 0x1413cc540, u8"Shield movement speed" }, // 盾移動速度
	{ 0x1413d09a8, u8"FV playable" }, // FVプレイアブル
	{ 0x1413cb220, u8"Distance from path end to start end event" }, // 終了イベントを開始するパス終端からの距離
	{ 0x1413d0cb0, u8"Flyer" }, // Flyer
	{ 0x1413ccd58, u8"First path relative position" }, // 最初のパス相対位置
	{ 0x1413ccee8, u8"Approach path relative position" }, // アプローチのパス相対位置
	{ 0x1413cd0f0, u8"Attack path relative position" }, // アタックのパス相対位置
	{ 0x1413cd358, u8"Attack preparation time" }, // アタック準備時間
	{ 0x1413cd488, u8"Attack time" }, // アタック時間
	{ 0x1413cd5d0, u8"Evacuation path relative position" }, // 退避のパス相対位置
	{ 0x1413cd6e0, u8"Retraction speed" }, // 退避速度
	{ 0x1413cd808, u8"Y-axis rotation angle" }, // Y軸回転角度
	{ 0x1413cd928, u8"Separation strength when boosting" }, // ブースト時の離れる強さ
	{ 0x1413d0e30, u8"Shield assault" }, // 盾襲撃
	{ 0x1413f25a8, u8"Recording interval (every F at 0)" }, // 記録間隔(0で毎F)
	{ 0x1413f26c8, u8"Number of items held" }, // 保持個数
	{ 0x1413e24f0, u8"Starting distance" }, // 開始距離
	{ 0x1413e25c8, u8"End distance" }, // 終了距離
	{ 0x1413f2b14, u8"Abyss" }, // 奈落
	{ 0x1413979cc, u8"Trajectory" }, // 軌跡
	{ 0x1413e0c70, u8"Initial velocity [m/s]" }, // 初速度[m/s]
	{ 0x1413fde88, u8"Deceleration [m/s^2]" }, // 減速度[m/s^2]
	{ 0x1413fdf38, u8"Minimum speed [m/s]" }, // 最低速度[m/s]
	{ 0x1413fdfd8, u8"Target unavailable time" }, // ターゲット不可時間
	{ 0x1413c6210, u8"Time to reorganize" }, // 仕切り直しになる時間
	{ 0x1413fed70, u8"Shooting motion speed" }, // 射撃モーション速度
	{ 0x1413c9a08, u8"Appearance height [m]" }, // 出現高さ[m]
	{ 0x1413e0e10, u8"Maximum speed [m/s]" }, // 最高速度[m/s]
	{ 0x1413e0f50, u8"Acceleration [m/s^2]" }, // 加速度[m/s^2]
	{ 0x1413e10b0, u8"Duration [s]" }, // 持続時間[s]
	{ 0x1413c94a8, u8"Bullet individual settings" }, // 弾個別設定
	{ 0x1413c8d68, u8"Appearance table" }, // 出現テーブル
	{ 0x1413fec80, u8"Storage time [s]" }, // 溜め時間[s]
	{ 0x1413dbcf0, u8"Shooting" }, // 射撃
	{ 0x1413c6f88, u8"Rightward movement restriction range" }, // 右方向の移動制限範囲
	{ 0x1413c7138, u8"Leftward movement restriction range" }, // 左方向の移動制限範囲
	{ 0x1413c72a0, u8"Start position of scroll up" }, // 上スクロールの開始位置
	{ 0x1413c7458, u8"Up scroll maximum distance" }, // 上スクロール最大距離
	{ 0x1413c7650, u8"Start position of scroll down" }, // 下スクロールの開始位置
	{ 0x1413c7968, u8"Is there a limit on path travel distance?" }, // パスの移動距離制限の有無
	{ 0x1413c7a58, u8"Minimum path movement limit" }, // パスの移動制限最小値
	{ 0x1413c7ba8, u8"Maximum path movement limit" }, // パスの移動制限最大値
	{ 0x1413c7d48, u8"Path smoothing" }, // パスのスムージング
	{ 0x1413ed9d0, u8"Normal running speed" }, // 通常時の走る速さ
	{ 0x1413edb28, u8"Running speed when boosting" }, // ブースト時の走る速さ
	{ 0x1413edc08, u8"Running speed when power boosted" }, // パワーブースト時の走る速さ
	{ 0x141397c78, u8"State of fugitive" }, // 逃亡状態
	{ 0x1413d1740, u8"Config" }, // コンフィグ
	{ 0x1413d2040, u8"Night Incident" }, // NightIncident
	{ 0x1413d3008, u8"Collision Y offset" }, // コリジョンのY方向オフセット
	{ 0x1413d30e0, u8"Relighting time after reflection (seconds)" }, // 反射後の再点灯時間(秒)
	{ 0x1413d4818, u8"ID for bonus determination" }, // ボーナス判定用ID
	{ 0x1413d4930, u8"In/out lane settings" }, // イン、アウトレーン設定
	{ 0x1413d5918, u8"Grid spacing" }, // グリッド間隔
	{ 0x1413d6198, u8"Material" }, // マテリアル
	{ 0x1413d6a9c, u8"SPEED" }, // SPEED
	{ 0x1413d6b4c, u8"Lr" }, // lr
	{ 0x1413d6ed0, u8"Weight" }, // weight
	{ 0x1413b4ccc, u8"Size" }, // size
	{ 0x1413e80b8, u8"Radius" }, // radius
	{ 0x1413d7298, u8"Force" }, // force
	{ 0x1413e5288, u8"MaxSpeed" }, // maxSpeed
	{ 0x1413d7480, u8"EnabledTime" }, // enabledTime
	{ 0x1413d7548, u8"DisabledTime" }, // disabledTime
	{ 0x1413d3b68, u8"Pattern" }, // パターン
	{ 0x1413bb0f8, u8"Time" }, // 時間
	{ 0x1413c46b0, u8"Move" }, // move
	{ 0x1413d8610, u8"Attack starting distance" }, // 攻撃開始距離
	{ 0x1413d86f8, u8"Attack end distance" }, // 攻撃終了距離
	{ 0x1413d8790, u8"Attack type" }, // 攻撃タイプ
	{ 0x1413d9f08, u8"Vibration generation cycle time" }, // 振動発生周期時間
	{ 0x1413da710, u8"FarLookAt" }, // farLookAt
	{ 0x1413da7f8, u8"NearLookAt" }, // nearLookAt
	{ 0x1413da868, u8"FarOffset" }, // farOffset
	{ 0x1413da9f0, u8"NearOffset" }, // nearOffset
	{ 0x1413dad98, u8"OffsetDistance" }, // offsetDistance
	{ 0x1413db3a0, u8"Camera type" }, // カメラタイプ
	{ 0x1413dbaf0, u8"Reset type" }, // リセットタイプ
	{ 0x1413dc740, u8"Support plane distance" }, // サポート平面距離
	{ 0x1413dc828, u8"App version CharacterControl" }, // アプリ版CharacterControl
	{ 0x1413dc910, u8"Moving system" }, // 移動システム
	{ 0x1413dca28, u8"Physical query mode" }, // 物理クエリモード
	{ 0x1413dcad0, u8"Use collision cache" }, // コリジョンキャッシュを利用
	{ 0x1413dd368, u8"Shape" }, // シェイプ
	{ 0x1413dd648, u8"Layer to collide with" }, // 衝突対象のレイヤー
	{ 0x1413dd770, u8"Trigger" }, // トリガー
	{ 0x1413dd858, u8"Output collision information to log" }, // 衝突情報をログに出力する
	{ 0x1413dd938, u8"Output collision information to screen" }, // 衝突情報を画面に出力する
	{ 0x1413de7b0, u8"Own layer" }, // 自身のレイヤー
	{ 0x1413df888, u8"Local offset of display position" }, // 表示位置のローカルオフセット
	{ 0x1413dfa10, u8"Do you use compass display?" }, // コンパス表示を使うか
	{ 0x1413dff28, u8"Projection speed" }, // 投射速度
	{ 0x1413e0268, u8"Number of circumference divisions" }, // 円周分割数
	{ 0x1413e0370, u8"First white fluid color" }, // 先頭 白流体色
	{ 0x1413e0510, u8"Ending white fluid color" }, // 末尾 白流体色
	{ 0x1413e06a8, u8"White fluid brightness" }, // 白流体輝度
	{ 0x1413d61a8, u8"Flowing speed" }, // 流れる速度
	{ 0x1413d6270, u8"Twist" }, // ひねり
	{ 0x1413d6370, u8"Helical rotation speed" }, // 螺旋回転速度
	{ 0x1413d6588, u8"Fluctuation offset" }, // 揺らぎオフセット
	{ 0x1413d6670, u8"Tube scale" }, // チューブのスケール
	{ 0x1413e07f8, u8"M_whiteFlow" }, // m_whiteFlow
	{ 0x1413d4768, u8"Black fluid radius" }, // 黒流体の半径
	{ 0x1413d4868, u8"Black fluid flowing speed" }, // 黒流体の流れる速度
	{ 0x1413d4a10, u8"Helical rotation speed of black fluid" }, // 黒流体の螺旋回転速度
	{ 0x1413d4b30, u8"Black fluid twist" }, // 黒流体ひねり
	{ 0x1413d4c10, u8"Rotating offset black" }, // 回転オフセット黒
	{ 0x1413d4d30, u8"Fluctuation offset black" }, // 揺らぎオフセット黒
	{ 0x1413d4e10, u8"Alpha threshold" }, // アルファ閾値
	{ 0x1413d4f58, u8"Black fluid noise 1 scale" }, // 黒流体ノイズ1スケール
	{ 0x1413d5098, u8"Black fluid noise 1 speed" }, // 黒流体ノイズ1速度
	{ 0x1413d51d8, u8"Black fluid noise 2 scale" }, // 黒流体ノイズ2スケール
	{ 0x1413d5300, u8"Black fluid noise 2 speed" }, // 黒流体ノイズ2速度
	{ 0x1413d53e8, u8"Black fluid line scale" }, // 黒流体ラインスケール
	{ 0x1413d54a0, u8"Black fluid tube scale" }, // 黒流体チューブのスケール
	{ 0x1413d5588, u8"Black fluid color" }, // 黒流体色
	{ 0x1413e0898, u8"M_blackFlow" }, // m_blackFlow
	{ 0x1413e10c0, u8"Is collision enabled?" }, // コリジョンが有効か
	{ 0x1413e13a8, u8"Cyloop radius" }, // Cyloop半径
	{ 0x1413e1470, u8"Target position upward offset" }, // ターゲット位置上方向オフセット
	{ 0x1413d85a0, u8"Defensive reaction" }, // 防御リアクション
	{ 0x1413d86a8, u8"Defense status" }, // 防御ステータス
	{ 0x1413d8730, u8"Maximum faint value" }, // 最大気絶値
	{ 0x1413cbec8, u8"Fainting time" }, // 気絶時間
	{ 0x1413d8898, u8"Maximum stagger value" }, // 最大よろめき値
	{ 0x1413f9ea8, u8"Stagger time" }, // よろめき時間
	{ 0x1413e1cd0, u8"Defense" }, // defence
	{ 0x1413d7080, u8"Attack waiting time" }, // 攻撃待機時間
	{ 0x1413d7330, u8"Do you allow special moves after parrying?" }, // パリィ後の必殺技を許可するか
	{ 0x1413d7408, u8"Speed of giving" }, // 与える速度
	{ 0x1413d74e0, u8"Parry method" }, // パリィ方式
	{ 0x1413d75e8, u8"Do you want to play a special camera when parrying?" }, // パリィ時に特殊カメラ再生するか
	{ 0x1413d7700, u8"Death practice" }, // 死亡時の練習
	{ 0x1413a7c40, u8"Attack" }, // attack
	{ 0x1413f3f8c, u8"Weight" }, // 重さ
	{ 0x1413f81c0, u8"Coefficient of friction" }, // 摩擦係数
	{ 0x1413d9868, u8"Plaque pressure coefficient" }, // 班緒圧係数
	{ 0x1413e2438, u8"Basic parameters" }, // 基本パラメータ
	{ 0x1413da280, u8"Contact position local" }, // 接触位置ローカル
	{ 0x1413da390, u8"Extrusion direction local" }, // 押し出す方向ローカル
	{ 0x1413da4a8, u8"Extrusion force (constant value part)" }, // 押し出す力(一定値部分)
	{ 0x1413e2528, u8"Extrusion speed" }, // 押し出す速度
	{ 0x1413d9dd8, u8"Explosion point local" }, // 爆発点ローカル
	{ 0x1413d9ef8, u8"Explosive power" }, // 爆発力
	{ 0x1413e25d8, u8"Diffusion rate" }, // 拡散速度
	{ 0x1413da8a0, u8"Maximum rotation angle of initial posture" }, // 初期姿勢の最大回転角
	{ 0x1413daa70, u8"Maximum angular velocity during blow-off (rad/s)" }, // 吹き飛び中の最大角速度(rad/s)
	{ 0x1413e2df8, u8"Number of vertical vertices" }, // 縦頂点数
	{ 0x1413e2f30, u8"Number of horizontal vertices" }, // 横頂点数
	{ 0x1413e3030, u8"Propagation speed" }, // 伝搬速度
	{ 0x1413e3170, u8"Reduction damper" }, // 減速ダンパー
	{ 0x1413e3308, u8"Movement amount on hit" }, // ヒット時の移動量
	{ 0x1413e3480, u8"Impact distance on hit" }, // ヒット時の影響距離
	{ 0x1413e35a8, u8"Vertex movement" }, // 頂点移動
	{ 0x1413e3698, u8"Transform movement" }, // Transform移動
	{ 0x1413e37b0, u8"Collision attributes/movement" }, // コリジョン属性/移動
	{ 0x1413e3908, u8"Collision Attribute/Parkour" }, // コリジョン属性/パルクール
	{ 0x1413e46a0, u8"Local offset of target display position" }, // 目標表示位置のローカルオフセット
	{ 0x1413e4788, u8"Crossing distance" }, // 交差距離
	{ 0x1413e4880, u8"Should I use object ID specification?" }, // オブジェクトID指定を使うか
	{ 0x1413e5e70, u8"Interpolation method 1" }, // 補間方法1
	{ 0x1413e5f00, u8"Calculation method 1" }, // 計算方法1
	{ 0x1413e6028, u8"Parameter variable 1" }, // 媒介変数1
	{ 0x1413e60c0, u8"Interpolation method 2" }, // 補間方法2
	{ 0x1413e6180, u8"Calculation method 2" }, // 計算方法2
	{ 0x1413e6288, u8"Parameter variable 2" }, // 媒介変数2
	{ 0x1413e6318, u8"Operation" }, // オペレーション
	{ 0x1413e63f8, u8"Preview time" }, // プレビュー時間
	{ 0x1413e71a0, u8"Preload usage" }, // 事前ロード使用
	{ 0x1413db9d8, u8"Route change flag" }, // ルート変更フラグ
	{ 0x1413dbaa8, u8"Don't end the event" }, // イベントを終了しない
	{ 0x1413dbb40, u8"How to treat players at the start" }, // 開始時のプレイヤーの扱い
	{ 0x1413dbbd8, u8"Camera interpolation time (s)" }, // カメラ補間時間(s)
	{ 0x1413dbca8, u8"Do you want to point the camera in a specific direction after finishing?" }, // 終了後に特定方向にカメラを向けるか
	{ 0x1413dbd48, u8"Point of interest after completion" }, // 終了後の注視点
	{ 0x1413e7270, u8"Playback parameters" }, // 再生パラメータ
	{ 0x1413e7770, u8"Automatic reversal" }, // 自動反転
	{ 0x1413e7850, u8"Forced reversal" }, // 強制反転
	{ 0x1413e8140, u8"Interpolation source parameter number" }, // 補間元パラメータ番号
	{ 0x1413e8278, u8"Interpolation destination parameter number" }, // 補間先パラメータ番号
	{ 0x1413e8558, u8"Interpolation method" }, // 補間方法
	{ 0x1413e94f8, u8"Drawing presets" }, // 描画プリセット
	{ 0x1413e95d0, u8"Geometry coordinate system" }, // ジオメトリ座標系
	{ 0x1413e9670, u8"Node coordinate system" }, // ノード座標系
	{ 0x1413e96b0, u8"Number of segments" }, // セグメント数
	{ 0x1413e9770, u8"Maximum segment length" }, // セグメント最大長
	{ 0x1413e9898, u8"Maximum length" }, // 最大長
	{ 0x1413ea4e8, u8"Scale value [for model size adjustment]" }, // スケール値[モデルサイズ調整用]
	{ 0x1413ea638, u8"Movement speed [seconds]" }, // 移動速度[秒速]
	{ 0x1413eaf48, u8"Change cycle time" }, // 変化サイクル時間
	{ 0x1413eb668, u8"Resource file" }, // リソースファイル
	{ 0x1413eba80, u8"Phantom size" }, // ファントムのサイズ
	{ 0x1413ebb38, u8"Do you have a phantom?" }, // ファントムを持つか
	{ 0x1413ec7e0, u8"Floating altitude" }, // 浮遊高度
	{ 0x1413ecbb8, u8"Destination object name" }, // 送信先オブジェクト名
	{ 0x1413ecc70, u8"Activity time" }, // 活動時間
	{ 0x1413ece00, u8"Start transition time" }, // 開始遷移時間
	{ 0x1413ece90, u8"End transition time" }, // 終了遷移時間
	{ 0x1413ed500, u8"Search radius" }, // 探索半径
	{ 0x1413ed620, u8"Search interval (distance)" }, // 探索間隔(距離)
	{ 0x1413ed740, u8"Search interval (angle)" }, // 探索間隔(角度)
	{ 0x1413ed7f0, u8"Lei length" }, // レイの長さ
	{ 0x1413ed898, u8"Make the player follow" }, // プレイヤーに追従させる
	{ 0x1413edde8, u8"Sender" }, // 送信相手
	{ 0x1413ee330, u8"Moving method" }, // 移動方法
	{ 0x1413ee3d8, u8"Number of injections" }, // 射出回数
	{ 0x1413ee490, u8"Injection interval" }, // 射出間隔
	{ 0x1413ee6a8, u8"Storage distance" }, // 格納距離
	{ 0x1413ee7f8, u8"Maximum movement speed" }, // 最大移動速度
	{ 0x1413ee8b0, u8"Minimum movement speed" }, // 最低移動速度
	{ 0x1413eead8, u8"Detonation time" }, // 起爆時間
	{ 0x1413eebd8, u8"Maximum angular velocity" }, // 最大角速度
	{ 0x1413eed00, u8"Minimum angular velocity" }, // 最低角速度
	{ 0x1413eedb8, u8"Missile field of view" }, // ミサイルの視界
	{ 0x1413ef890, u8"M_kind" }, // m_kind
	{ 0x1413efc30, u8"CameraResource0" }, // CameraResource0
	{ 0x1413efcc8, u8"CameraResource1" }, // CameraResource1
	{ 0x1413efd78, u8"CameraResource2" }, // CameraResource2
	{ 0x1413f01f8, u8"CameraResource" }, // CameraResource
	{ 0x1413f0670, u8"File name (under image/x64/raw/movie)" }, // ファイル名(image/x64/raw/movie 以下)
	{ 0x1413f07c8, u8"Audio track number" }, // オーディオトラック番号
	{ 0x1413f08c8, u8"Sub audio track number" }, // サブオーディオトラック番号
	{ 0x1413f0a00, u8"Extra audio track number" }, // エクストラオーディオトラック番号
	{ 0x1413dc440, u8"Impact range" }, // 影響範囲
	{ 0x1413f1460, u8"Guided path" }, // 誘導パス
	{ 0x1413f1510, u8"Starting point" }, // 開始地点
	{ 0x1413f1560, u8"Target point" }, // 目標地点
	{ 0x1413f15f0, u8"Start/end position type" }, // 開始・終了位置タイプ
	{ 0x1413ba668, u8"Destination" }, // destination
	{ 0x1413f1a10, u8"SearchPosition" }, // searchPosition
	{ 0x1413f1ae8, u8"Immediate" }, // immediate
	{ 0x1413f1e60, u8"Event file name" }, // イベントファイル名
	{ 0x1413f26d8, u8"Layer" }, // レイヤー
	{ 0x1413f27c0, u8"Sub: loop terrain left" }, // サブ：ループ地形左
	{ 0x1413f28b0, u8"Sub: loop terrain right" }, // サブ：ループ地形右
	{ 0x1413f29a8, u8"Sub: solid" }, // サブ：固体
	{ 0x1413f2ab8, u8"Sub: liquid" }, // サブ：液体
	{ 0x1413f2b48, u8"Sub: Through" }, // サブ：スルー
	{ 0x1413f2c70, u8"Cache usage" }, // キャッシュ利用
	{ 0x1413f35c8, u8"Display method" }, // 表示方法
	{ 0x1413f3640, u8"Text label" }, // テキストラベル
	{ 0x1413f36a8, u8"Audio cue" }, // 音声キュー
	{ 0x1413f3710, u8"Replacement tag" }, // 置換タグ
	{ 0x1413f3860, u8"Replacement label" }, // 置換ラベル
	{ 0x1413f3940, u8"Replacement texture" }, // 置換テクスチャ
	{ 0x1413f3a20, u8"Display time" }, // 表示時間
	{ 0x1413f3ae0, u8"Block or not" }, // ブロックするか
	{ 0x1413f3be8, u8"Start with no choices selected" }, // 選択肢を非選択で開始
	{ 0x1413f48f0, u8"Model name" }, // モデル名
	{ 0x1413f4a00, u8"Skeleton name" }, // スケルトン名
	{ 0x1413f4b60, u8"Animator name" }, // Animator名
	{ 0x1413f4c70, u8"Animation state name" }, // アニメーションステート名
	{ 0x1413f54d8, u8"Run before Simulate" }, // Simulate前実行
	{ 0x1413f56c0, u8"Search distance" }, // 探索距離
	{ 0x1413f57d0, u8"Number of ray divisions" }, // レイ分割数
	{ 0x1413f6988, u8"Mesh file name" }, // Meshファイル名
	{ 0x1413f6d48, u8"Local position" }, // ローカル位置
	{ 0x1413f6e18, u8"Local rotation" }, // ローカル回転
	{ 0x1413f6ed0, u8"Local scale" }, // ローカルスケール
	{ 0x1413f7100, u8"Texture" }, // 質感
	{ 0x1413f7278, u8"Move" }, // 動く
	{ 0x1413f803c, u8"Mass" }, // 質量
	{ 0x1413f8308, u8"Gravity scale [havok only]" }, // 重力スケール[havok only]
	{ 0x1413f83d8, u8"Coefficient of restitution" }, // 反発係数
	{ 0x1413f84a8, u8"Penetration permissible seismic intensity [havok only]" }, // 貫通許容震度[havok only]
	{ 0x1413f8618, u8"Angular velocity decay rate" }, // 角速度減衰率
	{ 0x1413f9540, u8"Width (diameter)" }, // 幅(直径)
	{ 0x1413f97d0, u8"Color: red" }, // 色：赤
	{ 0x1413f98c8, u8"Color: green" }, // 色：緑
	{ 0x1413f9a68, u8"Color: blue" }, // 色：青
	{ 0x1413f9bc8, u8"Color: α" }, // 色：α
	{ 0x1413fa8b0, u8"Filter" }, // フィルタ
	{ 0x1413faca0, u8"NearPlane" }, // nearPlane
	{ 0x1413fad28, u8"FarPlane" }, // farPlane
	{ 0x1413fae50, u8"LookAtDepth" }, // lookAtDepth
	{ 0x1413faf08, u8"SceneRange" }, // sceneRange
	{ 0x1413fb188, u8"Route position" }, // 経路位置
	{ 0x1413fb660, u8"Starting angle" }, // 開始角度
	{ 0x1413fb6d8, u8"Starting strength" }, // 開始強度
	{ 0x1413fb910, u8"Strength automatic calculation" }, // 強度自動計算
	{ 0x1413fc6e0, u8"Fix the top of the capsule to Y-UP" }, // カプセル上方向をY-UPに固定
	{ 0x1413fd048, u8"Directly specify movement information" }, // 移動情報の直指定
	{ 0x1413fd0c8, u8"Shape position" }, // シェイプ位置
	{ 0x1413fd150, u8"Cylinder top position" }, // シリンダートップ位置
	{ 0x1413fd230, u8"Amount of movement" }, // 移動量
	{ 0x1413fd2e8, u8"Number of repetitions" }, // 繰り返し数
	{ 0x1413fd458, u8"Count" }, // カウント
	{ 0x1413fde84, u8"Shape" }, // 形
	{ 0x1413fdf80, u8"Aspect" }, // アスペクト
	{ 0x1413fe060, u8"NearClip" }, // nearClip
	{ 0x1413fe0f0, u8"FarClip" }, // farClip
	{ 0x1413fe1a8, u8"Coverage judgment" }, // 遮蔽判定
	{ 0x1413fe7b8, u8"Radius/width" }, // 半径/幅
	{ 0x1413d6de8, u8"Display" }, // 表示
	{ 0x1413fea68, u8"Enemy?" }, // エネミーか
	{ 0x1413feb08, u8"Is it possible to focus?" }, // フォーカス可か
	{ 0x1413febb8, u8"Dies from damage" }, // ダメージで死ぬ
	{ 0x1413fec90, u8"Moving cycle" }, // 移動サイクル
	{ 0x1413fee68, u8"Idle time" }, // Idle時間
	{ 0x1413fef50, u8"Pause time" }, // Pause時間
	{ 0x141392870, u8"Disabled time" }, // Disabled時間
	{ 0x141392910, u8"NoReply time" }, // NoReply時間
	{ 0x141393420, u8"Does it move?" }, // 動くか
	{ 0x1413e5220, u8"MinSpeed" }, // minSpeed
	{ 0x141394550, u8"Albedo" }, // アルベド
	{ 0x141394650, u8"Reflectance" }, // リフレクタンス
	{ 0x141394718, u8"Smoothness" }, // スムースネス
	{ 0x141394ac0, u8"Texture name" }, // テクスチャ名
	{ 0x141394b98, u8"Vertex shader" }, // 頂点シェーダ
	{ 0x141394c58, u8"Pixel shader" }, // ピクセルシェーダ
	{ 0x1413955a8, u8"Color:R" }, // カラー:R
	{ 0x141395638, u8"Color: G" }, // カラー:G
	{ 0x1413956d0, u8"Color: B" }, // カラー:B
	{ 0x141395770, u8"Color: A" }, // カラー:A
	{ 0x141395950, u8"Shall I receive a shadow?" }, // 影を受けるか
	{ 0x141395a28, u8"Does it transform dynamically?" }, // 動的に変形するか
	{ 0x141396208, u8"Length of one vertical side" }, // 縦一辺の長さ
	{ 0x141396370, u8"Length of one side" }, // 横一辺の長さ
	{ 0x1413964c0, u8"Number of vertical grids" }, // 縦グリッド数
	{ 0x141396630, u8"Number of horizontal grids" }, // 横グリッド数
	{ 0x141396778, u8"Number of divisions in one vertical grid" }, // 縦1グリッドの分割数
	{ 0x141396870, u8"Number of divisions in one horizontal grid" }, // 横1グリッドの分割数
	{ 0x1413970b8, u8"Play vibration name" }, // 再生振動名
	{ 0x141397210, u8"Play type" }, // 再生タイプ
	{ 0x141397338, u8"Presence or absence of time decay" }, // 時間減衰の有無
	{ 0x141397478, u8"Presence of distance attenuation" }, // 距離減衰の有無
	{ 0x141397568, u8"Attenuation distance" }, // 減衰距離
	{ 0x141397640, u8"EaseOut time" }, // EaseOut時間
	{ 0x141397f60, u8"Animation name" }, // アニメーション名
	{ 0x141398058, u8"Is clipping enabled?" }, // クリッピング有効か
	{ 0x141398110, u8"Clipping height" }, // クリッピングの高さ
	{ 0x141398748, u8"Destination position offset" }, // 移動先位置オフセット
	{ 0x1413988a0, u8"Lock-on position" }, // ロックオン位置
	{ 0x1413989b8, u8"Recoil direction after attack" }, // 攻撃後跳ね返り方向
	{ 0x1413993d0, u8"Wind strength [m/s]" }, // 風の強さ[m/s]
	{ 0x141399528, u8"Effective range [m]" }, // 有効範囲[m]
	{ 0x1413996c0, u8"The rate at which the amount of movement is added to the speed" }, // 移動量を速度に加算する割合
	{ 0x141399830, u8"Wind flow type" }, // 風の流れタイプ
	{ 0x1413999d8, u8"Wind direction" }, // 風の向き
	{ 0x141399b40, u8"Fan flow angle [deg]" }, // 扇型流れ角度[deg]
	{ 0x141399cf8, u8"Path settings" }, // パス設定
	{ 0x141399e18, u8"Decay time (0 means no decay)" }, // 減衰時間(0は減衰なし)
	{ 0x141399f38, u8"Wind calculation range center point" }, // 風計算範囲中心点
	{ 0x14139a718, u8"Spring constant of distance variation" }, // 距離変動のバネ定数
	{ 0x14139a870, u8"Minimum azimuth offset" }, // 最小方位角オフセット
	{ 0x14139a9a0, u8"Maximum azimuth offset" }, // 最大方位角オフセット
	{ 0x14139aba0, u8"Distance for minimum azimuth offset" }, // 最小方位角オフセットになる距離
	{ 0x14139ad90, u8"Distance for maximum azimuth offset" }, // 最大方位角オフセットになる距離
	{ 0x14139afb0, u8"Spring constant of gaze point offset" }, // 注視点オフセットのバネ定数
	{ 0x14139b0b0, u8"Minimum upward offset of fixation point" }, // 注視点の最小上方オフセット
	{ 0x14139b160, u8"Maximum upward offset of the fixation point" }, // 注視点の最大上方オフセット
	{ 0x14139b1f0, u8"Forward offset of fixation point" }, // 注視点の前方オフセット
	{ 0x14139b278, u8"Tolerable distance for gaze point delay" }, // 注視点遅延の許容距離
	{ 0x14139b358, u8"Limit distance of gaze point delay" }, // 注視点遅延の限界距離
	{ 0x14139b458, u8"Spring constant of fixation point delay" }, // 注視点遅延のバネ定数
	{ 0x14139b5b8, u8"Time to maintain manual operation" }, // 手動操作を維持する時間
	{ 0x14139c0a8, u8"Minimum elevation angle" }, // 仰角最小値
	{ 0x14139c138, u8"Maximum elevation angle" }, // 仰角最大値
	{ 0x14139c790, u8"Minimum upward offset limit of fixation point" }, // 注視点の最小上方オフセットリミット
	{ 0x14139c8b0, u8"Maximum upward offset limit of point of interest" }, // 注視点の最大上方オフセットリミット
	{ 0x14139c9e0, u8"Distance at which the upward offset limit of the gaze point is the minimum value" }, // 注視点の上方オフセットリミットが最小値になる距離
	{ 0x14139cb20, u8"Distance at which the upward offset limit of the gaze point reaches its maximum value" }, // 注視点の上方オフセットリミットが最大値になる距離
	{ 0x14139cfa0, u8"Time to disable automatic operation after manual operation" }, // 手動操作後に自動操作を無効化する時間
	{ 0x14139d0c0, u8"Spring constant when tracking chain rotates" }, // 追跡チェーン回転時のバネ定数
	{ 0x14139d1a0, u8"Speed of shrinking the tracking chain" }, // 追跡チェーンを縮める速度
	{ 0x14139d280, u8"Number of segments allowed in a tracking chain" }, // 追跡チェーンの許容セグメント数
	{ 0x14139d3a0, u8"Minimum deceleration of inertia" }, // 慣性の最小減速度
	{ 0x14139d4d0, u8"Maximum angle to overshoot due to inertia" }, // 慣性によってオーバーシュートする最大角度
	{ 0x14139d5e8, u8"Spring constant when resetting the camera" }, // カメラリセット時のバネ定数
	{ 0x14139d6d8, u8"Boundary value of inner product considered to be downward" }, // 下向きとみなす内積の境界値
	{ 0x14139d790, u8"Normal target upward reflection rate" }, // 通常時のターゲット上方向反映割合
	{ 0x14139ffc0, u8"Acceleration corresponding to minimum distance" }, // 最小距離に対応する加速度
	{ 0x1413a00d8, u8"Acceleration corresponding to maximum distance" }, // 最大距離に対応する加速度
	{ 0x14139f990, u8"Minimum distance of gaze point delay" }, // 注視点遅延の最小限界距離
	{ 0x14139fa48, u8"Maximum limit distance of gaze point delay" }, // 注視点遅延の最大限界距離
	{ 0x14139e550, u8"Cyloop" }, // サイループ
	{ 0x14139e600, u8"Cyloop [Snipe]" }, // サイループ[スナイプ]
	{ 0x1413a1000, u8"[During Cyloop] Minimum elevation angle" }, // [サイループ中]仰角最小値
	{ 0x1413a1120, u8"[During Cyloop] Maximum elevation angle" }, // [サイループ中]仰角最大値
	{ 0x1413a1280, u8"[During Cyloop] Elevation offset" }, // [サイループ中]仰角オフセット
	{ 0x1413a16c0, u8"Panning delay spring constant" }, // パンニング遅延のバネ定数
	{ 0x1413a1790, u8"Close panning distance" }, // 近距離パンニング距離
	{ 0x1413a18d8, u8"Spring constant for close panning delay" }, // 近距離パンニング遅延のバネ定数
	{ 0x14139e660, u8"Lock-on [Standard]" }, // ロックオン[標準]
	{ 0x14139e728, u8"Lock on [Charger]" }, // ロックオン[Charger]
	{ 0x14139e798, u8"Lock on [Spider]" }, // ロックオン[Spider]
	{ 0x14139e830, u8"Lock on [Daruma]" }, // ロックオン[Daruma]
	{ 0x14139e870, u8"Lock on [Ninja]" }, // ロックオン[Ninja]
	{ 0x14139e920, u8"Lock on [Sumo]" }, // ロックオン[Sumo]
	{ 0x14139e9a0, u8"Wall running" }, // 壁走り
	{ 0x14139ea20, u8"Loop terrain" }, // ループ地形
	{ 0x14139eab0, u8"Battle" }, // バトル
	{ 0x1413a2710, u8"Minimum rotation speed during manual operation" }, // マニュアル操作時の最低回転速度
	{ 0x1413a2850, u8"Maximum rotation speed during manual operation" }, // マニュアル操作時の最高回転速度
	{ 0x1413a2998, u8"Distance scale [min]" }, // 距離スケール[最小]
	{ 0x1413a2b00, u8"Distance scale [maximum]" }, // 距離スケール[最大]
	{ 0x1413a2cb8, u8"Additional elevation offset [max]" }, // 追加の仰角オフセット[最大]
	{ 0x1413a2e78, u8"Gaze point upward offset scale [minimum]" }, // 注視点の上方オフセットスケール[最小]
	{ 0x1413a3028, u8"Gaze point upward offset scale [maximum]" }, // 注視点の上方オフセットスケール[最大]
	{ 0x1413a30f0, u8"Angle of view scale at lowest rate" }, // 最低レート時の画角スケール
	{ 0x1413a31c0, u8"Minimum angle of view" }, // 画角の最小限界値
	{ 0x14139eaf0, u8"Option" }, // オプション
	{ 0x1413a3df8, u8"[zRot direction] zRot" }, // [zRot方向]zRot
	{ 0x1413a3f78, u8"Frequency" }, // 周波数
	{ 0x1413a40a8, u8"Amplitude" }, // 振幅
	{ 0x1413a41d0, u8"Time decay rate" }, // 時間減衰率
	{ 0x1413a4298, u8"Does it attenuate over distance?" }, // 距離減衰するか？
	{ 0x1413a4b20, u8"Records" }, // records
	{ 0x1413a4fd8, u8"Starting record number" }, // 開始レコード番号
	{ 0x1413a5100, u8"Number of records" }, // レコード数
	{ 0x1413a5530, u8"Databases" }, // databases
	{ 0x1413a55c8, u8"Stage data" }, // ステージデータ
	{ 0x1413a6378, u8"Record number" }, // レコード番号
	{ 0x1413d3c08, u8"Damage name" }, // ダメージ名
	{ 0x1413d3db0, u8"Horizontal blow-off force" }, // 水平吹き飛び力
	{ 0x1413d3e78, u8"Vertical blow-off force" }, // 垂直吹き飛び力
	{ 0x1413d3f28, u8"Hitstop key name" }, // ヒットストップキー名
	{ 0x1413d3fc0, u8"Camera shake key name" }, // カメラシェイクキー名
	{ 0x1413d4070, u8"Practice space type" }, // 練習空間タイプ
	{ 0x1413a5784, u8"Data" }, // data
	{ 0x1413d4688, u8"Key name" }, // キー名
	{ 0x1413d4758, u8"Normal magnification" }, // 通常倍率
	{ 0x1413d4840, u8"Cyloop magnification" }, // サイループ倍率
	{ 0x1413d49c8, u8"Quick siroop magnification" }, // クイックサイループ倍率
	{ 0x1413d4b00, u8"Stun multiplier" }, // スタン倍率
	{ 0x1413d4bf8, u8"Fear multiplier" }, // 怯み倍率
	{ 0x1413a6578, u8"Raw meat time" }, // 生肉時間
	{ 0x1413a6678, u8"Undercooked meat time" }, // 生焼け肉時間
	{ 0x1413a6798, u8"Browned meat time" }, // こんがり肉時間
	{ 0x1413a6880, u8"Charred meat time" }, // 焦げ肉時間
	{ 0x1413a6918, u8"Preparation performance waiting time" }, // 準備演出待機時間
	{ 0x1413a6998, u8"Success performance waiting time" }, // 成功演出待機時間
	{ 0x1413a6a60, u8"Failure performance waiting time" }, // 失敗演出待機時間
	{ 0x1413a6b40, u8"Time to delay display of success result UI" }, // 成功時結果UIの表示を遅らせる時間
	{ 0x1413a6c40, u8"Time to delay display of result UI in case of failure" }, // 失敗時結果UIの表示を遅らせる時間
	{ 0x1413a7320, u8"Raw meat acquisition token" }, // 生肉獲得トークン
	{ 0x1413a73c8, u8"Undercooked meat acquisition token" }, // 生焼け肉獲得トークン
	{ 0x1413a74b8, u8"Browned Meat Acquisition Token" }, // こんがり肉獲得トークン
	{ 0x1413a75e8, u8"Charred Meat Acquisition Token" }, // 焦げ肉獲得トークン
	{ 0x1413a6d18, u8"Earned token" }, // 獲得トークン
	{ 0x1413a8fc0, u8"Number of failures using this data" }, // このデータを使う失敗数
	{ 0x1413a9458, u8"Time: hour" }, // 時間:時
	{ 0x1413a9550, u8"Hours: minutes" }, // 時間:分
	{ 0x1413a9688, u8"Boss physical strength value" }, // ボス体力値
	{ 0x1413a9100, u8"Boss rush stage" }, // ボスラッシュステージ
	{ 0x1413a7d20, u8"Parameters by difficulty level" }, // 難易度別パラメータ
	{ 0x1413a7eb8, u8"Initial number of rings" }, // 初期リング数
	{ 0x1413a7fa0, u8"Initial Cyloop Gauge" }, // 初期サイループゲージ
	{ 0x1413a8028, u8"Ready display time" }, // Ready表示時間
	{ 0x1413a80b0, u8"Clear display time" }, // Clear表示時間
	{ 0x1413a81a0, u8"Failed display time" }, // Faild表示時間
	{ 0x1413a8238, u8"Wait time from clear SE to fade" }, // クリアSEからフェードまでの待機時間
	{ 0x1413a82f0, u8"Waiting time from failure SE to selection UI display" }, // 失敗SEから選択UI表示までの待機時間
	{ 0x1413a83e0, u8"Master King Coco Line “Extremely!” Display time" }, // 達人王ココセリフ「尋常に！」表示時間
	{ 0x1413a8478, u8"Master King Coco “Game!” Transition delay time" }, // 達人王ココ「勝負！」遷移遅延時間
	{ 0x1413a85c8, u8"Master King Coco Line “Battle!” Display time" }, // 達人王ココセリフ「勝負！」表示時間
	{ 0x1413a8728, u8"Master King Coco Line “Excellent!” Display time" }, // 達人王ココセリフ「お見事！」表示時間
	{ 0x1413a9a38, u8"Maximum number of fragments generated. Maximum with negative value" }, // 破片最大生成数. 負の値で最大
	{ 0x1413a9d28, u8"Diffusion power" }, // 拡散力
	{ 0x1413af330, u8"Prerequisites" }, // 前提条件
	{ 0x1413af498, u8"Waiting time after defeat" }, // 撃破後待機時間
	{ 0x1413af578, u8"Enemy acceleration rate (normal 1.0)" }, // 敵の加速割合(通常1.0)
	{ 0x1413af638, u8"Enemy attack multiplier" }, // 敵の攻撃倍率
	{ 0x1413af7a0, u8"Enemy recovery interval" }, // 敵の回復までの間隔
	{ 0x1413af8c8, u8"Enemy recovery amount" }, // 敵の回復量
	{ 0x1413afa00, u8"Possible number of damage" }, // ダメージ可能回数
	{ 0x1413add60, u8"Trial stage" }, // 試練ステージ
	{ 0x1413ae000, u8"Title description display delay time" }, // 題目説明表示の遅延時間
	{ 0x1413ae488, u8"Master Coco Line “Extraordinary!” Display time" }, // 達人ココセリフ「尋常に！」表示時間
	{ 0x1413ae5b8, u8"Master Coco “Game!” Transition delay time" }, // 達人ココ「勝負！」遷移遅延時間
	{ 0x1413ae6c8, u8"Master Coco Line “Battle!” Display time" }, // 達人ココセリフ「勝負！」表示時間
	{ 0x1413ae870, u8"Master here line “Great job!” display time" }, // 達人ココセリフ「お見事！」表示時間
	{ 0x1413ae9e0, u8"Master Coco Trial" }, // 達人ココ試練
	{ 0x1413b03e8, u8"AutoSave" }, // AutoSave
	{ 0x1413b0508, u8"Gismo" }, // Gismo
	{ 0x1413b05e0, u8"Report" }, // Report
	{ 0x1413b06a0, u8"SkillTree" }, // SkillTree
	{ 0x1413b07b0, u8"Tutorial" }, // Tutorial
	{ 0x1413b0820, u8"ArcadeMode" }, // ArcadeMode
	{ 0x1413b08b8, u8"BattleChallenge" }, // BattleChallenge
	{ 0x1413b0978, u8"CyberChallenge" }, // CyberChallenge
	{ 0x1413b0a20, u8"Practice" }, // Practice
	{ 0x1413b0ac8, u8"Practice Timelimit" }, // Practice Timelimit
	{ 0x1413b0b78, u8"Credits" }, // Credits
	{ 0x1413b0c20, u8"Controlled character" }, // 操作キャラクタ
	{ 0x1413b2598, u8"Slope" }, // 斜面化
	{ 0x1413b2668, u8"Wall surface" }, // 壁面化
	{ 0x1413b2788, u8"Parkour possible" }, // パルクール可能
	{ 0x1413b1570, u8"Data" }, // データ
	{ 0x1413b2d80, u8"Playback interval time" }, // 再生間隔時間
	{ 0x1413b2e40, u8"Label table" }, // ラベルテーブル
	{ 0x1413b37f0, u8"Parameters per island" }, // 島ごとのパラメータ
	{ 0x1413b39b0, u8"Multiple text configuration parameters" }, // 複数テキスト構成パラメータ
	{ 0x1413b3b30, u8"TheEnd first notification count" }, // TheEndの初回通知のカウント
	{ 0x1413b3c90, u8"Theend notification count" }, // Theendの通知のカウント
	{ 0x1413b42d0, u8"Original resource name" }, // 元リソース名
	{ 0x1413b4450, u8"Half of each side" }, // 各辺の半分
	{ 0x1413b4b70, u8"Point of fixation" }, // 注視点
	{ 0x1413b4c84, u8"Perspective" }, // 視点
	{ 0x1413b67a0, u8"Number of phases" }, // フェイズ数
	{ 0x1413b68d0, u8"Limit time of each phase [sec]" }, // 各フェイズのリミット時間[sec]
	{ 0x1413b6a28, u8"Clear transition waiting time for each phase [sec]" }, // 各フェイズのクリア遷移待機時間[sec]
	{ 0x1413b6b08, u8"Result waiting time for each phase [sec]" }, // 各フェイズのリザルト待機時間[sec]
	{ 0x1413b55d8, u8"Rank time" }, // ランクタイム
	{ 0x1413b56e8, u8"All island rank time" }, // 全島ランクタイム
	{ 0x1413b6c20, u8"Rank time for each phase [sec]" }, // 各フェイズのランクタイム[sec]
	{ 0x1413b6d70, u8"Base BGM of each phase" }, // 各フェイズのベースBGM
	{ 0x1413b5090, u8"Battle rush stage" }, // バトルラッシュステージ
	{ 0x1413b5138, u8"Phase result display time" }, // フェイズリザルト表示時間
	{ 0x1413b7720, u8"Radius [OpenSmall]" }, // 半径[OpenSmall]
	{ 0x1413b77b0, u8"Radius [Open]" }, // 半径[Open]
	{ 0x1413b97f0, u8"Interpolation algorithm" }, // 補間アルゴリズム
	{ 0x1413b7820, u8"Interpolation [Close->OpenSmall]" }, // 補間[Close->OpenSmall]
	{ 0x1413b7898, u8"Interpolation [Close->Open]" }, // 補間[Close->Open]
	{ 0x1413b7980, u8"Interpolation [OpenSmall->Close]" }, // 補間[OpenSmall->Close]
	{ 0x1413b7a88, u8"Interpolation [OpenSmall->Open]" }, // 補間[OpenSmall->Open]
	{ 0x1413b7bb0, u8"Interpolation [Open->Close]" }, // 補間[Open->Close]
	{ 0x1413b7cf0, u8"Interpolation [Open->OpenSmall]" }, // 補間[Open->OpenSmall]
	{ 0x1413b9b28, u8"Experience points required to level up" }, // レベルアップに必要な経験値
	{ 0x1413b9bc8, u8"Required experience point increase according to level" }, // レベルに応じた必要経験値増加量
	{ 0x1413ba280, u8"Setting name" }, // 設定名
	{ 0x1413ba520, u8"Should moving sections be included in collisions?" }, // 移動区間も衝突対象とするか
	{ 0x1413b9f78, u8"Setting" }, // 設定
	{ 0x1413baaf8, u8"Thinning distance per level" }, // レベルごとの間引き距離
	{ 0x1413bb270, u8"Occurrence delay time" }, // 発生遅延時間
	{ 0x1413bb338, u8"Layer/player" }, // レイヤー/プレイヤー
	{ 0x1413bb3e8, u8"Layer/enemy" }, // レイヤー/エネミー
	{ 0x1413bb498, u8"Layer/Damaged Enemy" }, // レイヤー/被ダメージエネミー
	{ 0x1413bb518, u8"Layer/camera" }, // レイヤー/カメラ
	{ 0x1413bb568, u8"Layer/All other than above" }, // レイヤー/上記以外すべて
	{ 0x1413bfa58, u8"Points required to unlock skills" }, // スキル解放に必要なポイント
	{ 0x1413bfb38, u8"Required point increase according to skill level" }, // スキルレベルに応じた必要ポイント上昇量
	{ 0x1413bfc48, u8"Is it a gift skill?" }, // プレゼントスキルか
	{ 0x1413bba50, u8"Skill parameters" }, // スキルパラメータ
	{ 0x1413bf550, u8"Skill" }, // スキル
	{ 0x1413bc134, u8"↓" }, // ↓
	{ 0x1413bc224, u8"←" }, // ←
	{ 0x1413bc294, u8"→" }, // →
	{ 0x1413bf5e0, u8"Link" }, // リンク
	{ 0x1413c0038, u8"Starting index" }, // 開始インデックス
	{ 0x1413bbb18, u8"Skill Tree/Sonic" }, // スキルツリー/ソニック
	{ 0x1413bbb70, u8"Skill tree/Emmy" }, // スキルツリー/エミー
	{ 0x1413bbbf0, u8"Skill Tree/Knuckles" }, // スキルツリー/ナックルズ
	{ 0x1413bbc88, u8"Skill Tree/Tails" }, // スキルツリー/テイルス
	{ 0x1413c03e0, u8"Parallel movement limit" }, // 平行移動制限
	{ 0x1413c04c8, u8"Parallel movement speed (XZ)" }, // 平行移動速さ(XZ)
	{ 0x1413c05b8, u8"Parallel movement speed (Y)" }, // 平行移動速さ(Y)
	{ 0x1413c06f0, u8"Rotation speed" }, // 回転速さ
	{ 0x1413c0800, u8"Roll range (initial value ±x°)" }, // ロール範囲(初期値±x°)
	{ 0x1413c08e0, u8"Roll speed" }, // ロール速さ
	{ 0x1413c0a40, u8"Viewing angle adjustment range (initial value ±x°)" }, // 画角調整範囲(初期値±x°)
	{ 0x1413c0bb8, u8"View angle adjustment speed" }, // 画角調整速さ
	{ 0x1413c0cd8, u8"Speed multiplier \"fast\"" }, // スピード倍率「速い」
	{ 0x1413c0eb0, u8"Speed multiplier \"slow\"" }, // スピード倍率「遅い」
	{ 0x1413c1720, u8"Sepia - Saturation Offset [%]" }, // セピア-彩度オフセット[%]
	{ 0x1413c1858, u8"Sepia - Applicability [%]" }, // セピア-適用度[%]
	{ 0x1413c1980, u8"Sepia - hue [°]" }, // セピア-色相[°]
	{ 0x1413c1af8, u8"Saturation UP-Offset" }, // 彩度UP-オフセット
	{ 0x1413c1c28, u8"Warm color-R offset" }, // 暖色系-Rオフセット
	{ 0x1413c1df8, u8"Warm colors-G offset" }, // 暖色系-Gオフセット
	{ 0x1413c1f78, u8"Warm colors-B offset" }, // 暖色系-Bオフセット
	{ 0x1413c2050, u8"Cool color-R offset" }, // 寒色系-Rオフセット
	{ 0x1413c2108, u8"Cool color-G offset" }, // 寒色系-Gオフセット
	{ 0x1413c21d0, u8"Cool color-B offset" }, // 寒色系-Bオフセット
	{ 0x1413c22d0, u8"Retro style-contrast offset" }, // レトロ調-コントラストオフセット
	{ 0x1413c23c0, u8"Retro style-R offset" }, // レトロ調-Rオフセット
	{ 0x1413c24a8, u8"Retro style-G offset" }, // レトロ調-Gオフセット
	{ 0x1413c25c8, u8"Retro style-B offset" }, // レトロ調-Bオフセット
	{ 0x1413c2718, u8"Retro-saturation offset" }, // レトロ調-彩度オフセット
	{ 0x1413c3278, u8"Camera behavior" }, // カメラ挙動
	{ 0x1413c5c88, u8"Skip allowed action" }, // スキップ許容アクション
	{ 0x1413c5ef8, u8"Operation guide text label" }, // 操作ガイドテキストラベル
	{ 0x1413cc260, u8"Checker" }, // チェッカー
	{ 0x1413cc350, u8"Action list" }, // アクションリスト
	{ 0x1413cc3f8, u8"Count value" }, // カウント値
	{ 0x1413ca758, u8"Required skills" }, // 必要スキル
	{ 0x1413ca828, u8"Required skill number 64 and above" }, // 必要スキル64番以降
	{ 0x1413ca980, u8"Required clear title number" }, // 必須クリア題目番号
	{ 0x1413caa78, u8"Required clear subject number 64 or later" }, // 必須クリア題目番号64番以降
	{ 0x1413cabb0, u8"Required clear subject number 128 or later" }, // 必須クリア題目番号128番以降
	{ 0x1413cacc8, u8"Required clear subject number 192 or later" }, // 必須クリア題目番号192番以降
	{ 0x1413cae20, u8"After spin boost is released" }, // スピンブースト解禁後から
	{ 0x1413c9b38, u8"Practice topic" }, // 練習題目
	{ 0x1413c9b98, u8"Practice special text" }, // 練習特殊テキスト
	{ 0x1413ccfe8, u8"Ready time" }, // Ready時間
	{ 0x1413cd0e0, u8"Play time" }, // Play時間
	{ 0x1413cd2f8, u8"Waiting time after subject clear judgment" }, // 題目クリア判定後の待機時間
	{ 0x1413cd590, u8"Waiting time when rebuilding title 1" }, // 題目再構築時の待機時間1
	{ 0x1413cd6a0, u8"Waiting time when rebuilding titles 2" }, // 題目再構築時の待機時間2
	{ 0x1413c9be8, u8"Practice time control" }, // 練習時間制御
	{ 0x1413c9cc8, u8"Practice notification display time" }, // 練習通知表示時間
	{ 0x1413ce1d8, u8"Required skills 2" }, // 必要スキル2
	{ 0x1413ce2d0, u8"Displayed after clearing the tutorial" }, // チュートリアルクリア後から表示
	{ 0x1413ce3f8, u8"Displayed after arcade mode is released" }, // アーケードモード解禁後から表示
	{ 0x1413ce530, u8"Displayed after Cyber Power Boost is lifted" }, // 電脳パワーブースト解禁後から表示
	{ 0x1413ce5b0, u8"Displayed after VeryHard ban is lifted" }, // VeryHard解禁後から表示
	{ 0x1413ce678, u8"Displayed after the release of Strong and New Game" }, // 強くてニューゲーム解禁後から表示
	{ 0x1413ce750, u8"Displayed after spin boost is released" }, // スピンブースト解禁後から表示
	{ 0x1413ce880, u8"After clearing Dragon Master Coco" }, // 龍の達人ココクリア後から
	{ 0x1413c9d80, u8"Tips" }, // Tips
	{ 0x1413c9e20, u8"TipsMinimum display time" }, // Tips最小表示時間
	{ 0x1413cb4e0, u8"Practice list" }, // 練習一覧
	{ 0x1413cb648, u8"Tips list" }, // Tips一覧
	{ 0x1413c9f58, u8"Practice structure" }, // 練習構成
	{ 0x1413ceef0, u8"Player speed to make time pass faster" }, // 時間経過を早くするプレイヤー速度
	{ 0x1413cf010, u8"Time elapsed magnification when exceeding a certain speed" }, // 一定速度を超えた時の時間経過倍率
	{ 0x1413cf110, u8"Time added when returning from cyberspace (minutes)" }, // 電脳空間から戻ったときに加算する時間(分)
	{ 0x1413cf8a0, u8"Block number weight value" }, // ブロック数ウェイト値
	{ 0x1413d15d8, u8"Transition destination weight value" }, // 遷移先ウェイト値
	{ 0x1413d1720, u8"Block length by weather" }, // 天候別ブロック長
	{ 0x1413cff78, u8"Weather list" }, // 天候一覧
	{ 0x1413d0598, u8"Sunrise/sunny fixed start time" }, // 日出/晴れ固定開始時間
	{ 0x1413d06d0, u8"Sunrise/sunny fixed end time" }, // 日出/晴れ固定終了時間
	{ 0x1413d0750, u8"Sunset/Sunny fixed start time" }, // 日没/晴れ固定開始時間
	{ 0x1413d08b0, u8"Sunset/sunny fixed end time" }, // 日没/晴れ固定終了時間
	{ 0x1413d09e0, u8"Weather 1 block time" }, // 天候1ブロック時間
	{ 0x1413d0118, u8"Time control" }, // 時間制御
	{ 0x1413d19a8, u8"Local light brightness scale" }, // ローカルライト輝度スケール
	{ 0x1413d1ac8, u8"Override processing failure priority" }, // 処理落ちプライオリティを上書きする
	{ 0x1413d1bc0, u8"Processing priority" }, // 処理落ちプライオリティ
	{ 0x1413d20a0, u8"Noise scroll speed" }, // ノイズスクロール速度
	{ 0x1413d2198, u8"Blur scale" }, // ブラースケール
	{ 0x1413d2298, u8"Color gain" }, // カラーゲイン
	{ 0x1413d2380, u8"Noise gain" }, // ノイズゲイン
	{ 0x1413d29e0, u8"Interpolation parameters" }, // 補間パラメータ
	{ 0x1413d3158, u8"Aura" }, // オーラ
	{ 0x1413d32a0, u8"Weather" }, // 天候
	{ 0x1413d3698, u8"Presets" }, // presets
	{ 0x1413d51a0, u8"Quick Cyloop gauge recovery amount" }, // クイックサイループゲージの回復量
	{ 0x1413d58f8, u8"Warning display delay time" }, // 予兆表示遅延時間
	{ 0x1413d5e48, u8"Gaze frame" }, // 注視フレーム
	{ 0x1413d5f70, u8"Gaze frame [inverted]" }, // 注視フレーム[反転]
	{ 0x1413d60e0, u8"Gaze offset" }, // 注視オフセット
	{ 0x1413d6238, u8"Player offset" }, // プレイヤーオフセット
	{ 0x1413d63f0, u8"Tracking rate" }, // 追従率
	{ 0x1413d68a8, u8"Initial distance" }, // 初期距離
	{ 0x1413d6978, u8"Goal offset" }, // ゴールオフセット
	{ 0x1413d6a48, u8"Normal speed" }, // 通常時の速度
	{ 0x1413d6b10, u8"Speed when boosted" }, // ブースト時の速度
	{ 0x1413d6bc8, u8"Player movement width" }, // プレイヤー移動幅
	{ 0x1413d6d78, u8"Player movement height" }, // プレイヤー移動高さ
	{ 0x1413d6e60, u8"Close range" }, // 至近距離
	{ 0x1413d6f30, u8"Close range speed scale" }, // 至近距離の速度スケール
	{ 0x1413d7030, u8"Distance the boss moves with a missile hit" }, // ミサイルHitでボスが進む距離
	{ 0x1413d7158, u8"Boss deceleration time when missile hits" }, // ミサイルHit時のボス減速時間
	{ 0x1413d7268, u8"Boss speed scale when missile hits" }, // ミサイルHit時のボス速度スケール
	{ 0x1413d7370, u8"Boost delay time when missile hits" }, // ミサイルHit時のブースト遅延時間
	{ 0x1413d74a0, u8"Boost time when missile hits" }, // ミサイルHit時のブースト時間
	{ 0x1413d7580, u8"Boost speed when missile hits" }, // ミサイルHit時のブースト速度
	{ 0x1413d85f8, u8"Parent frame name" }, // 親フレーム名
	{ 0x1413f0b28, u8"Physical strength value" }, // 体力値
	{ 0x1413f0c60, u8"Stun time" }, // スタン時間
	{ 0x1413f0d68, u8"Stun value" }, // スタン値
	{ 0x1413f0e58, u8"Fear value" }, // 怯み値
	{ 0x1413f0fb8, u8"Experience point" }, // 経験値
	{ 0x1413f3ab8, u8"Battle start coordinates/center coordinates" }, // 戦闘開始座標・中心座標
	{ 0x1413f3b90, u8"Node name that serves as the reference for height [Origin of object if not set]" }, // 高さの基準となるノード名[未設定時はオブジェクトの原点]
	{ 0x1413f3ca8, u8"Speed of adjusting height" }, // 高さを調整する速度
	{ 0x1413f3d70, u8"Relative distance between player and boss [minimum]" }, // プレイヤーとボスの相対距離[最小]
	{ 0x1413f3e48, u8"Relative distance between player and boss [maximum]" }, // プレイヤーとボスの相対距離[最大]
	{ 0x1413f3ee0, u8"Height adjustment [at minimum relative distance]" }, // 高さ調整[相対距離最小時]
	{ 0x1413f4080, u8"Height adjustment [at maximum relative distance]" }, // 高さ調整[相対距離最大時]
	{ 0x1413f41c8, u8"Boss movement range [minimum]" }, // ボス移動範囲[最小]
	{ 0x1413f42c8, u8"Boss movement range [maximum]" }, // ボス移動範囲[最大]
	{ 0x1413f43e8, u8"Player initial appearance position" }, // プレイヤー初期出現位置
	{ 0x1413f44e8, u8"Player respawn position" }, // プレイヤー再出現位置
	{ 0x1413f4608, u8"Player respawn position 2" }, // プレイヤー再出現位置2
	{ 0x1413f10b8, u8"Range setting" }, // 範囲設定
	{ 0x1413f1128, u8"Range setting during Cyloop" }, // サイループ時の範囲設定
	{ 0x1413ed768, u8"Pillar object usage flag" }, // 柱オブジェクト使用フラグ
	{ 0x1413ed7c8, u8"Remaining HP when phase switching occurs [%]" }, // フェーズ切替発生残HP[%]
	{ 0x1413f6950, u8"Battle phase settings" }, // バトルフェーズ設定
	{ 0x1413e9018, u8"Interpolation type" }, // 補間タイプ
	{ 0x1413e90c0, u8"Name of the node to be focused on [If not set, indicates the origin of the object]" }, // 注視するノード名[未設定時.はオブジェクトの原点]
	{ 0x1413e91c8, u8"Gaze position adjustment" }, // 注視位置調整
	{ 0x1413e92d0, u8"Name of node to be focused on as sub [Origin of object if not set]" }, // サブとして注視するノード名[未設定時はオブジェクトの原点]
	{ 0x1413e93e8, u8"Sub gaze position adjustment" }, // サブ注視位置調整
	{ 0x1413e95f0, u8"Player position adjustment" }, // プレイヤー位置調整
	{ 0x1413f6fc8, u8"Debug display" }, // デバック表示
	{ 0x1413f14d0, u8"Phase number to display for debugging" }, // デバック表示するフェーズ番号
	{ 0x1413ee1b8, u8"Gimmick type [Bulk specification]" }, // ギミックタイプ[一括指定]
	{ 0x1413ee2b8, u8"Gimmick reuse standby time [Bulk specification]" }, // ギミック再使用待機時間[一括指定]
	{ 0x1413ee3b0, u8"Attack type [batch specification]" }, // アタックタイプ[一括指定]
	{ 0x1413ee438, u8"Attack reuse standby time [batch specification]" }, // アタック再使用待機時間[一括指定]
	{ 0x1413ef5d8, u8"Gimmick time phase" }, // ギミック時間位相
	{ 0x1413ef788, u8"Attack time phase" }, // アタック時間位相
	{ 0x1413ef858, u8"Individual specification flag [priority over batch specification contents]" }, // 個別指定フラグ[一括指定内容より優先]
	{ 0x1413ef9a8, u8"Gimmick type [for individual specification]" }, // ギミックタイプ[個別指定時用]
	{ 0x1413efa28, u8"Gimmick reuse standby time [for individual specification]" }, // ギミック再使用待機時間[個別指定時用]
	{ 0x1413efb10, u8"Attack type [for individual specification]" }, // アタックタイプ[個別指定時用]
	{ 0x1413efbc0, u8"Attack reuse standby time [for individual specification]" }, // アタック再使用待機時間[個別指定時用]
	{ 0x1413ee558, u8"Individual setting" }, // 個別設定
	{ 0x1413ee6c0, u8"Node name of the skeleton that generates the pillar" }, // 柱を生成するスケルトンのノード名
	{ 0x1413ee7e8, u8"Pillar health value" }, // 柱の体力値
	{ 0x1413ee888, u8"Boss body damage value when pillar is destroyed" }, // 柱破壊時ボス本体ダメージ値
	{ 0x1413ee998, u8"Boss body stun value when pillar is destroyed" }, // 柱破壊時ボス本体スタン値
	{ 0x1413eeb48, u8"Switch debug display" }, // デバック表示切り替え
	{ 0x1413e2e80, u8"Column object placement settings" }, // 柱オブジェクト配置設定
	{ 0x1413e1e38, u8"Node name" }, // ノード名
	{ 0x1413e41e0, u8"Collision name" }, // コリジョン名
	{ 0x1413e61d0, u8"Text (used when switching during contact or motion)" }, // テキスト(接触やモーション中の切り替え時に使用)
	{ 0x1413e2330, u8"Collision settings" }, // コリジョン設定
	{ 0x1413e2f78, u8"Player detection settings" }, // プレイヤー検知設定
	{ 0x1413eafb0, u8"Part specification [geometry number]" }, // 部位指定[ジオメトリ番号]
	{ 0x1413eb128, u8"Polygon specification [triangle number]" }, // ポリゴン指定[トライアングル番号]
	{ 0x1413eb2c0, u8"Type" }, // 種別
	{ 0x1413e87e8, u8"Effective distance" }, // 有効距離
	{ 0x1413e8960, u8"Continuous homing distance" }, // 連続ホーミング可能距離
	{ 0x1413eb340, u8"Homing possible point setting" }, // ホーミング可能ポイント設定
	{ 0x1413ec2f0, u8"Number of generated pieces" }, // 生成個数
	{ 0x1413ec3a0, u8"Generation interval" }, // 生成間隔
	{ 0x1413eb3f0, u8"Minion settings" }, // ミニオン設定
	{ 0x1413eb498, u8"Trigger settings" }, // トリガー設定
	{ 0x1413f1b30, u8"Time phase" }, // 時間位相
	{ 0x1413eb508, u8"Obstacle settings" }, // 障害物設定
	{ 0x1413e5c70, u8"Not used when down due to Cyloop" }, // サイループによるダウン時は未使用
	{ 0x1413e6c98, u8"Collision name (used for identification)" }, // コリジョン名(識別に使用)
	{ 0x1413eb558, u8"Attack point setting" }, // 攻撃ポイント設定
	{ 0x1413e6fd0, u8"Focus region specification [geometry number]" }, // フォーカス部位指定[ジオメトリ番号]
	{ 0x1413e70d8, u8"Focus polygon specification [triangle number]" }, // フォーカスポリゴン指定[トライアングル番号]
	{ 0x1413e7210, u8"Size scale" }, // サイズスケール
	{ 0x1413e72f8, u8"Launch speed" }, // 打ち出し速度
	{ 0x1413e7480, u8"Boss throw ratio" }, // ボススロー比率
	{ 0x1413e7548, u8"Player throw ratio" }, // プレイヤースロー比率
	{ 0x1413ed4f0, u8"Slow time" }, // スロー時間
	{ 0x1413e7620, u8"Slow interpolation start time" }, // スロー補間開始時間
	{ 0x1413e76f8, u8"Slow interpolation end time" }, // スロー補間終了時間
	{ 0x1413eb638, u8"Dash circle point setting" }, // ダッシュサークルポイント設定
	{ 0x1413eb730, u8"Mesh frame number to link" }, // リンクするメッシュフレーム番号
	{ 0x1413eb9c0, u8"Rotation adjustment" }, // 回転調整
	{ 0x1413e9dd8, u8"Mesh frame" }, // メッシュフレーム
	{ 0x1413ea068, u8"Debug display of various points" }, // 各種ポイントのデバック表示
	{ 0x1413ea120, u8"Debug display of mesh collision" }, // メッシュコリジョンのデバック表示
	{ 0x1413ea210, u8"Mesh number debug display" }, // メッシュ番号デバック表示
	{ 0x1413ea2b8, u8"Mesh number display distance" }, // メッシュ番号表示距離
	{ 0x1413ea3e8, u8"Mesh number display upper limit" }, // メッシュ番号表示上限
	{ 0x1413e3080, u8"Mesh frame settings" }, // メッシュフレーム設定
	{ 0x1413e2030, u8"Movement vector coefficient" }, // 移動量ベクトル係数
	{ 0x1413e21d0, u8"Attack settings" }, // 攻撃設定
	{ 0x1413e26a0, u8"Collision name for counter [homing collision for spa sonics]" }, // カウンター対象のコリジョン名[スパソニ用ホーミングコリジョン]
	{ 0x1413e44b0, u8"Valid only when parry is successful" }, // パリィ成功時のみ有効
	{ 0x1413e4560, u8"Cyloop radius [when parry is successful]" }, // サイループ半径[パリィ成功時]
	{ 0x1413e4690, u8"Rigid body only" }, // 剛体のみ
	{ 0x1413e3200, u8"Homing point setting for Super Sonic battle" }, // スパソニ戦用ホーミングポイント設定
	{ 0x1413e7dd8, u8"Number of sheets (Q Cyloop up)" }, // 枚数(Qサイループ上り)
	{ 0x1413e7f10, u8"Number of sheets (Q Sailoop down)" }, // 枚数(Qサイループ下り)
	{ 0x1413e7fc0, u8"Number of sheets (Sonic special move)" }, // 枚数(ソニック必殺技)
	{ 0x1413e8120, u8"Random initial velocity at the time of occurrence" }, // 発生時のランダム初速
	{ 0x1413e3358, u8"Ring release" }, // リング放出
	{ 0x1413d9bc8, u8"Air circulation path speed over the island" }, // 島上空巡回パス速度
	{ 0x1413d9d40, u8"Boss nest patrol path speed" }, // ボスの巣巡回パス速度
	{ 0x1413d9e98, u8"Boss nest approach path speed [minimum]" }, // ボスの巣接近パス速度[最低]
	{ 0x1413d9fa8, u8"Boss nest approach pass speed [maximum]" }, // ボスの巣接近パス速度[最高]
	{ 0x1413da080, u8"Climb pass speed" }, // 登頂パス速度
	{ 0x1413da178, u8"Combat pass speed" }, // 戦闘パス速度
	{ 0x1413da2b0, u8"Ring recovery pass speed" }, // リング回収パス速度
	{ 0x1413da3c8, u8"Distance at side" }, // 側面時の距離
	{ 0x1413da4f0, u8"Waiting time at the end of bombardment" }, // 砲撃終了時の待ち時間
	{ 0x1413da620, u8"Stagger durability value" }, // よろけ耐久値
	{ 0x1413da730, u8"[Insanity] Stagger durability value" }, // [発狂]よろけ耐久値
	{ 0x1413da840, u8"Stagger time" }, // よろけ時間
	{ 0x1413da948, u8"Time after bite and parry" }, // 噛みつきパリィ後の時間
	{ 0x1413dab60, u8"Time after scratch parry" }, // 引っかきパリィ後の時間
	{ 0x1413dad28, u8"Number of flinches leading to knockback" }, // ノックバックに至る怯み回数
	{ 0x1413daeb0, u8"Knockback durability value" }, // ノックバック耐久値
	{ 0x1413dafc0, u8"Knockback time" }, // ノックバック時間
	{ 0x1413db080, u8"Stuck time" }, // ピヨり時間
	{ 0x1413db0e0, u8"Cyloop detention time" }, // サイループの拘束時間
	{ 0x1413db198, u8"Cyloop second stage detention time" }, // サイループ第二段階の拘束時間
	{ 0x1413db2c8, u8"Cyloop escape HP percentage" }, // サイループ脱出HP割合
	{ 0x1413db408, u8"Cyloop escape time" }, // サイループの脱出時間
	{ 0x1413db5b0, u8"Relative altitude to fall to death" }, // 落下死する相対高度
	{ 0x1413db6b0, u8"Damage received from special moves" }, // 必殺技で受けるダメージ
	{ 0x1413db7c8, u8"Damage received in QTE1" }, // QTE1で受けるダメージ
	{ 0x1413db908, u8"Access node name when visiting" }, // 巡回時のアクセスノード名
	{ 0x1413e1fa0, u8"Damage speed" }, // ダメージ速度
	{ 0x1413dba18, u8"Attack judgment" }, // 攻撃判定
	{ 0x1413e49e0, u8"Homing node name" }, // ホーミングノード名
	{ 0x1413e4aa0, u8"Homing node name [reverse]" }, // ホーミングノード名[反転]
	{ 0x1413e4bc0, u8"Homing position" }, // ホーミング位置
	{ 0x1413dbae0, u8"Devotion judgment" }, // 喰らい判定
	{ 0x1413f2ed8, u8"Aiming time" }, // 狙う時間
	{ 0x1413dbb98, u8"Scratch" }, // 引っかき
	{ 0x1413dfc88, u8"Bullet radius" }, // 弾の半径
	{ 0x1413ee260, u8"Maximum speed of bullet" }, // 弾の最大速度
	{ 0x1413ee398, u8"Bullet acceleration" }, // 弾の加速度
	{ 0x1413ee468, u8"Acceleration rate when rebounding" }, // 跳ね返した時の加速率
	{ 0x1413e2de0, u8"Bullet firing interval" }, // 弾の発射間隔
	{ 0x1413ee760, u8"Range of firing pitch angle fluctuation" }, // 発射ピッチ角のブレ範囲
	{ 0x1413e3418, u8"Launch yaw angle blur range" }, // 発射ヨー角のブレ範囲
	{ 0x1413ee8f0, u8"Will it bounce off the boss?" }, // ボスに当たって跳ね返る？
	{ 0x1413dbc50, u8"Parryable bullet" }, // パリィ可能弾
	{ 0x1413e3578, u8"Number of shots" }, // 発射数
	{ 0x1413e3678, u8"Waiting time after firing" }, // 発射終了後の待ち時間
	{ 0x1413f3cd8, u8"Muzzle vertical rotation speed" }, // 銃口の垂直回転速度
	{ 0x1413f3da0, u8"Horizontal rotation speed of the muzzle" }, // 銃口の水平回転速度
	{ 0x1413f3e78, u8"Circle firing interval" }, // 円の発射間隔
	{ 0x1413f3f68, u8"Number of circles" }, // 円の数
	{ 0x1413f40c8, u8"Number of circles that can be parried" }, // パリィ可能な円の数
	{ 0x1413dbd70, u8"Spiral shooting" }, // 螺旋射撃
	{ 0x1413dbdf0, u8"Alternate spiral shooting" }, // 交互螺旋射撃
	{ 0x1413dbed0, u8"Vertical spiral shooting" }, // 上下螺旋射撃
	{ 0x1413f4920, u8"Gravitational acceleration" }, // 重力加速
	{ 0x1413f4c60, u8"Elevation interval" }, // 仰角の間隔
	{ 0x1413f4e30, u8"Azimuth spacing" }, // 方位角の間隔
	{ 0x1413f4f94, u8"Number of times" }, // 回数
	{ 0x1413f50d8, u8"Number of bullets that can be parried" }, // パリィ可能弾の回数
	{ 0x1413dc020, u8"Fountain shooting" }, // 噴水射撃
	{ 0x1413e2cb0, u8"Maximum acceleration of the bullet" }, // 弾の最大加速度
	{ 0x1413e3120, u8"Minimum pitch angle" }, // 最小ピッチ角
	{ 0x1413e3278, u8"Maximum pitch angle" }, // 最大ピッチ角
	{ 0x1413dc0c0, u8"Homing laser" }, // ホーミングレーザー
	{ 0x1413dc1c8, u8"Homing missile" }, // ホーミングミサイル
	{ 0x1413efb40, u8"Inner radius" }, // 内側の半径
	{ 0x1413efc20, u8"Outer radius" }, // 外側の半径
	{ 0x1413f1048, u8"Number of shots that can be parried" }, // パリィ可能弾の発射数
	{ 0x1413f0198, u8"Firing interval within one set" }, // 1セット内での発射間隔
	{ 0x1413f0340, u8"Number of shots per set" }, // 1セットの発射数
	{ 0x1413dc268, u8"Ripple laser" }, // リップルレーザー
	{ 0x1413f1a78, u8"Maximum yaw angle" }, // 最大ヨー角
	{ 0x1413f1af8, u8"Number of vertical shots" }, // 縦方向の発射数
	{ 0x1413f1b50, u8"Number of horizontal shots" }, // 横方向の発射数
	{ 0x1413f1c20, u8"Additional shots in front" }, // 正面の追加発射数
	{ 0x1413dc328, u8"Omnidirectional ripple laser" }, // 全方位リップルレーザー
	{ 0x1413dc400, u8"Lively ripple laser after going crazy" }, // 発狂後賑やかしリップルレーザー
	{ 0x1413f5670, u8"Bullet rotation speed" }, // 弾の回転速度
	{ 0x1413f5840, u8"Rotational acceleration of the bullet" }, // 弾の回転加速度
	{ 0x1413f59f0, u8"Shock wave portion ratio" }, // 衝撃波部分の比率
	{ 0x1413dc528, u8"Shock wave" }, // 衝撃波
	{ 0x1413e6100, u8"Rotational acceleration" }, // 回転加速度
	{ 0x1413e63b0, u8"Irradiation preparation time" }, // 照射準備時間
	{ 0x1413dc600, u8"Laser winder" }, // レーザーワインダー
	{ 0x1413dc680, u8"Random laser" }, // ランダムレーザー
	{ 0x1413d5638, u8"Counter laser" }, // カウンターレーザー
	{ 0x1413ef340, u8"Player position" }, // プレイヤー位置
	{ 0x1413ef510, u8"Moving range" }, // 移動範囲
	{ 0x1413ef630, u8"Set rotation speed" }, // セット回転速度
	{ 0x1413eee08, u8"Number of missiles launched" }, // ミサイル発射数
	{ 0x1413eeee0, u8"Missile launch interval" }, // ミサイル発射間隔
	{ 0x1413c14c0, u8"Missile" }, // missile
	{ 0x1413dc818, u8"Ring collection" }, // リング回収
	{ 0x1413dc8f8, u8"Damage multiplier" }, // ダメージ倍率
	{ 0x1413e8c80, u8"Boss position" }, // ボスの位置
	{ 0x1413e8d98, u8"Boss Y rotation" }, // ボスのY回転
	{ 0x1413e8ed8, u8"Boss interpolation type" }, // ボスの補間タイプ
	{ 0x1413e8fd8, u8"Boss interpolation time" }, // ボスの補間時間
	{ 0x1413e9080, u8"Player position" }, // プレイヤーの位置
	{ 0x1413e91e0, u8"Player movement width" }, // プレイヤーの移動幅
	{ 0x1413e9328, u8"Player movement height" }, // プレイヤーの移動高さ
	{ 0x1413e9460, u8"Rotation of player movement range" }, // プレイヤーの移動範囲の回転
	{ 0x1413e9578, u8"Player interpolation type" }, // プレイヤーの補間タイプ
	{ 0x1413e9630, u8"Player interpolation time" }, // プレイヤーの補間時間
	{ 0x1413dca60, u8"Positional relationship" }, // 位置関係
	{ 0x1413ebea8, u8"Player node" }, // プレイヤーノード
	{ 0x1413ec068, u8"Boss node" }, // ボスノード
	{ 0x1413ec188, u8"Number of player nodes" }, // プレイヤーノード数
	{ 0x1413ec220, u8"Number of boss nodes" }, // ボスノード数
	{ 0x1413dccd0, u8"Movement data for left and right swapping" }, // 左右入れ替えの移動データ
	{ 0x1413eca58, u8"Fire bar" }, // ファイアバー
	{ 0x1413e0a60, u8"Probability of appearance height [%] (low, medium, high)" }, // "出現高さの確率[%](低, 中, 高)"
	{ 0x1413ecdf8, u8"Arrangement" }, // 配置
	{ 0x1413f2a70, u8"Obstacle set" }, // 障害物セット
	{ 0x1413dcdf0, u8"Aura road" }, // オーラロード
	{ 0x1413e6ed8, u8"Launch offset" }, // 打ち出し先オフセット
	{ 0x1413e6fa8, u8"[Launching camera] Interpolation time" }, // [打ち出しカメラ]補間時間
	{ 0x1413e70a8, u8"[Launching camera] position offset" }, // [打ち出しカメラ]位置オフセット
	{ 0x1413e71e0, u8"[Launching camera] Gaze point offset" }, // [打ち出しカメラ]注視点オフセット
	{ 0x1413e72c0, u8"[Launching camera] roll" }, // [打ち出しカメラ]ロール
	{ 0x1413e73b8, u8"[Launching camera] Duration" }, // [打ち出しカメラ]持続時間
	{ 0x1413e7498, u8"[Falling camera] Interpolation time" }, // [落下カメラ]補間時間
	{ 0x1413e7568, u8"[Falling camera] distance" }, // [落下カメラ]距離
	{ 0x1413e75c8, u8"[Falling camera] Elevation angle" }, // [落下カメラ]仰角
	{ 0x1413e7640, u8"[Landing camera] Interpolation time" }, // [着地カメラ]補間時間
	{ 0x1413e7718, u8"[Landing camera] distance" }, // [着地カメラ]距離
	{ 0x1413e7800, u8"[Landing camera] Elevation angle" }, // [着地カメラ]仰角
	{ 0x1413e7878, u8"[Landing camera] Azimuth" }, // [着地カメラ]方位角
	{ 0x1413e7960, u8"[Safe gazing camera] Starting distance" }, // [金庫注視カメラ]開始距離
	{ 0x1413e7af0, u8"[Safe gazing camera] Interpolation time" }, // [金庫注視カメラ]補間時間
	{ 0x1413e7c28, u8"[Safe gazing camera] distance" }, // [金庫注視カメラ]距離
	{ 0x1413e7d60, u8"[Safe gazing camera] Elevation angle" }, // [金庫注視カメラ]仰角
	{ 0x1413e7eb8, u8"[Safe gazing camera] Azimuth" }, // [金庫注視カメラ]方位角
	{ 0x1413dced0, u8"Ejection from aura road to head" }, // オーラロードから頭部への射出
	{ 0x1413f2328, u8"Quick step" }, // クイックステップ
	{ 0x1413d9a58, u8"Chase" }, // チェイス
	{ 0x1413eb518, u8"Acceleration" }, // 加速度
	{ 0x1413ea978, u8"Rotational force" }, // 回転力
	{ 0x1413eaa90, u8"Maximum rotational force" }, // 最大回転力
	{ 0x1413eabd0, u8"Increase speed of rotational force" }, // 回転力の増加速度
	{ 0x1413eacc0, u8"Rotation resistance force" }, // 回転抵抗力
	{ 0x1413eadb0, u8"Straight ahead time" }, // 直進時間
	{ 0x1413eb6a0, u8"Individual parameters" }, // 個別パラメータ
	{ 0x1413eb7e0, u8"Individual parameters [poor]" }, // 個別パラメータ[粗悪]
	{ 0x1413e9fc8, u8"Up to specified HP percentage" }, // 指定HP割合まで
	{ 0x1413ea278, u8"Waiting time after launch" }, // 発射後の待ち時間
	{ 0x1413ea358, u8"Percentage of inferior products" }, // 粗悪品の割合
	{ 0x1413eb908, u8"Firing pattern" }, // 発射パターン
	{ 0x1413dd010, u8"Missile in chase" }, // チェイス中のミサイル
	{ 0x1413dd168, u8"Sequence" }, // シーケンス
	{ 0x1413dd270, u8"Madness sequence" }, // 発狂シーケンス
	{ 0x1413e57b0, u8"HP percentage" }, // HP割合
	{ 0x1413e5930, u8"Occurrence sequence" }, // 発生シーケンス
	{ 0x1413e59f0, u8"Forced interrupt" }, // 強制割り込み
	{ 0x1413dd350, u8"Interrupt settings" }, // 割り込み設定
	{ 0x1413e0610, u8"Time until transition to second half camera" }, // 後半カメラに遷移するまでの時間
	{ 0x1413dd418, u8"Cyloop constraint settings" }, // Cyloop拘束設定
	{ 0x1413ed458, u8"Slow magnification" }, // スロー倍率
	{ 0x1413ed600, u8"Slow end interpolation time" }, // スロー終了補間時間
	{ 0x1413ed718, u8"Does the camera also include slow motion?" }, // カメラもスローに含める？
	{ 0x1413ed1b8, u8"Slow setting" }, // スロー設定
	{ 0x1413dd510, u8"Parry production settings" }, // パリィ演出設定
	{ 0x1413dd668, u8"Attack sign setting" }, // 攻撃予兆設定
	{ 0x1413ec670, u8"Practice notification settings [when climbing]" }, // 練習通知設定[登頂時]
	{ 0x1413ec740, u8"Practice notification settings [during battle]" }, // 練習通知設定[戦闘時]
	{ 0x1413dd760, u8"Notification settings" }, // 通知設定
	{ 0x1413fa4a0, u8"Wait time after attack" }, // 攻撃後待機時間
	{ 0x1413fa568, u8"Attack speed multiplier" }, // 攻撃速度倍率
	{ 0x1413fa620, u8"Knockback occurrence condition [interruption counter number]" }, // ノックバック発生条件[割り込みカウンター回数]
	{ 0x1413fa718, u8"Action list" }, // アクション一覧
	{ 0x1413f83e8, u8"Attribute 1" }, // 属性1
	{ 0x1413f84c8, u8"Attribute 2" }, // 属性2
	{ 0x1413f8568, u8"Distance [minimum]" }, // 距離[最小]
	{ 0x1413f8630, u8"Distance [maximum]" }, // 距離[最大]
	{ 0x1413f86f0, u8"Altitude [minimum]" }, // 高度[最小]
	{ 0x1413f87f8, u8"Altitude [maximum]" }, // 高度[最大]
	{ 0x1413f88c8, u8"Angle [minimum]" }, // 角度[最小]
	{ 0x1413f89b0, u8"Angle [maximum]" }, // 角度[最大]
	{ 0x1413f8af0, u8"Minimum angle from front" }, // 正面からの最小角度
	{ 0x1413f8c60, u8"Remaining HP % [Minimum]" }, // 残HP %[最小]
	{ 0x1413f8dc0, u8"Remaining HP % [Maximum]" }, // 残HP %[最大]
	{ 0x1413f8e88, u8"Distance from center [0 is invalid]" }, // 中央からの距離[0は無効]
	{ 0x1413f9178, u8"Reuse time [Cannot be reused for more than 1000 seconds]" }, // 再使用時間[1000秒以上は再使用不能]
	{ 0x1413f92f0, u8"First use time limit" }, // 初回使用制限時間
	{ 0x1413f93e8, u8"Preparation time before attack" }, // 攻撃前準備時間
	{ 0x1413f9508, u8"Continuous use limit [unlimited at 0]" }, // 連続使用制限[0で無制限]
	{ 0x1413f9600, u8"Reuse time after continuous use [Cannot be reused for more than 1000 seconds]" }, // 連続使用後の再使用時間[1000秒以上は再使用不能]
	{ 0x1413f96a0, u8"Counter attack flag [When false, it is not used as a counter]" }, // カウンター攻撃フラグ[false時はカウンターとして使用しない]
	{ 0x1413f9770, u8"Attack flag for counter only [When false, it is not used as a normal attack]" }, // カウンター時専用攻撃フラグ[false時は通常攻撃として使用しない]
	{ 0x1413f6aa8, u8"Action settings" }, // アクション設定
	{ 0x1413f6b48, u8"Immediate action list [Activate remaining HP]" }, // 即時アクション一覧[残HP発動]
	{ 0x1413f7978, u8"Remaining HP for counter attack after slamming [%]" }, // 叩きつけ後カウンター攻撃使用残HP[%]
	{ 0x1413f7a28, u8"Remaining HP for counter attack while surfacing [%]" }, // 浮上中カウンター攻撃使用残HP[%]
	{ 0x1413f6c90, u8"List of actions while surfacing Cyloop [Activate remaining HP]" }, // サイループ浮上中アクション一覧[残HP発動]
	{ 0x1413fab78, u8"Supply mode HP threshold [%]" }, // 補給モードHP閾値[%]
	{ 0x1413fabe8, u8"Laser attack type during supply" }, // 補給中のレーザー攻撃タイプ
	{ 0x1413f6d88, u8"Ring supply mode setting" }, // リング補給モード設定
	{ 0x1413f6e78, u8"Damage effect regeneration HP threshold" }, // ダメージエフェクト再生HP閾値
	{ 0x1413f75c0, u8"Super Sonic battle setting" }, // スパソニ戦設定
	{ 0x1413a4930, u8"Orbital movement speed" }, // 周回移動速度
	{ 0x1413faee0, u8"Standby (non-combat) state" }, // 待機(非戦闘)状態
	{ 0x14139bef8, u8"Movement interval (grind not reached)" }, // 移動間隔(グラインド未達)
	{ 0x14139bfb0, u8"Movement interval (slamming failure)" }, // 移動間隔(叩きつけ不発)
	{ 0x14139c088, u8"Movement interval (falling from the head)" }, // 移動間隔(頭から落下)
	{ 0x1413fd084, u8"Stand-by" }, // 待機
	{ 0x141392fb0, u8"Movement speed (average)" }, // 移動速度(平均)
	{ 0x141393080, u8"First movement angle minimum" }, // 1回目移動角度最小
	{ 0x141393158, u8"Maximum first movement angle" }, // 1回目移動角度最大
	{ 0x141393200, u8"Second movement angle minimum" }, // 2回目移動角度最小
	{ 0x1413932c8, u8"Second movement angle maximum" }, // 2回目移動角度最大
	{ 0x1413933a8, u8"Waiting time for second move" }, // 2回目移動待ち時間
	{ 0x14139cd98, u8"Axis alignment speed before hitting [°/s]" }, // 叩き付け前の軸合わせ速度[°/s]
	{ 0x14139ced0, u8"Slapping motion speed" }, // 叩き付けモーション速度
	{ 0x14139cfd8, u8"Waiting time after hitting" }, // 叩き付け後の待ち時間
	{ 0x14139d100, u8"(When grinding) Arm-raising start motion speed" }, // (グラインド時)腕上げ開始モーション速度
	{ 0x14139d1c8, u8"(When grinding) End motion speed of arm raising" }, // (グラインド時)腕上げ終了モーション速度
	{ 0x14139d2d0, u8"(Failure) Arm raising start motion speed" }, // (失敗時)腕上げ開始モーション速度
	{ 0x14139d400, u8"(Failure) End motion speed of arm raising" }, // (失敗時)腕上げ終了モーション速度
	{ 0x14139a358, u8"Validity time" }, // 有効時間
	{ 0x14139dcc8, u8"Lock-on distance" }, // ロックオン成立距離
	{ 0x14139dde0, u8"Lock-on release distance" }, // ロックオン解除距離
	{ 0x14139e220, u8"Spring constant of panning delay (default value with negative value)" }, // パンニング遅延のバネ定数(負値でデフォルト値)
	{ 0x14139e360, u8"Spring constant for short-range panning delay (default value if negative)" }, // 近距離パンニング遅延のバネ定数(負値でデフォルト値)
	{ 0x14139d540, u8"Looking up camera" }, // 見上げカメラ
	{ 0x14139e788, u8"Occurrence interval" }, // 発生間隔
	{ 0x14139e820, u8"The number of occurrences" }, // 発生数
	{ 0x14139e8c0, u8"Cool time" }, // クールタイム
	{ 0x14139e980, u8"Maximum radius" }, // 最大半径
	{ 0x14139e9e8, u8"Appearance radius" }, // 出現半径
	{ 0x14139eaa0, u8"Waiting radius" }, // 待機半径
	{ 0x14139eb28, u8"Annihilation radius" }, // 消滅半径
	{ 0x14139ebe0, u8"Minimum height" }, // 最小高さ
	{ 0x14139ecd8, u8"Maximum height" }, // 最大高さ
	{ 0x14139ef78, u8"Model installation radius" }, // モデル設置半径
	{ 0x14139f0b0, u8"Effect generation distance" }, // エフェクト生成距離
	{ 0x14139d5d8, u8"Thorn ripple" }, // トゲ波紋
	{ 0x14139afd8, u8"Slap" }, // 叩き付け
	{ 0x14139b720, u8"Camera (playable)" }, // カメラ(プレイアブル)
	{ 0x14139b820, u8"Player speed (during motion camera)" }, // プレイヤー速度(モーションカメラ中)
	{ 0x1413d4280, u8"Damage" }, // damage
	{ 0x1413a3cc0, u8"Blowoff" }, // blowOff
	{ 0x14139b940, u8"Trap" }, // トラップ
	{ 0x1413a3308, u8"Landing node name" }, // 着地点ノード名
	{ 0x1413a33c8, u8"Landing point offset (node reference)" }, // 着地点オフセット(ノード基準)
	{ 0x1413a34f8, u8"Raycast offset (node reference)" }, // レイキャストオフセット(ノード基準)
	{ 0x1413a35f8, u8"Correction after landing point raycast" }, // 着地点レイキャスト後補正
	{ 0x1413a24e8, u8"Gaze point offset (origin reference)" }, // 注視点オフセット(原点基準)
	{ 0x1413a2620, u8"Camera position offset (point of interest reference)" }, // カメラ位置オフセット(注視点基準)
	{ 0x14139a4b0, u8"Start interpolation time" }, // 開始補間時間
	{ 0x14139b998, u8"Automatic jump" }, // 自動ジャンプ
	{ 0x14139b0a0, u8"Grind" }, // グラインド
	{ 0x14139b110, u8"Climb time limit" }, // 登頂制限時間
	{ 0x14139f758, u8"[Tracking thorns] Speed" }, // [追従トゲ]速度
	{ 0x14139f848, u8"[Tracking thorns] Rotation speed [deg/sec]" }, // [追従トゲ]回転速度[deg/sec]
	{ 0x14139f948, u8"[Tracking thorns] Needle generation time" }, // [追従トゲ]針生成時間
	{ 0x14139fa30, u8"[Tracking thorns] Width" }, // [追従トゲ]幅
	{ 0x14139fae0, u8"[Tracking thorns] Judgment height" }, // [追従トゲ]判定高さ
	{ 0x14139fba8, u8"[Tracking thorns] Rising effect width [m]" }, // [追従トゲ]上昇演出幅[m]
	{ 0x14139fc20, u8"[Tracking thorns] Damage generation width [m]" }, // [追従トゲ]ダメージ発生幅[m]
	{ 0x14139fd00, u8"[Tracking thorns] Descending production width [m]" }, // [追従トゲ]下降演出幅[m]
	{ 0x14139fdf0, u8"[Tracking thorns] Thorn Scale" }, // [追従トゲ]トゲスケール
	{ 0x14139fed8, u8"[Tracking thorns] Thorn installation interval (horizontal)" }, // [追従トゲ]トゲ設置間隔(横)
	{ 0x14139ff90, u8"[Tracking thorns] Thorn installation interval (front and back)" }, // [追従トゲ]トゲ設置間隔(前後)
	{ 0x1413a0090, u8"[Tracking thorns] Distance from Sonic that generates collision" }, // [追従トゲ]コリジョン生成を行うソニックとの距離
	{ 0x14139b1c8, u8"Tracking thorns" }, // 追従トゲ
	{ 0x1413faf58, u8"Climb phase" }, // 登頂フェーズ
	{ 0x1413fcfa8, u8"Supasoni movement limit radius" }, // スパソニ移動制限半径
	{ 0x1413fed08, u8"Movement interval" }, // 移動間隔
	{ 0x1413fe930, u8"Shield ride transition fear value (guard)" }, // 盾ライド移行怯み値(ガード)
	{ 0x1413fea20, u8"Movement transition flinch value (damage taken)" }, // 移動移行怯み値(被ダメ)
	{ 0x1413fd130, u8"Guard" }, // ガード
	{ 0x1413a8e70, u8"Distance of starting point" }, // 開始地点の距離
	{ 0x1413a8f20, u8"Start point left/right offset" }, // 開始地点左右オフセット
	{ 0x1413a9050, u8"Starting point altitude offset" }, // 開始地点高度オフセット
	{ 0x1413a9178, u8"Starting angle (counterclockwise)" }, // 開始角度(反時計回り)
	{ 0x1413a9300, u8"Turning acceleration [°/s^2]" }, // 旋回加速度[°/s^2]
	{ 0x1413a9418, u8"Maximum turning speed [°/s]" }, // 最大旋回速度[°/s]
	{ 0x1413a9518, u8"Moving area radius" }, // 移動エリア半径
	{ 0x1413a9650, u8"Minimum rolling strength" }, // 横揺れ強さ最小
	{ 0x1413a96e0, u8"Maximum rolling strength" }, // 横揺れ強さ最大
	{ 0x1413a9760, u8"Minimum roll duration" }, // 横揺れ継続時間最小
	{ 0x1413a98d0, u8"Maximum rolling duration" }, // 横揺れ継続時間最大
	{ 0x1413a99d8, u8"Rolling interval" }, // 横揺れ発生間隔
	{ 0x1413a9ad8, u8"Flight camera elevation angle" }, // 飛行カメラ仰角
	{ 0x1413a9be8, u8"Flight camera distance" }, // 飛行カメラ距離
	{ 0x1413a9d00, u8"Flight camera gaze point offset" }, // 飛行カメラ注視点オフセット
	{ 0x1413a9e10, u8"Tsuba approaching camera elevation angle" }, // 鍔迫り合いカメラ仰角
	{ 0x1413a9ed8, u8"Tsuba approaching camera azimuth" }, // 鍔迫り合いカメラ方位角
	{ 0x1413a9fc8, u8"Tsuba approaching camera distance" }, // 鍔迫り合いカメラ距離
	{ 0x1413aa100, u8"Hit camera elevation angle" }, // ヒットカメラ仰角
	{ 0x1413aa228, u8"Hit camera azimuth" }, // ヒットカメラ方位角
	{ 0x1413aa370, u8"Hit camera distance" }, // ヒットカメラ距離
	{ 0x1413aa470, u8"Continuous hit camera shake amplitude size" }, // 連続ヒットカメラシェイク振幅の大きさ
	{ 0x1413aa590, u8"Continuous hit camera shake frequency" }, // 連続ヒットカメラシェイク周波数
	{ 0x1413aa6c8, u8"Size of the camera shake amplitude when the tsuba approaches each other" }, // 鍔迫り合いカメラシェイク振幅の大きさ
	{ 0x1413aa758, u8"Tsuba approaching camera shake frequency" }, // 鍔迫り合いカメラシェイク周波数
	{ 0x1413aa870, u8"Number of simultaneous missile launches" }, // ミサイル同時発射数
	{ 0x1413aaac8, u8"First missile launch time" }, // ミサイル初弾発射時間
	{ 0x1413aac10, u8"Missile flight speed" }, // ミサイル飛行速度
	{ 0x1413aace0, u8"Missile rotation speed" }, // ミサイル旋回速度
	{ 0x1413aada8, u8"Missile midpoint distance" }, // ミサイル中間点距離
	{ 0x1413aae88, u8"Missile waypoint altitude offset" }, // ミサイル中間点高度オフセット
	{ 0x1413aaf90, u8"Missile launch angle" }, // ミサイル発射角度
	{ 0x1413ab018, u8"Missile launch wait time" }, // ミサイル発射開始待機時間
	{ 0x1413ab0d0, u8"Missile rise speed" }, // ミサイル上昇速度
	{ 0x1413ab1a0, u8"Missile rise time" }, // ミサイル上昇時間
	{ 0x1413ab210, u8"Missile hidden time" }, // ミサイル非表示時間
	{ 0x1413ab2b8, u8"Missile appearance distance" }, // ミサイル出現距離
	{ 0x1413ab3a0, u8"Missile appearance height" }, // ミサイル出現高さ
	{ 0x1413ab438, u8"Missile appearance left/right swing width" }, // ミサイル出現左右振れ幅
	{ 0x1413ab4d0, u8"Survival time after missile appears" }, // ミサイル出現後生存時間
	{ 0x1413ab5d8, u8"Survival time after missile appears (unhit)" }, // ミサイル出現後生存時間（未ヒット）
	{ 0x1413ab6c0, u8"Missile hit power" }, // ミサイルヒットパワー
	{ 0x1413ab810, u8"Missile hit power reduction rate" }, // ミサイルヒットパワー減少率
	{ 0x1413ab938, u8"Missile hit power duration" }, // ミサイルヒットパワー持続時間
	{ 0x1413aba08, u8"Maximum missile height fluctuation" }, // ミサイル高さブレ最大値
	{ 0x1413abae8, u8"Minimum missile height fluctuation" }, // ミサイル高さブレ最小値
	{ 0x1413abbd8, u8"Missile side shake" }, // ミサイル横ブレ
	{ 0x1413abca0, u8"Tsuba approaching time" }, // 鍔迫り合い時間
	{ 0x1413abd58, u8"Total damage dealt (maximum HP ratio)" }, // 削り合計与ダメ(最大HP比)
	{ 0x1413abe90, u8"Number of scraping hits" }, // 削りヒット数
	{ 0x1413abfe0, u8"Scraping hit interval" }, // 削りヒット間隔
	{ 0x1413ac170, u8"Final hit damage (maximum HP ratio)" }, // 最終段与ダメ(最大HP比)
	{ 0x1413ac2a0, u8"Final stage wince" }, // 最終段与ひるみ
	{ 0x1413ac3c0, u8"Final knockback distance" }, // 最終段ノックバック距離
	{ 0x1413ac4d0, u8"Recovery time after hit" }, // ヒット後硬直時間
	{ 0x1413ac618, u8"Missile hit pattern" }, // ミサイルヒットパターン
	{ 0x1413fd1e0, u8"Shield ride" }, // 盾ライド
	{ 0x141392848, u8"Rigidity time when parrying" }, // パリィ時硬直時間
	{ 0x141392a38, u8"Camera offset distance" }, // カメラオフセット距離
	{ 0x141392b28, u8"Camera fade time" }, // カメラフェード時間
	{ 0x1413fca70, u8"Interrupt attack" }, // 割り込み攻撃
	{ 0x1413fd2c8, u8"Normal knockback distance" }, // 通常ノックバック距離
	{ 0x1413fdfac, u8"Altitude" }, // 高度
	{ 0x1413fe070, u8"Rise time" }, // 上昇時間
	{ 0x1413fe128, u8"Floating time" }, // 浮遊維持時間
	{ 0x1413fe1d0, u8"Falling time" }, // 落下時間
	{ 0x1413fe2f0, u8"Slamming fall time" }, // 叩きつけ落下時間
	{ 0x1413fe3c0, u8"Slam delay" }, // 叩きつけ遅延
	{ 0x1413fe498, u8"Down time" }, // ダウン時間
	{ 0x1413fe510, u8"Slam counter effective HP ratio" }, // 叩きつけカウンター有効HP比
	{ 0x1413fe5c8, u8"Air guard effective HP ratio" }, // 空中ガード有効HP比
	{ 0x1413fd3b8, u8"Arial" }, // エリアル
	{ 0x1413fd488, u8"Ring supply" }, // リング供給
	{ 0x1413a60b8, u8"Hit effect scale" }, // ヒットエフェクトスケール
	{ 0x1413a61e8, u8"Button guide X coordinate offset" }, // ボタンガイドX座標オフセット
	{ 0x1413a62c8, u8"Button guide Y coordinate offset" }, // ボタンガイドY座標オフセット
	{ 0x1413a63a0, u8"Emergence altitude offset" }, // 出現高度オフセット
	{ 0x1413a64c0, u8"Normal flight speed" }, // 通常飛行速度
	{ 0x1413a6538, u8"Normal flight prediction line width" }, // 通常飛行予測線幅
	{ 0x1413a65d8, u8"Normal flight predicted line length (after wall reflection)" }, // 通常飛行予測線長さ(壁反射後)
	{ 0x1413a6718, u8"Normal flight forecast line display time" }, // 通常飛行予測線表示時間
	{ 0x1413a67e8, u8"Wall reflection direction correction initial value" }, // 壁反射方向補正初期値
	{ 0x1413a68a8, u8"Wall reflection direction correction addition value (wall hit)" }, // 壁反射方向補正加算値(壁hit)
	{ 0x1413a6938, u8"Wall reflection direction correction subtraction value (sonic hit)" }, // 壁反射方向補正減算値(ソニックhit)
	{ 0x1413a69d0, u8"Wall reflection direction correction" }, // 壁反射方向補正
	{ 0x1413a6a90, u8"Wall reflection direction correction cycle [every n times]" }, // 壁反射方向補正周期[n回ごと]
	{ 0x1413a6ba0, u8"Wall reflection direction lottery radius" }, // 壁反射方向抽選半径
	{ 0x1413a6c80, u8"Wall reflection stop time" }, // 壁反射停止時間
	{ 0x1413a6d60, u8"Flight speed immediately after wall reflection" }, // 壁反射直後の飛行速度
	{ 0x1413a6e30, u8"Acceleration start time after wall reflection" }, // 壁反射後の加速開始時間
	{ 0x1413a6f08, u8"Acceleration time from wall reflection to normal flight speed" }, // 壁反射から通常飛行速度への加速時間
	{ 0x1413a6fe8, u8"Parry aiming angle" }, // パリィ照準角度
	{ 0x1413a7110, u8"Parry aiming angular velocity [deg/s]" }, // パリィ照準角速度[deg/s]
	{ 0x1413a71f0, u8"Parry aiming time limit" }, // パリィ照準制限時間
	{ 0x1413a7268, u8"Parry aiming line width" }, // パリィ照準ライン幅
	{ 0x1413a7360, u8"Parry aiming line length" }, // パリィ照準ライン長さ
	{ 0x1413a7430, u8"Flight speed immediately after parry" }, // パリィ直後の飛行速度
	{ 0x1413a7528, u8"Maximum flight speed after parry" }, // パリィ後最高飛行速度
	{ 0x1413a7620, u8"Acceleration start time after parry" }, // パリィ後の加速開始時間
	{ 0x1413a76c0, u8"Acceleration time to maximum flight speed after parry" }, // パリィ後最高飛行速度への加速時間
	{ 0x1413a7800, u8"Predicted flight line width after parry" }, // パリィ後飛行予測線幅
	{ 0x1413a7928, u8"Flight prediction line display time after parry" }, // パリィ後飛行予測線表示時間
	{ 0x1413a7a80, u8"Post-parry time stop release time" }, // パリィ後時間停止解除時間
	{ 0x1413a7b60, u8"Time stop release distance after parry (distance from boss)" }, // パリィ後時間停止解除距離(ボスからの距離)
	{ 0x1413a7c18, u8"Camera after parry" }, // パリィ後カメラ
	{ 0x1413a7cd8, u8"Damage when hitting boss (maximum HP ratio)" }, // ボス命中時ダメージ(最大HP比)
	{ 0x1413a7e90, u8"Blow-off power when hitting player" }, // プレイヤー命中時吹き飛び力
	{ 0x141393844, u8"Shield" }, // 盾
	{ 0x1413a5400, u8"Player following strength" }, // プレイヤー追従強さ
	{ 0x1413a54d0, u8"Alignment strength" }, // 軸合わせ強さ
	{ 0x1413a5668, u8"Operation angular acceleration [°/s^2]" }, // 操作角加速度[°/s^2]
	{ 0x1413a56b0, u8"Maximum operating angular velocity [°/s]" }, // 操作角速度最大[°/s]
	{ 0x1413938f0, u8"Normal overhead camera" }, // 通常俯瞰カメラ
	{ 0x141393a18, u8"Overhead camera for aiming" }, // 照準用俯瞰カメラ
	{ 0x141393aa8, u8"Movement restart attack first half motion speed" }, // 移動再開攻撃前半モーション速度
	{ 0x1413fd5d8, u8"After going crazy" }, // 発狂後
	{ 0x1413fd648, u8"Madness HP [%]" }, // 発狂HP[%]
	{ 0x1413fd6c8, u8"Special move activation HP [%]" }, // 必殺技発動HP[%]
	{ 0x1413fc120, u8"Slapping camera time" }, // 叩きつけカメラ時間
	{ 0x1413fc278, u8"Slamming camera cut switching time" }, // 叩きつけカメラカット切替時間
	{ 0x1413fc380, u8"Slamming camera offset" }, // 叩きつけカメラオフセット
	{ 0x1413fc490, u8"Rising camera time" }, // 上昇カメラ時間
	{ 0x1413fc520, u8"Rising camera offset" }, // 上昇カメラオフセット
	{ 0x1413fd7e0, u8"Aerial camera" }, // エリアルカメラ
	{ 0x1413fd8b0, u8"Wall slam event start time" }, // 壁叩きつけられイベント開始時間
	{ 0x1413fb008, u8"Combat phase 1" }, // 戦闘フェーズ1
	{ 0x1413959e0, u8"Speed" }, // 速さ
	{ 0x141395ae0, u8"Left/right movement range" }, // 左右移動範囲
	{ 0x141395bc0, u8"Vertical movement range" }, // 上下移動範囲
	{ 0x141395ca8, u8"Player position [normal]" }, // プレイヤー位置[通常]
	{ 0x141395dd0, u8"Player position [remote]" }, // プレイヤー位置[遠隔]
	{ 0x141395e98, u8"Player position [proximity]" }, // プレイヤー位置[近接]
	{ 0x141395f78, u8"Player position [rush]" }, // プレイヤー位置[突進]
	{ 0x1413961a8, u8"Attack interval delay on hit" }, // ヒット時の攻撃間隔遅延
	{ 0x1413962d0, u8"Camera [normal]" }, // カメラ[通常]
	{ 0x141393e6c, u8"Running" }, // 走行
	{ 0x1413998b0, u8"Left/right (x) setting type" }, // 左右(x)設定タイプ
	{ 0x1413999f8, u8"Left and right (x) individual setting values" }, // 左右(x)個別設定値
	{ 0x141399b88, u8"Altitude (y)" }, // 高度(y)
	{ 0x141399d28, u8"Distance (z)" }, // 距離(z)
	{ 0x1413969a0, u8"[Straight line] spline node settings" }, // [直線]スプラインノード設定
	{ 0x141396a20, u8"[Straight line] [Ratio] Pause point" }, // [直線][比]一時停止地点
	{ 0x141396b30, u8"[Straight line] [Ratio] Pause deceleration start point" }, // [直線][比]一時停止減速開始地点
	{ 0x141396c58, u8"[Straight line] [Ratio] Pause acceleration completion point" }, // [直線][比]一時停止加速完了地点
	{ 0x141396d48, u8"[Straight line] Pause time" }, // [直線]一時停止時間
	{ 0x141396e68, u8"[Straight line] Speed before pause" }, // [直線]一時停止前速度
	{ 0x141396fb8, u8"[Straight line] Speed after pause" }, // [直線]一時停止後速度
	{ 0x1413970f8, u8"[Straight line] Left and right position at time of hit" }, // [直線]命中時左右位置
	{ 0x1413971e8, u8"[Straight line] [Ratio] U-turn return point" }, // [直線][比]Uターン復帰地点
	{ 0x1413972a8, u8"[Straight line] Sword attack delay" }, // [直線]剣攻撃ディレイ
	{ 0x1413973a8, u8"[Slalom] Spline node settings" }, // [スラローム]スプラインノード設定
	{ 0x1413974d0, u8"[Slalom] [Ratio] Pause point" }, // [スラローム][比]一時停止地点
	{ 0x1413975d0, u8"[Slalom] [Ratio] Pause deceleration start point" }, // [スラローム][比]一時停止減速開始地点
	{ 0x141397688, u8"[Slalom] [Ratio] Pause acceleration completion point" }, // [スラローム][比]一時停止加速完了地点
	{ 0x141397758, u8"[Slalom] Pause time" }, // [スラローム]一時停止時間
	{ 0x141397800, u8"[Slalom] Speed before pausing" }, // [スラローム]一時停止前速度
	{ 0x1413978f8, u8"[Slalom] Speed after pause" }, // [スラローム]一時停止後速度
	{ 0x141397a30, u8"[Slalom] Left and right position when hit" }, // [スラローム]命中時左右位置
	{ 0x141397b38, u8"[Slalom] [Ratio] U-turn return point" }, // [スラローム][比]Uターン復帰地点
	{ 0x141397c58, u8"[Slalom] Amplitude (farthest)" }, // [スラローム]振幅(最遠)
	{ 0x141397d48, u8"[Slalom] Amplitude (recent)" }, // [スラローム]振幅(最近)
	{ 0x141397dc0, u8"[Slalom] Minimum number of slalom times" }, // [スラローム]スラローム回数最小
	{ 0x141397e58, u8"[Slalom] Maximum number of slalom times" }, // [スラローム]スラローム回数最大
	{ 0x141397f00, u8"[Slalom] [Ratio] Slalom end point" }, // [スラローム][比]スラローム終了地点
	{ 0x141398030, u8"[Slalom] Sword attack delay" }, // [スラローム]剣攻撃ディレイ
	{ 0x141398178, u8"[Common] U-turn speed magnification (during camera direction)" }, // [共通]Uターン速度倍率(カメラ演出中)
	{ 0x141398200, u8"[Common] U-turn speed magnification (after camera ends)" }, // [共通]Uターン速度倍率(カメラ終了後)
	{ 0x1413982e0, u8"[Common] U-turn camera" }, // [共通]Uターンカメラ
	{ 0x141398390, u8"[Common] Orbit prediction line width" }, // [共通]軌道予測線幅
	{ 0x141393f48, u8"Shield attack" }, // 盾攻撃
	{ 0x14139a8d8, u8"Motion speed after double slash parry" }, // 2連斬りパリィ後モーション速度
	{ 0x141394010, u8"Sword attack" }, // 剣攻撃
	{ 0x1413941a0, u8"Knockback distance" }, // ノックバック距離
	{ 0x1413fb038, u8"Combat phase 2" }, // 戦闘フェーズ2
	{ 0x1413adee8, u8"Damage dealt (maximum HP ratio)" }, // 与ダメ(最大HP比)
	{ 0x1413fb0d0, u8"Sonic special move" }, // ソニック必殺技
	{ 0x1413a0920, u8"Appearance position offset (top)" }, // 出現位置オフセット(上)
	{ 0x1413a09f0, u8"Appearance angle (top) (set x, z)" }, // "出現角度(上)(x,zを設定)"
	{ 0x1413a0aa0, u8"Travel distance offset (top)" }, // 移動距離オフセット(上)
	{ 0x1413a0bd0, u8"Impact position offset (bottom)" }, // 着弾位置オフセット(下)
	{ 0x1413a0d40, u8"Movement start delay (bottom)" }, // 移動開始ディレイ(下)
	{ 0x1413a0dd8, u8"Travel distance (bottom)" }, // 移動距離(下)
	{ 0x1413a0e70, u8"Travel time (bottom)" }, // 移動時間(下)
	{ 0x1413fb178, u8"Arial spear" }, // エリアル槍
	{ 0x1413fb250, u8"Number of flinches that result in knockback" }, // ノックバックになるひるみ回数
	{ 0x1413fbac8, u8"Practice notification settings [climb]" }, // 練習通知設定[登頂]
	{ 0x1413fbbb0, u8"Practice notification settings [Battle 1]" }, // 練習通知設定[戦闘1]
	{ 0x1413fbcf8, u8"Practice notification settings [Battle 2]" }, // 練習通知設定[戦闘2]
	{ 0x1413aedd8, u8"Wave 1 name [valid only during bit attack]" }, // Wave1名[ビット攻撃時のみ有効]
	{ 0x1413aeec8, u8"Wave 2 name [effective only during bit attack]" }, // Wave2名[ビット攻撃時のみ有効]
	{ 0x1413af038, u8"Action time [Reflected when selecting standby etc.]" }, // アクション時間[待機など選択時に反映]
	{ 0x1413af180, u8"Next [1: Go back one step -1: Go back one step]" }, // 次へ[1:一つ後に進む -1:一つ前に戻る]
	{ 0x1413af2a0, u8"Bit attack reset" }, // ビット攻撃のリセット
	{ 0x1413af398, u8"Floating movement direction reversal" }, // 浮遊移動方向反転
	{ 0x1413af428, u8"Floating movement stop" }, // 浮遊移動停止
	{ 0x1413afac0, u8"Action list [Run in order]" }, // アクション一覧[順番に実行]
	{ 0x1413afc48, u8"List of actions when bits are destroyed [performed in order]" }, // ビット破壊時のアクション一覧[順番に実行]
	{ 0x1413afd88, u8"Required number of bits destroyed" }, // 必要ビット破壊数
	{ 0x1413b04e8, u8"Way angle" }, // Way角度
	{ 0x1413b05d8, u8"Number of ways" }, // Way数
	{ 0x1413b0720, u8"Distance limit" }, // 距離限界
	{ 0x1413b0800, u8"Blowing force at the time of contact" }, // 接触時の吹き飛ばし力
	{ 0x1413b0898, u8"Invalid operation time upon contact" }, // 接触時の操作無効時間
	{ 0x1413b0958, u8"Speed maintenance time during contact" }, // 接触時の速度維持時間
	{ 0x1413b1270, u8"Occurrence interval [distance]" }, // 発生間隔[距離]
	{ 0x1413b1390, u8"Occurrence delay [delay after collision is enabled]" }, // 発生遅延[コリジョン有効化後からの遅延]
	{ 0x1413b1490, u8"Starting position" }, // 起点位置
	{ 0x1413b1560, u8"End position" }, // 終点位置
	{ 0x1413b1620, u8"Orientation of starting point [Adjustment of curvature]" }, // 起点の向き[湾曲具合の調整]
	{ 0x1413b1738, u8"End point direction [curvature adjustment]" }, // 終点の向き[湾曲具合の調整]
	{ 0x1413b0a80, u8"For left-handed punching" }, // 左殴り用
	{ 0x1413b0b00, u8"For right-handed punching" }, // 右殴り用
	{ 0x1413b1e38, u8"Bullet scale value" }, // 弾のスケール値
	{ 0x1413b1fb8, u8"Time to reach target upon successful parry" }, // パリィ成功時に目標に到達する時間
	{ 0x1413b2a70, u8"Launch location node" }, // 発射位置ノード
	{ 0x1413b2b70, u8"Launch position adjustment" }, // 発射位置調整
	{ 0x1413b2cc0, u8"Front direction of launch position node [Basic X + Settings]" }, // 発射位置ノードの正面方向[基本X＋設定]
	{ 0x1413b2da8, u8"Axis of launch position node [for setting angle]" }, // 発射位置ノードの軸[角度設定時用]
	{ 0x1413b2ea8, u8"Straight ahead time [immediately after launch]" }, // 直進時間[発射直後]
	{ 0x1413b2fb0, u8"Maximum possible number of parries" }, // パリィ可能数最大値
	{ 0x1413b3060, u8"Number of simultaneous shots" }, // 同時発射数
	{ 0x1413b3160, u8"Number of shots" }, // 発射数回数
	{ 0x1413b3388, u8"Acceleration start delay time" }, // 加速開始遅延時間
	{ 0x1413b3420, u8"Acceleration time [initial velocity → speed]" }, // 加速時間[初速→速度]
	{ 0x1413b3670, u8"Firing interval [individual setting]" }, // 発射間隔[個別設定]
	{ 0x1413b3790, u8"Launch delay" }, // 発射遅延
	{ 0x1413b5340, u8"Parry flag" }, // パリィフラグ
	{ 0x1413b5478, u8"Launch direction angle adjustment [elevation angle]" }, // 発射方向角度調整[仰角]
	{ 0x1413b55b0, u8"Launch direction angle adjustment [rotation]" }, // 発射方向角度調整[回転]
	{ 0x1413b3910, u8"Setting each firing point" }, // 各発射ポイントの設定
	{ 0x1413b3aa0, u8"Camera viewpoint coordinates [local coordinates of boss]" }, // カメラ視点座標[ボスのローカル座標]
	{ 0x1413b3c50, u8"Camera gaze point coordinates [local coordinates of boss]" }, // カメラ注視点座標[ボスのローカル座標]
	{ 0x1413b3d38, u8"Camera start delay time" }, // カメラ開始遅延時間
	{ 0x1413b3df0, u8"Camera maintenance time" }, // カメラ維持時間
	{ 0x1413b3f00, u8"Camera interpolation time [start]" }, // カメラ補間時間[開始]
	{ 0x1413b4058, u8"Camera interpolation time [end]" }, // カメラ補間時間[終了]
	{ 0x1413b4160, u8"Camera viewpoint coordinates [player position reference for parry]" }, // カメラ視点座標[パリィ用プレイヤー位置基準]
	{ 0x1413b4280, u8"Camera gaze point coordinates [player position reference for parry]" }, // カメラ注視点座標[パリィ用プレイヤー位置基準]
	{ 0x1413b44d8, u8"Slow setting when parrying" }, // パリィ時のスロー設定
	{ 0x1413b2138, u8"Pursuit laser firing settings" }, // 追撃用レーザー発射設定
	{ 0x1413b2268, u8"Pursuit laser firing setting [when floating]" }, // 追撃用レーザー発射設定[浮遊時]
	{ 0x1413b2358, u8"Laser firing settings for normal use [camera not used]" }, // 通常時用レーザー発射設定[カメラは不使用]
	{ 0x1413b8878, u8"Interpolation time [at start]" }, // 補間時間[開始時]
	{ 0x1413b89a8, u8"Interpolation time [at end]" }, // 補間時間[終了時]
	{ 0x1413b8ae0, u8"Addition distance for each parameter [minimum]" }, // 各パラメータ加算距離[最小]
	{ 0x1413b8c18, u8"Addition distance for each parameter [maximum]" }, // 各パラメータ加算距離[最大]
	{ 0x1413b8e20, u8"Addition distance" }, // 加算距離
	{ 0x1413b8fd0, u8"Addition angle" }, // 加算角度
	{ 0x1413b9130, u8"Additional height adjustment" }, // 加算高さ調整
	{ 0x1413b5b20, u8"Various settings [normal]" }, // 各種設定[通常時]
	{ 0x1413b5d00, u8"Various settings [when floating]" }, // 各種設定[浮遊時]
	{ 0x1413b63a8, u8"Height when floating" }, // 浮遊時の高さ
	{ 0x1413b64e8, u8"Turning speed when floating" }, // 浮遊時の旋回速度
	{ 0x1413b6610, u8"Speed reduction rate when the bit is attacked while floating [1.0 is no deceleration]" }, // 浮遊時にビットが攻撃された際の速度減少率[1.0が減速無し]
	{ 0x1413b6750, u8"Speed reduction time when bit is attacked while floating" }, // 浮遊時にビットが攻撃された際の速度減少時間
	{ 0x1413b6888, u8"Movement speed when floating" }, // 浮遊時の移動速度
	{ 0x1413b69d0, u8"Movement radius when floating [For default, give priority when specifying a path]" }, // 浮遊時の移動半径[デフォルト時用、パス指定時はそちらを優先]
	{ 0x1413bbeb0, u8"Pattern list [Depends on remaining HP]" }, // パターン一覧[残HP依存]
	{ 0x1413bc018, u8"HP threshold at which the pattern switches [100 to 0]" }, // パターンが切り替わるHPの閾値[100～0]
	{ 0x1413b75d0, u8"Homing laser attack setting" }, // ホーミングレーザー攻撃設定
	{ 0x1413b76e8, u8"Discrepancy laser attack setting during punch attack" }, // 殴り攻撃時のバラマキレーザー攻撃設定
	{ 0x1413b7788, u8"Floating settings" }, // 浮遊設定
	{ 0x1413ba128, u8"Continuous firing homing laser parry" }, // 連続発射ホーミングレーザーパリィ時
	{ 0x1413ba1d8, u8"At the final homing laser parry of continuous firing" }, // 連続発射最後のホーミングレーザーパリィ時
	{ 0x1413ba2a0, u8"At the time of simultaneous firing homing laser parry" }, // 一斉発射ホーミングレーザーパリィ時
	{ 0x1413b7960, u8"Waiting time when QTE is successful" }, // QTE成功時の待機時間
	{ 0x1413f7438, u8"IsDebugDraw" }, // isDebugDraw
	{ 0x1413b7c18, u8"Boss battle skip [Sequence forced passage compatible]" }, // ボス戦スキップ[シーケンス強制通過用対応]
	{ 0x1413b9db8, u8"Practice notification settings [at the start of phase 1]" }, // 練習通知設定[フェーズ1開始時]
	{ 0x1413b9ec8, u8"Practice notification settings [at the start of Phase 2]" }, // 練習通知設定[フェーズ2開始時]
	{ 0x1413bc9a0, u8"Left and right firing angle" }, // 左右の発射角度
	{ 0x1413bcad8, u8"Anteroposterior firing angle" }, // 前後の発射角度
	{ 0x1413bcc58, u8"Mountain shape" }, // 山なり具合
	{ 0x1413bcd60, u8"Mountain position" }, // 山になる位置
	{ 0x1413dc9f8, u8"Camera gaze point offset (boss origin)" }, // カメラ注視点オフセット(ボス原点)
	{ 0x1413dcb18, u8"Camera position offset (boss origin)" }, // カメラ位置オフセット(ボス原点)
	{ 0x1413ccb08, u8"Camera angle of view" }, // カメラ画角
	{ 0x1413bd678, u8"Attack range setting" }, // 攻撃範囲設定
	{ 0x1413bd788, u8"Time to flinch after a successful parry" }, // パリィ成功後ひるみ時間
	{ 0x1413bd938, u8"Wait time until next attack when parry fails" }, // パリィ失敗時の次の攻撃までの待機時間
	{ 0x1413bdaf0, u8"Player fixed position when attacking [Boss standard]" }, // 攻撃時プレイヤー固定位置[ボス基準]
	{ 0x1413bdc60, u8"Waiting time for state movement upon successful parry" }, // パリィ成功時のステート移動の待機時間
	{ 0x1413bddf0, u8"Should I slow down when parrying?" }, // パリィしたときスローにするか
	{ 0x1413dd1a8, u8"Parry time after QTE" }, // QTE後のパリーの時間
	{ 0x1413c0110, u8"Wait time until the next bullet is fired. Cannot be used when firing continuously." }, // 次の弾を撃つまでの待機時間 連続発射の場合は使用不可
	{ 0x1413c0210, u8"Firing arm" }, // 発射する腕
	{ 0x1413c02f0, u8"Will the next bullet be fired in succession?" }, // 次の弾を連続で発射するか
	{ 0x1413c03c0, u8"Plus bullet velocity" }, // プラスする弾の速度
	{ 0x1413c0490, u8"Should I use a special camera when parrying?" }, // パリィ時に特別なカメラを使用するか
	{ 0x1413c0560, u8"Do you want to increase the playback speed of the motion?" }, // モーションの再生速度を上げるか
	{ 0x1413c1018, u8"Bullet behavior" }, // 弾の挙動
	{ 0x1413c1128, u8"Number of bullets fired" }, // 発射する弾の数
	{ 0x1413c1240, u8"Camera specification" }, // カメラの指定
	{ 0x1413c1328, u8"Camera end time" }, // カメラ終了時間
	{ 0x1413c13e8, u8"Camera advance start time" }, // カメラ先行開始時間
	{ 0x1413c9ab0, u8"Homing laser pattern" }, // ホーミングレーザーパターン
	{ 0x1413dd378, u8"Wait time before firing the first laser" }, // 最初のレーザーを撃つまでの待機時間
	{ 0x1413dd470, u8"Waiting time before firing the homing laser again" }, // 再度ホーミングレーザーを撃つまでの待機時間
	{ 0x1413dd5e8, u8"Player position upon successful QTE" }, // QTE成功時のプレイヤーの位置
	{ 0x1413bdf10, u8"Parameters after QTE" }, // QTE後のパラメーター
	{ 0x1413be7c8, u8"Size of bullet hit detection" }, // 弾の当たり判定の大きさ
	{ 0x1413bea28, u8"Damage caused by being hit by a bullet on Easy." }, // イージーでの弾を食らった時のダメージ.
	{ 0x1413bebe8, u8"Damage caused by being hit by a bullet in normal mode." }, // ノーマルでの弾を食らった時のダメージ.
	{ 0x1413beda8, u8"Damage caused by being hit by a bullet on hard." }, // ハードでの弾を食らった時のダメージ.
	{ 0x1413bef48, u8"Damage when hit by a bullet in Very Hard." }, // ベリーハードでの弾を食らった時のダメージ.
	{ 0x1413bf0d0, u8"Value of damage to enemy." }, // 敵へのダメージの値.
	{ 0x1413bf300, u8"Blow duration" }, // 吹き飛ばし持続時間
	{ 0x1413bf4b0, u8"Number of rings to set when hit by Genkidama" }, // 元気玉被弾時にセットするリング枚数
	{ 0x1413bf598, u8"Homing performance maximum [100]" }, // ホーミング性能 最大[100]
	{ 0x1413bf658, u8"Homing laser settings for each arm" }, // 腕毎のホーミングレーザー設定
	{ 0x1413bf788, u8"Motion playback speed" }, // モーションの再生速度
	{ 0x1413c2238, u8"Attack combination number" }, // 攻撃の組み合わせ番号
	{ 0x1413c2358, u8"Number of attacks" }, // 攻撃の数
	{ 0x1413c2458, u8"Will it be used for the lottery?" }, // 抽選に使用するか
	{ 0x1413c2a20, u8"Various settings [for homing laser]" }, // 各種設定[ホーミングレーザー時]
	{ 0x1413c3050, u8"Restraint time" }, // 拘束時間
	{ 0x1413c31d0, u8"Press time damage [maximum HP ratio]" }, // プレス時与ダメ[最大HP比]
	{ 0x1413c32e0, u8"Player altitude" }, // プレイヤー高度
	{ 0x1413c3400, u8"Press start delay" }, // プレス開始遅延
	{ 0x1413c3498, u8"Camera maintenance time when counter attack blows away" }, // カウンター攻撃吹き飛び時カメラ維持時間
	{ 0x1413c3608, u8"Counter attack starting damage value" }, // カウンター攻撃開始ダメージ値
	{ 0x1413c37b0, u8"Starting player warp position [based on boss]" }, // 開始時プレイヤーワープ位置[ボス基準]
	{ 0x1413c3940, u8"Do you use controller vibration when pressing?" }, // プレス時コントローラー振動を使用するか
	{ 0x1413c3a88, u8"Controller vibration name during press" }, // プレス時コントローラ振動名
	{ 0x1413c3c80, u8"Player position when pressing [Boss standard]" }, // プレス時プレイヤー位置[ボス基準]
	{ 0x1413cbc00, u8"Name of node to focus on [Origin of object if not set]" }, // 注視するノード名[未設定時はオブジェクトの原点]
	{ 0x1413ca6b4, u8"Posture" }, // 姿勢
	{ 0x1413cbf38, u8"Gaze point position offset [node position reference]" }, // 注視点位置オフセット[ノード位置基準]
	{ 0x1413cc060, u8"Camera position offset [node position reference]" }, // カメラ位置オフセット[ノード位置基準]
	{ 0x1413cc170, u8"Start delay" }, // 開始時遅延
	{ 0x1413cc338, u8"Start interpolation time" }, // 開始時補間時間
	{ 0x1413cc438, u8"Interpolation time at end" }, // 終了時補間時間
	{ 0x1413c9f00, u8"Start production camera" }, // 開始演出カメラ
	{ 0x1413ca368, u8"Gaze point position offset [Boss reference]" }, // 注視点位置オフセット[ボス基準]
	{ 0x1413ca478, u8"Camera position offset [Boss reference]" }, // カメラ位置オフセット[ボス基準]
	{ 0x1413c8500, u8"Fixed camera during press" }, // プレス時固定カメラ
	{ 0x1413caca0, u8"Enable camera vibration" }, // カメラ振動を有効にするか
	{ 0x1413cada8, u8"Camera vibration standby time" }, // カメラ振動待機時間
	{ 0x1413caed8, u8"Magnitude of vibration" }, // 振動の大きさ
	{ 0x1413cb100, u8"Vibration damping rate [rate that decreases according to remaining time]" }, // 振動の減衰率[残り時間に応じて小さくなる割合]
	{ 0x1413cb270, u8"Frequency [how many times it vibrates per second]" }, // 周波数[1秒間に何回振動するか]
	{ 0x1413c8628, u8"Camera vibration when pressing" }, // プレス時カメラ振動
	{ 0x1413c3ef0, u8"Duration of camera to blow away when counter starts" }, // カウンター開始時に吹き飛ばすカメラの持続時間
	{ 0x1413c40a8, u8"Laser hit detection thickness" }, // レーザーの当たり判定の太さ
	{ 0x1413c4290, u8"Laser hit detection length" }, // レーザーの当たり判定の長さ
	{ 0x1413c43e8, u8"Blowout camera duration" }, // 吹き飛ばしカメラの持続時間
	{ 0x1413c4520, u8"Player warp position when countering [Boss standard]" }, // カウンター時のプレイヤーワープ位置[ボス基準]
	{ 0x1413cc8d0, u8"Camera gaze point offset (Sonic origin)" }, // カメラ注視点オフセット(ソニック原点)
	{ 0x1413cc9f0, u8"Camera position offset (sonic origin)" }, // カメラ位置オフセット(ソニック原点)
	{ 0x1413c4730, u8"Camera for counter" }, // カウンター用カメラ
	{ 0x1413cb728, u8"Upper effect offset [Boss reference]" }, // 上エフェクトのオフセット[ボス基準]
	{ 0x1413cb7e8, u8"Lower effect offset [Boss reference]" }, // 下エフェクトのオフセット[ボス基準]
	{ 0x1413c4808, u8"Effect location" }, // エフェクトの場所
	{ 0x1413c4998, u8"Waiting time when counter is successful" }, // カウンター成功時の待機時間
	{ 0x1413c4b90, u8"Crushing throw setting" }, // 押しつぶしスロー設定
	{ 0x1413c4d18, u8"Blur effect distance from camera" }, // ブラーエフェクトのカメラからの距離
	{ 0x1413c6060, u8"Restraint time [after player movement is completed]" }, // 拘束時間[プレイヤー移動完了後から]
	{ 0x1413c62c0, u8"Ascent height" }, // 浮上高度
	{ 0x1413c6398, u8"Ascent start delay" }, // 浮上開始遅延
	{ 0x1413c64d0, u8"Ascent height change time" }, // 浮上高度変化時間
	{ 0x1413c6658, u8"Player movement time when surfacing" }, // 浮上時プレイヤー移動時間
	{ 0x1413c6780, u8"Curve degree when the player moves when floating" }, // 浮上時プレイヤー移動時カーブ度合
	{ 0x1413c6978, u8"Coordinates after player movement when surfacing [Boss standard]" }, // 浮上時プレイヤー移動後座標[ボス基準]
	{ 0x1413c6d28, u8"Do you use controller vibration when falling?" }, // 落下時コントローラー振動を使用するか
	{ 0x1413c6e90, u8"Controller vibration name when falling" }, // 落下時コントローラ振動名
	{ 0x1413c7060, u8"Warp position offset after fall [Boss reference]" }, // 落下後ワープ位置オフセット[ボス基準]
	{ 0x1413c82b0, u8"Camera maintenance time when laser is blown away" }, // レーザー吹き飛び時カメラ維持時間
	{ 0x1413c8408, u8"Camera at launch" }, // 打ち上げ時カメラ
	{ 0x1413c8740, u8"Fall camera" }, // 落下時カメラ
	{ 0x1413c8838, u8"Camera vibration when falling" }, // 落下時カメラ振動
	{ 0x1413c9028, u8"Laser start delay [after player rise is completed]" }, // レーザー開始遅延[プレイヤー上昇完了後から]
	{ 0x1413c92b0, u8"Blowing power" }, // 吹き飛ばし力
	{ 0x1413c9410, u8"Blowing force [upward flying ratio]" }, // 吹き飛ばし力[上方に飛ぶ比率]
	{ 0x1413c96f0, u8"Slow effect speed multiplier on contact" }, // 接触時スロー演出速度倍率
	{ 0x1413c9830, u8"Slow effect time on contact" }, // 接触時スロー演出時間
	{ 0x1413c99d8, u8"Slow effect interpolation time on contact" }, // 接触時スロー演出補間時間
	{ 0x1413cd1d8, u8"Cyloop effect 1st time" }, // サイループ効果1回目
	{ 0x1413cd2b8, u8"Cyloop effect second time" }, // サイループ効果2回目
	{ 0x1413cd3c8, u8"Damage rate during Cyloop effect" }, // サイループ効果中のダメージレート
	{ 0x1413cd510, u8"Attack waiting time after counter" }, // カウンター後の攻撃待機時間
	{ 0x1413cd990, u8"Penetration angle [X axis]" }, // 突き刺さり角度[X軸]
	{ 0x1413cda80, u8"Penetration angle [Y axis]" }, // 突き刺さり角度[Y軸]
	{ 0x1413cdb40, u8"Penetration angle [Z axis]" }, // 突き刺さり角度[Z軸]
	{ 0x1413cdc50, u8"Penetration position [Boss standard]" }, // 突き刺さり位置[ボス基準]
	{ 0x1413cde20, u8"Homing collision offset before piercing [gun standard]" }, // ホーミングコリジョンオフセット突き刺さり前[銃基準]
	{ 0x1413cdfa0, u8"After homing collision offset stabbing [gun standard]" }, // ホーミングコリジョンオフセット突き刺さり後[銃基準]
	{ 0x1413ce0f0, u8"Homing collision radius" }, // ホーミングコリジョン半径
	{ 0x1413ce708, u8"Down time" }, // ダウンしている時間
	{ 0x1413ce860, u8"HP that cannot be lower than that" }, // それ以下にならないHP
	{ 0x1413ce988, u8"Downtime in phase 4" }, // フェイズ4でのダウン時間
	{ 0x1413ceac0, u8"Enable player altitude fixation" }, // プレイヤー高度固定を有効化
	{ 0x1413cec80, u8"Single gun setting" }, // 銃単体設定
	{ 0x1413cf438, u8"Number of repetitions" }, // 繰り返し回数
	{ 0x1413cf6c0, u8"Waiting time before entering the event after the last bullet hits" }, // 最後の弾当たった後イベントに入るまでの待機時間
	{ 0x1413db278, u8"Reuse time" }, // 再使用時間
	{ 0x1413db4e8, u8"Damage multiplier when connecting cable" }, // ケーブル接続時のダメージ倍率
	{ 0x1413db678, u8"Recovery time when cable is connected" }, // ケーブル接続時の回復するまでの時間
	{ 0x1413db780, u8"Recovery amount when connecting cable" }, // ケーブル接続時の回復量
	{ 0x1413db8c0, u8"Minimum HP when connected with cable" }, // ケーブル接続時の最低HP
	{ 0x1413db9f8, u8"Attack pattern change HP" }, // 攻撃パターン変更HP
	{ 0x1413dbac8, u8"Phase change HP" }, // フェイズ変更HP
	{ 0x1413cff38, u8"Attack combination setting: Before handing over the rifle" }, // 攻撃の組み合わせ設定:ライフル受け渡し前
	{ 0x1413d0150, u8"Attack combination setting: After receiving the rifle" }, // 攻撃の組み合わせ設定:ライフル受け渡し後
	{ 0x1413d0248, u8"Settings when down" }, // ダウン時の設定
	{ 0x1413d0308, u8"Homing laser pattern setting" }, // ホーミングレーザーパターン設定
	{ 0x1413d0490, u8"Super Sonic 2 homing laser pattern setting before transformation" }, // スパソニ2変身前ホーミングレーザーパターン設定
	{ 0x1413d0620, u8"Homing laser pattern setting immediately after transformation of Super Sonic 2" }, // スパソニ2変身直後のホーミングレーザーパターン設定
	{ 0x1413d0710, u8"Homing laser basic settings" }, // ホーミングレーザー基本設定
	{ 0x1413d0858, u8"Homing laser settings for Cyloop" }, // サイループ用ホーミングレーザー設定
	{ 0x1413d0970, u8"Triple-hit stun homing laser setting" }, // 三連撃スタンホーミングレーザー設定
	{ 0x1413d0aa0, u8"Triple attack basic settings" }, // 三連撃基本設定
	{ 0x1413de228, u8"Camera gaze point offset (Super Sonic origin)" }, // カメラ注視点オフセット(スパソニ原点)
	{ 0x1413de2f8, u8"Camera position offset (Super Sonic origin)" }, // カメラ位置オフセット(スパソニ原点)
	{ 0x1413d3a90, u8"Camera settings for barrier damage" }, // バリアダメージ用カメラ設定
	{ 0x1413d3be0, u8"Camera settings for rear view" }, // うしろ周り用カメラ設定
	{ 0x1413d3c80, u8"How far away should you place the camera from the enemy (to reproduce the same camera as before the specification change) [Maximum 100]" }, // 敵からどれだけ離れた位置にカメラを置くか（仕様変更前と同じカメラ再現のための物）[最大100]
	{ 0x1413d3de0, u8"Camera height from the ground" }, // 地上からのカメラの高さ
	{ 0x1413d3e90, u8"Barrier HP in Easy" }, // イージーでのバリアのHP
	{ 0x1413d3f48, u8"Barrier HP in normal mode" }, // ノーマルでのバリアのHP
	{ 0x1413d4000, u8"Barrier HP on hard" }, // ハードでのバリアのHP
	{ 0x1413d4150, u8"Barrier HP in Very Hard" }, // ベリーハードでのバリアのHP
	{ 0x1413d41f8, u8"Tackle damage" }, // タックルのダメージ
	{ 0x1413d42c8, u8"Player distance after parry" }, // パリィ後のプレイヤーの距離
	{ 0x1413d45e8, u8"Collider offset [boss origin]" }, // コライダーのオフセット[ボス原点]
	{ 0x1413d4710, u8"Collider size" }, // コライダーの大きさ
	{ 0x1413d47f0, u8"Camera vibration during barrier attack" }, // バリア攻撃時のカメラ振動
	{ 0x1413d48d0, u8"Number of drumming during body contact" }, // 体当たり時のドラミング回数
	{ 0x1413d4ab8, u8"Player warp position at start [Boss origin]" }, // 開始時のプレイヤーワープ位置[ボス原点]
	{ 0x1413d4bc8, u8"Drumming duration" }, // ドラミングの継続時間
	{ 0x1413d4cd0, u8"Player inoperable time" }, // プレイヤーの操作不能時間
	{ 0x1413d6320, u8"Camera position offset [player position reference]" }, // カメラ位置オフセット[プレイヤー位置基準]
	{ 0x1413d4e78, u8"Over-the-shoulder camera 2" }, // 肩越しカメラ2
	{ 0x1413d4f88, u8"The position to wrap around “Boss origin”" }, // 回り込む位置「ボス原点」
	{ 0x1413d50b8, u8"Slow setting when turning around" }, // 回り込み時のスロー設定
	{ 0x1413d5178, u8"Hold time after wrapping" }, // 回り込み後のホールド時間
	{ 0x1413d0b38, u8"Basic ramming settings" }, // 体当たり基本設定
	{ 0x1413d2768, u8"Size of battle area" }, // 戦闘エリアの大きさ
	{ 0x1413d28d0, u8"Combat area limit collider height" }, // 戦闘エリア制限のコライダーの高さ
	{ 0x1413d2a78, u8"Sonic's new special move deals damage (maximum HP ratio)" }, // ソニック新必殺技与ダメ(最大HP比)
	{ 0x1413d2ba8, u8"Sonic's height after the event" }, // イベント後のソニックの高さ
	{ 0x1413d2d18, u8"Small rotation start angle" }, // 小回転開始角度
	{ 0x1413d2e70, u8"Medium rotation start angle" }, // 中回転開始角度
	{ 0x1413d2f38, u8"Giant rotation start angle" }, // 大回転開始角度
	{ 0x1413d2ff0, u8"Rotation standby time" }, // 回転待機時間
	{ 0x1413d30a0, u8"Angle that rotates at a constant angle even during rotation standby time" }, // 回転待機時間中でも一定角度で回転する角度
	{ 0x1413d31c8, u8"Minimum HP of boss at Super Sonic 1" }, // スパソニ1の時のボスの最小HP
	{ 0x1413d32d0, u8"Boss damage multiplier at Super Sonic 1" }, // スパソニ1の時のボスのダメージ倍率
	{ 0x1413d0d60, u8"Settings during battle" }, // バトル中の設定
	{ 0x1413d0e88, u8"Cyloop effect settings" }, // サイループ効果の設定
	{ 0x1413d9258, u8"Start with Fase4" }, // Fase4から始める
	{ 0x1413d93a0, u8"Allow movies to be skipped" }, // ムービーをスキップできるようにする
	{ 0x1413d9528, u8"Enable Cyloop effect camera debug display" }, // サイループ効果カメラデバッグ表示を有効化
	{ 0x1413d96a8, u8"Cyloop effect related camera to display debugging" }, // デバッグ表示するサイループ効果関連カメラ
	{ 0x1413d9878, u8"Enable Cyloop effect player position display" }, // サイループ効果プレイヤー位置表示を有効化
	{ 0x1413d0fd8, u8"Debug settings" }, // デバッグ設定
	{ 0x1413d7648, u8"Collision height offset" }, // コリジョンの高さオフセット
	{ 0x1413d7768, u8"Distance when attacking" }, // 攻撃時の距離
	{ 0x1413d7868, u8"Radius at Cyloop" }, // Cyloop時の半径
	{ 0x1413d7988, u8"Target available time" }, // ターゲット可能時間
	{ 0x1413d7b10, u8"Targeting distance" }, // ターゲット可能距離
	{ 0x1413dbfe8, u8"Camera gaze point offset (cable origin)" }, // カメラ注視点オフセット(ケーブル原点)
	{ 0x1413dc0f8, u8"Camera position offset (cable origin)" }, // カメラ位置オフセット(ケーブル原点)
	{ 0x1413dc4a8, u8"Camera display time" }, // カメラ表示時間
	{ 0x1413dc5a8, u8"Do you use camera direction?" }, // カメラ演出を使用するか
	{ 0x14139339c, u8"Camera" }, // camera
	{ 0x1413d8458, u8"Cable settings" }, // ケーブル設定
	{ 0x1413da030, u8"Practice notification settings [at the start of battle]" }, // 練習通知設定[戦闘開始時]
	{ 0x1413da0d8, u8"Practice notification settings [Just Parry]" }, // 練習通知設定[ジャストパリィ]
	{ 0x1413da190, u8"Practice notification settings [cable destruction]" }, // 練習通知設定[ケーブル破壊]
	{ 0x1413de848, u8"Target parameters" }, // ターゲットパラメータ
	{ 0x1413dff10, u8"Play with remaining HP" }, // HP残量で再生
	{ 0x1413dfff0, u8"HP remaining during playback [%]" }, // 再生時のHP残量[%]
	{ 0x1413e00e8, u8"Play with specified wave" }, // 指定waveで再生
	{ 0x1413e01d0, u8"Wave ID" }, // wave ID
	{ 0x1413e0290, u8"Play after how many seconds?" }, // 何秒経過で再生
	{ 0x1413e03ac, u8"Audio" }, // 音声
	{ 0x1413de9a0, u8"Line settings" }, // セリフ設定
	{ 0x1413df978, u8"Play WAVE ID" }, // 再生WAVE ID
	{ 0x1413deae0, u8"Practice notification settings [at start]" }, // 練習通知設定[開始時]
	{ 0x1413dec70, u8"Practice notification settings [when firing beam]" }, // 練習通知設定[ビーム発射時]
	{ 0x1413e09a8, u8"Hit point threshold" }, // ヒットポイント閾値
	{ 0x1413e0bb0, u8"HP rate after Sonic Rush event" }, // ソニック突進イベント後のHP率
	{ 0x1413dedf8, u8"UI settings" }, // UI設定
	{ 0x1413f0334, u8"Asm name" }, // asm名
	{ 0x1413f0400, u8"Normal output with size ratio 1.0" }, // サイズの比率1.0で通常出力
	{ 0x1413f04f8, u8"Stage to load on editor" }, // エディタ上でロードするステージ
	{ 0x1413f0618, u8"Position to set on editor" }, // エディタ上でセットする位置
	{ 0x1413e6638, u8"Boss common resources" }, // ボス共通リソース
	{ 0x1413f26f8, u8"Reference position model node name" }, // 基準位置のモデルノード名
	{ 0x1413f2c60, u8"Search tag" }, // 検索用tag
	{ 0x1413f34e0, u8"ID of next point (-1 disables)" }, // 次のポイントのID(-1で無効)
	{ 0x1413f3828, u8"HoldPoints" }, // holdPoints
	{ 0x1413e6768, u8"Hold Point" }, // HoldPoint
	{ 0x1413f2920, u8"Is it a physical collision?" }, // 物理コリジョンかどうか
	{ 0x1413f2a48, u8"Size * When spherical, x is the radius" }, // サイズ ※球体時xが半径
	{ 0x1413f30d8, u8"ColParts" }, // colParts
	{ 0x1413e6808, u8"Collision" }, // Collision
	{ 0x1413f18d0, u8"Durable value" }, // 耐久値
	{ 0x1413f7040, u8"Offset to the center of the upper leftmost block" }, // 左上最奥ブロック中央までのオフセット
	{ 0x1413f79c8, u8"Shape array" }, // 形状配列
	{ 0x1413fcc58, u8"Score increase amount" }, // スコア増加量
	{ 0x1413fccc8, u8"Meter increase amount when magnification is 1x" }, // 倍率1倍時のメーター増加量
	{ 0x1413fcd68, u8"Meter increase amount when magnification is 2x" }, // 倍率2倍時のメーター増加量
	{ 0x1413fce58, u8"Meter increase amount when magnification is 4x" }, // 倍率4倍時のメーター増加量
	{ 0x1413fcf18, u8"Meter increase amount when magnification is 8x" }, // 倍率8倍時のメーター増加量
	{ 0x1413fcf70, u8"Meter increase amount when magnification is 16x" }, // 倍率16倍時のメーター増加量
	{ 0x1413fd020, u8"Meter increase amount when magnification is 32x" }, // 倍率32倍時のメーター増加量
	{ 0x1413fd0a0, u8"Meter increase amount when magnification is 64x" }, // 倍率64倍時のメーター増加量
	{ 0x1413fd100, u8"Meter increase amount when magnification is 128x" }, // 倍率128倍時のメーター増加量
	{ 0x1413fd1a8, u8"Meter increase amount when magnification is 256x" }, // 倍率256倍時のメーター増加量
	{ 0x141394950, u8"Score addition delay (seconds)" }, // スコア加算ディレイ(秒)
	{ 0x1413f8c18, u8"Dash roller" }, // ダッシュローラー
	{ 0x1413f8d38, u8"Dash panel" }, // ダッシュパネル
	{ 0x1413f8df8, u8"Jump board" }, // ジャンプボード
	{ 0x1413f8ed0, u8"Jump selector" }, // ジャンプセレクター
	{ 0x1413f9048, u8"Horizontal bar" }, // 鉄棒
	{ 0x1413f9158, u8"Select cannon" }, // セレクトキャノン
	{ 0x1413ebc98, u8"Spring" }, // スプリング
	{ 0x1413f9348, u8"Wide spring" }, // ワイドスプリング
	{ 0x1413f9410, u8"Spring classic" }, // スプリングクラシック
	{ 0x1413f94fc, u8"Balloon" }, // 風船
	{ 0x1413f95d8, u8"Up reel" }, // アップリール
	{ 0x1413f9648, u8"Wall jump" }, // 壁ジャンプ
	{ 0x1413f96f8, u8"Propeller spring" }, // プロペラスプリング
	{ 0x1413f9800, u8"Spring pole" }, // スプリングポール
	{ 0x1413f99e8, u8"Combo 1" }, // コンボ1
	{ 0x1413f9b50, u8"Combo 2" }, // コンボ2
	{ 0x1413f9c30, u8"Combo finish" }, // コンボフィニッシュ
	{ 0x1413f9d30, u8"Accel rush combo 1" }, // アクセルラッシュコンボ1
	{ 0x1413f9de8, u8"Accelerator rush combo 2" }, // アクセルラッシュコンボ2
	{ 0x1413f9e50, u8"Accel Rush Combo Finish" }, // アクセルラッシュコンボフィニッシュ
	{ 0x1413c3c38, u8"Charge attack" }, // チャージアタック
	{ 0x1413e5490, u8"Deathblow" }, // 必殺技
	{ 0x1413fa080, u8"Passive special move" }, // 受け身必殺技
	{ 0x1413c3b00, u8"Sonic boom" }, // ソニックブーム
	{ 0x1413fa258, u8"Crusher" }, // クラッシャー
	{ 0x1413c3b90, u8"Loop kick" }, // ループキック
	{ 0x1413c3b48, u8"Spin slash" }, // スピンスラッシュ
	{ 0x1413f2880, u8"Stomping" }, // ストンピング
	{ 0x1413c3a28, u8"Homing shot" }, // ホーミングショット
	{ 0x1413c3bd0, u8"Cross slash" }, // クロススラッシュ
	{ 0x1413e3f18, u8"Ring" }, // リング
	{ 0x1413fa668, u8"Homing attack on object" }, // オブジェクトへのホーミングアタック
	{ 0x1413fa6f0, u8"Homing attack on enemy" }, // 敵へのホーミングアタック
	{ 0x1413c43b0, u8"Air trick" }, // エアトリック
	{ 0x1413fa7c0, u8"Chain booster" }, // チェインブースター
	{ 0x1413c3850, u8"Parry" }, // パリィ
	{ 0x1413fa8c8, u8"Quick siroop" }, // クイックサイループ
	{ 0x1413fa970, u8"Defeat small fry" }, // 雑魚撃破
	{ 0x1413fa9d8, u8"Defeat small boss" }, // 小ボス撃破
	{ 0x1413faa50, u8"Slingshot" }, // スリングショット
	{ 0x1413fd740, u8"Score acquisition interval (seconds)" }, // スコア取得間隔(秒)
	{ 0x1413fab38, u8"Grind rail" }, // グラインドレール
	{ 0x1413fabbc, u8"Pulley" }, // 滑車
	{ 0x1413fac48, u8"Fan" }, // ファン
	{ 0x1413fd868, u8"Minimum speed to reflect score acquisition" }, // スコア取得を反映させる最小速度
	{ 0x1413fd988, u8"Maximum speed to reflect score acquisition" }, // スコア取得を反映させる最高速度
	{ 0x1413f25f0, u8"Boost" }, // ブースト
	{ 0x1413fad98, u8"Power boost" }, // パワーブースト
	{ 0x1413fae60, u8"Sand ski" }, // サンドスキー
	{ 0x1413faef8, u8"Acceleration rail" }, // 加速レール
	{ 0x141395b18, u8"Minimum altitude to reflect magnification addition" }, // 倍率加算を反映させる最低高度
	{ 0x141395bf8, u8"Maximum altitude to reflect magnification addition" }, // 倍率加算を反映させる最高高度
	{ 0x141395d10, u8"Correction magnification at maximum altitude" }, // 最高高度時の補正倍率
	{ 0x1413c4780, u8"Spin boost" }, // スピンブースト
	{ 0x1413feab8, u8"Max value of meter" }, // メーターのマックス値
	{ 0x1413feb50, u8"Meter reduction interval (seconds)" }, // メーター減少間隔(秒)
	{ 0x1413fec00, u8"Meter reduction amount" }, // メーター減少量
	{ 0x1413fecc0, u8"Time (seconds) until meter starts decreasing immediately after reaching level" }, // レベル到達直後メーター減少開始するまでの時間(秒)
	{ 0x1413fedb0, u8"Meter % that starts meter decrease immediately after meter addition" }, // メーター加算直後メーター減少を開始するメーター%
	{ 0x1413feec0, u8"Time (seconds) until meter starts decreasing after meter addition" }, // メーター加算直後メーター減少開始するまでの時間(秒)
	{ 0x1413fefa0, u8"Time (seconds) from when the meter reaches 0 until the magnification is reset" }, // メーターが0になってから倍率リセットするまでの時間(秒)
	{ 0x141392ca0, u8"Score acquisition information for each action" }, // アクションごとのスコア獲得情報
	{ 0x141392da0, u8"Meter information when magnification is 1x" }, // 倍率1倍時のメーター情報
	{ 0x141392ea0, u8"Meter information when magnification is 2x" }, // 倍率2倍時のメーター情報
	{ 0x141392f48, u8"Meter information at 4x magnification" }, // 倍率4倍時のメーター情報
	{ 0x141393010, u8"Meter information at 8x magnification" }, // 倍率8倍時のメーター情報
	{ 0x1413930f8, u8"Meter information at 16x magnification" }, // 倍率16倍時のメーター情報
	{ 0x141393178, u8"Meter information at 32x magnification" }, // 倍率32倍時のメーター情報
	{ 0x1413931d8, u8"Meter information at 64x magnification" }, // 倍率64倍時のメーター情報
	{ 0x141393290, u8"Meter information at 128x magnification" }, // 倍率128倍時のメーター情報
	{ 0x141393330, u8"Meter information when magnification is 256x" }, // 倍率256倍時のメーター情報
	{ 0x1413933f8, u8"Gimmick reuse standby time (seconds)" }, // ギミック再使用待機時間(秒)
	{ 0x141393550, u8"Enable multiplier increase when acquiring boosters" }, // ブースター取得時の倍率上昇を有効化
	{ 0x141393b68, u8"Time limit (seconds)" }, // 制限時間(秒)
	{ 0x141394e90, u8"S rank score" }, // Sランクスコア
	{ 0x141394f40, u8"A rank score" }, // Aランクスコア
	{ 0x141395030, u8"B rank score" }, // Bランクスコア
	{ 0x1413951a8, u8"C rank score" }, // Cランクスコア
	{ 0x141393c60, u8"Easy" }, // イージー
	{ 0x1413db138, u8"Normal" }, // ノーマル
	{ 0x141393dc8, u8"Hard" }, // ハード
	{ 0x141393e58, u8"Very hard" }, // ベリーハード
	{ 0x141396398, u8"Action chain information linked to signboard ID" }, // 看板IDに紐づいたアクションチェイン情報
	{ 0x1413981e0, u8"Speed at start of fall" }, // 落下開始時の速さ
	{ 0x1413982c0, u8"Gravitational acceleration (when falling)" }, // 重力加速度(落下時)
	{ 0x141398fc0, u8"Attack range [minimum]" }, // 攻撃範囲[最小]
	{ 0x141399158, u8"Attack range [maximum]" }, // 攻撃範囲[最大]
	{ 0x141398608, u8"Large slave device parameters" }, // 大型子機パラメータ
	{ 0x14139d620, u8"Bullet collision radius" }, // 弾コリジョン半径
	{ 0x14139d718, u8"Travel distance limit" }, // 移動距離限界
	{ 0x14139d860, u8"Threshold of rotation speed at which damage occurs [No damage below this]" }, // ダメージが発生する回転速度の閾値[これ以下はダメージ無し]
	{ 0x14139d9f0, u8"Damage generation range adjustment" }, // ダメージ発生範囲調整
	{ 0x14139db38, u8"Time to stop attacking when attacked" }, // 攻撃された際に攻撃を中断する時間
	{ 0x14139de20, u8"Firing effect shortest playback interval" }, // 発射エフェクト最短再生間隔
	{ 0x14139df78, u8"Number of rings earned" }, // 獲得リング数
	{ 0x141398728, u8"Small handset parameters" }, // 小型子機パラメータ
	{ 0x14139e648, u8"Text [for notes]" }, // テキスト[メモ用]
	{ 0x141399f88, u8"Presence or absence of parry" }, // パリィの有無
	{ 0x14139a098, u8"Attack time [in units of 1 round]" }, // 攻撃時間[1巡単位]
	{ 0x14139a268, u8"Type of bullet" }, // 弾のタイプ
	{ 0x14139a370, u8"Type of movement" }, // 動きのタイプ
	{ 0x14139a4a0, u8"Reciprocating angle" }, // 往復角度
	{ 0x14139a568, u8"Rotation time phase" }, // 回転時間位相
	{ 0x14139a5e8, u8"Number of bullets fired" }, // 射出数 弾
	{ 0x14139a700, u8"Injection speed bullet" }, // 射出速度 弾
	{ 0x14139a7c0, u8"Additive injection speed bullet [2nd shot onwards]" }, // 加算射出速度 弾[2発目以降]
	{ 0x14139a890, u8"Acceleration value [0 means no speed change, negative value means deceleration]" }, // 加速値[0で速度変化無し、負の値で減速]
	{ 0x14139a9f8, u8"Maximum acceleration/deceleration value" }, // 加減速最大値
	{ 0x14139ac20, u8"Acceleration/deceleration delay time" }, // 加減速遅延時間
	{ 0x14139ade4, u8"Number of ways" }, // WAY数
	{ 0x14139af08, u8"WAY angle width" }, // WAY 角度幅
	{ 0x14139b080, u8"Firing interval time phase" }, // 発射間隔時間位相
	{ 0x14139b138, u8"Periodic firing interval setting [disabled with 0]" }, // 周期発射間隔設定[0で無効]
	{ 0x14139b1d8, u8"Periodic firing interval" }, // 周期発射間隔
	{ 0x14139b250, u8"Homing accuracy [0 is no guidance]" }, // ホーミング精度[0は無誘導]
	{ 0x14139b338, u8"Homing start delay" }, // ホーミング開始遅延
	{ 0x14139b418, u8"Homing angle limit [0.0 to 180.0f]" }, // ホーミング可能角度限界[0.0 ～ 180.0f]
	{ 0x14139b570, u8"Low flying type homing [only when tracking laser is set]" }, // 低空飛行型ホーミング[追尾レーザー設定時のみ]
	{ 0x14139b6a8, u8"Types of overhead cameras" }, // 俯瞰カメラの種類
	{ 0x14139b750, u8"Rotation speed of the entire wave [reverse rotation with negative values]" }, // Wave全体の回転速度[負の値で逆回転]
	{ 0x14139b870, u8"Boss body movement speed when floating [reverse with negative value]" }, // 浮遊時のボス本体移動速度[負の値で逆]
	{ 0x14139b980, u8"Arrangement shape" }, // 配置形状
	{ 0x14139e6d0, u8"Bullet pattern parameters" }, // 弾パターンパラメータ
	{ 0x141398830, u8"Small subunit launch projectile pattern parameter group" }, // 小型子機発射弾パターンパラメータ郡
	{ 0x14139cf18, u8"Interpolation time [when changing layout]" }, // 補間時間[配置変更時]
	{ 0x14139d020, u8"Individual interpolation start delay time" }, // 個々の補間開始遅延時間
	{ 0x14139d150, u8"Height adjustment when switching" }, // 切り替え時の高さ調整
	{ 0x14139c778, u8"Distance [front direction]" }, // 距離[正面方向]
	{ 0x14139ca98, u8"Deployment angle" }, // 展開角度
	{ 0x14139d228, u8"Fan arrangement settings" }, // 扇状配置設定
	{ 0x141398990, u8"Small handset layout shape parameters" }, // 小型子機配置形状パラメータ
	{ 0x14139e8a8, u8"Ring behavior" }, // リングの挙動
	{ 0x14139e938, u8"How long subtitles are displayed" }, // 字幕が表示される時間
	{ 0x14139e9b0, u8"Accessible distance" }, // アクセスできる距離
	{ 0x14139ea40, u8"Ring multiple times" }, // リング回複数
	{ 0x1413a0fb8, u8"Speed transition margin" }, // 速度遷移マージン
	{ 0x1413a10a0, u8"Time to move on to the next step" }, // 次の段階に移る時間
	{ 0x1413a1320, u8"Movement stop seconds" }, // 移動停止秒
	{ 0x1413a1428, u8"Does the speed decrease?" }, // 速度減衰するか
	{ 0x1413a1e38, u8"Explosion position offset minimum value" }, // 爆発位置オフセット最小値
	{ 0x1413a1f28, u8"Explosion position offset maximum value" }, // 爆発位置オフセット最大値
	{ 0x1413a1ff8, u8"Range of explosion" }, // 爆発が届く範囲
	{ 0x1413a2138, u8"Minimum force magnitude" }, // 力の大きさ最小値
	{ 0x1413a2218, u8"Maximum force magnitude" }, // 力の大きさ最大値
	{ 0x1413a1548, u8"Explosion parameters" }, // 爆発パラメータ
	{ 0x1413a2770, u8"Direction of force (will be normalized later)" }, // 力の方向(後で正規化されます)
	{ 0x1413a15e8, u8"Impulse parameters" }, // インパルスパラメータ
	{ 0x1413a1b28, u8"DebrisPhaseParameters" }, // debrisPhaseParameters
	{ 0x1413a2f88, u8"EffectDebrisParameters" }, // effectDebrisParameters
	{ 0x1413a3218, u8"Connection challenge number/starting end" }, // 接続チャレンジ番号/始端
	{ 0x1413a32b8, u8"Connection challenge number/termination" }, // 接続チャレンジ番号/終端
	{ 0x1413a36d0, u8"Connection challenge number list" }, // 接続チャレンジ番号リスト
	{ 0x1413a3a58, u8"Connect" }, // connect
	{ 0x1413a6b28, u8"Diffusion power magnification" }, // 拡散力の倍率
	{ 0x1413a6c18, u8"Multiplier of force received from outside" }, // 外部から受けた力の倍率
	{ 0x1413a7178, u8"[For advanced users] Linked to motion stop" }, // [上級者向け]モーションの停止に連動する
	{ 0x1413a75c0, u8"Do not apply instance drawing" }, // インスタンス描画適用しない
	{ 0x1413a7bb8, u8"Collision shape" }, // コリジョン形状
	{ 0x1413a7fd0, u8"MESH collision name" }, // MESHコリジョン名
	{ 0x1413a8040, u8"Collision position offset" }, // コリジョン位置オフセット
	{ 0x1413a8100, u8"Event collision size offset" }, // イベントコリジョンサイズオフセット
	{ 0x1413a8668, u8"Range_in" }, // range_in
	{ 0x1413a8798, u8"Range_distance" }, // range_distance
	{ 0x1413aa7d8, u8"Rigid material" }, // 剛体のマテリアル
	{ 0x1413ac3a0, u8"Gravity scaling" }, // 重力スケーリング
	{ 0x1413ac7a0, u8"Linear speed upper limit" }, // 直線速度の上限
	{ 0x1413aa8a0, u8"Physical behavior parameters" }, // 物理挙動パラメータ
	{ 0x1413abb48, u8"Motion name" }, // モーション名
	{ 0x1413abc48, u8"Frame synchronization" }, // フレーム同期
	{ 0x1413abcd0, u8"Stops at the last frame when loop playback stops" }, // ループ再生停止時、最終フレームで停止する
	{ 0x14122eab8, u8"Uv-anim" }, // uv-anim
	{ 0x14122eab0, u8"Pt-anim" }, // pt-anim
	{ 0x14122eaa0, u8"Mat-anim" }, // mat-anim
	{ 0x1413ad4d8, u8"Mirage animation" }, // Mirageアニメーション
	{ 0x1413acc48, u8"Motion type" }, // モーションタイプ
	{ 0x1413acd30, u8"Shaft" }, // 軸
	{ 0x1413acdcc, u8"Power" }, // 力
	{ 0x1413ace30, u8"Speed scale" }, // 速度スケール
	{ 0x1413ad610, u8"Program control motion" }, // プログラム制御モーション
	{ 0x1413addb8, u8"Sound name" }, // サウンド名
	{ 0x1413ad760, u8"Sound" }, // サウンド
	{ 0x1413ab098, u8"KILL time" }, // KILL時間
	{ 0x1413ab180, u8"Broken motion name" }, // 壊れモーション名
	{ 0x1413ab1f8, u8"Debris parameters" }, // 破片パラメータ
	{ 0x1413ad860, u8"Lifespan management" }, // 寿命管理
	{ 0x1413a8ab0, u8"[Constantly plays when you are not doing anything]" }, // "[何もしていない時,常時再生]"
	{ 0x1413a8c50, u8"[At the time of collision contact]" }, // [コリジョン接触時]
	{ 0x1413a8d18, u8"[When leaving collision]" }, // [コリジョン離脱時]
	{ 0x1413a8e30, u8"[During collision contact]" }, // [コリジョン接触中]
	{ 0x1413a8eb0, u8"[When the player moves during collision contact]" }, // [コリジョン接触中にプレイヤーが動いた時]
	{ 0x1413a8f68, u8"[When damage is added]" }, // [ダメージが加わった時]
	{ 0x1413a9000, u8"Ignore the effects of the meteor shower and respawn by re-ranging." }, // 流星雨の影響を無視し、再レンジインでリスポーンする
	{ 0x1413a9a88, u8"Contact damage" }, // 接触ダメージ
	{ 0x1413a9b78, u8"No damage from spin attack" }, // スピンアタックではダメージを受けない
	{ 0x1413a9c40, u8"Takes damage when ridden" }, // 乗られた時、ダメージを受ける
	{ 0x1413a9d50, u8"Bounce when destroyed by air attack" }, // 空中攻撃で破壊した際のバウンド
	{ 0x1413bcd38, u8"Basic physical fitness" }, // 基礎体力
	{ 0x1413bcdf0, u8"Initial number of remaining machines" }, // 初期残機数
	{ 0x1413bcec0, u8"Number of failures required to increase remaining lives" }, // 残機を増やすのに必要な失敗数
	{ 0x1413bd0c8, u8"Maximum number of remaining planes" }, // 最大残機数
	{ 0x1413bd2b8, u8"Special gauge increase amount" }, // 必殺ゲージ上昇量
	{ 0x1413bd488, u8"Special gauge maximum value" }, // 必殺ゲージ最大値
	{ 0x1413bd6b0, u8"Number of laser generated" }, // レーザー生成本数
	{ 0x1413bd860, u8"Laser generation interval" }, // レーザー生成インターバル
	{ 0x1413bd9f0, u8"Attribute bullet generation interval" }, // 属性弾生成インターバル
	{ 0x1413bdbd0, u8"Own aircraft movement speed" }, // 自機移動速度
	{ 0x1413bdcc0, u8"Invincibility time when respawning" }, // リスポーン時無敵時間
	{ 0x1413bde60, u8"Special move time" }, // 必殺技時間
	{ 0x1413bdfc8, u8"Body hit judgment size" }, // 本体当たり判定サイズ
	{ 0x1413be158, u8"Barrier hit judgment size" }, // バリア当たり判定サイズ
	{ 0x1413be300, u8"Attack range development time" }, // 攻撃範囲展開時間
	{ 0x1413be448, u8"Attack range expansion size" }, // 攻撃範囲展開サイズ
	{ 0x1413be5d8, u8"Damage judgment position" }, // ダメージ判定位置
	{ 0x1413be728, u8"Barrier position" }, // バリア位置
	{ 0x1413be870, u8"Attack range deployment position" }, // 攻撃範囲展開位置
	{ 0x1413ae2c0, u8"Own machine parameters" }, // 自機パラメータ
	{ 0x1413ba090, u8"Bullet width" }, // 弾の幅
	{ 0x1413bbd88, u8"Minimum bullet Yaw" }, // 弾のYawの最小値
	{ 0x1413bbee8, u8"Maximum bullet Yaw" }, // 弾のYawの最大値
	{ 0x1413bbff8, u8"Initial velocity of the bullet in the depth direction" }, // 弾の奥行方向への初速
	{ 0x1413ba2d8, u8"Power of bullet" }, // 弾の威力
	{ 0x1413bc760, u8"Minimum trajectory length" }, // 軌跡の長さの最小値
	{ 0x1413bc868, u8"Width of trajectory appearance" }, // 軌跡見た目の幅
	{ 0x1413bc1b0, u8"Trajectory parameters" }, // 軌跡のパラメータ
	{ 0x1413ae348, u8"Homing laser parameters" }, // ホーミングレーザーパラメータ
	{ 0x1413ba1b8, u8"Bullet model scale" }, // 弾のモデルのスケール
	{ 0x1413ae428, u8"Attribute bullet parameters" }, // 属性弾パラメータ
	{ 0x1413ae518, u8"Parameters when hacking" }, // ハッキング時のパラメータ
	{ 0x1413af110, u8"Distance threshold from boss when firing laser" }, // レーザー発射時のボスとの距離の閾値
	{ 0x1413af230, u8"Special gauge increase amount when making a mistake" }, // ミス時の必殺ゲージ上昇量
	{ 0x1413af378, u8"Hit detection drawing size" }, // 当たり判定描画サイズ
	{ 0x1413aecc8, u8"The End battle parameters" }, // ジエンド戦のパラメータ
	{ 0x1413af698, u8"Average rising speed (m/s)" }, // 上昇平均速度(m/s)
	{ 0x1413af818, u8"Falling speed (m/s)" }, // 落下速度(m/s)
	{ 0x1413af918, u8"HA bounce strength" }, // HAのバウンドの強さ
	{ 0x1413b0f58, u8"Pattern combination" }, // 絵柄組み合わせ
	{ 0x1413b1078, u8"Number of revolutions" }, // 回転数
	{ 0x1413b3590, u8"Pictorial pattern" }, // 絵柄
	{ 0x1413b36b8, u8"Rotation speed (minimum)" }, // 回転速度（最小）
	{ 0x1413b37d0, u8"Rotation speed (maximum)" }, // 回転速度（最大）
	{ 0x1413b3988, u8"Time until automatic stop (minimum)" }, // 自動停止までの時間（最小）
	{ 0x1413b3ba8, u8"Time until automatic stop (maximum)" }, // 自動停止までの時間（最大）
	{ 0x1413b1550, u8"Reel settings" }, // リール設定
	{ 0x1413b15e8, u8"Spin interval (sec)" }, // スピン間隔（秒）
	{ 0x1413b3108, u8"Probability [0.0~100.0]" }, // 確率[0.0~100.0]
	{ 0x1413b16f0, u8"Probability table" }, // 確率テーブル
	{ 0x1413b17e0, u8"Guaranteed settings based on rotation speed" }, // 回転数による保証設定
	{ 0x1413b2b38, u8"Number of fishing coins paid out" }, // 払出釣りコイン数
	{ 0x1413b18d8, u8"Payment table" }, // 支払いテーブル
	{ 0x1413b19e8, u8"Number of fishing coins paid out when missing" }, // 外れ時の払出釣りコイン数
	{ 0x1413b1b50, u8"Get a confirmed role even if it's by eye-catching" }, // 目押しでも確定役を出す
	{ 0x1413b1c70, u8"Re-rotation performance speed" }, // 再回転演出速度
	{ 0x1413b1df8, u8"Reverse rotation performance speed" }, // 逆回転演出速度
	{ 0x1413b1f40, u8"Performance speed one step at a time" }, // １段ずつ演出速度
	{ 0x1413b4140, u8"Maximum generation interval" }, // 生成最大間隔
	{ 0x1413b4250, u8"Minimum generation interval" }, // 生成最小間隔
	{ 0x1413b4328, u8"Maximum height of generated position" }, // 生成位置の最大高さ
	{ 0x1413b4430, u8"Minimum height of generated position" }, // 生成位置の最小高さ
	{ 0x1413b4530, u8"Maximum horizontal distance of generation position" }, // 生成位置の最大水平距離
	{ 0x1413b4610, u8"Minimum horizontal distance of generation position" }, // 生成位置の最小水平距離
	{ 0x1413b47f8, u8"Maximum length" }, // 最大長さ
	{ 0x1413b4980, u8"Minimum length" }, // 最小長さ
	{ 0x1413b4b18, u8"Maximum tilt angle" }, // 最大傾き角
	{ 0x1413b4c38, u8"Minimum tilt angle" }, // 最小傾き角
	{ 0x1413b4e00, u8"Minimum travel time" }, // 最小移動時間
	{ 0x1413b4f20, u8"Maximum fade time" }, // 最大フェード時間
	{ 0x1413b5070, u8"Minimum fade time" }, // 最小フェード時間
	{ 0x1413b5178, u8"Occurrence range angle [camera front reference]" }, // 発生範囲角度[カメラ正面基準]
	{ 0x1413b52c8, u8"DIFFUSE_COLOR" }, // DIFFUSE_COLOR
	{ 0x1413b5f80, u8"Distance from Sonic that does not cause objects to immediately respawn" }, // オブジェクトを即リスポーンさせないソニックからの距離
	{ 0x1413b6168, u8"Interval days" }, // 間隔日数
	{ 0x1413b6300, u8"Time of occurrence" }, // 発生時
	{ 0x1413b6488, u8"Occurrence" }, // 発生分
	{ 0x1413b65a0, u8"Meteor shower duration" }, // 流星雨継続時間
	{ 0x1413b66f8, u8"Meteor shower continuation" }, // 流星雨継続分
	{ 0x1413b6828, u8"Seed acquisition magnification for the 3rd island" }, // 第3弾島用タネ入手倍率
	{ 0x1413b6980, u8"Kodama acquisition magnification for the 3rd island" }, // 第3弾島用コダマ入手倍率
	{ 0x1413b6a98, u8"Interval days for 3rd island" }, // 第3弾島用間隔日数
	{ 0x1413b6bc0, u8"Meteor effect settings" }, // 流星エフェクト設定
	{ 0x1413b6c70, u8"Bonus slot settings" }, // ボーナススロット設定
	{ 0x1413ba570, u8"Time until seed collection" }, // タネ回収までの時間
	{ 0x1413ba658, u8"SeedType" }, // seedType
	{ 0x1413bbf50, u8"Collision Half the length of each side" }, // コリジョン 各辺の長さの半分
	{ 0x1413bc078, u8"Collision offset" }, // コリジョン オフセット
	{ 0x1413bc488, u8"Panel" }, // パネル
	{ 0x1413bebb0, u8"Horizontal rotating mirror" }, // 横回転鏡
	{ 0x1413bed30, u8"Vertical rotating mirror" }, // 縦回転鏡
	{ 0x1413bee58, u8"Moving mirror" }, // 移動鏡
	{ 0x1413c20b8, u8"SequenceType" }, // sequenceType
	{ 0x1413c24c8, u8"Target fixation time" }, // ターゲット注視時間
	{ 0x1413c2ff0, u8"Catchable distance" }, // キャッチ可能距離
	{ 0x1413c3308, u8"Appearance delay time" }, // 出現遅延時間
	{ 0x1413c2918, u8"Target" }, // ターゲット
	{ 0x1413c2a10, u8"Production camera" }, // 演出カメラ
	{ 0x1413c49f8, u8"Gravitational acceleration during projection (when falling)" }, // 投射時の重力加速度(落下時)
	{ 0x1413c6e58, u8"Maximum correction effective distance" }, // 補正有効最大距離
	{ 0x1413c7018, u8"Correction effective shortest distance" }, // 補正有効最短距離
	{ 0x1413c7190, u8"Correction effective angle" }, // 補正有効角度
	{ 0x1413c7330, u8"Correction factor (0-1)" }, // 補正率(0-1)
	{ 0x1413c7ac8, u8"Weight" }, // 重量
	{ 0x1413c7fc8, u8"Maximum acceleration" }, // 最大加速度
	{ 0x1413c8190, u8"Speed multiplier" }, // 速度倍率
	{ 0x1413c84dc, u8"Correction" }, // 補正
	{ 0x1413c9f80, u8"Number of times it breaks when continuously receiving wave motion cannons" }, // 波動砲を受け続けた時に壊れる回数
	{ 0x1413cab70, u8"Initial speed when riding" }, // 乗る時の初速
	{ 0x1413cac90, u8"Length of ride" }, // 乗る時の尺
	{ 0x1413cad88, u8"Initial speed when descending" }, // 降りる時の初速
	{ 0x1413cae88, u8"Step when descending" }, // 降りる時の尺
	{ 0x1413cafe0, u8"Number of rotations for getting on and off" }, // 乗り降りの自転数
	{ 0x1413cb0d8, u8"Number of revolutions for getting on and off" }, // 乗り降りの公転数
	{ 0x1413cefc8, u8"Width of running line" }, // 走行ラインの幅
	{ 0x1413cf0d0, u8"Length of the road" }, // 道の長さ
	{ 0x1413cf2b0, u8"Obstacle spacing" }, // 障害物の配置間隔
	{ 0x1413cf410, u8"Distance to notify when an obstacle is approaching" }, // 障害物の接近を知らせる距離
	{ 0x1413cf568, u8"Obstacle size" }, // 障害物サイズ
	{ 0x1413cf710, u8"Color of the road" }, // 道の色
	{ 0x1413cf7e0, u8"Obstacle color" }, // 障害物の色
	{ 0x1413cf928, u8"Camera viewing angle" }, // カメラ視野角
	{ 0x1413cfa98, u8"Camera upward offset" }, // カメラ上方オフセット
	{ 0x1413cfb90, u8"Camera shake width" }, // カメラの揺れ幅
	{ 0x1413d7db8, u8"Number of consecutive successes until escape" }, // 脱出するまでの連続成功回数
	{ 0x1413d7f58, u8"Timing of success x: beginning y: ending" }, // 成功のタイミング x:始まり y:終わり
	{ 0x1413d8100, u8"Time until oxygen gauge runs out" }, // 酸素ゲージが尽きるまでの時間
	{ 0x1413d82a0, u8"Maximum height when confined" }, // 閉じ込め時の最大の高さ
	{ 0x1413d8430, u8"Time to transition to failure" }, // 失敗に移行するまでの時間
	{ 0x1413d85c0, u8"Failure sign motion time" }, // 失敗予兆モーション時間
	{ 0x1413d8b84, u8"Physical strength" }, // 体力
	{ 0x1413d8d50, u8"Attack multiplier" }, // 攻撃倍率
	{ 0x1413d8ee0, u8"Number of experience item drops" }, // 経験値アイテムドロップ数
	{ 0x1413d9058, u8"Total experience points" }, // 合計経験値
	{ 0x1413d97d0, u8"Waiting time from the end of Dead motion to being killed" }, // Deadモーション終了からKillされるまでの待ち時間
	{ 0x1413d99c0, u8"Body cylinder collision radius" }, // 本体円柱コリジョン半径
	{ 0x1413d9ae0, u8"Body cylinder collision height" }, // 本体円柱コリジョン高さ
	{ 0x1413d9bf0, u8"Damage collision radius offset" }, // ダメージコリジョンの半径オフセット
	{ 0x1413d9d70, u8"Radius offset for bringing in sonic" }, // ソニックを取り込むの半径オフセット
	{ 0x1413d9ec0, u8"Distance to detect cliffs and obstacles" }, // 崖や障害物を検知する距離
	{ 0x1413da068, u8"Drop ring" }, // ドロップリング
	{ 0x1413db840, u8"Minimum turning speed" }, // 最少旋回速度
	{ 0x1413db950, u8"Maximum turning speed" }, // 最大旋回速度
	{ 0x1413dba78, u8"Minimum speed for maximum turning speed" }, // 旋回速度が最大になる最少速度
	{ 0x1413dbb68, u8"Maximum speed at which turning speed is minimum." }, // 旋回速度が最少になる最大速度.
	{ 0x1413dbce0, u8"Bounce height" }, // 弾む高さ
	{ 0x1413dbd80, u8"Bouncy interval" }, // 弾む間隔
	{ 0x1413dbe58, u8"Time interval to update movement point" }, // 移動地点を更新する時間間隔
	{ 0x1413dbf88, u8"Strength of force pushed by HA" }, // HAによって押させる力の強さ
	{ 0x1413da140, u8"Patrol settings" }, // パトロール設定
	{ 0x1413da210, u8"Escape action setting" }, // 脱出アクション設定
	{ 0x1413da918, u8"Common parameters" }, // 共通パラメータ
	{ 0x1413daad8, u8"Level linear interpolation parameters" }, // レベル線形補間パラメータ
	{ 0x1413dabf0, u8"Parameters by level band" }, // レベル帯別パラメータ
	{ 0x1413dd720, u8"Rotation speed to sonic" }, // ソニックへの回転速度
	{ 0x1413dd7d8, u8"Distance from stop position to Sonic" }, // 停止位置からソニックまでの距離
	{ 0x1413dd8d0, u8"Offset from Sonic" }, // ソニックからのオフセット
	{ 0x1413ddf00, u8"Backward travel time" }, // 後退移動時間
	{ 0x1413de270, u8"HA wait time" }, // HA待機時間
	{ 0x1413de340, u8"Rotational speed on path" }, // パス上の回転速度
	{ 0x1413de3d8, u8"Movement speed on the path" }, // パス上の移動速度
	{ 0x1413de4e0, u8"Movement speed to the path" }, // パスへの移動速度
	{ 0x1413de590, u8"Spline tension" }, // スプラインのテンション
	{ 0x1413de620, u8"Spline speed" }, // スプラインの速度
	{ 0x1413de710, u8"Injection speed when horizontal bar action fails" }, // 鉄棒アクション失敗時の射出速度
	{ 0x1413dfb38, u8"Waiting time until start of rush" }, // 突進開始までの待ち時間
	{ 0x1413e00a8, u8"Lock-on valid time" }, // ロックオン有効時間
	{ 0x1413e01b8, u8"Lock-on distance" }, // ロックオン距離
	{ 0x1413e0d08, u8"Minimum time required to die" }, // 死亡までに掛ける最低時間
	{ 0x1413e0ea8, u8"Maximum time to death" }, // 死亡までに掛ける最大時間
	{ 0x1413e0ff0, u8"Downward raycast distance" }, // 下方レイキャスト距離
	{ 0x1413e1138, u8"Initial falling speed during stomping" }, // ストンピング時落下初速度
	{ 0x1413e1258, u8"Maximum speed when stomping" }, // ストンピング時最大速度
	{ 0x1413e1370, u8"Acceleration during stomping" }, // ストンピング時加速度
	{ 0x1413e1c88, u8"Glide speed" }, // 滑空速度
	{ 0x1413e1dd8, u8"Force applied in the Y direction when gliding" }, // 滑空時にY方向に加える力
	{ 0x1413e2008, u8"Time until shaken off" }, // 振り落とされるまでの時間
	{ 0x1413e21b0, u8"Turning motion speed" }, // 旋回モーション速度
	{ 0x1413e2668, u8"Maximum speed at which turning speed is minimum" }, // 旋回速度が最少になる最大速度
	{ 0x1413e27d8, u8"Height of transition to shake-off" }, // 振り落としに遷移する高さ
	{ 0x1413e2e70, u8"LaserInterval" }, // laserInterval
	{ 0x1413e33d0, u8"Distance from the bird that determines that Sonic is riding on a bird" }, // ソニックを鳥に乗っていると判定する鳥からの距離
	{ 0x1413e3a50, u8"Forward raycast distance" }, // 前方レイキャスト距離
	{ 0x1413e3b58, u8"Backward raycast distance" }, // 後方レイキャスト距離
	{ 0x1413e4260, u8"Visibility detection delay after horizontal bar action" }, // 鉄棒アクション後の視界検知ディレイ
	{ 0x1413e4b38, u8"Back box collision radius" }, // 背中ボックスコリジョン半径
	{ 0x1413e4c88, u8"Back box collision offset" }, // 背中ボックスコリジョンオフセット
	{ 0x1413e4e18, u8"Back damage collision radius" }, // 背中ダメージコリジョン半径
	{ 0x1413e4fc8, u8"Back damage collision offset" }, // 背中ダメージコリジョンオフセット
	{ 0x1413e51d8, u8"Rush box collision radius" }, // 突進ボックスコリジョン半径
	{ 0x1413e53b8, u8"Rush box collision offset" }, // 突進ボックスコリジョンオフセット
	{ 0x1413e55e0, u8"Viewing angle" }, // 視野角
	{ 0x1413e5790, u8"Viewing distance" }, // 視野距離
	{ 0x1413e6b48, u8"Box collision radius for HA" }, // HA用ボックスコリジョン半径
	{ 0x1413e6bf8, u8"Box collision offset for HA" }, // HA用ボックスコリジョンオフセット
	{ 0x1413e6cc0, u8"HA target position offset" }, // HAターゲット位置オフセット
	{ 0x1413e6dc8, u8"HA target radius" }, // HAターゲット半径
	{ 0x1413e6ec0, u8"HA target height" }, // HAターゲット高さ
	{ 0x1413e6f98, u8"Wandering settings" }, // 徘徊設定
	{ 0x1413e7058, u8"Proximity setting" }, // 接近設定
	{ 0x1413e7140, u8"Laser state settings" }, // レーザーステート設定
	{ 0x1413e7238, u8"Rush setting" }, // 突進設定
	{ 0x1413e72e8, u8"Reverse setting" }, // 後退設定
	{ 0x1413e73a8, u8"Horizontal bar settings" }, // 鉄棒設定
	{ 0x1413e7440, u8"Glide settings when boarding Sonic" }, // ソニック搭乗時の滑空設定
	{ 0x1413e7538, u8"Death setting" }, // 死亡設定
	{ 0x1413e7580, u8"Raycast settings" }, // レイキャスト設定
	{ 0x1413e7600, u8"Camera settings after landing" }, // 着地後カメラ設定
	{ 0x1413e5ff8, u8"Bird-type enemy body parameters" }, // 鳥型エネミー本体のパラメータ
	{ 0x1413e7ff0, u8"Speed information (bound)" }, // 速度情報（行き）
	{ 0x1413e8078, u8"Speed information (return)" }, // 速度情報（戻り）
	{ 0x1413e8100, u8"Collider length" }, // コライダーの長さ
	{ 0x1413e8258, u8"Collider radius" }, // コライダーの半径
	{ 0x1413e8370, u8"Lifespan" }, // 寿命
	{ 0x1413e9660, u8"Moving deceleration" }, // 移動減速度
	{ 0x1413e96e8, u8"Turn angle limit" }, // 方向転換角度制限
	{ 0x1413e97c0, u8"Charge time" }, // チャージ時間
	{ 0x1413e9908, u8"[Sonic set] Rotation speed" }, // [ソニックセット]回転速度
	{ 0x1413ea090, u8"Travel deceleration when braking" }, // ブレーキ時の移動減速度
	{ 0x1413ea188, u8"Deceleration when taking damage" }, // 被ダメージ時の減速度
	{ 0x1413ea298, u8"Recovery time after rush attack" }, // 突進攻撃後の硬直時間
	{ 0x1413ea380, u8"Collision offset rate by speed" }, // 速度によるコリジョンオフセット率
	{ 0x1413ea9f0, u8"Movement acceleration" }, // 移動加速度
	{ 0x1413eab68, u8"Meandering amplitude" }, // 蛇行振幅
	{ 0x1413eac68, u8"Meandering period" }, // 蛇行周期
	{ 0x1413ead50, u8"Meandering front rotation speed" }, // 蛇行正面回転速度
	{ 0x1413eae10, u8"Meandering back rotation speed" }, // 蛇行背後回転速度
	{ 0x1413eaeb8, u8"Parry retreat distance" }, // パリー後退距離
	{ 0x1413eb008, u8"Explosion time reduced by parry" }, // パリーによる爆発時間短縮
	{ 0x1413eb178, u8"[For Sonic Set] Cooldown time after parry" }, // [ソニックセット用]パリー後クールダウン時間
	{ 0x1413eb388, u8"Overhead camera distance" }, // 俯瞰カメラ距離
	{ 0x1413eb458, u8"Overhead camera elevation angle" }, // 俯瞰カメラ仰角
	{ 0x1413ebd00, u8"Angle of slope that can be climbed" }, // 登れるスロープの角度
	{ 0x1413ec2d0, u8"Viewing distance margin" }, // 視野距離マージン
	{ 0x1413ed6d0, u8"Deceleration when changing direction" }, // 方向転換時の減速度
	{ 0x1413ed828, u8"Minimum standby/patrol switching time" }, // 待機/パトロール切り替え最低時間
	{ 0x1413ed8d8, u8"Maximum standby/patrol switching time" }, // 待機/パトロール切り替え最大時間
	{ 0x1413ec408, u8"Settings you are aiming for" }, // 狙い中の設定
	{ 0x1413ec478, u8"Rush attack settings" }, // 突進攻撃の設定
	{ 0x1413ec518, u8"Mouse fireworks chase setting" }, // ねずみ花火追いかけの設定
	{ 0x1413ec5c8, u8"Self-destruction warning time" }, // 自爆予兆時間
	{ 0x1413ec6b8, u8"Explosion collision radius" }, // 爆発コリジョン半径
	{ 0x1413ec7a8, u8"Use unique shard settings?" }, // 固有の破片設定を使用するか
	{ 0x1413ec830, u8"Dedicated fragment setting (explosion)" }, // 専用破片設定(爆発)
	{ 0x1413ec8c0, u8"Dedicated fragment setting (Cyloop death)" }, // 専用破片設定(サイループ死亡)
	{ 0x1413ee840, u8"Closest distance" }, // 最接近距離
	{ 0x1413eee50, u8"Electric shock defense occurrence time" }, // 電撃防御発生時間
	{ 0x1413eef30, u8"Electric defense re-occurrence standby time" }, // 電撃防御再発生待機時間
	{ 0x1413ef560, u8"Electric defense maximum distance" }, // 電撃防御最大距離
	{ 0x1413ef6d8, u8"Lightning defense warning time" }, // 電撃防御予兆時間
	{ 0x1413f0088, u8"Waiting time before being killed" }, // Killされるまでの待ち時間
	{ 0x1413f0230, u8"Body circle collision radius" }, // 本体円コリジョン半径
	{ 0x1413f0448, u8"Attack collision radius offset" }, // 攻撃コリジョンの半径オフセット
	{ 0x1413f0b08, u8"Downtime after damage" }, // ダメージ後停止時間
	{ 0x1413f0c48, u8"Approach movement setting" }, // 接近移動設定
	{ 0x1413f0d20, u8"Electric shock defense omen setting" }, // 電撃防御予兆設定
	{ 0x1413f0e20, u8"Electric defense settings" }, // 電撃防御設定
	{ 0x1413f1d68, u8"Reference point height: core center" }, // 基準点高さ：コア中心
	{ 0x1413f1df8, u8"Distance between inner shell and body coordinates" }, // 内殻と本体の配置座標の距離
	{ 0x1413f1ef8, u8"Distance between outer shell and body coordinates" }, // 外殻と本体の配置座標の距離
	{ 0x1413f1f78, u8"Inner shell radius: stable adjacent distance" }, // 内殻半径：安定隣接距離
	{ 0x1413f2050, u8"Outer shell radius: stable adjacent distance" }, // 外殻半径：安定隣接距離
	{ 0x1413f2110, u8"Inner shell local behavior maximum acceleration" }, // 内殻ローカル挙動最大加速度
	{ 0x1413f2248, u8"Outer shell local behavior maximum acceleration" }, // 外殻ローカル挙動最大加速度
	{ 0x1413f23c0, u8"Inner shell local behavior maximum velocity" }, // 内殻ローカル挙動最大速度
	{ 0x1413f24c0, u8"Outer shell local behavior maximum velocity" }, // 外殻ローカル挙動最大速度
	{ 0x1413f2658, u8"Number of inner shells" }, // 内殻数
	{ 0x1413f2760, u8"Number of outer shells" }, // 外殻数
	{ 0x1413f0ef8, u8"Inner shell/outer shell settings" }, // 内殻・外殻設定
	{ 0x1413f3618, u8"Boomerang collision radius" }, // ブーメランコリジョン半径
	{ 0x1413f36d8, u8"Boomerang collision height" }, // ブーメランコリジョン高さ
	{ 0x1413f37a0, u8"Boomerang collision position offset" }, // ブーメランコリジョン位置オフセット
	{ 0x1413f38e0, u8"Boomerang camera minimum maintenance time" }, // ブーメラン用カメラ最小維持時間
	{ 0x1413f3a90, u8"Grace time when you lose sight of your goal" }, // 目標を見失った際の猶予時間
	{ 0x1413f3c80, u8"Shield launch time" }, // シールドの打ちあげ時間
	{ 0x1413f3d48, u8"Shield launch height" }, // シールドの打ちあげ高さ
	{ 0x1413f3e30, u8"Boomerang speed" }, // ブーメラン速度
	{ 0x1413f3eb8, u8"Guard starting distance" }, // ガード開始距離
	{ 0x1413f4020, u8"Boomerang starting distance" }, // ブーメラン開始距離
	{ 0x1413f4438, u8"Number of revolutions of parried boomerang" }, // パリィしたブーメランの回転数
	{ 0x1413f4518, u8"Offset of parried boomerang" }, // パリィしたブーメランのズレオフセット
	{ 0x1413f4638, u8"Parried boomerang speed" }, // パリィしたブーメランの速度
	{ 0x1413fac38, u8"SV scale" }, // SVスケール
	{ 0x1413facb0, u8"FV scale" }, // FVスケール
	{ 0x1413fad70, u8"Appearance effect timer" }, // 出現エフェクトタイマー
	{ 0x1413fae98, u8"Body collision radius" }, // 本体コリジョン半径
	{ 0x1413faf38, u8"Body collision height" }, // 本体コリジョン高さ
	{ 0x1413fafe0, u8"Body collision offset" }, // 本体コリジョンオフセット
	{ 0x1413fb050, u8"Beam collision radius" }, // ビームコリジョン半径
	{ 0x1413fb108, u8"Beam collision height" }, // ビームコリジョン高さ
	{ 0x1413fb200, u8"Length of each side of beam collision for SV" }, // SV用ビームコリジョン各辺の長さ
	{ 0x1413fb310, u8"Missile collision radius" }, // ミサイルコリジョン半径
	{ 0x1413fb3f8, u8"Missile collision height" }, // ミサイルコリジョン高さ
	{ 0x1413fb4d0, u8"Missile collision offset" }, // ミサイルコリジョンオフセット
	{ 0x141393268, u8"Player following rotation speed [°/s]" }, // プレイヤー追従回転速度[°/s]
	{ 0x141393380, u8"Damage collision" }, // ダメージコリジョン
	{ 0x1413fed60, u8"Survival time (s)" }, // 生存時間(s)
	{ 0x1413fee58, u8"Attack interval (s)" }, // 攻撃間隔(s)
	{ 0x141392c50, u8"Bullet speed [m/s]" }, // 弾の速度[m/s]
	{ 0x141392d48, u8"Vulcan rotation speed [°/s]" }, // バルカン回転速度[°/s]
	{ 0x1413fe4e8, u8"Vulcan" }, // バルカン
	{ 0x141392998, u8"Bomb falling speed [m/s]" }, // 爆弾落下速度[m/s]
	{ 0x14139dca8, u8"Minimum turning force" }, // 最小旋回力
	{ 0x14139ddc8, u8"Maximum turning force" }, // 最大旋回力
	{ 0x14139df10, u8"Minimum speed for maximum turning force" }, // 最大旋回力になる最小速度
	{ 0x14139e040, u8"Maximum speed resulting in minimum turning force" }, // 最小旋回力になる最大速度
	{ 0x141395ab0, u8"Field of view" }, // 視野
	{ 0x141397aa0, u8"Attackable distance" }, // 攻撃可能距離
	{ 0x141397bb0, u8"Possible attack angle" }, // 攻撃可能角度
	{ 0x141397c88, u8"Speed given when attack hits" }, // 攻撃ヒット時に与える速度
	{ 0x141395b78, u8"Battle" }, // 戦闘
	{ 0x141395be8, u8"Deformation probability" }, // 変形確率
	{ 0x1413cd6d8, u8"Common" }, // common
	{ 0x141394b20, u8"Humanoid settings" }, // 人型設定
	{ 0x141394c30, u8"[Code] Minimum number of drops" }, // [コード]最小ドロップ数
	{ 0x141394d90, u8"[Code] Maximum number of drops" }, // [コード]最大ドロップ数
	{ 0x141394ed0, u8"[Code X] Maximum number of drops" }, // [コードX]最大ドロップ数
	{ 0x14139ca30, u8"Tag name" }, // タグ名
	{ 0x14139cb00, u8"Number of generated pieces" }, // 生成ピース数
	{ 0x14139cc40, u8"Piece speed minimum timing" }, // ピース速度最小タイミング
	{ 0x14139cd40, u8"Volume shape" }, // ボリューム形状
	{ 0x14139ce40, u8"Humanoid node name" }, // 人型ノード名
	{ 0x14139e4f8, u8"Offset [model node coordinate system]" }, // オフセット[モデルノード座標系]
	{ 0x14139e5b8, u8"Rotation [Volume coordinate system ZYX]" }, // ローテーション[ボリューム座標系ZYX]
	{ 0x14139cf88, u8"Humanoid volume" }, // 人型ボリューム
	{ 0x14139d048, u8"Oval node name" }, // 卵型ノード名
	{ 0x14139d170, u8"Egg-shaped volume" }, // 卵型ボリューム
	{ 0x14139d208, u8"Add effect" }, // エフェクト付ける
	{ 0x14139d2b0, u8"[DEBUG] Volume drawing" }, // [DEBUG]ボリューム描画
	{ 0x14139d368, u8"[DEBUG] Volume axis drawing" }, // [DEBUG]ボリューム軸描画
	{ 0x14139d488, u8"[DEBUG] Node axis drawing" }, // [DEBUG]ノード軸描画
	{ 0x141395008, u8"[Transformation] Node mapping information" }, // [変形]ノードマッピング情報
	{ 0x141398900, u8"Stun value reduction start time" }, // スタン値減少開始時間
	{ 0x141398a10, u8"Stun value reduction speed" }, // スタン値減少速度
	{ 0x141398b60, u8"Fear value reduction start time" }, // 怯み値減少開始時間
	{ 0x141398ce0, u8"Fear value reduction speed" }, // 怯み値減少速度
	{ 0x141399038, u8"Attack motion speed" }, // 攻撃モーション速度
	{ 0x14139c070, u8"Wall duration" }, // 壁の持続時間
	{ 0x14139c118, u8"Wall strength" }, // 壁の体力
	{ 0x14139c260, u8"Guard duration" }, // ガードの持続時間
	{ 0x14139c3c0, u8"Guard cooldown time" }, // ガードのクールタイム
	{ 0x141398140, u8"Super armor" }, // スーパーアーマー
	{ 0x1413981d0, u8"Does it transform?" }, // 変形するか
	{ 0x14139b930, u8"Action probability" }, // 行動確率
	{ 0x141399a30, u8"Action probability table" }, // 行動確率テーブル
	{ 0x141399b68, u8"Cooldown time by action" }, // 行動別クールタイム
	{ 0x141399d50, u8"Waiting time after action" }, // 行動後待機時間
	{ 0x141399ea8, u8"Distance considered short distance" }, // 近距離とみなす距離
	{ 0x14139a018, u8"Probability table ID at short distance" }, // 近距離時確率テーブルID
	{ 0x14139a110, u8"Long distance probability table ID" }, // 遠距離時確率テーブルID
	{ 0x14139a220, u8"Do you guard against HA?" }, // HAに反応してガードするか
	{ 0x14139a330, u8"Does it build a wall in response to HA?" }, // HAに反応して壁を作るか
	{ 0x14139a458, u8"A wall with thorns?" }, // トゲ付き壁か
	{ 0x14139a528, u8"Number of waves" }, // ウェーブ数
	{ 0x14139a5a8, u8"Wave rise time" }, // ウェーブ上昇時間
	{ 0x14139a648, u8"Maintenance time after all waves rise" }, // 全ウェーブ上昇後の維持時間
	{ 0x14139a778, u8"Wave fall time" }, // ウェーブ下降時間
	{ 0x14139a850, u8"Wave initial radius" }, // ウェーブ初回半径
	{ 0x14139a940, u8"Increase radius after 2nd wave" }, // ウェーブ２つ目以降増加半径
	{ 0x14139ab00, u8"Generation interval time after the second wave" }, // ウェーブ２つ目以降生成間隔時間
	{ 0x14139ad10, u8"Wave damage width [m]" }, // ウェーブダメージ発生幅[m]
	{ 0x14139ae78, u8"Wave collision height" }, // ウェーブコリジョンの高さ
	{ 0x14139af70, u8"Number of consecutive attacks" }, // 連撃数
	{ 0x141396bd8, u8"Average level" }, // 平均レベル
	{ 0x141396d08, u8"Maximum number of simultaneous attacks" }, // 同時攻撃最大数
	{ 0x141396e00, u8"Great technique cooldown time [Currently only wave attacks]" }, // 大技クールタイム[現状ウェーブ攻撃のみ]
	{ 0x141396f30, u8"Maximum number of identical humanoid actions" }, // 人型同一行動最大数
	{ 0x1413973f0, u8"Average level reference parameter" }, // 平均レベル基準パラメータ
	{ 0x141395750, u8"Team battle parameters" }, // 集団戦用パラメータ
	{ 0x14139e860, u8"Width/radius" }, // 幅/半径
	{ 0x14139ebc0, u8"Stomping effective distance" }, // ストンピング有効距離
	{ 0x14139eca0, u8"Maximum number of stomping" }, // ストンピング最大回数
	{ 0x14139edb0, u8"Interpal time after final attack" }, // 最終攻撃後のインターパル時間
	{ 0x14139eef8, u8"Range distance" }, // 射程距離
	{ 0x14139efb8, u8"Stomping altitude" }, // ストンピング上昇高度
	{ 0x14139f120, u8"Stomping rise time" }, // ストンピング上昇時間
	{ 0x14139f220, u8"Rising animation play time" }, // 上昇アニメーション再生時間
	{ 0x14139f338, u8"Tracking speed" }, // 追跡速度
	{ 0x14139f3d0, u8"Trampling speed" }, // 踏みつけ速度
	{ 0x14139f508, u8"Shock wave radius" }, // 衝撃波半径
	{ 0x1413a3fe0, u8"Initial speed" }, // 初期速度
	{ 0x14139f5e8, u8"Player bounce" }, // プレイヤー跳ね飛ばし
	{ 0x1413a0188, u8"Lock-on minimum effective distance" }, // ロックオン最小有効距離
	{ 0x1413a1528, u8"Camera after stomp" }, // ストンプ後のカメラ
	{ 0x1413a1c50, u8"CharacterController Collision" }, // CharacterControllerコリジョン
	{ 0x1413a1d10, u8"Movement related" }, // 移動関連
	{ 0x1413a1e28, u8"Battle related" }, // バトル関連
	{ 0x1413a74e0, u8"Wandering speed" }, // うろうろ速度
	{ 0x1413a7608, u8"Pursuit speed" }, // 追っかけ速度
	{ 0x1413a7680, u8"Speed of running away" }, // 逃げ回る速度
	{ 0x1413a7790, u8"Escape speed [during player grind]" }, // 逃げ回る速度[プレイヤーグラインド中]
	{ 0x1413a78e8, u8"Speed when the player is riding" }, // プレイヤー乗っている際の速度
	{ 0x1413a7b18, u8"Maximum rail length" }, // レール最大長さ
	{ 0x1413a9ac0, u8"Field of view distance (when searching for enemy)" }, // 視野距離(索敵時)
	{ 0x1413a9bd0, u8"Field of view (combat)" }, // 視野距離(戦闘時)
	{ 0x1413a9ce8, u8"Distance to hide weaknesses" }, // 弱点を隠す距離
	{ 0x1413a9df8, u8"Distance to expose weak points" }, // 弱点を出す距離
	{ 0x1413a9eb8, u8"Downtime during defense" }, // 防御時の停止時間
	{ 0x1413a9fa8, u8"Defense barrage firing angle" }, // 防御弾幕の発射角度
	{ 0x1413aa0d8, u8"Muzzle bending speed when aiming" }, // エイム時銃口の曲がる速さ
	{ 0x1413aa208, u8"Distance at which the muzzle begins to contract" }, // 銃口が縮み始める距離
	{ 0x1413aa338, u8"Distance at which the muzzle finishes retracting" }, // 銃口が縮み終わる距離
	{ 0x1413aa438, u8"Maximum muzzle angle when sniping" }, // スナイプ時の銃口最大角度
	{ 0x1413aa558, u8"Maximum muzzle angle when defending" }, // 防御時の銃口最大角度
	{ 0x1413abb70, u8"Defense strength" }, // 防御体力
	{ 0x1413abf68, u8"Turning speed while sniping" }, // スナイプ中の旋回速度
	{ 0x1413ac0d8, u8"Turning speed while charging" }, // チャージ中の旋回速度
	{ 0x1413ac210, u8"Turning speed during deviation firing" }, // 偏差射撃中の旋回速度
	{ 0x1413ac348, u8"Preparation for firing a snipe bullet" }, // スナイプ弾発射の前隙
	{ 0x1413ac488, u8"Snipe bullet speed" }, // スナイプ弾の弾速
	{ 0x1413ac5b8, u8"After firing a snipe bullet" }, // スナイプ弾発射の後隙
	{ 0x1413ac6d8, u8"Standard adjustment time for deviation shooting" }, // 偏差射撃の標準合わせ時間
	{ 0x1413ac808, u8"Maximum deflection angle for deviation shooting" }, // 偏差射撃の最大ブレ角度
	{ 0x1413ac8c8, u8"Defense barrage speed" }, // 防御弾幕の弾速
	{ 0x1413ac990, u8"Defense barrage rotation speed" }, // 防御弾幕の回転速度
	{ 0x1413aca18, u8"Defense barrage motion speed" }, // 防御弾幕のモーション速度
	{ 0x1413acab8, u8"Defense barrage lifespan" }, // 防御弾幕の寿命
	{ 0x1413ae148, u8"Effective height of the sirloop" }, // サイループが有効な高さ
	{ 0x1413ae330, u8"Dedicated debris settings" }, // 専用破片設定
	{ 0x1413afeb0, u8"Overhead camera starting height" }, // 俯瞰カメラ開始高さ
	{ 0x1413b0090, u8"Overhead camera end height" }, // 俯瞰カメラ終了高さ
	{ 0x1413b3578, u8"Jumping height" }, // 飛び下がり高さ
	{ 0x1413b3720, u8"Jumping horizontal distance" }, // 飛び下がり水平距離
	{ 0x1413b3820, u8"Airborne time" }, // 滞空時間
	{ 0x1413b39e8, u8"Barrage arrangement" }, // 弾幕配置
	{ 0x1413b3b98, u8"Barrage height" }, // 弾幕高さ
	{ 0x1413b3cc0, u8"Barrage width" }, // 弾幕幅
	{ 0x1413b3d80, u8"Average bullet distance" }, // 平均弾間隔
	{ 0x1413b3e70, u8"Bomb percentage (%)" }, // 爆弾の割合(%)
	{ 0x1413b3f58, u8"Bullet falling speed" }, // 弾落下速度
	{ 0x1413b40d8, u8"Bullet life" }, // 弾寿命
	{ 0x1413b4210, u8"Bullet life elapsed magnification" }, // 弾寿命経過倍率
	{ 0x1413b53c8, u8"Body rigid cylinder collision radius" }, // 本体剛体円柱コリジョン半径
	{ 0x1413b54f8, u8"Body rigid cylinder collision height" }, // 本体剛体円柱コリジョン高さ
	{ 0x1413b5600, u8"Body attack cylinder collision radius" }, // 本体攻撃円柱コリジョン半径
	{ 0x1413b5720, u8"Body attack cylinder collision height" }, // 本体攻撃円柱コリジョン高さ
	{ 0x1413b5950, u8"Target cursor offset" }, // ターゲットカーソルオフセット
	{ 0x1413b9a30, u8"Wolf node name" }, // 狼型ノード名
	{ 0x1413b9af0, u8"Wolf-shaped volume" }, // 狼型ボリューム
	{ 0x1413b72e8, u8"Draw distance" }, // 引き距離
	{ 0x1413b7a40, u8"Gazing camera parameters" }, // 注視カメラパラメータ
	{ 0x1413b7b18, u8"Attack interval (normal)" }, // 攻撃間隔（通常）
	{ 0x1413b7cc8, u8"Attack interval (when parry is successful)" }, // 攻撃間隔（パリィ成功時）
	{ 0x1413b7e08, u8"Attack interval (when revived)" }, // 攻撃間隔（復活時）
	{ 0x1413b7eb0, u8"Attack interval (first time)" }, // 攻撃間隔（初回）
	{ 0x1413b7f68, u8"Attack interval (zero revival)" }, // 攻撃間隔（復活ゼロ）
	{ 0x1413b80b0, u8"Falling time" }, // 転倒時間
	{ 0x1413b81f0, u8"Interval between turning around and disappearing" }, // 振り向き後消えるまでの間隔
	{ 0x1413b8310, u8"Interval until display after model disappears" }, // モデル消滅後表示するまでの間隔
	{ 0x1413b8440, u8"Interval until transition to rotation behavior after appearance at specified position" }, // 指定位置出現後回転行動に遷移するまでの間隔
	{ 0x1413b85c0, u8"Reaction time" }, // リアクション時間
	{ 0x1413b8718, u8"Forced stop cancellation grace time" }, // 強制停止解除猶予時間
	{ 0x1413b8830, u8"Shooting angle interval" }, // 撃ちだし角度間隔
	{ 0x1413bd4c0, u8"Event type" }, // イベントタイプ
	{ 0x1413bd668, u8"Start time" }, // 開始時間
	{ 0x1413bd810, u8"End time [for interval]" }, // 終了時間[インターバル用]
	{ 0x1413bbf00, u8"Event information" }, // イベント情報
	{ 0x1413be558, u8"Local coordinates" }, // ローカル座標
	{ 0x1413be698, u8"Local posture" }, // ローカル姿勢
	{ 0x1413bcc38, u8"Animation time" }, // アニメーション時間
	{ 0x1413be000, u8"Node information during animation" }, // アニメーション中のノード情報
	{ 0x1413bbfd8, u8"Node information" }, // ノード情報
	{ 0x1413bc9d8, u8"Local movement value [difference from previous time]" }, // ローカル移動値[前回からの差分]
	{ 0x1413bcb30, u8"Local rotation value [difference from previous time]" }, // ローカル回転値[前回からの差分]
	{ 0x1413bc648, u8"Delta motion information during animation" }, // アニメーション中のデルタモーション情報
	{ 0x1413bc0e0, u8"Delta motion information" }, // デルタモーション情報
	{ 0x1413bc138, u8"Animation play time" }, // アニメーション再生時間
	{ 0x1413bec80, u8"Animation information" }, // アニメーション情報
	{ 0x141392aa0, u8"Enemy search distance [normal]" }, // 索敵距離[通常時]
	{ 0x141392b68, u8"Enemy search distance [when off camera]" }, // 索敵距離[カメラ外時]
	{ 0x1413fd330, u8"Visible camera distance" }, // 見え切りカメラ距離
	{ 0x1413fd410, u8"Full-view camera offset" }, // 見え切りカメラオフセット
	{ 0x1413fd570, u8"Visible camera angle" }, // 見え切りカメラアングル
	{ 0x1413c0d80, u8"Turning radius [minimum]" }, // 回転半径[最小]
	{ 0x1413c0f28, u8"Turning radius [maximum]" }, // 回転半径[最大]
	{ 0x1413c1080, u8"Initial state maintenance time" }, // 初期状態維持時間
	{ 0x1413c1198, u8"Expansion time" }, // 拡大時間
	{ 0x1413c12a0, u8"Expanded state maintenance time" }, // 拡大状態維持時間
	{ 0x1413c1350, u8"Reduction time" }, // 縮小時間
	{ 0x1413c1490, u8"Reduced state maintenance time" }, // 縮小状態維持時間
	{ 0x1413c1580, u8"Number of pieces" }, // 個数
	{ 0x1413c4048, u8"C type laser parameters" }, // C型レーザーパラメータ
	{ 0x1413c1c08, u8"Number of generation" }, // 生成数
	{ 0x1413c1db8, u8"Number of generation [during parkour]" }, // 生成数[パルクール中]
	{ 0x1413c2168, u8"Tracking performance [angular velocity per second]" }, // 追従性能[秒間角速度]
	{ 0x1413c2218, u8"Tracking start delay time" }, // 追従開始遅延時間
	{ 0x1413c2320, u8"Tracking performance maximum time" }, // 追従性能最大化時間
	{ 0x1413c23f8, u8"Meandering movement amount [No meandering at 0.0]" }, // 蛇行移動量[0.0で蛇行無し]
	{ 0x1413c41d0, u8"Electric shock parameters" }, // 電撃パラメータ
	{ 0x1413c2d40, u8"Survival time [applicable only to movement types]" }, // 生存時間[移動タイプにのみ適用]
	{ 0x1413c34f0, u8"Unoperable time [arm]" }, // 操作不能時間[腕]
	{ 0x1413c3698, u8"Dash speed [arm]" }, // ダッシュ速度[腕]
	{ 0x1413c3830, u8"Unoperable time [ground]" }, // 操作不能時間[地上]
	{ 0x1413c39e0, u8"Dash speed [ground]" }, // ダッシュ速度[地上]
	{ 0x1413c2f38, u8"Dash type settings" }, // ダッシュタイプ設定
	{ 0x1413c30a0, u8"Back type setting" }, // バックタイプ設定
	{ 0x1413c4350, u8"Dash circle parameters" }, // ダッシュサークルパラメータ
	{ 0x1413c4b70, u8"Blow range [radius]" }, // 吹き飛ばし範囲[半径]
	{ 0x1413c4d80, u8"Blow range [height]" }, // 吹き飛ばし範囲[高さ]
	{ 0x1413c4ee8, u8"Instant death range [radius]" }, // 即死範囲[半径]
	{ 0x1413c4450, u8"Shock wave parameters" }, // 衝撃波パラメータ
	{ 0x1413bff78, u8"Related object parameters" }, // 関連オブジェクトパラメータ
	{ 0x1413c6258, u8"C type laser" }, // C型レーザー
	{ 0x1413c6340, u8"Electric shock" }, // 電撃
	{ 0x1413c6428, u8"Arm wave during arm attack" }, // 腕攻撃時の腕のウェーブ
	{ 0x1413c6530, u8"Circle bullet when attacking arm [upper arm]" }, // 腕攻撃時のサークル弾[腕上]
	{ 0x1413a8988, u8"Left" }, // 左
	{ 0x1413b566c, u8"Center" }, // 中央
	{ 0x1413a8900, u8"Right" }, // 右
	{ 0x1413c5b20, u8"Left angle adjustment" }, // 左 角度調整
	{ 0x1413c5bd0, u8"Center angle adjustment" }, // 中央 角度調整
	{ 0x1413c5cf8, u8"Right angle adjustment" }, // 右 角度調整
	{ 0x1413c6638, u8"Circle bullet placement on arm" }, // 腕上のサークル弾配置
	{ 0x1413c6748, u8"Circle bullet when attacking with arm" }, // 腕攻撃時のサークル弾
	{ 0x1413c68f0, u8"Circle bullet back type occurrence frequency" }, // サークル弾のバックタイプ発生頻度
	{ 0x1413c6a50, u8"Number of circle bullets generated" }, // サークル弾の生成数
	{ 0x1413c6b38, u8"Circle bullet generation range [angle]" }, // サークル弾の生成範囲[角度]
	{ 0x1413c6c48, u8"Circle bullet final jump distance" }, // サークル弾最終ジャンプ距離
	{ 0x1413c6f30, u8"Number of consecutive attacks during arm attack" }, // 腕攻撃時の連続攻撃回数
	{ 0x1413c0088, u8"Parameters for each phase [by arm destruction status]" }, // 各フェーズ別パラメータ[腕破壊状況別]
	{ 0x1413c7b60, u8"Distance to retreat (within)" }, // 後退する距離(内)
	{ 0x1413c7cf0, u8"Stopping distance (within)" }, // 立ち止まる距離(内)
	{ 0x1413c7ea8, u8"Distance considered as short distance (within)" }, // 近距離とみなす距離(内)
	{ 0x1413c8070, u8"Distance considered as medium distance (outside)" }, // 中距離とみなす距離(外)
	{ 0x1413c8220, u8"Finished distance (inside)" }, // 走り終了距離(内)
	{ 0x1413c8340, u8"Rush end distance (inside)" }, // 突進終了距離(内)
	{ 0x1413c8488, u8"Distance to use parry (within)" }, // パリィを使用する距離(内)
	{ 0x1413c85d0, u8"Disappearance time at start of special move" }, // 必殺技開始の消失時間
	{ 0x1413c8720, u8"Disappearance time at the end of special move" }, // 必殺技終了の消失時間
	{ 0x1413c8878, u8"Waiting time after special move warp ends" }, // 必殺技ワープ終了後の待ち時間
	{ 0x1413c89f8, u8"Interval management travel time (minimum)" }, // 間合い管理移動時間(最少)
	{ 0x1413c8b30, u8"Interval management travel time (maximum)" }, // 間合い管理移動時間(最大)
	{ 0x1413c97bc, u8"AI" }, // AI
	{ 0x1413cbd68, u8"HP percentage that becomes angry" }, // 怒り状態になるHP割合
	{ 0x1413cc128, u8"Strike starting combo time" }, // 打撃始動コンボ時間
	{ 0x1413cc1d8, u8"Surrounding start combo time" }, // 囲み始動コンボ時間
	{ 0x1413cc2e8, u8"Launch start combo time" }, // 打ち上げ始動コンボ時間
	{ 0x1413cc4f8, u8"Attack preliminary movement motion speed" }, // 攻撃予備動作モーション速度
	{ 0x1413cc5d0, u8"Clone's preliminary attack motion speed" }, // 分身の攻撃予備動作モーション速度
	{ 0x1413ccd20, u8"General settings" }, // 汎用設定
	{ 0x1413c9fb8, u8"Time of not attacking at the start of battle" }, // 戦闘開始時の攻撃してこない時間
	{ 0x1413ca098, u8"Continuous warp attack usage interval" }, // 連続ワープ攻撃の使用間隔
	{ 0x1413ca1a0, u8"Close range action interval (minimum)" }, // 近距離行動インターバル(最少)
	{ 0x1413ca2b0, u8"Close range action interval (maximum)" }, // 近距離行動インターバル(最大)
	{ 0x1413ca3b8, u8"Medium distance action interval (minimum)" }, // 中距離行動インターバル(最少)
	{ 0x1413ca4f8, u8"Medium distance action interval (maximum)" }, // 中距離行動インターバル(最大)
	{ 0x1413ca5f0, u8"Guard counterattack interval (minimum)" }, // ガード反撃インターバル(最少)
	{ 0x1413ca708, u8"Guard counterattack interval (maximum)" }, // ガード反撃インターバル(最大)
	{ 0x1413ca7e8, u8"Interval after dash (minimum)" }, // ダッシュ後インターバル(最少)
	{ 0x1413ca918, u8"Interval after dash (max)" }, // ダッシュ後インターバル(最大)
	{ 0x1413caa30, u8"Parry usage interval (minimum)" }, // パリィ使用インターバル(最少)
	{ 0x1413cab40, u8"Parry usage interval (maximum)" }, // パリィ使用インターバル(最大)
	{ 0x1413cac50, u8"Parry duration" }, // パリィの持続時間
	{ 0x1413cad38, u8"Slash combo cooldown" }, // 斬りつけコンボのクールタイム
	{ 0x1413cae60, u8"Clone special attack cooldown time" }, // 分身特攻のクールタイム
	{ 0x1413ced80, u8"Number of clones appearing" }, // 分身出現数
	{ 0x1413cce48, u8"Continuous warp attack" }, // 連続ワープ攻撃
	{ 0x1413cd388, u8"Vertical slash activation rate" }, // 縦斬り発動率
	{ 0x1413cd4d8, u8"Horizontal slash activation rate" }, // 横斬り発動率
	{ 0x1413cd600, u8"Slash combo activation rate" }, // 斬りつけコンボ発動率
	{ 0x1413cd710, u8"Back slash activation rate" }, // 背面斬り発動率
	{ 0x1413cd830, u8"Dash activation rate" }, // ダッシュ発動率
	{ 0x1413cd960, u8"Rushing slash activation rate" }, // 突進斬り発動率
	{ 0x1413cda40, u8"Clone rush slash activation rate" }, // 分身突進斬り発動率
	{ 0x1413cdaf8, u8"Special move activation rate" }, // 必殺技発動率
	{ 0x1413cdba8, u8"Should I make my special move a time-stopping attack?" }, // 必殺技を時止め攻撃にするか
	{ 0x1413cdd78, u8"Time from first hit to guard" }, // 初撃からガードまでの時間
	{ 0x1413cdf08, u8"Laser activation rate when guarding" }, // ガード時レーザー発動率
	{ 0x1413ce040, u8"Laser activation rate during parry" }, // パリィ時レーザー発動率
	{ 0x1413ce158, u8"Laser activation rate during Cyloop" }, // Cyloop時レーザー発動率
	{ 0x1413ce278, u8"Continuous laser firing rate" }, // 連続レーザー発動率
	{ 0x1413ce3b0, u8"Should I use parry?" }, // パリィを使用するか
	{ 0x1413cb9a0, u8"Phases" }, // phases
	{ 0x1413cfeb0, u8"Waiting time until tracking starts" }, // 追跡開始までの待ち時間
	{ 0x1413d0040, u8"Tracking time" }, // 追跡時間
	{ 0x1413d01c8, u8"Time from end of tracking to disappearance" }, // 追跡終了から消失までの時間
	{ 0x1413d0298, u8"Radius when placing a circle" }, // 円配置時の半径
	{ 0x1413d0390, u8"Following angle limit (horizontal)" }, // 追従角度制限(横)
	{ 0x1413d0568, u8"Following angle limit (vertical)" }, // 追従角度制限(縦)
	{ 0x1413d06c0, u8"Rotational deceleration" }, // 回転減速度
	{ 0x1413d0788, u8"Regeneration time to loop the Muzzle effect" }, // Muzzuleエフェクトをループにする再生成時間
	{ 0x1413d2480, u8"Color when enabled" }, // 有効時の色
	{ 0x1413d24f0, u8"Color when disabled" }, // 無効時の色
	{ 0x1413d2658, u8"Timer height" }, // タイマーの高さ
	{ 0x1413d3408, u8"Rail Y offset" }, // レールYオフセット
	{ 0x1413d3510, u8"Camera elevation angle" }, // カメラ仰角
	{ 0x1413d35d0, u8"Distance from camera gaze point" }, // カメラ注視点からの距離
	{ 0x1413d36c0, u8"Camera angle of view when grinding" }, // グラインド時カメラ画角
	{ 0x1413d3800, u8"Middle rail radius" }, // 真ん中のレール半径
	{ 0x1413d3908, u8"Camera gaze point Y offset when grinding" }, // グラインド時カメラ注視点Yオフセット
	{ 0x1413d3a30, u8"Camera rotation angle when grinding (when moving clockwise)" }, // グラインド時カメラ回転角度（時計回りに移動時）
	{ 0x1413d3b90, u8"Camera rotation angle when grinding (when moving counterclockwise)" }, // グラインド時カメラ回転角度（反時計回りに移動時）
	{ 0x1413d3d08, u8"Camera angle changeable by stick input" }, // スティック入力による変更可能カメラ角度
	{ 0x1413d3e20, u8"Maximum angle per second that changes with stick input" }, // スティック入力で変わる最大の毎秒角度
	{ 0x1413d27f0, u8"Camera when attacking rails (2 rails)" }, // レール攻撃時カメラ(レール2本)
	{ 0x1413d2938, u8"Camera when attacking rails (3 rails)" }, // レール攻撃時カメラ(レール3本)
	{ 0x1413d1040, u8"Root settings" }, // Root設定
	{ 0x1413d1110, u8"Normal bullet settings" }, // 通常弾設定
	{ 0x1413d1f58, u8"Time flies to the timer" }, // タイマーまで飛んでいく時間
	{ 0x1413d11b8, u8"Restraint release gimmick setting" }, // 拘束解除ギミック設定
	{ 0x1413d18f8, u8"Spacing between lasers" }, // レーザー同士の間隔
	{ 0x1413d1280, u8"Laser settings" }, // レーザー設定
	{ 0x1413d13e0, u8"Adsorption attack setting" }, // 吸着攻撃設定
	{ 0x1413d47d0, u8"Bullet parameter ID" }, // 弾丸パラメータのID
	{ 0x1413d4918, u8"Y offset" }, // Yオフセット
	{ 0x1413d4a80, u8"Z offset" }, // Zオフセット
	{ 0x1413d4b80, u8"Skyward offset" }, // 上空向きオフセット
	{ 0x1413d4c88, u8"Launch angle [vertical]" }, // 発射角[縦]
	{ 0x1413d4db8, u8"Maximum angle [one side]" }, // 最大角度[片側]
	{ 0x1413d4eb0, u8"Firing duration [seconds]" }, // 発射継続時間[秒]
	{ 0x1413d5010, u8"Maximum acceleration amount" }, // 最大加速量
	{ 0x1413d5110, u8"Time moving in a straight line" }, // 直線で進む時間
	{ 0x1413d5270, u8"Velocity when falling" }, // 落下時の速度
	{ 0x1413d53a8, u8"Wait time before start" }, // 開始前待機時間
	{ 0x1413d5430, u8"Waiting time after completion" }, // 終了後待機時間
	{ 0x1413d5f60, u8"Bullet type" }, // 弾丸タイプ
	{ 0x1413d6200, u8"Deceleration force [seconds]" }, // 減速力[秒間]
	{ 0x1413d66e8, u8"Blow-off force" }, // 吹き飛び力
	{ 0x1413d67a0, u8"Parry rebound bullet speed" }, // パリィ跳ね返し弾の弾速
	{ 0x1413d6840, u8"Parry bounce bullet damage value" }, // パリィ跳ね返し弾のダメージ値
	{ 0x1413d6920, u8"Parry ricochet stagger value" }, // パリィ跳ね返し弾のよろめき値
	{ 0x1413d6a00, u8"Stun value of parry rebound bullet" }, // パリィ跳ね返し弾のスタン値
	{ 0x1413d6ad0, u8"Hit effect name" }, // ヒットエフェクト名
	{ 0x1413d7450, u8"Number of launch ports" }, // 発射口の数
	{ 0x1413d7520, u8"Launch port position offset" }, // 発射口の位置オフセット
	{ 0x1413d7678, u8"Launch port rotation offset: X axis" }, // 発射口の回転オフセット:X軸
	{ 0x1413d77a0, u8"Launch port rotation offset: Y axis" }, // 発射口の回転オフセット:Y軸
	{ 0x1413d78a0, u8"Launch port appearance time offset" }, // 発射口の出現時間オフセット
	{ 0x1413d79a8, u8"Offset to laser firing" }, // レーザー発射までのオフセット
	{ 0x1413d7b80, u8"Minimum time for each laser firing" }, // 各レーザー発射の最小時間
	{ 0x1413d7c20, u8"Maximum time for each laser firing" }, // 各レーザー発射の最大時間
	{ 0x1413d7cf0, u8"Number of lasers per launch port" }, // 発射口1つあたりのレーザーの数
	{ 0x1413d7e98, u8"Laser distance" }, // レーザーの間隔
	{ 0x1413d8048, u8"Time from laser termination to firing port disappearance" }, // レーザー終了から発射口消失までの時間
	{ 0x1413d81e0, u8"Laser speed" }, // レーザーのスピード
	{ 0x1413d8330, u8"Laser target random range/minimum" }, // レーザーのターゲットランダム範囲・最小
	{ 0x1413d8518, u8"Laser target random range/maximum" }, // レーザーのターゲットランダム範囲・最大
	{ 0x1413d8670, u8"Laser tracking time" }, // レーザーの追跡時間
	{ 0x1413d98f0, u8"Player following" }, // Player追従
	{ 0x1413d9ac8, u8"Firing interval [angle]" }, // 発射間隔[角度]
	{ 0x1413d9d20, u8"Rotation speed [angle/sec]" }, // 回転速度[角度/秒]
	{ 0x1413d9e70, u8"Amount of rotation until end of state" }, // ステート終了までの回転量
	{ 0x1413da6f8, u8"Angle in 3 directions" }, // 3方向の角度
	{ 0x1413db0c0, u8"Number of firing ports (one side)" }, // 発射口の数（片側）
	{ 0x1413db250, u8"Launch port rotation offset" }, // 発射口の回転オフセット
	{ 0x1413db6f0, u8"Number of shots per attack" }, // 一度の攻撃における発射回数
	{ 0x1413db808, u8"Random range of laser emission direction" }, // レーザーの射出方向のランダム範囲
	{ 0x1413db930, u8"Laser irradiation time" }, // レーザー照射時間
	{ 0x1413dbb18, u8"Interval until next laser firing" }, // 次のレーザー発射までの間隔
	{ 0x1413dc5f0, u8"Tracking angle" }, // 追跡角度
	{ 0x1413dc6b0, u8"Number of shots" }, // 発射回数
	{ 0x1413dcf18, u8"Bullet: Damage value" }, // 弾：ダメージ値
	{ 0x1413dcf98, u8"Bullet: Fear value" }, // 弾：怯み値
	{ 0x1413dd070, u8"Bullet: Movement speed" }, // 弾：移動速度
	{ 0x1413dd200, u8"Bullet: Gravity" }, // 弾：重力
	{ 0x1413dd310, u8"Bullet: Speed when surrounded by Cyloop" }, // 弾：サイループで囲んだ時の速度
	{ 0x1413dd3f0, u8"Bullet: Time until explosion after burial" }, // 弾：埋没後爆発までの時間
	{ 0x1413dd550, u8"Bullet: Survival time while throwing" }, // 弾：投擲中の生存時間
	{ 0x1413decf8, u8"Height swing width" }, // 高さの揺れ幅
	{ 0x1413dee78, u8"Rotation speed" }, // 回転スピード
	{ 0x1413defb8, u8"Amplitude speed" }, // 振幅スピード
	{ 0x1413dfe98, u8"Vertical number" }, // 縦の数
	{ 0x1413e0068, u8"Vertical spacing" }, // 縦の間隔
	{ 0x1413e0170, u8"Waiting time in vertical position" }, // 縦置き状態の待ち時間
	{ 0x1413e0f20, u8"Speed of departure" }, // 離れるスピード
	{ 0x1413e2648, u8"Index for attack parameters" }, // 攻撃パラメータ用Index
	{ 0x1413e27c8, u8"Probability of occurrence" }, // 発生確率
	{ 0x1413e28a8, u8"Cooling time after occurrence" }, // 発生後冷却時間
	{ 0x1413e32e8, u8"Distance traveled by vibration" }, // 振動が伝わる距離
	{ 0x1413e3840, u8"Hit distance" }, // ヒット距離
	{ 0x1413e3a08, u8"Restraint performance time" }, // 拘束演出時間
	{ 0x1413e3af8, u8"Darkening start distance" }, // 暗転開始距離
	{ 0x1413e3c20, u8"Dark fade time" }, // 暗転フェード時間
	{ 0x1413e3d20, u8"Dark opening delay time" }, // 暗転開け遅延時間
	{ 0x1413e3e60, u8"Tangent to parabola (starting point)" }, // 放物線の接線(開始点)
	{ 0x1413e3f90, u8"Tangent to parabola (end point)" }, // 放物線の接線(終了点)
	{ 0x1413e45c8, u8"Bullet parameters" }, // 弾丸パラメータ
	{ 0x1413e1808, u8"Simple rotation" }, // 単純回転
	{ 0x1413e1958, u8"Tracking vertical laser" }, // 追跡縦レーザー
	{ 0x1413e19f8, u8"Vortex rotation laser" }, // 渦回転レーザー
	{ 0x1413e1a70, u8"Cross laser" }, // クロスレーザー
	{ 0x1413e1b30, u8"Cross bullet" }, // クロス弾
	{ 0x1413e1c00, u8"Giant laser" }, // 巨大レーザー
	{ 0x1413e1c70, u8"Circumferential vertical bullet" }, // 円周縦置き弾
	{ 0x1413ebda8, u8"Number of bullets per side" }, // 一辺当たりの弾数
	{ 0x1413ebf40, u8"Number that produces a regular tetrahedron" }, // 正四面体を出す数
	{ 0x1413ec1c0, u8"Angle difference during continuous firing" }, // 連続発射時の角度差
	{ 0x1413ec250, u8"Waiting time after the bullet comes out" }, // 弾が出てからの待機時間
	{ 0x1413e1d00, u8"Circumferential tetrahedral bullet" }, // 円周正四面体弾
	{ 0x1413e8f60, u8"Ejection effect scale" }, // 射出エフェクトのスケール
	{ 0x1413e9db8, u8"Spacing between bullets" }, // 弾ごとの間隔
	{ 0x1413e9f68, u8"Number of bullets per side" }, // 一辺の弾数
	{ 0x1413ea050, u8"Probability of going up" }, // 上にいく確率
	{ 0x1413ea158, u8"Probability of going down" }, // 下にいく確率
	{ 0x1413ea260, u8"Probability of going forward" }, // 前にいく確率
	{ 0x1413ea348, u8"Waiting time Min" }, // 待機時間Min
	{ 0x1413ea4a8, u8"Standby time Max" }, // 待機時間Max
	{ 0x1413e1d68, u8"Horizontal vertical bullet" }, // 水平上下弾
	{ 0x1413ed448, u8"Length of one side" }, // 一辺の長さ
	{ 0x1413e1e20, u8"3 artillery shells in the air" }, // 空中3砲台弾
	{ 0x1413eaef0, u8"Minimum distance for scattering" }, // ばらまきの最低距離
	{ 0x1413eb070, u8"Maximum scattering distance" }, // ばらまきの最高距離
	{ 0x1413e1f20, u8"Bullet over player's head" }, // プレイヤー頭上弾
	{ 0x1413e9140, u8"Radius of launch port" }, // 発射口の半径
	{ 0x1413e1fe0, u8"Revolver shape gatling bullet" }, // リボルバー形状ガトリング弾
	{ 0x1413ee160, u8"Firing angle difference between bullets" }, // 弾丸間の発射角度差
	{ 0x1413e20d8, u8"Winder bullet" }, // ワインダー弾
	{ 0x1413eca38, u8"Firing interval [seconds]" }, // 発射間隔[秒]
	{ 0x1413e4ee0, u8"Overtaking bullet" }, // 追い越し弾
	{ 0x1413e5058, u8"Airborne bullet" }, // 空中浮遊弾
	{ 0x1413e51c0, u8"Land mine bomb" }, // 地雷ばらまき弾
	{ 0x1413e2390, u8"Diffused laser" }, // 拡散レーザー
	{ 0x1413e54c0, u8"Tracking laser shot" }, // 追跡レーザーショット
	{ 0x1413e5650, u8"Sure hit attack" }, // 必中攻撃
	{ 0x1413e7680, u8"Percentage of physical strength that changes state" }, // 状態変化する体力割合
	{ 0x1413e7738, u8"Time to run away" }, // 逃げてる時間
	{ 0x1413e7838, u8"Movement speed while running away" }, // 逃亡中移動速度
	{ 0x1413e78a8, u8"Damage deceleration" }, // ダメージ時減速力
	{ 0x1413e79a0, u8"Minimum speed during damage" }, // ダメージ時最低速度
	{ 0x1413e7b50, u8"Damage deceleration time" }, // ダメージ時減速時間
	{ 0x1413e7c68, u8"Acceleration power when recovering from damage" }, // ダメージ復帰時加速力
	{ 0x1413e6ae8, u8"When HP is ○% or more" }, // HPが○％以上の時
	{ 0x1413e6bc8, u8"Grind set name" }, // グラインドセット名
	{ 0x1413e7f68, u8"Pattern based on remaining HP" }, // 残存HPによるパターン
	{ 0x1413e6f80, u8"Distance for near/far discrimination" }, // 近/遠判別用距離
	{ 0x1413e7088, u8"Attack type [near/far]" }, // 攻撃タイプ[近/遠]
	{ 0x1413e7178, u8"Index for attack parameters [near/far]" }, // 攻撃パラメータ用Index[近/遠]
	{ 0x1413e65e8, u8"Parameters exclusive to levels 1 to 19" }, // レベル1から19専用パラメータ
	{ 0x1413f7ffc, u8"Interval" }, // 間隔
	{ 0x1413f80d0, u8"Injection pitch [minimum]" }, // 射出ピッチ[最小]
	{ 0x1413f8278, u8"Injection pitch [maximum]" }, // 射出ピッチ[最大]
	{ 0x1413f8380, u8"Injection speed [minimum]" }, // 射出速度[最小]
	{ 0x1413f8440, u8"Injection speed [maximum]" }, // 射出速度[最大]
	{ 0x1413f8518, u8"Aiming angle range" }, // 狙い撃ちの角度範囲
	{ 0x1413f8608, u8"Movement radius" }, // 移動半径
	{ 0x1413f8830, u8"Stop time after shooting" }, // 撃ち切った後の停止時間
	{ 0x1413f8920, u8"Feather survival time" }, // 羽根の生存時間
	{ 0x1413f8a08, u8"How to spread table" }, // ばら撒き方テーブル
	{ 0x1413ee960, u8"Explosion effect scale" }, // 爆発エフェクトスケール
	{ 0x1413eea70, u8"Explosion time" }, // 爆発時間
	{ 0x1413ef6c8, u8"Head height" }, // 頭部の高さ
	{ 0x1413ef810, u8"Maximum body strength" }, // 胴体の最大体力
	{ 0x1413ef958, u8"Fuselage radius" }, // 胴体の半径
	{ 0x1413efa18, u8"Body height" }, // 胴体の高さ
	{ 0x1413efb00, u8"Height of base" }, // 土台の高さ
	{ 0x1413efc10, u8"Part spacing" }, // 部品の間隔
	{ 0x1413efcb0, u8"Ring radius" }, // リングの半径
	{ 0x1413efd60, u8"Ring height" }, // リングの高さ
	{ 0x1413efe70, u8"Ring rotation speed" }, // リングの回転速度
	{ 0x1413eff88, u8"Strength of gravity" }, // 重力の強さ
	{ 0x1413f02d0, u8"Stomping transition distance" }, // ストンピング移行距離
	{ 0x1413f03c0, u8"[Quest] Number of Kodama possessed to reach maximum parameter" }, // [クエスト]最大パラメータになるコダマ所持数
	{ 0x1413f70d0, u8"Lower limit of firing pitch angle" }, // 発射ピッチ角の下限
	{ 0x1413f7148, u8"Upper limit of firing pitch angle" }, // 発射ピッチ角の上限
	{ 0x1413f7218, u8"Lower range of firing pitch angle" }, // 発射ピッチ角の下ブレ範囲
	{ 0x1413f7358, u8"Upper range of firing pitch angle" }, // 発射ピッチ角の上ブレ範囲
	{ 0x1413f74d0, u8"Strafing" }, // 掃射
	{ 0x1413f7550, u8"Deviation shooting" }, // 偏差射撃
	{ 0x1413f7830, u8"Waiting time after shooting" }, // 射撃後の待ち時間
	{ 0x1413f05d0, u8"Shooting 3way" }, // 射撃3way
	{ 0x1413f0718, u8"Shooting 5way" }, // 射撃5way
	{ 0x1413f0820, u8"Explosive bullet" }, // 爆発弾
	{ 0x1413f5230, u8"Pillar of fire survival time" }, // 火柱の生存時間
	{ 0x1413f0908, u8"Pillar of fire" }, // 火柱
	{ 0x1413fb6c0, u8"Time between pillars of fire" }, // 火柱間の時間
	{ 0x1413fb780, u8"Distance between pillars of fire" }, // 火柱間の距離
	{ 0x1413fb890, u8"Tracking limit angle" }, // 追跡制限角度
	{ 0x1413fb968, u8"Maximum turning angle" }, // 最大旋回角度
	{ 0x1413fbaa8, u8"Number of fire pillars" }, // 火柱の数
	{ 0x1413f09e8, u8"Wave bomb" }, // ウェーブボム
	{ 0x1413f9490, u8"Climb altitude" }, // 上昇高度
	{ 0x1413f9a38, u8"Difference in fall time of fuselage" }, // 胴体の落下時間差
	{ 0x1413f9bb0, u8"Pre-rotation time" }, // 事前の回転時間
	{ 0x1413f9ce0, u8"Ring rise time" }, // リング上昇時間
	{ 0x1413f9dc0, u8"Waiting time after completion" }, // 終了後の待ち時間
	{ 0x1413f9f80, u8"Stagger distance" }, // よろめき距離
	{ 0x1413fa0e8, u8"Radius of impact transmitted to the ground" }, // 地面に衝撃が伝わる半径
	{ 0x1413fa208, u8"Maximum radius of the wave" }, // 波の最大半径
	{ 0x1413fa328, u8"Radius of wave appearance" }, // 波の出現半径
	{ 0x1413fa3e0, u8"Wave waiting radius" }, // 波の待機半径
	{ 0x1413fa468, u8"Wave extinction radius" }, // 波の消滅半径
	{ 0x1413fa530, u8"Minimum wave height" }, // 波の最小高さ
	{ 0x1413fa5f8, u8"Maximum height of waves" }, // 波の最大高さ
	{ 0x1413fa6d8, u8"Wave survival time" }, // 波の生存時間
	{ 0x1413fa7a0, u8"Wave blowing force" }, // 波の吹っ飛び力X
	{ 0x1413fa830, u8"Wave blowing force Y" }, // 波の吹っ飛び力Y
	{ 0x1413fa8f8, u8"Laser radius" }, // レーザーの半径
	{ 0x1413fa9e8, u8"Laser irradiation time" }, // レーザーの照射時間
	{ 0x1413faad8, u8"Initial speed of laser rotation" }, // レーザーの回転初速
	{ 0x1413fab58, u8"Laser rotational acceleration" }, // レーザーの回転加速度
	{ 0x1413fabc8, u8"Maximum laser rotation speed" }, // レーザーの回転最高速
	{ 0x1413fac78, u8"Laser acceleration delay time" }, // レーザーの加速遅延時間
	{ 0x1413f0af0, u8"[Thorns] Stomping" }, // [棘]ストンピング
	{ 0x1413f0bd0, u8"Stomping [Kodama Escort Minimum]" }, // ストンピング[コダマ護衛 最小]
	{ 0x1413f0ce0, u8"Stomping [Kodama Escort Max]" }, // ストンピング[コダマ護衛 最大]
	{ 0x1413f0d90, u8"Stomping [Kodama Escort 2 Minimum]" }, // ストンピング[コダマ護衛2 最小]
	{ 0x1413f0e90, u8"Stomping [Kodama Escort 2 Max]" }, // ストンピング[コダマ護衛2 最大]
	{ 0x1413f2dd0, u8"Camera gaze point offset" }, // カメラ注視点オフセット
	{ 0x1413f2f58, u8"Camera azimuth" }, // カメラ方位角
	{ 0x1413f3120, u8"Camera roll" }, // カメラロール
	{ 0x1413f3250, u8"Camera movement direction" }, // カメラの移動方向
	{ 0x1413f3308, u8"Camera movement speed" }, // カメラ移動速度
	{ 0x1413f33d0, u8"Interpolation time to return to normal camera" }, // 通常カメラに戻す補間時間
	{ 0x1413f3498, u8"Fuselage fracture interval" }, // 胴体の破壊間隔
	{ 0x1413f3508, u8"Wait time until attack state" }, // 攻撃ステートまでの待機時間
	{ 0x1413f0f90, u8"Cyloop reaction" }, // サイループリアクション
	{ 0x1413f1038, u8"[Mad] Shooting" }, // [発狂]射撃
	{ 0x1413f10f0, u8"[Madness] Shooting 3way" }, // [発狂]射撃3way
	{ 0x1413f1248, u8"[Madness] Shooting 5way" }, // [発狂]射撃5way
	{ 0x1413f1340, u8"[Crazy] Triple stomping first half" }, // [発狂]三連ストンピング前半
	{ 0x1413f1410, u8"[Crazy] Triple stomping final" }, // [発狂]三連ストンピング最後
	{ 0x1413f14c0, u8"[Insanity] Pillar of Fire" }, // [発狂]火柱
	{ 0x1413f4948, u8"Damage speed multiplier" }, // ダメージ速度倍率
	{ 0x1413f3af8, u8"Blowing force scale" }, // 吹き飛ばす力スケール
	{ 0x1413f3c48, u8"Minimum blow-off force" }, // 最小の吹き飛び力
	{ 0x1413f3d28, u8"Maximum blow-off force" }, // 最大の吹き飛び力
	{ 0x1413f3e00, u8"Blow-off force when destroying Cyloop" }, // サイループ破壊時の吹き飛び力
	{ 0x1413f4210, u8"Specify the number of fragments to be generated in 5 stages (If the value is negative, the system setting value will be used.)" }, // 破片の生成数5段階指定(負の値の場合、システム設定値を使う.)
	{ 0x1413f4320, u8"Diffusion point center position offset" }, // 拡散点中心位置オフセット
	{ 0x1413f4c18, u8"Small body fragments" }, // 体の小破片
	{ 0x1413f4cf0, u8"Small body fragment (for Cyloop reaction)" }, // 体の小破片(サイループリアクション用)
	{ 0x1413f1540, u8"Debris" }, // 破片
	{ 0x1413f15b8, u8"Camera vibration when spinning" }, // スピンピング時のカメラ振動
	{ 0x1413f1630, u8"Camera vibration when spinning [Kodama Escort]" }, // スピンピング時のカメラ振動[コダマ護衛]
	{ 0x1413f16a0, u8"Camera vibration when spinning [Kodama Escort 2]" }, // スピンピング時のカメラ振動[コダマ護衛2]
	{ 0x1413f1758, u8"Camera vibration when the torso falls" }, // 胴体落下時のカメラ振動
	{ 0x1413f5958, u8"Number of laser shots" }, // レーザー発射本数
	{ 0x1413f59e0, u8"Barrier deployment" }, // バリア展開
	{ 0x1413f5af8, u8"[Thorns] Number of laser shots" }, // [棘]レーザー発射本数
	{ 0x1413f5bc8, u8"[Thorns] Barrier deployment" }, // [棘]バリア展開
	{ 0x1413f5c78, u8"Normal bullet" }, // 通常弾
	{ 0x1413f5d20, u8"Special bullet" }, // 特殊弾
	{ 0x1413f5d80, u8"Number of torsos that can be destroyed during stomping" }, // ストンピング中の胴体破壊可能数
	{ 0x1413f5ed8, u8"Thorn development" }, // トゲ展開
	{ 0x1413f5f78, u8"Head torso collision radius" }, // 頭部胴体コリジョン半径
	{ 0x1413f60d0, u8"Head torso collision height offset" }, // 頭部胴体コリジョン高さオフセット
	{ 0x1413f2978, u8"Level parameters (for quest)" }, // レベルパラメータ(クエスト用)
	{ 0x1413f2a88, u8"Parameters by level band (for quest)" }, // レベル帯別パラメータ(クエスト用)
	{ 0x1413fc078, u8"Appearance position table number" }, // 出現位置テーブル番号
	{ 0x1413fc574, u8"Table" }, // table
	{ 0x1413fe098, u8"Fall time when the battle ends" }, // 戦闘終了になる落下時間
	{ 0x1413fe178, u8"Distance at which the battle ends" }, // 戦闘終了になる距離
	{ 0x1413931b8, u8"Appearance position" }, // 出現位置
	{ 0x1413fea98, u8"Appearance position table" }, // 出現位置テーブル
	{ 0x1413feb20, u8"Attack interval [s]" }, // 攻撃間隔[s]
	{ 0x1413febe0, u8"Waiting time after loop [s]" }, // ループ後の待ち時間[s]
	{ 0x1413fee90, u8"Shooting motion loop count" }, // 射撃モーションループ回数
	{ 0x141392a58, u8"Distance lower limit [s]" }, // 距離下限[s]
	{ 0x1413fec50, u8"Duration" }, // duration
	{ 0x141393798, u8"Minimum waiting time" }, // 待機時間最小
	{ 0x141393870, u8"Maximum waiting time" }, // 待機時間最大
	{ 0x1413939a8, u8"Action selection probability [%] (attack, guard)" }, // "行動選択確率[%](攻撃,ガード)"
	{ 0x1413946e8, u8"Proximity maintenance time" }, // 近接状態維持時間
	{ 0x141394788, u8"HA recoil at the start of melee (upward)" }, // 近接開始時のHA反動(上方向)
	{ 0x141394848, u8"HA recoil at the start of melee (backwards)" }, // 近接開始時のHA反動(後方向)
	{ 0x141394920, u8"Melee attack preliminary motion speed" }, // 近接攻撃予備動作モーション速度
	{ 0x1413954a0, u8"Position (relative to point of interest)" }, // 位置(注視点からの相対)
	{ 0x141395580, u8"Angle of view [°]" }, // 画角[°]
	{ 0x141395610, u8"Start interpolation time [s]" }, // 開始補間時間[s]
	{ 0x1413956b8, u8"End interpolation time [s]" }, // 終了補間時間[s]
	{ 0x141394af8, u8"Camera direction at the start of melee" }, // 近接開始時のカメラ演出
	{ 0x141394cb0, u8"Auto lock on camera" }, // 自動ロックオンカメラ
	{ 0x141394e00, u8"Automatic jump" }, // 自動飛び移り
	{ 0x141395f00, u8"Number of items kept (wandering)" }, // 保持個数(徘徊)
	{ 0x141396008, u8"Number of items held (confrontation)" }, // 保持個数(対峙)
	{ 0x1413960c8, u8"Number of items held (battle)" }, // 保持個数(戦闘)
	{ 0x1413968d8, u8"Movement speed [m/s]" }, // 移動速度[m/s]
	{ 0x141396cc8, u8"Non-combat time to return to wandering" }, // 徘徊に戻る非戦闘時間
	{ 0x141397540, u8"Speed [m/s]" }, // 速度[m/s]
	{ 0x141397ac0, u8"Wandering state" }, // 徘徊状態
	{ 0x141397bc8, u8"Confrontation state" }, // 対峙状態
	{ 0x141397d38, u8"Proximity state" }, // 近接状態
	{ 0x141398710, u8"Attack sequence" }, // 攻撃シーケンス
	{ 0x1413992e0, u8"Distance to start turning" }, // 曲がり始めるまでの距離
	{ 0x141399380, u8"Distance until the end of the turn" }, // 曲がり終わるまでの距離
	{ 0x141399488, u8"Detonation time per unit distance (s/m)" }, // 単位距離あたりの誘爆する時間(s/m)
	{ 0x141399cc8, u8"Normal posture adjustment speed" }, // 通常姿勢調整速度
	{ 0x141399e40, u8"Danger posture judgment angle" }, // 危険姿勢判定角度
	{ 0x141399f68, u8"Dangerous posture duration" }, // 危険姿勢継続可能時間
	{ 0x14139a0d0, u8"Player input influence ratio" }, // プレイヤー入力影響比率
	{ 0x14139a180, u8"Speed of returning to stable posture by input" }, // 入力による安定姿勢に復帰する速度
	{ 0x14139a2d8, u8"Speed of returning to dangerous position" }, // 危険姿勢に戻る速度
	{ 0x14139a430, u8"Ratio of automatic return to stable posture" }, // 安定姿勢に自動復帰する比率
	{ 0x14139a918, u8"Attitude adjustment speed by input" }, // 入力による姿勢調整速度
	{ 0x14139aad8, u8"Stable posture return speed without input" }, // 入力なし時安定姿勢復帰速度
	{ 0x14139ace8, u8"Tail rotation acceleration due to input" }, // 入力による尻尾回転加速度
	{ 0x14139ae58, u8"Input influence attenuation factor" }, // 入力影響減衰因子
	{ 0x14139b230, u8"Threshold for returning to stable posture" }, // 安定姿勢に戻る閾値
	{ 0x14139b7d0, u8"Tail node minimum rotation speed [deg/s]" }, // 尻尾ノード最小回転速度[deg/s]
	{ 0x14139b900, u8"Tail node maximum rotation speed [deg/s]" }, // 尻尾ノード最大回転速度[deg/s]
	{ 0x14139b9d8, u8"Tail node rotation speed acceleration [deg/s]" }, // 尻尾ノード回転速度加速度[deg/s]
	{ 0x14139bed0, u8"Setting applicable HP rate" }, // 設定適用HP率
	{ 0x1413a04a0, u8"Probability of not taking action" }, // 行動しない確率
	{ 0x14139bf70, u8"Ski action probability" }, // スキーアクション確率
	{ 0x14139c030, u8"Weakness exposure time reduction threshold" }, // 弱点露出時間減少閾値
	{ 0x14139c500, u8"Rigid body collision radius while diving" }, // 潜り中剛体コリジョン半径
	{ 0x14139c5c8, u8"Rigid body collision height while diving" }, // 潜り中剛体コリジョン高さ
	{ 0x14139c6e0, u8"Damage collision radius while struggling" }, // もがく中ダメージコリジョン半径
	{ 0x14139c838, u8"Struggling block collision radius" }, // もがく中ブロックコリジョン半径
	{ 0x14139c968, u8"Struggling medium block collision length" }, // もがくジ中ブロックコリジョン長さ
	{ 0x14139ca60, u8"Damage collision radius when returning to ground" }, // 地面に戻るときダメージコリジョン半径
	{ 0x14139cb98, u8"Block collision radius when returning to ground" }, // 地面に戻るときブロックコリジョン半径
	{ 0x14139cca0, u8"Collision height when returning to ground" }, // 地面に戻るときコリジョン高さ
	{ 0x14139cdd8, u8"Weak point collision radius" }, // 弱点コリジョン半径
	{ 0x14139cef8, u8"Grab collision radius" }, // 掴みコリジョン半径
	{ 0x14139cff8, u8"Jump attack collision radius" }, // 飛び出し攻撃コリジョン半径
	{ 0x1413a3f58, u8"Tail full length development time" }, // 尻尾全長展開時間
	{ 0x1413a4068, u8"Minimum length between tail nodes" }, // 尻尾ノード間最小長さ
	{ 0x1413a41a8, u8"Maximum length between tail nodes" }, // 尻尾ノード間最大長さ
	{ 0x1413a4270, u8"Ground contact determination start distance while skiing" }, // スキー中の接地判定開始距離
	{ 0x1413a4320, u8"Landing judgment end distance while skiing" }, // スキー中の接地判定終了距離
	{ 0x1413a4480, u8"Landing judgment end distance while skiing [jump]" }, // スキー中の接地判定終了距離[ジャンプ]
	{ 0x1413a45a0, u8"Landing judgment end distance while skiing [camera]" }, // スキー中の接地判定終了距離[カメラ]
	{ 0x1413a46d8, u8"Tail rotation setting [normal]" }, // 尻尾回転設定[通常]
	{ 0x1413a4828, u8"Tail rotation setting [pylon/bomb]" }, // 尻尾回転設定[パイロン/爆弾]
	{ 0x14139d800, u8"Tail related settings" }, // 尻尾関連設定
	{ 0x14139d8f0, u8"Tail left/right control settings" }, // 尻尾左右制御設定
	{ 0x14139da50, u8"Tail up and down control settings" }, // 尻尾上下制御設定
	{ 0x14139db18, u8"Tail movement control settings" }, // 尻尾移動制御設定
	{ 0x14139ff68, u8"Explosion time of a passing bomb" }, // 通り過ぎた爆弾の爆破時間
	{ 0x1413a0060, u8"Time to automatically detonate after landing" }, // 接地後に自動で爆破する時間
	{ 0x14139dbd8, u8"Drop bomb settings" }, // 投下爆弾設定
	{ 0x14139dc80, u8"Camera settings to avoid getting stuck in" }, // めり込み回避用カメラ設定
	{ 0x14139dce8, u8"Ski camera settings" }, // スキーカメラ設定
	{ 0x14139f438, u8"Battle face parameters" }, // バトルフェースパラメータ
	{ 0x14139ec88, u8"Sensor range" }, // センサー範囲
	{ 0x14139ed88, u8"Sensor out of range waiting time" }, // センサー範囲外待ち時間
	{ 0x14139eec8, u8"Rotation angle limit when landing on pass [deg]" }, // パス上着地時回転角度制限[deg]
	{ 0x14139f4f0, u8"Jump attack" }, // 飛び出し攻撃
	{ 0x1413a0ed0, u8"Maximum weak point exposure time" }, // 弱点露出時間最大値
	{ 0x1413a0fa0, u8"Normal movement speed" }, // 通常移動速度
	{ 0x1413a1080, u8"Movement speed while exposing weak point" }, // 弱点露出中移動速度
	{ 0x1413a11c8, u8"Movement speed when grabbing ends" }, // 掴み終了時移動速度
	{ 0x1413a1348, u8"Movement speed during homing HA" }, // ホーミングHA中移動速度
	{ 0x1413a1440, u8"Jump [high] movement speed" }, // ジャンプ[高]移動速度
	{ 0x1413a1560, u8"Jump [low] movement speed" }, // ジャンプ[低]移動速度
	{ 0x1413a1648, u8"Time to struggle on the ground" }, // 地上もがく時間
	{ 0x1413a1708, u8"Time to go back underground" }, // 地下に戻る時間
	{ 0x14139f5d8, u8"Skiing" }, // スキー
	{ 0x1413a1d90, u8"Maximum width of meandering movement" }, // 蛇行移動最大幅
	{ 0x1413a1e98, u8"Meandering movement period" }, // 蛇行移動周期
	{ 0x1413a1f60, u8"Start time to interpolate to meandering trajectory" }, // 蛇行軌道へ補間する開始時間
	{ 0x1413a2048, u8"End time to interpolate to meandering trajectory" }, // 蛇行軌道へ補間する終了時間
	{ 0x1413a2158, u8"Angle offset that is considered to be a dangerous posture during meandering [deg]" }, // 蛇行中危険姿勢とみなす角度オフセット[deg]
	{ 0x14139f650, u8"Meandering movement" }, // 蛇行移動
	{ 0x1413a26e8, u8"Foreshadowing movement speed [missile]" }, // 予兆移動速度[ミサイル]
	{ 0x1413a2830, u8"Attack movement minimum speed" }, // 攻撃移動最小速度
	{ 0x1413a2978, u8"Attack movement maximum speed" }, // 攻撃移動最大速度
	{ 0x1413a2ae0, u8"Maximum waiting time for posture return" }, // 姿勢復帰最大待ち時間
	{ 0x1413a2c88, u8"Attack rotation start distance" }, // 攻撃回転開始距離
	{ 0x1413a2e48, u8"Attack rotation start distance UI offset" }, // 攻撃回転開始距離UIオフセット
	{ 0x1413a3008, u8"Rotation start angular velocity [deg/s]" }, // 回転開始角速度[deg/s]
	{ 0x1413a30d0, u8"Rotation end angular velocity [deg/s]" }, // 回転終了角速度[deg/s]
	{ 0x1413a3190, u8"Input end calculation merge [pylon]" }, // 入力終了計算マージ[パイロン]
	{ 0x1413a3248, u8"Input end calculation merge [missile]" }, // 入力終了計算マージ[ミサイル]
	{ 0x14139f708, u8"Pylon/bomb common" }, // パイロン/爆弾共通
	{ 0x1413a3750, u8"Foreshadowing movement speed" }, // 予兆移動速度
	{ 0x1413a3818, u8"Omen duration" }, // 予兆継続時間
	{ 0x1413a39c0, u8"Pylon appearance time" }, // パイロン出現時間
	{ 0x1413a3aa0, u8"Pylon exit time" }, // パイロン退場時間
	{ 0x1413b4b08, u8"Thrust activation rate" }, // 突き発動率
	{ 0x1413b4c20, u8"Stomp activation rate" }, // ストンプ発動率
	{ 0x1413b4d30, u8"Bullet release rate" }, // 弾放出発動率
	{ 0x1413b4e58, u8"Chain bullet activation rate" }, // 連鎖弾発動率
	{ 0x1413b4f90, u8"Tracking wave activation rate" }, // 追跡ウェーブ発動率
	{ 0x1413b50e0, u8"Straight laser firing rate" }, // 直線レーザー発動率
	{ 0x1413b51e8, u8"Tracking laser activation rate" }, // 追跡レーザー発動率
	{ 0x1413b5320, u8"Back jump activation rate" }, // バックジャンプ発動率
	{ 0x1413aa3c0, u8"Close range activation rate" }, // 近距離発動率
	{ 0x1413aa4c0, u8"Long range activation rate" }, // 遠距離発動率
	{ 0x1413b3eb4, u8"Thrust" }, // 突き
	{ 0x1413b3f98, u8"Stomp" }, // ストンプ
	{ 0x1413b40ec, u8"Bullet" }, // 弾
	{ 0x1413b4238, u8"Chain bullet" }, // 連鎖弾
	{ 0x1413b4300, u8"Tracking wave" }, // 追跡ウェーブ
	{ 0x1413b4400, u8"Straight line laser" }, // 直線レーザー
	{ 0x1413e2420, u8"Tracking laser" }, // 追跡レーザー
	{ 0x1413b45a8, u8"Back jump" }, // バックジャンプ
	{ 0x1413aa5e0, u8"Post-action interval" }, // 行動後インターバル
	{ 0x1413abac0, u8"[Wandering] Minimum movement waiting time" }, // [徘徊]最小移動待機時間
	{ 0x1413abbb0, u8"[Wandering] Maximum movement standby time" }, // [徘徊]最大移動待機時間
	{ 0x1413abc80, u8"[Jump] Movement speed" }, // [ジャンプ]移動速度
	{ 0x1413abd38, u8"[Thus] Attack starting distance" }, // [突き]攻撃開始距離
	{ 0x1413abe68, u8"[Wave] Attack starting distance" }, // [波]攻撃開始距離
	{ 0x1413abfc0, u8"[Wave] Model scale" }, // [波]モデルスケール
	{ 0x1413ac140, u8"[Wave] Installation radius around one needle model" }, // [波]針モデル１つ辺りの設置半径
	{ 0x1413ac288, u8"[Wave] Cooldown time" }, // [波]クールタイム
	{ 0x1413ac388, u8"[Bullet] Attack starting distance" }, // [弾]攻撃開始距離
	{ 0x1413ac4b8, u8"[Bullet] Cooldown time" }, // [弾]クールタイム
	{ 0x1413ac5f0, u8"[Bullet] No movement time after firing" }, // [弾]発射後移動禁止時間
	{ 0x1413ac728, u8"[Straight laser] Attack starting distance" }, // [直線レーザー]攻撃開始距離
	{ 0x1413ac838, u8"[Straight laser] Preliminary time" }, // [直線レーザー]予兆時間
	{ 0x1413ac8e8, u8"[Straight laser] Duration" }, // [直線レーザー]持続時間
	{ 0x1413ac9c0, u8"[Straight laser] Left and right angle" }, // [直線レーザー]左右角度
	{ 0x1413aca58, u8"[Straight laser] Cool down time" }, // [直線レーザー]クール時間
	{ 0x1413acaf8, u8"[Tracking laser] Attack starting distance" }, // [追跡レーザー]攻撃開始距離
	{ 0x1413acbe0, u8"[Tracking laser] Omen time" }, // [追跡レーザー]予兆時間
	{ 0x1413accd0, u8"[Tracking laser] Duration" }, // [追跡レーザー]持続時間
	{ 0x1413acda0, u8"[Tracking laser] Tracking speed" }, // [追跡レーザー]追跡速度
	{ 0x1413aced8, u8"[Chain bullet] Attack starting distance" }, // [連鎖弾]攻撃開始距離
	{ 0x1413acf88, u8"[Chain bullet] Jump starting distance" }, // [連鎖弾]ジャンプ開始距離
	{ 0x1413ad098, u8"[Chain bullet] Cooldown time" }, // [連鎖弾]クールタイム
	{ 0x1413ad190, u8"[Tracking Wave] Attack starting distance" }, // [追跡ウェーブ]攻撃開始距離
	{ 0x1413ad238, u8"[Tracking Wave] Jump start distance" }, // [追跡ウェーブ]ジャンプ開始距離
	{ 0x1413ad368, u8"[Tracking Wave] Speed" }, // [追跡ウェーブ]速度
	{ 0x1413ad480, u8"[Tracking wave] Rotation speed [deg/sec]" }, // [追跡ウェーブ]回転速度[deg/sec]
	{ 0x1413ad5d0, u8"[Tracking wave] Needle generation time" }, // [追跡ウェーブ]針生成時間
	{ 0x1413ad6f8, u8"[Tracking wave] Width" }, // [追跡ウェーブ]幅
	{ 0x1413ad800, u8"[Tracking wave] Judgment height" }, // [追跡ウェーブ]判定高さ
	{ 0x1413ad970, u8"[Tracking wave] Rising effect width [m]" }, // [追跡ウェーブ]上昇演出幅[m]
	{ 0x1413ada80, u8"[Tracking wave] Damage width [m]" }, // [追跡ウェーブ]ダメージ発生幅[m]
	{ 0x1413adc20, u8"[Tracking wave] Descending production width [m]" }, // [追跡ウェーブ]下降演出幅[m]
	{ 0x1413adcf0, u8"[Tracking Wave] Cooldown Time" }, // [追跡ウェーブ]]クールタイム
	{ 0x1413add78, u8"[Thorn Bomb] Generation time" }, // [トゲ爆弾]生成時間
	{ 0x1413ade88, u8"[Thorn Bomb] Maximum generation radius" }, // [トゲ爆弾]最大生成半径
	{ 0x1413adf38, u8"[Thorn Bomb] Survival time after appearance" }, // [トゲ爆弾]出現後生存時間
	{ 0x1413adfe0, u8"[Thorn Bomb] Flight Speed" }, // [トゲ爆弾]飛行速度
	{ 0x1413ae0c0, u8"[Thorn Bomb] Number of hits required to destroy the protector" }, // [トゲ爆弾]プロテクター破壊までの必要ヒット数
	{ 0x1413ae198, u8"Camera vibration when moving" }, // 移動時のカメラ振動
	{ 0x1413ae218, u8"Camera vibration when attacking" }, // 攻撃時のカメラ振動
	{ 0x1413b77e0, u8"Rising production width [m]" }, // 上昇演出幅[m]
	{ 0x1413b7840, u8"Damage occurrence width [m]" }, // ダメージ発生幅[m]
	{ 0x1413b78e0, u8"Descending width [m]" }, // 下降演出幅[m]
	{ 0x1413b7b00, u8"Model maximum height" }, // モデル最大高さ
	{ 0x1413b7cb8, u8"Performance time" }, // 演出時間
	{ 0x1413b7dd8, u8"Pause start time [invalid if less than 0]" }, // 一時停止開始時間[0以下で無効]
	{ 0x1413b7e90, u8"Pause maintenance time" }, // 一時停止維持時間
	{ 0x1413ae310, u8"Wave attack settings" }, // ウェーブ攻撃設定
	{ 0x1413ae400, u8"[Camera gaze] Armor destruction" }, // [カメラ注視]アーマー破壊
	{ 0x1413ae4f8, u8"[Camera gaze] Launch" }, // [カメラ注視]打ち上げ
	{ 0x1413ae610, u8"[Camera gaze] Shoot down" }, // [カメラ注視]撃ち落とし
	{ 0x1413ae710, u8"[Camera gaze] Jump up legs" }, // [カメラ注視]脚跳ね上げ
	{ 0x1413ae840, u8"[Camera gaze] Falling through the air" }, // [カメラ注視]空中落下
	{ 0x1413aea20, u8"[Compilation] Behavior type table" }, // [総集編]行動タイプテーブル
	{ 0x1413aeb08, u8"[Complete] Action type detailed settings" }, // [総集編]行動タイプ詳細設定
	{ 0x1413b5ec0, u8"One leg down time" }, // 片足ダウン時間
	{ 0x1413b6088, u8"Complete down time" }, // 完全ダウン時間
	{ 0x1413b6270, u8"Time until protector reloading" }, // プロテクター再装填までの時間
	{ 0x1413b0ba0, u8"HP percentage to switch phases" }, // フェーズを切り替えるHP割合
	{ 0x1413b0c58, u8"Walking wave setting ID" }, // 歩行ウェーブの設定ID
	{ 0x1413b0d68, u8"Stomp wave setting ID" }, // ストンプウェーブの設定ID
	{ 0x1413b0ea8, u8"Press wave setting ID" }, // プレスウェーブの設定ID
	{ 0x1413b1010, u8"Do you counterattack when you have no armor?" }, // アーマーなし時足囲みに反撃するか
	{ 0x1413b1110, u8"Box strategy type" }, // 囲み攻略タイプ
	{ 0x1413b1208, u8"[Bullets] Radius" }, // [弾]半径
	{ 0x1413b1368, u8"[Bullets] Blast radius" }, // [弾]爆破半径
	{ 0x1413b1458, u8"[Bullets] Number of shots [one side]" }, // [弾]発射数[片側]
	{ 0x1413b1528, u8"[Bullets] Initial velocity" }, // [弾]初速
	{ 0x1413b15d8, u8"[Bullets] Deceleration force" }, // [弾]減速力
	{ 0x1413b16d8, u8"[Bullets] Minimum speed" }, // [弾]最低速度
	{ 0x1413b1838, u8"[Bullets] Bullet speed after parry" }, // [弾]パリィ後の弾丸速度
	{ 0x1413b1948, u8"[Bullets] Minimum survival time" }, // [弾]最小生存時間
	{ 0x1413b1ab8, u8"[Bullets] Percentage of bullets that can be parried [0.0~1.0]" }, // [弾]パリィ可能弾の割合[0.0~1.0]
	{ 0x1413b1bd8, u8"[Bullets] Maximum number of bullets [horizontal axis]" }, // [弾]弾丸最大数[横軸]
	{ 0x1413b1d90, u8"[Bullets] Number of bullets [vertical axis]" }, // [弾]弾丸数[縦軸]
	{ 0x1413b1ef0, u8"[Bullets] Number of consecutive shots" }, // [弾]連続発射回数
	{ 0x1413b20a8, u8"[Chain bullet] Lifespan time" }, // [連鎖弾]寿命時間
	{ 0x1413b2230, u8"[Chain bullet] Follow-up time" }, // [連鎖弾]追従時間
	{ 0x1413b2340, u8"[Chain bullet] Bullet speed" }, // [連鎖弾]弾速
	{ 0x1413b2470, u8"[Chain bullet] Firing interval" }, // [連鎖弾]発射間隔
	{ 0x1413b25b8, u8"[Chain bullet] Rotation speed" }, // [連鎖弾]回転速度
	{ 0x1413b26e0, u8"[Chain bullet] Total number of bullets" }, // [連鎖弾]弾列合計数
	{ 0x1413b2858, u8"Armor reload enabled" }, // アーマー再装填有効
	{ 0x1413b6ae0, u8"Thrust type 0: Normal 1: Wave" }, // 突き種類 0:通常 1:ウェーブ
	{ 0x1413b6be0, u8"Number of thrust attacks" }, // 突き攻撃回数
	{ 0x1413b6d00, u8"Interval between consecutive thrusts" }, // 連続突きの間隔
	{ 0x1413b6e60, u8"Stomp type 0: Normal 1: Wave" }, // ストンプ種類 0:通常 1:ウェーブ
	{ 0x1413b2ba8, u8"Behavior settings" }, // 行動設定
	{ 0x1413b7388, u8"Diving end height" }, // ダイビング終了高さ
	{ 0x1413b2d60, u8"Diving settings" }, // ダイビング設定
	{ 0x1413b2e10, u8"Slow multiplier after thrust attack" }, // 突き攻撃後スロー倍率
	{ 0x1413b86e0, u8"Attack method" }, // 攻撃方法
	{ 0x1413b87d0, u8"Do you use it?" }, // 使用するか
	{ 0x1413b88a0, u8"Attack interval (seconds)" }, // 攻撃間隔（秒）
	{ 0x1413b8f60, u8"Attacks used and their settings" }, // 使用する攻撃とその設定
	{ 0x1413b90e0, u8"Remaining HP percentage that becomes a trigger" }, // トリガーとなる残りHP割合
	{ 0x1413b94d0, u8"Time for the arc laser to arrive at the rail" }, // 弧状レーザーがレールに到着する時間
	{ 0x1413b95f0, u8"Number of patterns" }, // パターン数
	{ 0x1413b96b8, u8"Polygon radius" }, // ポリゴンの半径
	{ 0x1413b9828, u8"Texture tiling distance" }, // テクスチャのタイリング距離
	{ 0x1413b9900, u8"Fluctuation period" }, // ゆらぎ周期
	{ 0x1413b99c0, u8"Fluctuation amplitude" }, // ゆらぎ振幅
	{ 0x1413b9a48, u8"UV scroll speed" }, // UVスクロールスピード
	{ 0x1413b9b08, u8"Primary color" }, // プライマリーカラー
	{ 0x1413b9bb0, u8"Edge vertex color" }, // 端の頂点カラー
	{ 0x1413b9cc0, u8"Center vertex color" }, // 中央の頂点カラー
	{ 0x1413b9d88, u8"Color intensity" }, // カラー強度
	{ 0x1413b9ea8, u8"Patachen interval (seconds)" }, // パタチェン間隔（秒）
	{ 0x1413b9fb0, u8"Edge width scale" }, // 端の幅スケール
	{ 0x1413ba0a0, u8"Interpolation distance of edge width scale" }, // 端の幅スケールの補間距離
	{ 0x1413baa50, u8"Firing direction" }, // 発射方向
	{ 0x1413baae8, u8"Lifespan (seconds)" }, // 寿命（秒）
	{ 0x1413bac78, u8"Bullet speed to rail" }, // レールまでの弾速
	{ 0x1413bada8, u8"Bullet speed on rail" }, // レール上での弾速
	{ 0x1413bae60, u8"Height offset" }, // 高さオフセット
	{ 0x1413bb0c0, u8"Rail change delay" }, // レール変更ディレイ
	{ 0x1413bb140, u8"Turning time by spline" }, // スプラインによる転向時間
	{ 0x1413bb218, u8"Distance between spline start and end points" }, // スプライン始点終点間距離
	{ 0x1413bb2d8, u8"Should I change the rails?" }, // レールを変更するか
	{ 0x1413bb5a8, u8"Parameters of homing bullet" }, // 追尾這い弾のパラメータ
	{ 0x1413bb630, u8"Parameters of reverse tracking bullet" }, // 逆追尾這い弾のパラメータ
	{ 0x1413bb720, u8"Lane fixed crawling bullet parameters" }, // レーン固定這い弾のパラメータ
	{ 0x1413bbaf8, u8"Hoop rail bullet speed" }, // フープレール弾速
	{ 0x1413bbba0, u8"Hoop rail bullet rotation angle/sec" }, // フープレール弾回転角度/秒
	{ 0x1413bbc38, u8"Time taken to jump to center" }, // 中心へのジャンプにかかる時間
	{ 0x1413bbcb0, u8"Time taken to jump to runner-up" }, // 次点へのジャンプにかかる時間
	{ 0x1413bbd30, u8"Jump spline tangent Y value to center" }, // 中心へのジャンプスプラインタンジェントY値
	{ 0x1413bbe10, u8"Jump spline end point tangent magnification to next point (this * (X,0,Z))" }, // "次点へのジャンプスプライン終点タンジェント倍率（this*（X,0,Z））"
	{ 0x1413bbf88, u8"Jump point Y offset" }, // ジャンプ地点Yオフセット
	{ 0x1413bc0a8, u8"Jump point XZ offset (backwards)" }, // ジャンプ地点XZオフセット（奥方向）
	{ 0x1413bc178, u8"Jump target position height to center platform" }, // 中心足場へのジャンプ目標位置高さ
	{ 0x1413bc250, u8"Horizontal offset of jump target position to central platform" }, // 中心足場へのジャンプ目標位置の水平オフセット
	{ 0x1413bc378, u8"Jump time to central platform" }, // 中心足場へのジャンプ時間
	{ 0x1413bc450, u8"Waiting time after arriving at Hoop Rail" }, // フープレール到着後の待機時間
	{ 0x1413bc618, u8"Rail division distance" }, // レール分割距離
	{ 0x1413bc730, u8"Collision height" }, // コリジョン高さ
	{ 0x1413bc818, u8"Collision radius addend" }, // コリジョン半径加数
	{ 0x1413bc920, u8"Effect scale (height) when firing" }, // 発射時エフェクトスケール（高さ）
	{ 0x1413bca50, u8"Effect scale when firing (horizontal)" }, // 発射時エフェクトスケール（横）
	{ 0x1413bcba8, u8"Maximum effect scale (horizontal)" }, // 最大エフェクトスケール（横）
	{ 0x1413bccc8, u8"Time until effect scale reaches maximum" }, // エフェクトスケールが最大になるまでの時間
	{ 0x1413bdd68, u8"Angular velocity before and after laser attack (degrees/second)" }, // レーザー攻撃前後の角速度（度／秒）
	{ 0x1413bdee8, u8"Laser angular velocity (degrees/second)" }, // レーザー角速度（度／秒）
	{ 0x1413be0a8, u8"Rotation angle" }, // 回転角度
	{ 0x1413be280, u8"Old laser radius" }, // 旧レーザー半径
	{ 0x1413be4c0, u8"Initial firing position minimum offset distance" }, // 初期発射位置最小オフセット距離
	{ 0x1413be650, u8"Initial firing position maximum offset distance" }, // 初期発射位置最大オフセット距離
	{ 0x1413be770, u8"Arc laser settings" }, // 弧状レーザー設定
	{ 0x1413c0860, u8"Attack phase" }, // アタックフェーズ
	{ 0x1413c0900, u8"Initial attack standby time (crawling bullet)" }, // 初期攻撃待機時間（這い弾）
	{ 0x1413c0aa0, u8"Initial attack standby time (laser)" }, // 初期攻撃待機時間（レーザー）
	{ 0x1413c0bd0, u8"Attackable time" }, // 攻撃可能時間
	{ 0x1413c0d50, u8"Hoop rail bullet attack interval (seconds)" }, // フープレール弾攻撃間隔（秒）
	{ 0x1413c0f00, u8"Temporary scaffolding top surface height offset" }, // 仮足場上面高度オフセット
	{ 0x1413c1058, u8"Temporary scaffold lower surface height offset" }, // 仮足場下面高度オフセット
	{ 0x1413c1188, u8"Temporary scaffolding radius" }, // 仮足場半径
	{ 0x1413c1278, u8"Camera elevation angle when grinding" }, // グラインド時カメラ仰角
	{ 0x1413c1428, u8"Distance from camera gaze point when grinding" }, // グラインド時カメラ注視点からの距離
	{ 0x1413c18c8, u8"Camera elevation angle when barrier is destroyed" }, // バリア破壊時カメラ仰角
	{ 0x1413c1a30, u8"Distance from camera gaze point when barrier is destroyed" }, // バリア破壊時カメラ注視点からの距離
	{ 0x1413c1b70, u8"Camera interpolation time when barrier is destroyed" }, // バリア破壊時カメラ補間時間
	{ 0x1413c1d10, u8"Camera duration when barrier is destroyed" }, // バリア破壊時カメラ継続時間
	{ 0x1413c1ea0, u8"Activate camera when barrier is destroyed" }, // バリア破壊時カメラ有効化
	{ 0x1413c1fc0, u8"Enable stun" }, // スタンを有効化
	{ 0x1413c2080, u8"Enable flinch" }, // 怯みを有効化
	{ 0x1413bfbd0, u8"Strider body parameters" }, // ストライダー本体のパラメータ
	{ 0x1413c2a68, u8"Inner rail radius" }, // 内側のレール半径
	{ 0x1413c2c60, u8"Outer rail radius" }, // 外側のレール半径
	{ 0x1413c2ea0, u8"Inner rail Y offset" }, // 内側のレールYオフセット
	{ 0x1413c3028, u8"Middle rail Y offset" }, // 真ん中のレールYオフセット
	{ 0x1413c31a8, u8"Outer rail Y offset" }, // 外側のレールYオフセット
	{ 0x1413c32a0, u8"Number of hoop rails fired at once" }, // 一度に発射するフープレールの数
	{ 0x1413c33b0, u8"How many seconds before the hoop rail launch should the foreshadowing effect be played?" }, // フープレール発射何秒前に予兆エフェクトを再生するか
	{ 0x1413c3478, u8"Hoop rail bullet radius" }, // フープレール弾半径
	{ 0x1413c35c0, u8"Hoop rail bullet search distance (based on outer rail points)" }, // フープレール弾検索距離（外側レールの点基準）
	{ 0x1413c3778, u8"Maximum jump distance from hoop rail" }, // フープレールからの最大ジャンプ距離
	{ 0x1413c38e0, u8"Permissible distance from the hoop rail to the PositionIndicator where the jump is made" }, // フープレールからのジャンプが行われるPositionIndicatorまでの許容距離
	{ 0x1413bfcc0, u8"Rail-related parameters (those used outside the rail object)" }, // レール関連パラメータ（レールオブジェクト外で使用するもの）
	{ 0x1413c4500, u8"Attack platform radius" }, // 攻撃時足場半径
	{ 0x1413c4708, u8"Scaffolding height during attack" }, // 攻撃時足場高さ
	{ 0x1413bfd88, u8"Scaffold related parameters during attack" }, // 攻撃時足場関連パラメータ
	{ 0x1413c4cd0, u8"Base height" }, // ベース部分の高さ
	{ 0x1413c4e70, u8"Base radius" }, // ベース部分の半径
	{ 0x1413c5010, u8"Base collision offset" }, // ベース部分のコリジョンオフセット
	{ 0x1413c51e0, u8"Height of pillar part" }, // 柱部分の高さ
	{ 0x1413c5350, u8"Radius of column part" }, // 柱部分の半径
	{ 0x1413c5498, u8"Additional height for rigid body collisions" }, // 剛体コリジョンの追加の高さ
	{ 0x1413c56a0, u8"Rope collision thickness (rigid body)" }, // ロープコリジョンの厚さ(剛体))
	{ 0x1413c5848, u8"Rope collision thickness (damage)" }, // ロープコリジョンの厚さ(ダメージ)
	{ 0x1413c5980, u8"Thickness of rope area determination" }, // ロープエリア判定の厚み
	{ 0x1413c6000, u8"Current damage to enemies" }, // エネミーへの電流ダメージ
	{ 0x1413c9b08, u8"Guide display distance" }, // ガイド表示距離
	{ 0x1413c9b78, u8"Raycast distance" }, // レイキャスト距離
	{ 0x1413c9c18, u8"Collision detection capsule height" }, // 衝突判定のカプセルの高さ
	{ 0x1413c9cf8, u8"Maximum travel distance" }, // 最大移動距離
	{ 0x1413c9dc8, u8"Maximum number of reflections" }, // 最大反射回数
	{ 0x1413c9ed0, u8"Maximum number of reflections to inflated Sumo" }, // 膨らみ状態のSumoへの最大反射回数
	{ 0x1413c9fe8, u8"Due to the position offset" }, // ため中の位置オフセット
	{ 0x1413ca0f0, u8"Launch angle limit" }, // 発射角度制限
	{ 0x1413ca1e0, u8"Firing angle variation speed" }, // 発射角度変動速度
	{ 0x1413ca2f0, u8"Initial firing speed" }, // 発射初速度
	{ 0x1413ca4d8, u8"Addition rate per reflection" }, // 反射ごとの加算速度
	{ 0x1413ca5c8, u8"Injection speed when canceling" }, // キャンセル時の射出速度
	{ 0x1413ca6f8, u8"String tension" }, // 弦の引き尺
	{ 0x1413ca7c0, u8"Timescale while aiming" }, // エイム中のタイムスケール
	{ 0x1413ca8f8, u8"Time limit while aiming" }, // エイム中の時間制限
	{ 0x1413cc550, u8"Camera interpolation time" }, // カメラ補間時間
	{ 0x1413cc620, u8"Camera height" }, // カメラの高さ
	{ 0x1413caa10, u8"Top-down camera settings" }, // 見下ろしカメラ設定
	{ 0x1413c6178, u8"Slingshot settings" }, // スリングショット設定
	{ 0x1413cb540, u8"Prediction line width" }, // 予測線の幅
	{ 0x1413cb690, u8"Maximum distance of predicted line" }, // 予測線の最大距離
	{ 0x1413cb788, u8"Prediction line tiling distance" }, // 予測線のタイリング距離
	{ 0x1413cb830, u8"UV scroll minimum speed" }, // UVスクロール最低速度
	{ 0x1413cb948, u8"UV scroll maximum speed" }, // UVスクロール最高速度
	{ 0x1413cba48, u8"Number of reflections for UV scrolling to reach maximum speed" }, // UVスクロールが最高速度になる反射回数
	{ 0x1413cbba8, u8"Vertex color per reflection" }, // 反射ごとの頂点カラー
	{ 0x1413cbde0, u8"Should the line color be gradated?" }, // 線の色をグラデーションさせるか
	{ 0x1413cbf08, u8"Change the entire line depending on the number of reflections" }, // 反射回数に応じて線全体を変える
	{ 0x1413c6230, u8"Prediction line settings" }, // 予測線設定
	{ 0x1413c6a40, u8"Admission time" }, // 入場時間
	{ 0x1413c6b28, u8"Exit time" }, // 退場時間
	{ 0x1413c6c38, u8"Number of ropes" }, // ロープの数
	{ 0x1413c6e10, u8"Minimum swing width during reflection" }, // 反射時の最小揺れ幅
	{ 0x1413c6fa8, u8"Maximum swing width during reflection" }, // 反射時の最大揺れ幅
	{ 0x1413c7158, u8"Swing width attenuation" }, // 揺れ幅減衰
	{ 0x1413c72c8, u8"Shaking time" }, // 揺れ時間
	{ 0x1413c75e8, u8"Time when the bending slowly returns" }, // しなりがゆっくり戻るときの時間
	{ 0x1413c8208, u8"Polygon width" }, // ポリゴンの幅
	{ 0x1413c8a90, u8"Color intensity animation time" }, // カラー強度アニメーション時間
	{ 0x1413c8c58, u8"Minimum color intensity" }, // カラー強度最低値
	{ 0x1413c8e20, u8"Maximum color intensity" }, // カラー強度最高値
	{ 0x1413c8f68, u8"Patachen spacing" }, // パタチェン間隔
	{ 0x1413c9278, u8"Polygon roll rotation angle" }, // ポリゴンのロール回転角度
	{ 0x1413c7818, u8"Current setting" }, // 電流設定
	{ 0x1413c6320, u8"Rope settings" }, // ロープ設定
	{ 0x1413ccc38, u8"Height of stairs that can be climbed" }, // 登れる段さの高さ
	{ 0x1413ccd00, u8"Rigid body collision height" }, // 剛体コリジョンの高さ
	{ 0x1413ccdc8, u8"Rigid body collision radius" }, // 剛体コリジョンの半径
	{ 0x1413ccf68, u8"Event collision height" }, // イベントコリジョンの高さ
	{ 0x1413cd060, u8"Event collision radius" }, // イベントコリジョンの半径
	{ 0x1413cd190, u8"Collision height offset during slingshot" }, // スリングショット中のコリジョンの高さオフセット
	{ 0x1413cd3a0, u8"Bumper blowing speed" }, // バンパー吹き飛ばし速度
	{ 0x1413cd4f0, u8"Bumper inoperable time" }, // バンパー操作不能時間
	{ 0x1413cd620, u8"Amount of damage to enemy" }, // エネミーへのダメージ量
	{ 0x1413cd748, u8"Survival time after parrying" }, // パリーした後の生存時間
	{ 0x1413cd848, u8"Deceleration after parry" }, // パリー後の減速度
	{ 0x1413ce390, u8"Stiffness time due to pack" }, // パックによる硬直時間
	{ 0x1413ce4e0, u8"Expanding time" }, // 膨らんでいる時間
	{ 0x1413ce608, u8"Crouching time" }, // うずくまり時間
	{ 0x1413ce6d8, u8"Visual height offset" }, // visualの高さオフセット
	{ 0x1413ce798, u8"HP gauge display offset" }, // HPゲージ表示オフセット
	{ 0x1413ce8f8, u8"Guard effect cooldown time" }, // ガードエフェクトのクールタイム
	{ 0x1413d9c58, u8"Number of pucks thrown" }, // パックを投げる個数
	{ 0x1413cf0c0, u8"Pack settings" }, // パック設定
	{ 0x1413d3f90, u8"Is this pattern valid?" }, // このパターンが有効か
	{ 0x1413d4020, u8"Current ON/OFF of each rope" }, // 各ロープの電流ON/OFF
	{ 0x1413db7e8, u8"Pattern table" }, // パターンテーブル
	{ 0x1413cf1d8, u8"Current rotation (octagonal)" }, // 電流ローテーション(8角)
	{ 0x1413cf270, u8"Current rotation (7 corners)" }, // 電流ローテーション(7角)
	{ 0x1413cf3a0, u8"Current rotation (hexagonal)" }, // 電流ローテーション(6角)
	{ 0x1413cf478, u8"Current rotation (pentagonal)" }, // 電流ローテーション(5角)
	{ 0x1413cf5c0, u8"Current rotation (4 corners)" }, // 電流ローテーション(4角)
	{ 0x1413d1878, u8"Node name to attach" }, // アタッチするノード名
	{ 0x1413d1b10, u8"Tag (used by programmer)" }, // タグ(プログラマ利用)
	{ 0x1413d1c08, u8"Properties" }, // プロパティ
	{ 0x1413d1db0, u8"Color during debug display" }, // デバッグ表示時の色
	{ 0x1413cfba8, u8"Taken damage" }, // 被ダメージ
	{ 0x1413cfc68, u8"Damage dealt" }, // 与ダメージ
	{ 0x1413cfd80, u8"Puck detection" }, // パック検知
	{ 0x1413cfe58, u8"For Cyloop bulging condition" }, // サイループ膨らみ状態用
	{ 0x1413cff08, u8"Blow to avoid crushing" }, // 押しつぶし回避用の吹き飛ばし
	{ 0x1413d00a0, u8"For arm sonic detection" }, // 腕のソニック検知用
	{ 0x1413d01b8, u8"Simple rigid body" }, // 簡易剛体
	{ 0x1413d0270, u8"Movement radius" }, // 移動用半径
	{ 0x1413d0340, u8"Transport height" }, // 移動用高さ
	{ 0x1413d0508, u8"Target marker display offset" }, // ターゲットマーカー表示オフセット
	{ 0x1413d0690, u8"Combo minimum radius" }, // コンボ最小半径
	{ 0x1413d0738, u8"Combo max radius" }, // コンボ最大半径
	{ 0x1413d08f0, u8"Combo height" }, // コンボ高さ
	{ 0x1413d22e0, u8"Small boss common settings" }, // 小ボス共通設定
	{ 0x1413da1d8, u8"Change cooldown of current rope" }, // 電流の流すロープの変更クールダウン
	{ 0x1413da330, u8"No current flow time after receiving damage" }, // ダメージを受けた後に電流を流さない時間
	{ 0x1413da448, u8"Blow speed when touching current damage" }, // 電流ダメージに触れた際の吹き飛ばし速度
	{ 0x1413da540, u8"Deceleration when blown away by current damage" }, // 電流ダメージで吹き飛ばされた際の減速度
	{ 0x1413da6a8, u8"Time to twitch with current rope" }, // 電流ロープでビリビリする時間
	{ 0x1413da7c0, u8"Amount of current damage received for each number of reflections" }, // 反射回数ごとの受ける電流ダメージ量
	{ 0x1413da8c0, u8"Bending distance when hitting the rope" }, // ロープにヒットした際のしなり距離
	{ 0x1413daaa0, u8"Number of times it bounces off the rope when inflated" }, // 膨らみ状態でロープに跳ね返る回数
	{ 0x1413dac70, u8"Blowing speed when touching the rope while inflated" }, // 膨らみ状態でロープに触れた際の吹き飛ばし速度
	{ 0x1413dae30, u8"Deceleration after the number of bounces on the rope in the inflated state" }, // 膨らみ状態でロープで跳ね返る回数後の減速度
	{ 0x1413daf60, u8"Spring constant of panning delay to be observed when touching with rope in inflated state" }, // 膨らみ状態でロープで触れた際に注視するパンニング遅延のバネ定数
	{ 0x1413db020, u8"Maximum angle of reflection of the rope in the inflated state" }, // 膨らみ状態でのロープの反射最大角度
	{ 0x1413dc078, u8"Retraction speed per number of reflections" }, // 反射回数ごとの後退速度
	{ 0x1413dc158, u8"Reverse deceleration" }, // 後退減速度
	{ 0x1413dc238, u8"The angle at which it is determined that the hit occurred from the front." }, // 前からヒットしたと判定する角度
	{ 0x1413dc388, u8"Time to face" }, // 正面を向ける時間
	{ 0x1413d9508, u8"Horizontal tracking limit angle" }, // 水平方向追従制限角
	{ 0x1413d9718, u8"Deceleration during rotation" }, // 回転時の減速度
	{ 0x1413d25a8, u8"Move settings" }, // 移動設定
	{ 0x1413dc808, u8"Throwing distance" }, // 投げる距離
	{ 0x1413dc9d8, u8"Offset when throwing" }, // 投げる時のオフセット
	{ 0x1413dcaf8, u8"Rotation when throwing (left)" }, // 投げる時の回転(左)
	{ 0x1413dcc18, u8"Rotation when throwing (right)" }, // 投げる時の回転(右)
	{ 0x1413dcd68, u8"Projection angle limit" }, // 投射角度制限
	{ 0x1413dce78, u8"Offset the target point when throwing multiple shots" }, // 複数投げる際の目標地点をオフセット
	{ 0x1413d2730, u8"Puck throwing settings" }, // パック投げ設定
	{ 0x1413dbb08, u8"Number of combos" }, // コンボ数
	{ 0x1413dbbf8, u8"Reflects towards Sumo during combo" }, // コンボの際にSumoに向かって反射する
	{ 0x1413dbc68, u8"The width of the blur reflected towards Sumo during a combo" }, // コンボの際にSumoに向かって反射するブレ幅
	{ 0x1413d2880, u8"Combo settings" }, // コンボ設定
	{ 0x1413d6fc0, u8"Foot uplay cast distance" }, // 足のアップレイキャスト距離
	{ 0x1413d7120, u8"Foot downlay cast distance" }, // 足のダウンレイキャスト距離
	{ 0x1413d43b8, u8"Axis of rotation in knee local space" }, // 膝のローカルスペースにおける回転軸
	{ 0x1413d44e0, u8"Termination of the foot in the local space of the ankle" }, // 足首のローカルスペースにおける足の終端部
	{ 0x1413d45b0, u8"Ankle height considered to be landing" }, // 着地しているとみなす足首の高さ
	{ 0x1413d46c0, u8"Ankle height at which the foot is considered fully raised" }, // 足が完全に上がっているとみなす足首の高さ
	{ 0x1413d47b0, u8"Maximum height that the ankle can reach" }, // 足首が届く最大の高さ
	{ 0x1413d48f8, u8"Minimum height that the ankle can reach" }, // 足首が届く最小の高さ
	{ 0x1413d4a68, u8"Knee angle limit" }, // 膝の角度制限
	{ 0x1413d4c48, u8"Gain when switching ground ON/OFF" }, // "接地のON,OFF切り替え時のゲイン"
	{ 0x1413d4d68, u8"Gain as ground altitude increases" }, // 地面高度が上昇するときのゲイン
	{ 0x1413d4e48, u8"Gain when ground altitude decreases" }, // 地面高度が下降するときのゲイン
	{ 0x1413d4fb8, u8"Gain when the foot touches the ground" }, // 足を地面へ接地させる場合のゲイン
	{ 0x1413d50e0, u8"Gain when lifting the foot off the ground" }, // 足を地面から離す場合のゲイン
	{ 0x1413d5210, u8"Gain when switching between locked and unlocked legs" }, // 足のロック状態とアンロック状態の切り替え時のゲイン
	{ 0x1413d5358, u8"Ankle rotation gain" }, // 足首の回転のゲイン
	{ 0x1413d5408, u8"Hip bone name" }, // 腰骨の名前
	{ 0x1413d54c8, u8"Knee bone name" }, // 膝骨の名前
	{ 0x1413d5528, u8"Name of heel bone" }, // かかと骨の名前
	{ 0x1413d5618, u8"Toe bone name" }, // つまさき骨の名前
	{ 0x1413d7210, u8"Foot IK parameters" }, // 足のIKパラメータ
	{ 0x1413d7308, u8"Hand uplay cast distance" }, // 手のアップレイキャスト距離
	{ 0x1413d7418, u8"Hand downlay cast distance" }, // 手のダウンレイキャスト距離
	{ 0x1413d74f0, u8"Hand sweep radius" }, // 手のSweep半径
	{ 0x1413d7620, u8"Gain of changing hand position" }, // 手の設置位置変更のゲイン
	{ 0x1413d7750, u8"Hand IK IN interpolation time" }, // 手IKのIN補間時間
	{ 0x1413d7850, u8"Hand IK OUT interpolation time" }, // 手IKのOUT補間時間
	{ 0x1413d62d0, u8"Axis of rotation in elbow local space" }, // 肘のローカルスペースにおける回転軸
	{ 0x1413d6448, u8"End of the hand in the local space of the wrist" }, // 手首のローカルスペースにおける手の終端部
	{ 0x1413d65a8, u8"Normal in local space of back of hand" }, // 手の甲のローカルスペースにおける法線
	{ 0x1413d6690, u8"Elbow angle limit" }, // 肘の角度制限
	{ 0x1413d67c8, u8"Do you want the wrist rotation to follow the target?" }, // 手首の回転をターゲットに追従させるか
	{ 0x1413d6870, u8"Shoulder bone name" }, // 肩骨の名前
	{ 0x1413d6910, u8"Elbow bone name" }, // 肘骨の名前
	{ 0x1413d69e0, u8"Wrist bone name" }, // 手首骨の名前
	{ 0x1413d7970, u8"Hand IK parameters" }, // 手のIKパラメータ
	{ 0x1413d2988, u8"IK settings" }, // IK設定
	{ 0x1413d37d8, u8"Pack speed" }, // パック速度
	{ 0x1413d38e8, u8"Time to start decelerating" }, // 減速を開始する時間
	{ 0x1413d39f8, u8"Pack deceleration" }, // パック減速度
	{ 0x1413d3b78, u8"Pack life" }, // パック寿命
	{ 0x1413d2ad8, u8"Cyloop settings" }, // サイループ設定
	{ 0x1413d83e0, u8"Behavior pattern settings" }, // 行動パターン設定
	{ 0x1413dd780, u8"Interval table" }, // インターバルテーブル
	{ 0x1413dd828, u8"Action table" }, // アクションテーブル
	{ 0x1413dd908, u8"Number of bases destroyed to go to the next stage" }, // 次の段階にいくための土台撃破数
	{ 0x1413dddb8, u8"Firing time" }, // 発射時間
	{ 0x1413ddf78, u8"Orientation change speed" }, // 向き変更速度
	{ 0x1413de070, u8"Orientation change start time" }, // 向き変更開始時間
	{ 0x1413df598, u8"[Camera gaze] Damage" }, // [カメラ注視]ダメージ
	{ 0x1413e4298, u8"Speed toward the player" }, // プレーヤーの方向へ向く速度
	{ 0x1413e43a8, u8"Speed towards the camera" }, // カメラの方向へ向く速度
	{ 0x1413e4470, u8"Speed at which to change direction of movement" }, // 移動方向を変更する速度
	{ 0x1413e4528, u8"Movement speed during tracking" }, // 追跡時の移動速度
	{ 0x1413e4630, u8"Movement speed when approaching" }, // 接近時の移動速度
	{ 0x1413e4748, u8"Movement speed to next platform" }, // 次の土台への移動速度
	{ 0x1413e4808, u8"Traveling speed when returning home" }, // 帰宅時の移動速度
	{ 0x1413e4990, u8"Movement speed when turning" }, // ターン時の移動速度
	{ 0x1413e4c38, u8"Deceleration judgment distance" }, // 減速判定距離
	{ 0x1413e4f20, u8"Deceleration spring coefficient" }, // 減速バネ係数
	{ 0x1413e50f8, u8"Vertical movement width" }, // 垂直方向移動幅
	{ 0x1413e52e0, u8"Vertical displacement damper" }, // 垂直方向移動ダンパー
	{ 0x1413e5530, u8"Vertical movement spring coefficient" }, // 垂直方向移動バネ係数
	{ 0x1413e56e0, u8"Vertical movement rotation speed" }, // 垂直方向移動回転速度
	{ 0x1413e58a0, u8"Tracking judgment distance" }, // 追跡判定距離
	{ 0x1413e59b0, u8"Approach judgment distance" }, // 接近判定距離
	{ 0x1413e5aa0, u8"Return home determination distance" }, // 帰宅判定距離
	{ 0x1413e5c00, u8"Tracking target radius" }, // 追跡目標半径
	{ 0x1413e5d60, u8"Tracking target altitude" }, // 追跡目標高度
	{ 0x1413e5e28, u8"Track migration time" }, // 追跡移行時間
	{ 0x1413df770, u8"Bullet parameters" }, // 弾パラメータ
	{ 0x1413e2c30, u8"Number of installations (foundation installation)" }, // 設置数(土台設置)
	{ 0x1413e2d18, u8"Number of installations (installed around the player)" }, // 設置数(プレーヤー周辺設置)
	{ 0x1413e2e50, u8"Installation altitude (foundation installation)" }, // 設置高度(土台設置)
	{ 0x1413e2fc0, u8"Estimated installation time (installation around the player)" }, // 設置予測時間(プレーヤー周辺設置)
	{ 0x1413e30e8, u8"Installation search radius" }, // 設置検索半径
	{ 0x1413e31d0, u8"Installation range (installation around the player)" }, // 設置範囲(プレーヤー周辺設置)
	{ 0x1413e3380, u8"Installation altitude (installed around the player)" }, // 設置高度(プレーヤー周辺設置)
	{ 0x1413e3520, u8"Installation interval (installation around the player)" }, // 設置間隔(プレーヤー周辺設置)
	{ 0x1413e3620, u8"Timer start time" }, // タイマー起動時間
	{ 0x1413e3788, u8"Fluctuation range of timer start time" }, // タイマー起動時間の揺らぎ幅
	{ 0x1413e3980, u8"Timer" }, // タイマー
	{ 0x1413e3b98, u8"Maximum distance when selecting target" }, // ターゲット選択時の最大距離
	{ 0x1413df850, u8"Mine parameters" }, // 機雷パラメータ
	{ 0x1413e69b0, u8"Minimum installation radius" }, // 設置半径の最小値
	{ 0x1413e6ac8, u8"Maximum installation radius" }, // 設置半径の最大値
	{ 0x1413e6b88, u8"Maximum distance from the player at the installation location" }, // 設置位置のプレーヤーからの距離の最大値
	{ 0x1413e6c40, u8"Minimum installation height" }, // 最小設置高度
	{ 0x1413e6d00, u8"Maximum installation height" }, // 最大設置高度
	{ 0x1413e6e40, u8"Estimated travel time" }, // 移動予測時間
	{ 0x1413e7160, u8"Stop time when firing" }, // 発射時停止時間
	{ 0x1413e7288, u8"Distance to decelerate" }, // 減速する距離
	{ 0x1413e7420, u8"Player tracking radius" }, // プレイヤー追跡半径
	{ 0x1413e7518, u8"Player tracking speed" }, // プレイヤー追跡速度
	{ 0x1413e75e0, u8"Ring suction time" }, // リング吸い込み時間
	{ 0x1413e76d8, u8"Ring suction interval" }, // リング吸い込み間隔
	{ 0x1413e77b8, u8"Ring suction radius" }, // リング吸い込み半径
	{ 0x1413df950, u8"Ring hole parameters" }, // リングホールパラメータ
	{ 0x1413dfa80, u8"Action parameters initial" }, // アクションパラメータ 初期
	{ 0x1413dfbb0, u8"Action parameters 2nd stage" }, // アクションパラメータ ２段階目
	{ 0x1413dfcb8, u8"Action parameters 3rd stage" }, // アクションパラメータ ３段階目
	{ 0x1413dfdd8, u8"Action parameter 4th stage" }, // アクションパラメータ ４段階目
	{ 0x1413dfee0, u8"Action parameter 5th stage" }, // アクションパラメータ ５段階目
	{ 0x1413d8504, u8"Hp" }, // hp
	{ 0x1413e0b98, u8"Ring reduction time" }, // リング減少時間
	{ 0x1413e0d78, u8"Pursuit time" }, // 追撃時間
	{ 0x1413e0ef0, u8"Basic debuff value" }, // 基本デバフ値
	{ 0x1413e1040, u8"Additional debuff value" }, // 追加デバフ値
	{ 0x1413e11b0, u8"Camera distance" }, // カメラの距離
	{ 0x1413e1298, u8"Camera distance when attacking" }, // 攻撃時のカメラの距離
	{ 0x1413e13d0, u8"Camera minimum elevation angle" }, // カメラの最小仰角
	{ 0x1413e14f0, u8"Camera maximum elevation angle" }, // カメラの最大仰角
	{ 0x1413e1670, u8"Camera gaze point upward offset" }, // カメラの注視点上方オフセット
	{ 0x1413e1898, u8"Upward offset of camera gaze point during attack" }, // 攻撃時のカメラの注視点上方オフセット
	{ 0x1413dffd0, u8"Other parameters" }, // その他パラメータ
	{ 0x1413e8038, u8"Rush speed" }, // 突進スピード
	{ 0x1413e80c0, u8"Number of consecutive attacks" }, // 連続攻撃回数
	{ 0x1413e8850, u8"Number of bullets" }, // 弾の数
	{ 0x1413e8a38, u8"Follow rotation speed" }, // 追従回転速度
	{ 0x1413e8b38, u8"Consecutive number of times" }, // 連続回数
	{ 0x1413e8bf0, u8"Consecutive number interval" }, // 連続回数の間隔
	{ 0x1413e94e8, u8"Duration" }, // 継続時間
	{ 0x1413e9688, u8"Waiting time after completion" }, // 終了後待ち時間
	{ 0x1413eb850, u8"Rush time" }, // 突進時間
	{ 0x1413e9ed0, u8"Ground burner spin" }, // 地上バーナースピン
	{ 0x1413e9ff8, u8"Aerial burner spin" }, // 空中バーナースピン
	{ 0x1413ea0b8, u8"Roulette" }, // ルーレット
	{ 0x141399550, u8"Laser" }, // レーザー
	{ 0x1413ea238, u8"Default follow rotation speed" }, // デフォルト追従回転速度
	{ 0x1413ea3d0, u8"Shotgun" }, // ショットガン
	{ 0x1413ea4c8, u8"Winder" }, // ワインダー
	{ 0x1413f0008, u8"Waiting time for each bullet" }, // 格弾ごとの待ち時間
	{ 0x1413ea590, u8"Straight bullet" }, // 直線弾
	{ 0x1413f21e0, u8"Angle of both wings" }, // 両翼の角度
	{ 0x1413ea69c, u8"Fan bullet" }, // 扇弾
	{ 0x1413ea7b8, u8"Radiation bullet" }, // 放射弾
	{ 0x1413f0880, u8"Damage percentage required to end the first half of the air battle" }, // 前半空中戦終了に必要なダメージ割合
	{ 0x1413f09c0, u8"Percentage of physical strength transitioning to the second half of the game" }, // 後半戦に移行する体力割合
	{ 0x1413f0aa0, u8"Damage percentage required to end the second half of the aerial battle" }, // 後半空中戦終了に必要なダメージ割合
	{ 0x1413f0b80, u8"Time during chance time/first half" }, // チャンスタイム時の時間・前半
	{ 0x1413f0cb0, u8"Time during chance time/second half" }, // チャンスタイム時の時間・後半
	{ 0x1413f0df0, u8"Will we do burner spin in the first half?" }, // 前半戦、バーナースピンを行うか
	{ 0x1413f0ec0, u8"In the second half of the game, should the attack be referenced only for the first time? " }, // 後半戦、攻撃参照を初回のみにするか
	{ 0x1413ee028, u8"Launch force" }, // 打ち上げ力
	{ 0x1413ee140, u8"Launch time" }, // 打ち上げ時間
	{ 0x1413ee250, u8"Floating time" }, // 浮遊時間
	{ 0x1413ee388, u8"Falling force" }, // 落下力
	{ 0x1413ea940, u8"Cyloop reaction time" }, // サイループ反応時
	{ 0x1413eaa18, u8"Second half of the game, action table on the ground" }, // 後半戦、地上時の行動テーブル
	{ 0x1413eab38, u8"Second half of the game, action table while in the air" }, // 後半戦、空中時の行動テーブル
	{ 0x1413eced8, u8"Collision radius of each weak point" }, // 各弱点のコリジョン半径
	{ 0x1413ecfb0, u8"Combo radius of each weak point" }, // 各弱点のコンボ半径
	{ 0x1413ed0a0, u8"Y offset of each weak point" }, // 各弱点のYオフセット
	{ 0x1413ed168, u8"Collision radius of body (cyloop reaction)" }, // ボディ（サイループ反応）のコリジョン半径
	{ 0x1413ed248, u8"Combo radius of body (cyloop reaction)" }, // ボディ（サイループ反応）のコンボ半径
	{ 0x1413ed340, u8"Y offset of body (cyloop reaction)" }, // ボディ（サイループ反応）のYオフセット
	{ 0x1413ed9b0, u8"Parameters for each attack" }, // 攻撃ごとのパラメータ
	{ 0x1413ee868, u8"Rising acceleration" }, // 上昇加速度
	{ 0x1413ee938, u8"Normal rotation speed minimum value" }, // 通常時回転速度　最小値
	{ 0x1413eea48, u8"Normal rotation speed maximum value" }, // 通常時回転速度　最大値
	{ 0x1413eeba0, u8"Tornado rotation speed" }, // 竜巻時回転速度
	{ 0x1413eece0, u8"Rotational acceleration during tornado" }, // 竜巻時回転加速度
	{ 0x1413eede8, u8"Occurrence height offset" }, // 発生高さオフセット
	{ 0x1413eeeb8, u8"Frequency of occurrence" }, // 発生頻度
	{ 0x1413f1cf0, u8"Debris" }, // デブリ
	{ 0x1413edaa0, u8"Storm production parameters" }, // 嵐演出パラメータ
	{ 0x1413f5b40, u8"Y offset of camera position at midpoint (at the start of bombing)" }, // 中間地点でのカメラ位置のYオフセット（爆撃開始時）
	{ 0x1413f7680, u8"Duration (permanent if 0)" }, // 持続時間(0の場合は永続)
	{ 0x1413f7718, u8"Distance (default if 0)" }, // 距離(0の場合はデフォルト)
	{ 0x1413f7930, u8"Y offset in lock-on direction" }, // ロックオン方向のYオフセット
	{ 0x1413f5c18, u8"First stage camera settings (at the start of movement)" }, // 第1段階のカメラ設定（移動開始時）
	{ 0x1413f5cc8, u8"Second stage camera settings (at the end of the attack)" }, // 第2段階のカメラ設定（攻撃終了時）
	{ 0x1413f6228, u8"Bomb fall speed" }, // 爆撃の落下速度
	{ 0x1413f6368, u8"Bombing interval" }, // 爆撃のインターバル
	{ 0x1413f64e0, u8"Blast attack range" }, // 爆風攻撃範囲
	{ 0x1413f6698, u8"BombPreDistEffectRange" }, // bombPreDistEffectRange
	{ 0x1413f6800, u8"Bomb survival time" }, // 爆弾の生存時間
	{ 0x1413f6928, u8"Time to start bombing" }, // 爆撃を始めるまでの時間
	{ 0x1413f69e0, u8"Movement speed until bombing starts (if set to 0, same as normal movement speed)" }, // 爆撃を始めるまでの移動速度（0の場合は通常移動速度と同じ
	{ 0x1413f6f50, u8"(Limited to the first run of Fugitive State) The time when the battle is canceled while on the run and you are not on the grind." }, // (逃亡ステートの初回限定)逃亡中に戦闘が解除されるグラインドに乗っていない時間
	{ 0x1413f7080, u8"Time not on the grind where combat is canceled while on the run" }, // 逃亡中に戦闘が解除されるグラインドに乗っていない時間
	{ 0x1413f7118, u8"Time to disable HA at start of escape" }, // 逃亡開始時にHAを無効にする時間
	{ 0x1413f7d38, u8"Parameters related to speed change" }, // 速度変更関連のパラメータ
	{ 0x1413f7de0, u8"Sonic's normal movement speed when accelerating" }, // アクセル状態のソニックの通常移動速度
	{ 0x1413f7ed0, u8"Sonic's boosted movement speed when accelerating" }, // アクセル状態のソニックのブースト移動速度
	{ 0x1413f7fa0, u8"Camera parameters for warp" }, // ワープ用のカメラのパラメータ
	{ 0x1413f8060, u8"Camera parameters during escape grind" }, // 逃亡用のグラインド中のカメラパラメータ
	{ 0x1413fbd80, u8"[Dash ring] Appearance start distance" }, // [ダッシュリング]出現開始距離
	{ 0x1413fbec8, u8"[Dash ring] Disappearance start distance" }, // [ダッシュリング]消滅開始距離
	{ 0x1413fbfd8, u8"[Bird enemy] Appearance Start Distance" }, // [鳥型エネミー]出現開始距離
	{ 0x1413fc0e8, u8"[Bird enemy] Disappearance start distance" }, // [鳥型エネミー]消滅開始距離
	{ 0x1413fc248, u8"[Bullet] Appearance start distance" }, // [弾]出現開始距離
	{ 0x1413fc358, u8"[Bullet] Disappearance start distance" }, // [弾]消滅開始距離
	{ 0x1413f8160, u8"Parameters of objects that appear along the way" }, // 道中に出現させるオブジェクト群のパラメータ
	{ 0x1413ab898, u8"LaunchParam" }, // launchParam
	{ 0x1413f8960, u8"Movement speed when wandering" }, // 徘徊時の移動速度
	{ 0x1413f8a68, u8"Maximum rail length" }, // レールの最大の長さ
	{ 0x1413f8b68, u8"Speed control parameters during transition rail access" }, // 移行レールアクセス時の速度制御パラメータ
	{ 0x1413f8ce0, u8"Distance between locator and player entering approach phase" }, // 接近フェーズに入るロケーターとプレイヤーの距離
	{ 0x1413f8dd0, u8"Distance of first lap path" }, // 最初に入る周回パスの距離
	{ 0x1413fc848, u8"Normal recovery amount" }, // 通常時の回復量
	{ 0x1413fc8e8, u8"Recovery amount when boosting" }, // ブースト時の回復量
	{ 0x1413f8ef0, u8"Boost gauge recovery parameters during acceleration" }, // 加速時のブーストゲージ回復パラメータ
	{ 0x1413f9050, u8"Parameters for attack state" }, // 攻撃ステート用のパラメータ
	{ 0x1413f91b0, u8"Parameters for combat phase flag management" }, // 戦闘フェーズフラグ管理用のパラメータ
	{ 0x1413fb650, u8"Minimum elevation angle" }, // 最低仰角
	{ 0x1413fb6b0, u8"Maximum elevation angle" }, // 最大仰角
	{ 0x1413f9298, u8"Camera parameters during close combat" }, // 近接戦闘時のカメラパラメータ
	{ 0x1413f9390, u8"Distance to prohibit gimmick appearance" }, // ギミック出現を禁止する距離
	{ 0x1413f9458, u8"Time from attention to warp" }, // 注目してからワープするまでの時間
	{ 0x1413f9590, u8"Time taken for camera to return to locator after warp" }, // ワープ後にカメラがロケーターに戻るまでの時間
	{ 0x1413f9658, u8"Time until main body appears after warp" }, // ワープ後に本体が出現するまで時間
	{ 0x1413f9720, u8"Distance between player and main unit to increase homing distance" }, // ホーミング可能距離を広げるプレイヤーと本体の距離
	{ 0x1413fdde8, u8"Lane information of the path to appear" }, // 出現させるパスのレーン情報
	{ 0x1413fdec0, u8"Acceleration lane information" }, // 加速レーン情報
	{ 0x1413fae38, u8"Escape pass" }, // 逃亡用のパス
	{ 0x1413f98a8, u8"ID for use of escape pass" }, // 逃亡用のパスの使用ID
	{ 0x1413f99c0, u8"Camera parameters during bombing" }, // 爆撃時のカメラパラメータ
	{ 0x141393ca8, u8"Reference wave thickness" }, // 基準波の太さ
	{ 0x141393d48, u8"Success judgment rate of reference wave (normal)" }, // 基準波の成功判定割合(通常)
	{ 0x141393e20, u8"Success judgment rate of reference wave (long press)" }, // 基準波の成功判定割合(長押)
	{ 0x141393fa8, u8"Thickness of single wave" }, // 単発波の太さ
	{ 0x141394078, u8"Reference wave edge thickness" }, // 基準波の縁の太さ
	{ 0x141394178, u8"Timing display edge thickness" }, // タイミング表示の縁の太さ
	{ 0x141394298, u8"Number of fish shadows" }, // 魚影の数
	{ 0x141394328, u8"Time for the fish to start approaching the float (minimum)" }, // 魚がウキに接近を始める時間(最少)
	{ 0x1413943a0, u8"Distance to the float from which the fish escapes" }, // 魚が逃げるウキとの距離
	{ 0x141394488, u8"Number of times the float twitches before hitting (minimum)" }, // ヒット前のウキのピクつき回数(最少)
	{ 0x1413945a8, u8"Number of times the float twitches before hitting (maximum)" }, // ヒット前のウキのピクつき回数(最大)
	{ 0x1413946b0, u8"Float twitch interval before hit (minimum)" }, // ヒット前のウキのピクつき間隔(最少)
	{ 0x141394750, u8"Float twitch interval before hit (maximum)" }, // ヒット前のウキのピクつき間隔(最大)
	{ 0x141394828, u8"Input acceptance start time" }, // 入力受付開始時間
	{ 0x1413948f8, u8"Transition delay to fishing camera" }, // 釣りカメラへの移行ディレイ
	{ 0x141394a18, u8"Delay time until cut-in display when fishing is successful" }, // 釣り成功時カットイン表示までの遅延時間
	{ 0x141394b48, u8"Cut-in display time" }, // カットインの表示時間
	{ 0x141394bf8, u8"Delay time from cut-in to fishing" }, // カットイン後釣り上げまでの遅延時間
	{ 0x141394d10, u8"Maximum number of Golden Chests available" }, // ゴールデンチェストの入手上限数
	{ 0x141395740, u8"Appearance rate" }, // 出現率
	{ 0x1413960a0, u8"Fish group id" }, // 魚グループID
	{ 0x1413961d8, u8"Types of fish shadows" }, // 魚影の種類
	{ 0x1413962b8, u8"Success pattern ID" }, // 成功パターンID
	{ 0x1413963e8, u8"Wave pattern ID" }, // 波パターンID
	{ 0x141396520, u8"Number of skill pieces acquired" }, // 獲得スキルピース数
	{ 0x141396688, u8"Number of tokens earned" }, // 獲得トークン数
	{ 0x1413967c8, u8"Is it a rare fish?" }, // レア魚か
	{ 0x1413968a0, u8"Fishing motion" }, // 釣り上げモーション
	{ 0x141396978, u8"Toast display type" }, // Toast表示タイプ
	{ 0x1413969f8, u8"Fishing position offset" }, // 釣り上げ位置オフセット
	{ 0x141396ae8, u8"Fishing rotation offset" }, // 釣り上げ回転オフセット
	{ 0x141397390, u8"Number of catches" }, // 釣り上げ回数
	{ 0x141397528, u8"Fish id" }, // 魚ID
	{ 0x1413994e0, u8"Number of successes" }, // 成功回数
	{ 0x141399688, u8"Number of failures" }, // 失敗回数
	{ 0x141397850, u8"Success pattern setting" }, // 成功パターン設定
	{ 0x14139c0f0, u8"Reference wave speed" }, // 基準波の速さ
	{ 0x14139c1d0, u8"Minimum interval of wave generation (1st time)" }, // 波発生の最小間隔(1回目)
	{ 0x14139c348, u8"Maximum interval of wave generation (1st time)" }, // 波発生の最大間隔(1回目)
	{ 0x14139c4a8, u8"Minimum interval between wave occurrences (second and subsequent times)" }, // 波発生の最小間隔(2回目以降)
	{ 0x14139c580, u8"Maximum interval between wave occurrences (second and subsequent times)" }, // 波発生の最大間隔(2回目以降)
	{ 0x14139c6a8, u8"Minimum deviation of wave radius" }, // 波半径の最少ずれ幅
	{ 0x14139c818, u8"Maximum deviation of wave radius" }, // 波半径の最大ずれ幅
	{ 0x14139c920, u8"Number of wave patterns (number of settings for \"Wave\" item)" }, // 波のパターン数 (「波」項目の設定数)
	{ 0x14139bb50, u8"Type of wave" }, // 波の種類
	{ 0x14139bc80, u8"Wave radius" }, // 波の半径
	{ 0x14139bdb0, u8"Wave thickness (long push waves only)" }, // 波の太さ(長押波のみ)
	{ 0x14139b540, u8"Waves" }, // waves
	{ 0x14139ca40, u8"Wave (set from above)" }, // 波 (上から詰めて設定)
	{ 0x141397950, u8"Wave pattern settings" }, // 波パターン設定
	{ 0x141397a18, u8"Fish group settings" }, // 魚グループ設定
	{ 0x141397ad0, u8"Fish settings" }, // 魚設定
	{ 0x141399260, u8"SpawnFishId" }, // spawnFishId
	{ 0x141398978, u8"Appearing fish ID" }, // 出現する魚ID
	{ 0x141398348, u8"Fixed fishing table size" }, // 固定釣果テーブルサイズ
	{ 0x1413983e8, u8"Fishing complete achievement unlock table position" }, // 釣果コンプリート実績解除テーブル位置
	{ 0x1413984e0, u8"Fixed fishing results list" }, // 固定釣果一覧
	{ 0x141398a70, u8"Fixed fishing results" }, // 固定釣果
	{ 0x141398aa8, u8"Number of fish coins consumed" }, // 消費するフィッシュコイン数
	{ 0x141398c10, u8"Acquired token multiplier" }, // 獲得トークン倍率
	{ 0x141397c38, u8"Fishing spot setting" }, // 釣りスポット設定
	{ 0x14139af90, u8"Required number of successes for single wave" }, // 単発波の必要成功回数
	{ 0x14139b050, u8"Required number of successful long push waves" }, // 長押波の必要成功回数
	{ 0x14139b128, u8"Single wave" }, // 単発波
	{ 0x14139b1b8, u8"Long push wave" }, // 長押波
	{ 0x141397cc8, u8"Tutorial settings" }, // チュートリアル設定
	{ 0x14139aa78, u8"Number of sales" }, // 販売数
	{ 0x14139a168, u8"By spot" }, // スポット別
	{ 0x14139a258, u8"Selling price" }, // 販売価格
	{ 0x14139a408, u8"Sales price/VeryHard" }, // 販売価格/VeryHard
	{ 0x14139a548, u8"Earn per purchase" }, // 1購入ごとの獲得数
	{ 0x141397d68, u8"Exchange settings" }, // 交換設定
	{ 0x14139f078, u8"Camera gaze point offset [Master here standard]" }, // カメラ注視点オフセット[達人ココ基準]
	{ 0x14139f1c0, u8"Camera position offset [Master here standard]" }, // カメラ位置オフセット[達人ココ基準]
	{ 0x14139f388, u8"Camera posture" }, // カメラ姿勢
	{ 0x1413a0340, u8"Multiple stacked height differences of Sonic sticking Kodama" }, // ソニックくっつきコダマの複数重ね高さ差
	{ 0x1413a0478, u8"Minimum Kodama blow-off distance" }, // 最小コダマ吹き飛び距離
	{ 0x1413a05e8, u8"Maximum Kodama blow-off distance" }, // 最大コダマ吹き飛び距離
	{ 0x1413a0708, u8"Minimum Kodama blowout height" }, // 最小コダマ吹き飛び高さ
	{ 0x1413a07e8, u8"Maximum Kodama blowout height" }, // 最大コダマ吹き飛び高さ
	{ 0x1413a08e8, u8"Kodama model scale" }, // コダマモデルスケール
	{ 0x1413a09b0, u8"Kodama model scale (when attached)" }, // コダマモデルスケール(くっつき時)
	{ 0x1413a0a78, u8"Capsule collision radius" }, // カプセルコリジョン半径
	{ 0x1413a0b88, u8"Capsule collision height" }, // カプセルコリジョン高さ
	{ 0x1413a0d00, u8"Capsule collision offset" }, // カプセルコリジョンのオフセット
	{ 0x1413a0e38, u8"Kodama rotation angle/sec" }, // コダマ回転角度/秒
	{ 0x1413a0f10, u8"Minimum Kodama jump speed (vertical direction)" }, // 最小コダマジャンプ速度(垂直方向)
	{ 0x1413a1050, u8"Maximum Kodama jump speed (vertical direction)" }, // 最大コダマジャンプ速度(垂直方向
	{ 0x1413a1148, u8"Minimum Kodama jump speed (horizontal direction)" }, // 最小コダマジャンプ速度(水平方向)
	{ 0x1413a12c0, u8"Maximum Kodama jump speed (horizontal direction)" }, // 最大コダマジャンプ速度(水平方向)
	{ 0x1413a13f8, u8"Kodama jump interval" }, // コダマジャンプ間隔
	{ 0x1413a14f8, u8"Radius of contact with Sonic" }, // ソニックと接触する範囲の半径
	{ 0x1413a1608, u8"Time from start of jump to collection" }, // ジャンプ開始から回収までの時間
	{ 0x1413a1698, u8"Kodama jump speed after collection Y" }, // 回収後コダマジャンプ速度Y
	{ 0x1413a1760, u8"Kodama jump speed multiplier after collection" }, // 回収後コダマジャンプ速度倍率
	{ 0x1413a18a0, u8"Kodama Jump flight time after recovery" }, // 回収後コダマジャンプ滞空時間
	{ 0x1413a1990, u8"Herd judgment distance" }, // 群れ判定距離
	{ 0x1413a1aa8, u8"Distance between Kodama during HA" }, // HA時のコダマ間距離
	{ 0x1413a1b88, u8"Maximum inclination of each Kodama when moving and bending (degrees)" }, // 移動しなり時の各コダマの最大傾斜(度)
	{ 0x1413a1ca8, u8"Bending time immediately after stopping" }, // 停止直後しなりの時間
	{ 0x1413a1dc8, u8"Maximum inclination (degree) of each Kodama when bending immediately after stopping" }, // 停止直後しなり時の各コダマの最大傾斜(度)
	{ 0x1413a1ed0, u8"Stop left/right bend cycle time" }, // 停止左右しなりの周期時間
	{ 0x1413a1f88, u8"Maximum inclination (degree) of each Kodama when stopping left and right bending" }, // 停止左右しなり時の各コダマの最大傾斜(度)
	{ 0x1413ef5c8, u8"Kodama Escort" }, // コダマ護衛
	{ 0x1413a31e0, u8"Normal speed when moving" }, // 移動時の通常速度
	{ 0x1413a3290, u8"Speed when decelerating while moving" }, // 移動時の減速した時の速度
	{ 0x1413a32e0, u8"Distance at which to start decelerating when moving" }, // 移動時の減速を始める距離
	{ 0x1413a3380, u8"Distance from the center of two characters during homing" }, // ホーミング時の2キャラの中央からの距離
	{ 0x1413a3718, u8"Camera at the start of the trial" }, // 試練開始時カメラ
	{ 0x1413a37d8, u8"Camera at end of trial" }, // 試練終了時カメラ
	{ 0x1413a3f88, u8"Fade out time when changing characters" }, // キャラ交代時フェードアウト時間
	{ 0x1413a40d0, u8"Distance to the tracking location for the climbing part" }, // 登頂パートの追跡場所への距離
	{ 0x1413a41f8, u8"Normal speed when moving to the summit part" }, // 登頂パート移動時の通常速度
	{ 0x1413a42c8, u8"Speed when decelerating when moving to the summit part" }, // 登頂パート移動時の減速した時の速度
	{ 0x1413a43c8, u8"Distance at which to start decelerating when moving the climbing part" }, // 登頂パート移動時の減速を始める距離
	{ 0x1413a4530, u8"Distance from the center of the two characters during homing in the climbing part" }, // 登頂パートのホーミング時の2キャラの中央からの距離
	{ 0x1413a4640, u8"Normal speed when homing during the climbing part" }, // 登頂パートのホーミング時の移動時の通常速度
	{ 0x1413a4740, u8"Speed when decelerating when moving during homing in the climbing part" }, // 登頂パートのホーミング時の移動時の減速した時の速度
	{ 0x1413a48d0, u8"Distance at which to start decelerating during homing during the climbing part" }, // 登頂パートのホーミング時の移動時の減速を始める距離
	{ 0x1413a4ae0, u8"Normal speed when moving Super Sonic part" }, // スーパーソニックパートの移動時の通常速度
	{ 0x1413a4c30, u8"Speed when decelerating when moving Super Sonic part" }, // スーパーソニックパートの移動時の減速した時の速度
	{ 0x1413a4e20, u8"Distance to start decelerating when moving Super Sonic part" }, // スーパーソニックパートの移動時の減速を始める距離
	{ 0x1413a4f80, u8"Normal speed of movement during homing in Super Sonic part" }, // スーパーソニックパートのホーミング時の移動時の通常速度
	{ 0x1413a5090, u8"Speed when decelerating when moving during homing in Super Sonic part" }, // スーパーソニックパートのホーミング時の移動時の減速した時の速度
	{ 0x1413a5230, u8"Distance to start decelerating when moving during homing in Super Sonic part" }, // スーパーソニックパートのホーミング時の移動時の減速を始める距離
	{ 0x1413a5378, u8"Initial position of the summit part" }, // 登頂パートの初期位置
	{ 0x1413a5488, u8"Initial position for Super Sonic" }, // スーパーソニック用の初期位置
	{ 0x1413a6bc0, u8"Number of Kodama required to clear" }, // クリアに必要なコダマ数
	{ 0x1413a8158, u8"Kodama rolling speed" }, // コダマ転がり速度
	{ 0x1413a81d8, u8"Kodama initial stop speed" }, // コダマ停止初速度
	{ 0x1413a82b0, u8"Kodama stop time" }, // コダマ停止時間
	{ 0x1413a83a0, u8"Group radius" }, // 群れとして括る半径
	{ 0x1413a8430, u8"Detection radius offset when moving in groups" }, // 群れ集まる移動時検知半径オフセット
	{ 0x1413a8538, u8"Pack leader minimum time" }, // 群れリーダー最短時間
	{ 0x1413a8690, u8"Distance back to center line" }, // 中央線に戻る距離
	{ 0x1413a87c0, u8"Angle of return to center line" }, // 中央線に戻る角度
	{ 0x1413a7398, u8"How many seconds should it try to generate a bomb?" }, // 何秒おきに爆弾の生成を試みるか
	{ 0x1413a7480, u8"How many animals you collect will increase the bomb probability by one level." }, // 何匹回収で爆弾確率が1段階上がるか
	{ 0x1413a7588, u8"How many animals do you collect to increase the bomb probability by two stages?" }, // 何匹回収で爆弾確率が2段階上がるか
	{ 0x1413a7658, u8"Bomb occurrence probability % (1st stage)" }, // 爆弾発生確率%（１段階目）
	{ 0x1413a7730, u8"Bomb occurrence probability % (2nd stage)" }, // 爆弾発生確率%（２段階目）
	{ 0x1413a7878, u8"Bomb occurrence probability % (3rd stage)" }, // 爆弾発生確率%（３段階目）
	{ 0x1413a79e8, u8"Maximum number of bombs that can appear at the same time" }, // 同時出現爆弾数上限
	{ 0x1413a7af8, u8"Distance limit between bombs" }, // 爆弾同士発生距離制限
	{ 0x1413a8e98, u8"Sandstorm time" }, // 砂嵐時間
	{ 0x1413a8f90, u8"Time not to generate bombs after sandstorm clears" }, // 砂嵐解除後爆弾を生成しない時間
	{ 0x1413a90c0, u8"Interpolation time during sandstorm transition" }, // 砂嵐遷移時の補間時間
	{ 0x1413a91c8, u8"Interpolation time when sandstorm ends" }, // 砂嵐終了時の補間時間
	{ 0x1413a5dc8, u8"Bomb settings" }, // 爆弾設定
	{ 0x1413a9600, u8"Use time limits" }, // 時間制限を使用する
	{ 0x1413a9720, u8"Caution Remaining time to start animation (seconds)" }, // Cautionアニメ開始残り時間(秒)
	{ 0x1413a9820, u8"(Plays after Caution) Warning animation start remaining time (seconds)" }, // (Cautionの後に再生する)Warningアニメ開始残り時間(秒)
	{ 0x1413a9958, u8"Caution Should I use animation?" }, // Cautionアニメを使うか
	{ 0x1413a9a18, u8"Should I use a warning animation?" }, // Warningアニメを使うか
	{ 0x1413ef4f0, u8"Grassland island Kodama collection" }, // 草原島コダマ集め
	{ 0x1413ef658, u8"Collecting desert island Kodama" }, // 砂漠島コダマ集め
	{ 0x1413abf28, u8"Percentage of scattered boxes" }, // 散らばる箱の割合
	{ 0x1413ac0a0, u8"Durability value of box (small)" }, // 箱の耐久値(小)
	{ 0x1413ac1d8, u8"Durability of box (medium)" }, // 箱の耐久値(中)
	{ 0x1413ac310, u8"Durability value of box (large)" }, // 箱の耐久値(大)
	{ 0x1413ac458, u8"Box friction coefficient" }, // 箱の摩擦係数
	{ 0x1413ac568, u8"Box weight" }, // 箱のおもさ
	{ 0x1413ac698, u8"Gravitational acceleration of the box" }, // 箱の重力加速度
	{ 0x1413ac7c0, u8"Box velocity decay rate" }, // 箱の速度減衰率
	{ 0x1413ac898, u8"Box angular velocity decay rate" }, // 箱の角速度減衰率
	{ 0x1413ac960, u8"Max speed of box" }, // 箱の最大速度
	{ 0x1413ad2b0, u8"Collision radius (seedling)" }, // コリジョン半径（苗）
	{ 0x1413ad3f0, u8"Collision radius (flower)" }, // コリジョン半径（花）
	{ 0x1413ad540, u8"Collision height (seedling)" }, // コリジョン高さ（苗）
	{ 0x1413ad6a0, u8"Collision height (flower)" }, // コリジョン高さ（花）
	{ 0x1413ad790, u8"Effect offset (seedling)" }, // エフェクトオフセット（苗）
	{ 0x1413ad8c8, u8"Effect offset (flower)" }, // エフェクトオフセット（花）
	{ 0x1413ad9d8, u8"Distance to attract bees" }, // ハチを引き寄せる距離
	{ 0x1413adb60, u8"Time to attract bees (seconds)" }, // ハチを引き寄せる時間(秒)
	{ 0x1413ae098, u8"Target point offset" }, // 目標点のオフセット
	{ 0x1413ae170, u8"Target point offset (flower)" }, // 目標点のオフセット(花)
	{ 0x1413ae238, u8"Attack collision radius" }, // 攻撃コリジョンの半径
	{ 0x1413ae2d8, u8"Bee size" }, // ハチ大きさ
	{ 0x1413ae3e8, u8"Bee movement speed" }, // ハチ移動速度
	{ 0x1413ae560, u8"Angle of reflection from wall (degrees)" }, // 壁から反射する角度(度)
	{ 0x1413ae670, u8"Herd movement coefficient" }, // 群れ移動係数
	{ 0x1413ae7a8, u8"Destination movement coefficient" }, // 目的地移動係数
	{ 0x1413ae968, u8"Repulsion transfer coefficient" }, // 反発移動係数
	{ 0x1413aea98, u8"Waiting time after attack (seconds)" }, // 攻撃後の待機時間(秒)
	{ 0x1413aebb8, u8"Waiting time in flower (seconds)" }, // 花にいる待機時間(秒)
	{ 0x1413af308, u8"Sphere collision radius" }, // スフィアスコリジョン半径
	{ 0x1413af8e8, u8"Mowing collision" }, // 草刈りコリジョン
	{ 0x1413af9d0, u8"Flowers where bees gather" }, // ハチの集まる花
	{ 0x1413afaf4, u8"Hachi" }, // ハチ
	{ 0x1413ef538, u8"Mowing" }, // 草刈り
	{ 0x1413b0b28, u8"Number of items required" }, // 必要なアイテム数
	{ 0x1413b07f0, u8"Collecting parts" }, // 部品集め
	{ 0x1413b1c30, u8"Interruption collision radius" }, // 中断コリジョン半径
	{ 0x1413b1dd8, u8"Break collision height" }, // 中断コリジョン高さ
	{ 0x1413ef708, u8"Daruma battle" }, // ダルマ戦
	{ 0x1413ef7f8, u8"Bridge diving" }, // 橋ダイビング
	{ 0x1413b3a08, u8"Display size (1-25 levels)" }, // 表示サイズ(1-25段階)
	{ 0x1413b3be8, u8"Display time (seconds)" }, // 表示時間(秒)
	{ 0x1413b3cd0, u8"Display offset" }, // 表示オフセット
	{ 0x1413b4508, u8"Quest clear points" }, // クエストクリアポイント
	{ 0x1413b5048, u8"Boost consumption multiplier" }, // ブースト消費倍率
	{ 0x1413c1048, u8"Player" }, // プレイヤー
	{ 0x1413b4660, u8"Number of point parameters to enable" }, // 有効にするポイントパラメータの数
	{ 0x1413b5400, u8"Number of collisions surrounded at once" }, // 一度に囲んだコリジョン数
	{ 0x1413b5520, u8"Points earned" }, // 獲得ポイント
	{ 0x1413b4728, u8"Point criteria" }, // ポイント基準
	{ 0x1413b4898, u8"Obtained score UI" }, // 獲得スコアUI
	{ 0x1413b5938, u8"Change time (seconds)" }, // 変更時間(秒)
	{ 0x1413b49c0, u8"Camera for checking results" }, // 結果確認用カメラ
	{ 0x1413b6790, u8"Turning angle" }, // 旋回角度
	{ 0x1413b68c0, u8"Turning time" }, // 旋回時間
	{ 0x1413b7340, u8"Attribute value" }, // 属性値
	{ 0x1413b7880, u8"Beam length" }, // ビーム長
	{ 0x1413b7950, u8"Action time" }, // 行動時間
	{ 0x1413b7ab8, u8"Do you use animation?" }, // アニメーションを使用か
	{ 0x1413b7be0, u8"Key frame" }, // キーフレーム
	{ 0x1413b8e88, u8"Bullet model scale" }, // 弾モデルスケール
	{ 0x1413b9020, u8"Bullet collision radius [last boss]" }, // 弾コリジョン半径[ラスボス]
	{ 0x1413b9170, u8"Bullet model scale [last boss]" }, // 弾モデルスケール[ラスボス]
	{ 0x1413b92b0, u8"Bullet model camera direction offset rate [last boss]" }, // 弾モデルカメラ方向オフセット率[ラスボス]
	{ 0x1413b9458, u8"Extra thick beam collision radius" }, // 極太ビームコリジョン半径
	{ 0x1413b8808, u8"Bullet pattern parameters" }, // 弾パターンパラメータ郡
	{ 0x1413ba5c0, u8"Rotating laser extinguishing time" }, // 回転レーザーの消える時間
	{ 0x1413ba6c8, u8"Preliminary time of rotating laser" }, // 回転レーザーの予兆時間
	{ 0x1413ba838, u8"Time for the extremely thick laser to disappear" }, // 極太レーザーの消える時間
	{ 0x1413ba9c0, u8"Sign time of extremely thick laser" }, // 極太レーザーの予兆時間
	{ 0x1413bb0e0, u8"HP [small]" }, // HP[小]
	{ 0x1413bb180, u8"HP [medium]" }, // HP[中]
	{ 0x1413bb258, u8"HP [large]" }, // HP[大]
	{ 0x1413bb7d0, u8"The enemy" }, // ザコ敵
	{ 0x1413bb878, u8"Core enemy" }, // コア敵
	{ 0x1413ce0c8, u8"Is it valid?" }, // 有効か
	{ 0x1413ce1c8, u8"Number of keys used" }, // 鍵の使用数
	{ 0x1413ce588, u8"Giant tower setting for the third island" }, // 第3弾島用巨大タワー設定
	{ 0x1413ceda8, u8"Chaos Emerald Safe Setting" }, // カオスエメラルド金庫設定
	{ 0x1413cf178, u8"Island" }, // 島
	{ 0x1413cf228, u8"Third island" }, // 第3弾用島
	{ 0x1413cfa78, u8"Kodama acquisition radius (m)" }, // コダマの入手半径(m)
	{ 0x1413cfb68, u8"Kodama display fade time" }, // コダマの表示フェード時間
	{ 0x1413cfc30, u8"Fade time when popping out of treasure chest" }, // 宝箱から飛び出す際のフェード時間
	{ 0x1413cfd98, u8"Tutorial display delay" }, // チュートリアル表示のディレイ
	{ 0x1413d0b18, u8"Maximum tracking number" }, // 最大追従数
	{ 0x1413d0c10, u8"Tracking speed of Kodama" }, // 追従コダマの速度
	{ 0x1413d0df0, u8"Turning speed of following Kodama" }, // 追従コダマの旋回速度
	{ 0x1413d0f00, u8"Stopping distance of following Kodama" }, // 追従コダマの停止距離
	{ 0x1413d0ff0, u8"Disappearance distance of following Kodama" }, // 追従コダマの消滅距離
	{ 0x1413d10d8, u8"Redisplay stopping time of following Kodama" }, // 追従コダマの再表示立ち止まり時間
	{ 0x1413d2870, u8"Tracking settings" }, // 追従設定
	{ 0x1413d2e60, u8"Normal Kodama" }, // 通常コダマ
	{ 0x1413d02d8, u8"Number of Kodama required for LVUP (by difficulty level)" }, // LVUPに必要なコダマ数(難易度別)
	{ 0x1413d0418, u8"Kodama number required for LVUP for friends (by difficulty level)" }, // LVUPに必要なコダマ数フレンズ用(難易度別)
	{ 0x1413d05c8, u8"Emmy second part Koko required number to collect" }, // エミー後編ココ集め必要数
	{ 0x1413d2f28, u8"Elder Kodama" }, // 長老コダマ
	{ 0x1413d1700, u8"(by difficulty level)" }, // (難易度別)
	{ 0x1413d1950, u8"Number of seeds required for LVUP (by level)" }, // LVUPに必要なタネ数(レベル別)
	{ 0x1413d1a38, u8"Number of seeds required for friends to LVUP (by level)" }, // フレンズがLVUPに必要なタネ数(レベル別)
	{ 0x1413d2fc8, u8"Sage Kodama" }, // 仙人コダマ
	{ 0x1413d1fe8, u8"Collision radius of Kodama body (m)" }, // コダマ本体のコリジョン半径(m)
	{ 0x1413d2158, u8"Vertical offset of following Kodama" }, // 追従コダマの垂直方向オフセット
	{ 0x1413d2268, u8"Horizontal offset of following Kodama" }, // 追従コダマの水平方向オフセット
	{ 0x1413d3060, u8"New species of Kodama" }, // 新種コダマ
	{ 0x14139a008, u8"Kodama" }, // コダマ
	{ 0x1413d3470, u8"Maximum number of injections for injection events (short)" }, // 注入イベント(短)の最大注入数
	{ 0x1413d3580, u8"Maximum number of injections for injection event (medium)" }, // 注入イベント(中)の最大注入数
	{ 0x1413cf3d8, u8"NPC" }, // NPC
	{ 0x1413cf4f0, u8"Number of additional keys for portal" }, // ポータルの追加鍵数
	{ 0x1413e3c08, u8"Coordinate system of point of interest" }, // 注視点の座標系
	{ 0x1413e3cd8, u8"Local offset of point of interest" }, // 注視点のローカルオフセット
	{ 0x1413e3e18, u8"World offset of point of interest" }, // 注視点のワールドオフセット
	{ 0x1413e3f28, u8"Coordinate system of camera position" }, // カメラ位置の座標系
	{ 0x1413e4068, u8"Local offset of camera position" }, // カメラ位置のローカルオフセット
	{ 0x1413e4138, u8"World offset of camera position" }, // カメラ位置のワールドオフセット
	{ 0x1413e5350, u8"Camera parameter group" }, // カメラパラメータ郡
	{ 0x1413e58e8, u8"Scene name" }, // シーン名
	{ 0x1413e62c8, u8"Acceleration [m/sec]" }, // 加速度[m/sec]
	{ 0x1413e6388, u8"Deceleration [m/sec]" }, // 減速度[m/sec]
	{ 0x1413e64a8, u8"Overspeed deceleration [m/sec]" }, // オーバースピード減速度[m/sec]
	{ 0x1413e65b8, u8"Turning force [deg/sec]" }, // 旋回力[deg/sec]
	{ 0x1413e6728, u8"Attenuation start speed of speed ratio turning force attenuation" }, // 速度比旋回力減衰の減衰開始速度
	{ 0x1413e6828, u8"Attenuation rate of speed ratio turning force attenuation" }, // 速度比旋回力減衰の減衰レート
	{ 0x1413e6938, u8"Maximum damping force of speed ratio turning force damping" }, // 速度比旋回力減衰の最大減衰力
	{ 0x1413e6a58, u8"Attack radius when landing" }, // 接地時の攻撃半径
	{ 0x1413e6b28, u8"Camera shake name" }, // カメラシェイク名
	{ 0x1413e7118, u8"Player upper offset" }, // プレイヤー上方オフセット
	{ 0x1413e7248, u8"Player lateral offset" }, // プレイヤー横方向オフセット
	{ 0x1413e74b8, u8"Distance interpolation time" }, // 距離の補間時間
	{ 0x1413e7be0, u8"Lockon level up time" }, // ロックオンレベルアップ時間
	{ 0x1413e7d08, u8"Lock-on search distance" }, // ロックオン探索距離
	{ 0x1413e7e78, u8"Maximum lock-on distance" }, // ロックオン最大距離
	{ 0x1413e8010, u8"Movement speed after attack" }, // 攻撃後移動速度
	{ 0x1413e80a0, u8"Movement time after attack" }, // 攻撃後移動時間
	{ 0x1413e81b8, u8"Waiting time before attack start" }, // 攻撃開始前待機時間
	{ 0x1413e8308, u8"Attack movement time" }, // 攻撃移動時間
	{ 0x1413e8478, u8"Wait time before damage occurs" }, // ダメージ発生前待機時間
	{ 0x1413e85c8, u8"Damage occurrence interval" }, // ダメージ発生間隔
	{ 0x1413e86d0, u8"Waiting interval after damage occurs" }, // ダメージ発生後待機間隔
	{ 0x1413e8818, u8"Hitstop on object collision" }, // オブジェクト衝突時のヒットストップ
	{ 0x1413e89a0, u8"Blowing kiss camera" }, // 投げキッスカメラ
	{ 0x1413eac18, u8"Distance to player" }, // プレイヤーまでの距離
	{ 0x1413e8a50, u8"Heart disappearing camera" }, // ハート消失カメラ
	{ 0x1413e9240, u8"Attack radius" }, // 攻撃半径
	{ 0x1413e9390, u8"Cyline effect time" }, // サイラインのエフェクト時間
	{ 0x1413e94c0, u8"Sai line rotation period time" }, // サイラインの回転周期時間
	{ 0x1413e9988, u8"Appearance waiting time" }, // 出現待ち時間
	{ 0x1413f8980, u8"Weight value without drop" }, // ドロップなしのウェイト値
	{ 0x1413f8a88, u8"10 ring weight value" }, // 10リングのウェイト値
	{ 0x1413f8bf8, u8"Power seed weight value" }, // 力の種のウェイト値
	{ 0x1413f8d88, u8"Weight value of defensive seeds" }, // 守りの種のウェイト値
	{ 0x1413f8e38, u8"Sequence item weight value" }, // シーケンスアイテムのウェイト値
	{ 0x1413f8fd8, u8"Portal Fragment Weight Value" }, // ポータルの欠片のウェイト値
	{ 0x1413f9130, u8"Skill piece weight value" }, // スキルピースのウェイト値
	{ 0x1413f9268, u8"Skill piece (Amy) weight value" }, // スキルピース(Amy)のウェイト値
	{ 0x1413f93b8, u8"Skill piece (Knuckles) weight value" }, // スキルピース(Knuckles)のウェイト値
	{ 0x1413f94b0, u8"Skill piece (Tails) weight value" }, // スキルピース(Tails)のウェイト値
	{ 0x1413f7d70, u8"Normal weight" }, // 通常時のウェイト
	{ 0x1413f7e60, u8"Weight during battle" }, // バトル中のウェイト
	{ 0x1413f7f38, u8"Number of ring drops/single item" }, // リングドロップ枚数/単体
	{ 0x1413f8018, u8"Number of ring drops/combined" }, // リングドロップ枚数/複合
	{ 0x1413f8138, u8"10 ring drop number" }, // 10リングドロップ枚数
	{ 0x1413f82c0, u8"Number of skill piece drops" }, // スキルピースドロップ数
	{ 0x1413f83a8, u8"Amount of experience points per skill piece" }, // スキルピース1つあたりの経験値量
	{ 0x1413f8468, u8"Rare drop cooldown time [sec]" }, // レアドロップクールタイム[sec]
	{ 0x1413e9c00, u8"[Aerial Psyhammer] Maximum bonus multiplier" }, // [空中サイハンマー]最大ボーナス倍率
	{ 0x1413e9d78, u8"[Aerial Psyhammer] Minimum bonus fall distance" }, // [空中サイハンマー]最小ボーナス落下距離
	{ 0x1413e9f28, u8"[Aerial Psyhammer] Maximum bonus fall distance" }, // [空中サイハンマー]最大ボーナス落下距離
	{ 0x1413e8e78, u8"Minimum attack power" }, // 最小攻撃力
	{ 0x1413e8fb8, u8"Maximum attack power" }, // 最大攻撃力
	{ 0x1413e9058, u8"Damage random fluctuation range" }, // ダメージランダム変動幅
	{ 0x1413e9198, u8"Damage Random Variation Range/Spa Sonic" }, // ダメージランダム変動幅/スパソニ
	{ 0x1413e92b0, u8"Afterimage damage value rate" }, // 残像ダメージ値レート
	{ 0x1413e9440, u8"Afterimage stun value rate" }, // 残像スタン値レート
	{ 0x1413e9540, u8"Afterimage frightening value rate" }, // 残像怯み値レート
	{ 0x1413a3c90, u8"Offensive power" }, // 攻撃力
	{ 0x1413cefa0, u8"Minimum defense rate" }, // 最低防御率
	{ 0x1413cf0a0, u8"Highest defense rate" }, // 最高防御率
	{ 0x1413cf1b0, u8"Minimum guaranteed ring drop number" }, // 最低保障リングドロップ数
	{ 0x1413a3dd8, u8"Defense power" }, // 防御力
	{ 0x1413a3e40, u8"Critical damage rate" }, // クリティカル時のダメージレート
	{ 0x1413a3f30, u8"Probability of critical hit" }, // クリティカルの発生確率
	{ 0x1413a4038, u8"Critical probability/Super Sonic" }, // クリティカルの発生確率/スパソニ
	{ 0x1413a4180, u8"Damage rate while down" }, // ダウン中のダメージレート
	{ 0x1413a5420, u8"Damage rate/sonic" }, // ダメージレート/ソニック
	{ 0x1413a54e8, u8"Damage rate/Supersonic" }, // ダメージレート/スーパーソニック
	{ 0x1413a5588, u8"Damage multiplier in accel mode" }, // アクセルモード時のダメージ倍率
	{ 0x1413a55e0, u8"Damage multiplier in manual mode" }, // マニュアルモード時のダメージ倍率
	{ 0x1413a56f0, u8"Stagger value" }, // よろめき値
	{ 0x1413a57c8, u8"Damage speed maintenance time" }, // ダメージ速度維持時間
	{ 0x1413a5918, u8"Combo meter increase amount" }, // コンボメーター増加量
	{ 0x1413a5b18, u8"Combo meter increase/when accelerating" }, // コンボメーター増加量/アクセル時
	{ 0x1413a5c60, u8"Combo meter increase amount/Super Sonic" }, // コンボメーター増加量/スパソニ
	{ 0x1413a5cb8, u8"Combo meter increase amount/Super Sonica accelerator" }, // コンボメーター増加量/スパソニアクセル時
	{ 0x1413a5d50, u8"Quick Cyloop gauge increase amount" }, // クイックサイループゲージ増加量
	{ 0x1413a5dd8, u8"Quick Cyloop gauge increase/when accelerating" }, // クイックサイループゲージ増加量/アクセル時
	{ 0x1413a5ed8, u8"Quick Cyloop Gauge Increase/Spa Sonic" }, // クイックサイループゲージ増加量/スパソニ
	{ 0x1413a5fe0, u8"Quick Cyloop Gauge Increase/Super Sonica Acceleration" }, // クイックサイループゲージ増加量/スパソニアクセル時
	{ 0x1413a6108, u8"Quick Sailoop gauge increase amount/when guarding" }, // クイックサイループゲージ増加量/ガード時
	{ 0x1413a6230, u8"Quick Cyloop Gauge Increase/Guard & Acceleration" }, // クイックサイループゲージ増加量/ガード&アクセル時
	{ 0x1413a6328, u8"Damage speed applied to gimmicks" }, // ギミックに与えるダメージ速度
	{ 0x1413a63f8, u8"Time to exclude from target after hit" }, // ヒット後ターゲットから除外する時間
	{ 0x1413a6558, u8"Hit SE" }, // ヒットSE
	{ 0x1413a65b0, u8"Hit effect/sonic" }, // ヒットエフェクト/ソニック
	{ 0x1413a66a0, u8"Hit Effect/Supersonic" }, // ヒットエフェクト/スーパーソニック
	{ 0x1413a6770, u8"Hitstop/Sonic" }, // ヒットストップ/ソニック
	{ 0x1413a6850, u8"Hitstop/Defeat/Sonic" }, // ヒットストップ/撃破/ソニック
	{ 0x1413a68e8, u8"Hitstop/Boss Defeat/Sonic" }, // ヒットストップ/ボス撃破/ソニック
	{ 0x1413a6968, u8"Hitstop/Supersonic" }, // ヒットストップ/スーパーソニック
	{ 0x1413a6a28, u8"Hitstop/Defeat/Super Sonic" }, // ヒットストップ/撃破/スーパーソニック
	{ 0x1413a6af0, u8"Hit camera vibration/sonic" }, // ヒットカメラ振動/ソニック
	{ 0x1413a6be8, u8"Hit camera vibration/defeat/sonic" }, // ヒットカメラ振動/撃破/ソニック
	{ 0x1413a6cd0, u8"Hit camera vibration/Boss defeat/Sonic" }, // ヒットカメラ振動/ボス撃破/ソニック
	{ 0x1413a6da0, u8"Hit camera vibration/supersonic" }, // ヒットカメラ振動/スーパーソニック
	{ 0x1413a6e98, u8"Hit camera vibration/defeat/supersonic" }, // ヒットカメラ振動/撃破/スーパーソニック
	{ 0x1413a6f70, u8"Hit controller vibration/sonic" }, // ヒットコントローラ振動/ソニック
	{ 0x1413a7088, u8"Hit controller vibration/supersonic" }, // ヒットコントローラ振動/スーパーソニック
	{ 0x14139ef98, u8"Spin Jump" }, // Spin Jump
	{ 0x14139f068, u8"Spin Dash" }, // Spin Dash
	{ 0x14139f1a8, u8"Homing Attack" }, // Homing Attack
	{ 0x14139f2a0, u8"Aerial Homing Attack" }, // Aerial Homing Attack
	{ 0x14139f328, u8"Pursuit Kick" }, // Pursuit Kick
	{ 0x1413c3ec8, u8"Stomping" }, // Stomping
	{ 0x14139f470, u8"Stomping Attack" }, // Stomping Attack
	{ 0x14139f550, u8"Bound Stomping Last" }, // Bound Stomping Last
	{ 0x1413c4250, u8"Sliding" }, // Sliding
	{ 0x14139f640, u8"Loop Kick" }, // Loop Kick
	{ 0x1413bca08, u8"Crasher" }, // Crasher
	{ 0x14139f790, u8"Spin Slash Homing" }, // Spin Slash Homing
	{ 0x14139f888, u8"Spin Slash" }, // Spin Slash
	{ 0x14139f910, u8"Spin Slash Last" }, // Spin Slash Last
	{ 0x14139f9f8, u8"Sonic Boom" }, // Sonic Boom
	{ 0x14139fa90, u8"Cross Slash" }, // Cross Slash
	{ 0x14139fb48, u8"Homing Shot" }, // Homing Shot
	{ 0x14139fbe0, u8"Charge Attack" }, // Charge Attack
	{ 0x14139fc08, u8"Charge Attack Last" }, // Charge Attack Last
	{ 0x141235e3c, u8"Cyloop" }, // Cyloop
	{ 0x14139fd50, u8"Quick Cyloop" }, // Quick Cyloop
	{ 0x14139fe30, u8"Quick Cyloop Aerial" }, // Quick Cyloop Aerial
	{ 0x14139feb0, u8"Accele Combo 1" }, // Accele Combo 1
	{ 0x14139ff58, u8"Accele Combo 2" }, // Accele Combo 2
	{ 0x1413a0020, u8"Accele Combo 1 Aerial" }, // Accele Combo 1 Aerial
	{ 0x1413a0118, u8"Accele Combo 2 Aerial" }, // Accele Combo 2 Aerial
	{ 0x1413a01d8, u8"Combo Finish" }, // Combo Finish
	{ 0x1413a0270, u8"Combo Finish Front" }, // Combo Finish Front
	{ 0x1413a03b8, u8"Combo Finish Back" }, // Combo Finish Back
	{ 0x1413a0460, u8"Combo Finish Left" }, // Combo Finish Left
	{ 0x1413a0588, u8"Combo Finish Right" }, // Combo Finish Right
	{ 0x1413a06b0, u8"Accele Combo Finish" }, // Accele Combo Finish
	{ 0x1413a0788, u8"Accele Combo Finish Front" }, // Accele Combo Finish Front
	{ 0x1413a0858, u8"Accele Combo Finish Back" }, // Accele Combo Finish Back
	{ 0x1413a08c8, u8"Accele Combo Finish Left" }, // Accele Combo Finish Left
	{ 0x1413a0978, u8"Accele Combo Finish Right" }, // Accele Combo Finish Right
	{ 0x1413bc748, u8"Smash" }, // Smash
	{ 0x1413a0ad8, u8"Smash Last" }, // Smash Last
	{ 0x1413a0b78, u8"Slingshot" }, // Slingshot
	{ 0x1413a0cd0, u8"Knuckles Punch1" }, // Knuckles Punch1
	{ 0x1413a0da8, u8"Knuckles Punch2" }, // Knuckles Punch2
	{ 0x1413a0e20, u8"Knuckles Uppercut" }, // Knuckles Uppercut
	{ 0x1413a0eb0, u8"Knuckles MaximumHeatKnuckle" }, // Knuckles MaximumHeatKnuckle
	{ 0x1413a0f50, u8"Knuckles MaximumHeatKnuckle Last" }, // Knuckles MaximumHeatKnuckle Last
	{ 0x1413a1038, u8"Amy Tarot Attack" }, // Amy Tarot Attack
	{ 0x1413a10f0, u8"Amy Tarot Attack2" }, // Amy Tarot Attack2
	{ 0x1413a1250, u8"Amy Tarot Rolling" }, // Amy Tarot Rolling
	{ 0x1413a1330, u8"Amy Charm Attack" }, // Amy Charm Attack
	{ 0x1413a1418, u8"Tails Spanner" }, // Tails Spanner
	{ 0x1413a14e0, u8"Tails Spanner Float" }, // Tails Spanner Float
	{ 0x1413a15d0, u8"Tails Power Boost" }, // Tails Power Boost
	{ 0x1413a1680, u8"Tails Wave Cannon" }, // Tails Wave Cannon
	{ 0x1413a16e8, u8"Tails Wave Cannon Finish" }, // Tails Wave Cannon Finish
	{ 0x1413f4128, u8"Damage settings" }, // ダメージ設定
	{ 0x14139f280, u8"Water resistance coefficient when underwater" }, // 水中時の水抵抗係数
	{ 0x14139f370, u8"Deceleration during breathing" }, // 息継ぎ時の減速
	{ 0x14139f418, u8"Breathing restraint time" }, // 息継ぎの拘束時間
	{ 0x14139f528, u8"Acceleration in the direction of gravity" }, // 重力方向にかかる加速度
	{ 0x1413f4268, u8"Underwater-specific behavior" }, // 水中専用挙動
	{ 0x1413ac9f8, u8"Basic value of initial velocity in normal direction" }, // 法線方向の初速基本値
	{ 0x1413acaa0, u8"Initial velocity in the upward direction" }, // 上方向の初速
	{ 0x1413acb60, u8"Upward initial velocity in the air" }, // 上方向の初速　空中
	{ 0x1413acc70, u8"Minimum speed when jumping off the edge" }, // エッジからの飛び出しの場合の最低速度
	{ 0x1413acd68, u8"Time available for transition to other aerial actions" }, // 他の空中アクションへ移行できる時間
	{ 0x1413acdf8, u8"Time to transition to wall movement" }, // 壁移動へ移行できる時間
	{ 0x1413f4300, u8"BASE jump" }, // BASEジャンプ
	{ 0x1413abcb8, u8"[Normal] Slope sliding force" }, // [通常]斜面滑り力
	{ 0x1413abdc8, u8"[Normal] Brake force" }, // [通常]ブレーキ力
	{ 0x1413abec8, u8"[Inside S-shape] Slope sliding force" }, // [S字内]斜面滑り力
	{ 0x1413ac010, u8"[Within S-shape] Brake force" }, // [S字内]ブレーキ力
	{ 0x1413f1b68, u8"End speed" }, // 終了速度
	{ 0x1413ac248, u8"Whether to use gravitational acceleration relative to the input direction" }, // 入力方向に対する重力加速度を使用するか
	{ 0x1413f43d8, u8"Ball movement" }, // ボール移動
	{ 0x1413e6a48, u8"UV:U start" }, // UV:U開始
	{ 0x1413e6b18, u8"UV:U end" }, // UV:U終了
	{ 0x1413f4488, u8"Movement trajectory" }, // 移動軌跡
	{ 0x1413a8138, u8"Effect playback interval" }, // エフェクトの再生間隔
	{ 0x1413a81b8, u8"Effect playback time" }, // エフェクトの再生時間
	{ 0x1413a8280, u8"Effect generation position offset" }, // エフェクト発生位置オフセット
	{ 0x1413a8368, u8"Effect overlap offset" }, // エフェクトオーバーラップオフセット
	{ 0x1413f4578, u8"Aura train" }, // オーラトレイン
	{ 0x1413e3c78, u8"Number of ring levels" }, // リングレベル段数
	{ 0x1413e3d68, u8"Number of speed levels" }, // スピードレベル段数
	{ 0x1413e3ec8, u8"Attack level number" }, // 攻撃レベル段数
	{ 0x1413e3fe8, u8"Defense level number" }, // 防御レベル段数
	{ 0x1413f4680, u8"Number of level stages" }, // レベル段階数
	{ 0x1413f4788, u8"Barrier wall" }, // 結界壁
	{ 0x1413cd820, u8"Island rate" }, // 島別レート
	{ 0x1413cd478, u8"Difficulty" }, // 難易度
	{ 0x14139c478, u8"Combo meter decrease speed/normal" }, // コンボメーター減少速度/通常
	{ 0x14139c548, u8"Combo meter decrease speed/acceleration mode" }, // コンボメーター減少速度/アクセルモード
	{ 0x14139c680, u8"Reduction amount when receiving damage/normal" }, // 被ダメージ時の減少量/通常
	{ 0x14139c7e0, u8"Reduction amount when receiving damage/acceleration mode" }, // 被ダメージ時の減少量/アクセルモード
	{ 0x14139c8e8, u8"Combo inflated rate/acceleration mode" }, // コンボ水増しレート/アクセルモード
	{ 0x1413f4888, u8"Accelerator mode" }, // アクセルモード
	{ 0x1413eb3b8, u8"Long press judgment time" }, // 長押し判定時間
	{ 0x1413b8198, u8"Combo basics" }, // コンボ基本
	{ 0x1413b9c48, u8"Timeout" }, // タイムアウト
	{ 0x1413b82a0, u8"Move between combos/Sonic" }, // コンボ間移動/ソニック
	{ 0x1413b8360, u8"Move between combos/Super Sonic" }, // コンボ間移動/スーパーソニック
	{ 0x1413ba238, u8"Moving rotation speed" }, // 移動回転速度
	{ 0x1413b84c8, u8"Combo movement correction during Super Sonic" }, // コンボ移動補正 スーパーソニック時
	{ 0x1413baf40, u8"Transition destination/normal" }, // 遷移先/通常
	{ 0x1413bafd0, u8"Transition destination/air" }, // 遷移先/空中
	{ 0x1413bb070, u8"Transition destination/blank" }, // 遷移先/空打ち
	{ 0x1413ba428, u8"Root" }, // Root
	{ 0x1413ba5e8, u8"Accele Combo 3" }, // Accele Combo 3
	{ 0x1413ba630, u8"Accele Combo 4" }, // Accele Combo 4
	{ 0x1413ba6b0, u8"Aerial Accele Combo 1" }, // Aerial Accele Combo 1
	{ 0x1413ba748, u8"Aerial Accele Combo 2" }, // Aerial Accele Combo 2
	{ 0x1413ba7c0, u8"Aerial Accele Combo 3" }, // Aerial Accele Combo 3
	{ 0x1413ba8a0, u8"Aerial Accele Combo 4" }, // Aerial Accele Combo 4
	{ 0x1413ba9b4, u8"Behind" }, // Behind
	{ 0x1413baa00, u8"Guarded" }, // Guarded
	{ 0x1413c3864, u8"Avoid" }, // Avoid
	{ 0x1413c3fe0, u8"AirBoost" }, // AirBoost
	{ 0x1413baa40, u8"AfterAirBoost" }, // AfterAirBoost
	{ 0x1413baad0, u8"Knuckles CyKnuckle" }, // Knuckles CyKnuckle
	{ 0x1413bab58, u8"Amy TarotAttack" }, // Amy TarotAttack
	{ 0x1413bab88, u8"Amy TarotAttack2" }, // Amy TarotAttack2
	{ 0x1413babe0, u8"Amy TarotRolling" }, // Amy TarotRolling
	{ 0x1413bac50, u8"Amy CyHammer" }, // Amy CyHammer
	{ 0x1413bacb8, u8"Amy CharmAttack" }, // Amy CharmAttack
	{ 0x1413b8598, u8"Combo route" }, // コンボルート
	{ 0x1413f5248, u8"Combo" }, // コンボ
	{ 0x1413eb690, u8"Amount of damage" }, // ダメージ量
	{ 0x1413eb818, u8"Effect time [sec]" }, // 効果時間[sec]
	{ 0x1413eb960, u8"Time to first damage [sec]" }, // 初回ダメージまでの時間[sec]
	{ 0x1413eba38, u8"Damage interval time from 2nd time onwards [sec]" }, // 2回目以降のダメージ間隔時間[sec]
	{ 0x1413f5308, u8"Parry poison effect" }, // パリィ毒効果
	{ 0x1413b2fe0, u8"Number of transparent lines" }, // 透明ラインの本数
	{ 0x1413b4648, u8"Circumference/number of divisions" }, // 円周/分割数
	{ 0x1413b4718, u8"Circumference/radius" }, // 円周/半径
	{ 0x1413b47b8, u8"Circumference/radius fluctuation period" }, // 円周/半径の揺らぎ周期
	{ 0x1413b4948, u8"Circumference/radius fluctuation amplitude" }, // 円周/半径の揺らぎ振幅
	{ 0x1413b4a88, u8"Circumference/radius fluctuation speed" }, // 円周/半径の揺らぎ速度
	{ 0x1413b4b30, u8"Circumference/scale" }, // 円周/スケール
	{ 0x1413b4c58, u8"Texture/filename" }, // テクスチャ/ファイル名
	{ 0x1413b4d00, u8"Texture/flowing speed" }, // テクスチャ/流れる速度
	{ 0x1413b4e38, u8"Texture/twist period" }, // テクスチャ/ひねり周期
	{ 0x1413b4f70, u8"Texture/rotation period" }, // テクスチャ/回転周期
	{ 0x1413b50c0, u8"Texture/rotation phase" }, // テクスチャ/回転位相
	{ 0x1413b51b8, u8"Texture/Starting Alpha Distance" }, // テクスチャ/先頭アルファ距離
	{ 0x1413b52f0, u8"Texture/Tail Alpha Distance" }, // テクスチャ/末尾アルファ距離
	{ 0x1413b5428, u8"Position offset/rotation period" }, // 位置オフセット/回転周期
	{ 0x1413b5550, u8"Position offset/rotational phase" }, // 位置オフセット/回転位相
	{ 0x1413b5640, u8"Position offset/radius" }, // 位置オフセット/半径
	{ 0x1413b6340, u8"Texture/color 1" }, // テクスチャ/色1
	{ 0x1413b64a8, u8"Texture/color 2" }, // テクスチャ/色2
	{ 0x1413b65d0, u8"Texture/brightness" }, // テクスチャ/輝度
	{ 0x1413b6720, u8"Texture/flash color 1" }, // テクスチャ/フラッシュ色1
	{ 0x1413b6930, u8"Texture/flash brightness" }, // テクスチャ/フラッシュ輝度
	{ 0x1413b4118, u8"Texture/flash time" }, // テクスチャ/フラッシュ時間
	{ 0x1413b3070, u8"Transparent line parameters" }, // 透明ラインのパラメータ
	{ 0x1413b3140, u8"Number of opaque lines" }, // 不透明ラインの本数
	{ 0x1413b3a78, u8"Texture/color" }, // テクスチャ/色
	{ 0x1413b3c10, u8"Texture/Alpha Threshold" }, // テクスチャ/アルファしきい値
	{ 0x1413b7178, u8"Speed x" }, // 速度X
	{ 0x1413b72d0, u8"Speed Y" }, // 速度Y
	{ 0x1413b3d18, u8"Texture/noise" }, // テクスチャ/ノイズ
	{ 0x1413b3da8, u8"Texture/Line Scale" }, // テクスチャ/ラインスケールX
	{ 0x1413b3ec0, u8"Texture/line scale Y" }, // テクスチャ/ラインスケールY
	{ 0x1413b3fc8, u8"Texture/flash color" }, // テクスチャ/フラッシュ色
	{ 0x1413b3250, u8"Opaque line parameters" }, // 不透明ラインのパラメータ
	{ 0x1413b3308, u8"Number of cross lines" }, // クロスラインの本数
	{ 0x1413aec88, u8"Texture/pattern" }, // テクスチャ/パターン
	{ 0x1413aed30, u8"Texture/distortion" }, // テクスチャ/歪み
	{ 0x1413aedc0, u8"Starting color" }, // 開始色
	{ 0x1413aee78, u8"Terminal color" }, // 末端色
	{ 0x1413aeff8, u8"Starting color brightness" }, // 開始色輝度
	{ 0x1413af170, u8"Terminal color brightness" }, // 末端色輝度
	{ 0x1413af280, u8"Flash/start color" }, // フラッシュ/開始色
	{ 0x1413af3c0, u8"Flash/terminal color" }, // フラッシュ/末端色
	{ 0x1413af460, u8"Flash/start color brightness" }, // フラッシュ/開始色輝度
	{ 0x1413af538, u8"Flash/terminal color brightness" }, // フラッシュ/末端色輝度
	{ 0x1413af5f0, u8"Flash/time" }, // フラッシュ/時間
	{ 0x1413af700, u8"Pattern/parallax" }, // パターン/視差
	{ 0x1413af840, u8"Pattern/scroll speed" }, // パターン/スクロール速度
	{ 0x1413af950, u8"Pattern/density" }, // パターン/濃さ
	{ 0x1413afa88, u8"Pattern/tiling" }, // パターン/タイリング
	{ 0x1413afbc0, u8"Glitch/terminal resolution" }, // グリッチ/末端の解像度X
	{ 0x1413afd08, u8"Glitch/end resolution Y" }, // グリッチ/末端の解像度Y
	{ 0x1413afe18, u8"Glitch/displacement amount of each stage at the end" }, // グリッチ/末端の各段のズレ量
	{ 0x1413affc8, u8"Glitch/maximum length that width changes" }, // グリッチ/幅が変化する最大の長さ
	{ 0x1413b0148, u8"Glitch/tip scroll speed" }, // グリッチ/先端のスクロール速度
	{ 0x1413b0238, u8"Glitch/tip noise change rate" }, // グリッチ/先端のノイズ変化速度
	{ 0x1413b0308, u8"Glitch/tip position offset" }, // グリッチ/先端の位置オフセット
	{ 0x1413b0488, u8"Glitch/tip contrast" }, // グリッチ/先端のコントラスト
	{ 0x1413b0598, u8"Glitch/terminal scroll speed" }, // グリッチ/末端のスクロール速度
	{ 0x1413b06c8, u8"Glitch/terminal noise rate of change" }, // グリッチ/末端のノイズ変化速度
	{ 0x1413b07c0, u8"Glitch/end position offset" }, // グリッチ/末端の位置オフセット
	{ 0x1413b0868, u8"Glitch/terminal contrast" }, // グリッチ/末端のコントラスト
	{ 0x1413b0938, u8"Line/width_model width" }, // ライン/幅_モデルの幅
	{ 0x1413b0a40, u8"Line/width_display width" }, // ライン/幅_表示の幅
	{ 0x1413b0b10, u8"Line/border width" }, // ライン/境界の幅
	{ 0x1413b0bf0, u8"Line/border color intensity" }, // ライン/境界の色の濃さ
	{ 0x1413b0ca8, u8"Line/boundary normal fade" }, // ライン/境界の法線フェード
	{ 0x1413b0e08, u8"Distortion/Overall UV Scale" }, // 歪み/全体のUVスケール
	{ 0x1413b0f30, u8"UV scale with low distortion/period" }, // 歪み/周期低のUVスケール
	{ 0x1413b1098, u8"UV scale of distortion/period height" }, // 歪み/周期高のUVスケール
	{ 0x1413b11b0, u8"Distortion/frequency blend ratio" }, // 歪み/周波数のブレンド割合
	{ 0x1413b1328, u8"Distortion/scroll speed" }, // 歪み/スクロール速度
	{ 0x1413b1400, u8"Distortion/strength" }, // 歪み/強さ
	{ 0x1413b14e0, u8"Disappearance/disappearance ratio" }, // 消失/消失比率
	{ 0x1413b1580, u8"Disappearance/fade-out time ratio" }, // 消失/フェードアウト時間比率
	{ 0x1413b1680, u8"Position/height offset" }, // 位置/高さオフセット
	{ 0x1413b33a8, u8"Cross line parameters" }, // クロスラインのパラメータ
	{ 0x1413c79f8, u8"Trajectory/Quick" }, // 軌跡/Quick
	{ 0x1413c7b00, u8"Kiseki/Super Sonic" }, // 軌跡/スパソニ
	{ 0x1413c7c08, u8"Kiseki/Super Sonic/Quick" }, // 軌跡/スパソニ/Quick
	{ 0x1413c7da8, u8"Aura color" }, // オーラカラー
	{ 0x1413c8038, u8"Minimum speed at which Cyloop can be performed" }, // Cyloopを行える最低速度
	{ 0x1413f9fa8, u8"Damage value multiplier" }, // ダメージ値倍率
	{ 0x1413fa128, u8"Stun value multiplier" }, // スタン値倍率
	{ 0x1413fa238, u8"Fear value multiplier" }, // 怯み値倍率
	{ 0x1413fa5d8, u8"Parameters by multiplex number" }, // 多重数別パラメータ
	{ 0x1413f9cd8, u8"Multiplex" }, // 多重
	{ 0x1413c81f8, u8"Shape effect" }, // 形状効果
	{ 0x1413c8300, u8"QuickCyloop gauge consumption" }, // QuickCyloopのゲージ消費量
	{ 0x1413c8440, u8"QuickCyloop gauge recovery amount per hour" }, // QuickCyloopのゲージの時間単位回復量
	{ 0x1413c8570, u8"QuickCyloop gauge recovery amount per hour during mini-games" }, // ミニゲーム中のQuickCyloopのゲージの時間単位回復量
	{ 0x1413c86c0, u8"Friends QuickCyloop gauge ring recovery amount" }, // フレンズQuickCyloopのゲージのリング回復量
	{ 0x1413bde38, u8"Movable slope change [deg]" }, // 移動可能な傾斜変化量[deg]
	{ 0x1413bdfb0, u8"Ground search slope [deg]" }, // 地面探索傾斜[deg]
	{ 0x1413be130, u8"Ground search tilt/boost [deg]" }, // 地面探索傾斜/ブースト中[deg]
	{ 0x1413be2e0, u8"Maximum slope of plane [deg]" }, // 平面の最大傾斜[deg]
	{ 0x1413be428, u8"Maximum slope slope [deg]" }, // 斜面の最大傾斜[deg]
	{ 0x1413be5b8, u8"Maximum slope of wall [deg]" }, // 壁面の最大傾斜[deg]
	{ 0x1413be708, u8"Support surface that can stand upright" }, // 直立可能なサポート面
	{ 0x1413be820, u8"Support surface that can be driven from the air" }, // 空中から走行可能なサポート面
	{ 0x1413be920, u8"Runable support surface" }, // 走行可能なサポート面
	{ 0x1413bea00, u8"Does it stick to the ground and move?" }, // 地面に張り付いて移動するか
	{ 0x1413beb70, u8"Will it slow down due to wall collision during boost?" }, // ブースト中の壁衝突で減速するか
	{ 0x1413bed00, u8"Is it possible to ground to wall attributes?" }, // 壁面属性に接地可能か
	{ 0x1413bee40, u8"Type ahead time" }, // 先行入力時間
	{ 0x1413befe0, u8"Number of rings owned" }, // リング所持数
	{ 0x1413bf140, u8"Number of rings owned/LvMax" }, // リング所持数/LvMax
	{ 0x1413bf250, u8"Ring collection range" }, // リング回収範囲
	{ 0x141393afc, u8"General purpose" }, // 汎用
	{ 0x1413ee320, u8"Speed during turning" }, // 旋回中速度
	{ 0x1413c5d28, u8"Normal" }, // 通常
	{ 0x1413f7e40, u8"Normal [maximum]" }, // 通常[最大]
	{ 0x1413f7f88, u8"Boost [maximum]" }, // ブースト[最大]
	{ 0x1413f8048, u8"Boost LvMax" }, // ブーストLvMax
	{ 0x1413f8118, u8"Boost LvMax [maximum]" }, // ブーストLvMax[最大]
	{ 0x1413f8250, u8"Overspeed maximum speed" }, // オーバースピード最高速度
	{ 0x1413f8358, u8"Maximum speed limit [minimum]" }, // 最大速度リミット[最小]
	{ 0x1413f8418, u8"Maximum speed limit [maximum]" }, // 最大速度リミット[最大]
	{ 0x1413f8500, u8"Stop judgment speed" }, // 停止判定速度
	{ 0x1413ec430, u8"Maximum falling speed" }, // 最高落下速度
	{ 0x1413f9e98, u8"Maximum force" }, // 最大力
	{ 0x1413f9f68, u8"Maximum force [maximum]" }, // 最大力[最大]
	{ 0x1413fa0a8, u8"Speed damper range" }, // 速度ダンパー範囲
	{ 0x1413fa1e0, u8"Minimum jump" }, // 最低躍度
	{ 0x1413fa308, u8"Maximum jump" }, // 最高躍度
	{ 0x1413f8780, u8"Minimum deceleration in neutral" }, // ニュートラル時最小減速度
	{ 0x1413f8868, u8"Maximum deceleration in neutral" }, // ニュートラル時最大減速度
	{ 0x1413f8900, u8"Acceleration/Autorun" }, // 加速度/オートラン
	{ 0x1413f89e8, u8"Deceleration/Autorun" }, // 減速度/オートラン
	{ 0x1413f1588, u8"Swing deceleration/start angle" }, // 旋回減速/開始角度
	{ 0x1413f1610, u8"Turning deceleration/maximum angle" }, // 旋回減速/最大角度
	{ 0x1413ef068, u8"Slope acceleration" }, // 斜面加速度
	{ 0x1413ef128, u8"Slope deceleration" }, // 斜面減速度
	{ 0x1413f8f80, u8"Deceleration when crouching" }, // しゃがみ時の減速度
	{ 0x1413f90c8, u8"Acceleration sensitivity" }, // 加速感度
	{ 0x1413f9200, u8"Speed affecting motion during underwater boost" }, // 水中ブースト時のモーションに影響する速度
	{ 0x1413f1480, u8"Basic turning force" }, // 基本旋回力
	{ 0x1413ec620, u8"Basic turning force [maximum]" }, // 基本旋回力[最大]
	{ 0x1413ec720, u8"Standard speed of basic turning force" }, // 基本旋回力の基準速度
	{ 0x1413ec960, u8"Whether angle-specific turning force damping is always applied" }, // 角度比旋回力減衰を常に適用するか
	{ 0x1413eca00, u8"Influence rate of angle-specific turning force attenuation in the front direction" }, // 角度比旋回力減衰の正面方向への影響率
	{ 0x1413ecca8, u8"Turning force during auto run" }, // オートラン時の旋回力
	{ 0x1413ed8c8, u8"Walking speed" }, // 歩き速度
	{ 0x1413ed998, u8"Sneaking speed" }, // 忍び歩き速度
	{ 0x1413eda68, u8"Stealth animation playback speed" }, // 忍び歩きのアニメーション再生速度
	{ 0x1413edb78, u8"Walking animation playback speed" }, // 歩きのアニメーション再生速度
	{ 0x1413edce8, u8"Running animation playback speed" }, // 走りのアニメーション再生速度
	{ 0x1413ede70, u8"Boost animation playback speed" }, // ブーストのアニメーション再生速度
	{ 0x1413edf98, u8"Sample time for left/right tilt slope" }, // 左右傾きの傾きのサンプル時間
	{ 0x1413ee080, u8"Minimum blend angle for left/right tilt" }, // 左右傾きの最小ブレンド角度
	{ 0x1413ee190, u8"Maximum blend angle for left/right tilt" }, // 左右傾きの最大ブレンド角度
	{ 0x1413ee2f8, u8"Blend speed for left/right tilt" }, // 左右傾きのブレンド速度
	{ 0x1413ee3e8, u8"Blend speed when left/right tilt returns to center" }, // 左右傾きが中央に戻る際のブレンド速度
	{ 0x1413ee4a0, u8"Minimum walking transition time" }, // 最低歩き移行動作時間
	{ 0x1413ee5b8, u8"Angle of falling animation when falling" }, // 落下時に落下アニメーションになる角度
	{ 0x141393cf8, u8"Running motion" }, // 走りモーション
	{ 0x1413ab4a8, u8"Minimum rotational speed in the longitudinal direction" }, // 前後方向の最小回転速度
	{ 0x1413ab590, u8"Maximum rotational speed in the longitudinal direction" }, // 前後方向の最大回転速度
	{ 0x1413ab670, u8"Minimum horizontal rotation speed" }, // 左右方向の最小回転速度
	{ 0x1413ab7d0, u8"Maximum rotation speed in left and right direction" }, // 左右方向の最大回転速度
	{ 0x1413ab278, u8"Standard" }, // standard
	{ 0x14139f19c, u8"Loop" }, // loop
	{ 0x141393d38, u8"Posture rotation" }, // 姿勢回転
	{ 0x1413b2d18, u8"Speed decay rate at the start of braking" }, // ブレーキ開始時の速度減衰割合
	{ 0x1413b2f08, u8"Braking force on the ground" }, // 地上でのブレーキ力
	{ 0x1413b3000, u8"Braking force in the air" }, // 空中でのブレーキ力
	{ 0x1413b3098, u8"Speed of ending brake" }, // ブレーキを終了する速度
	{ 0x1413b3198, u8"Time until forced stop in stopped state" }, // 停止状態で強制停止するまでの時間
	{ 0x1413a6e70, u8"Brake" }, // ブレーキ
	{ 0x1413ef8a8, u8"Speed that allows quick turns" }, // クイックターンが可能な速度
	{ 0x1413ef9e0, u8"Angle considered as turn input" }, // ターン入力と見なす角度
	{ 0x1413efad8, u8"Starting speed after a turn" }, // ターン後の走り出し速度
	{ 0x1413efb88, u8"Do you want to avoid falling off the edge during a turn?" }, // ターン中縁から落ちないようにするか
	{ 0x141393ee0, u8"Turn" }, // ターン
	{ 0x1413e16e0, u8"Time to form a ball" }, // ボールになるまでの時間
	{ 0x1413e1930, u8"Maximum jump fixed time [sec]" }, // 最大ジャンプ固定時間[sec]
	{ 0x1413e1ac8, u8"Acceleration time [sec]" }, // 加速時間[sec]
	{ 0x1413eb2d8, u8"Jumping power" }, // ジャンプ力
	{ 0x1413e1d58, u8"Upward acceleration force" }, // 上昇加速力
	{ 0x1413e1f58, u8"Minimum jump force" }, // 最小ジャンプ力
	{ 0x1413eeb90, u8"Gravity size" }, // 重力サイズ
	{ 0x1413e27b8, u8"Horizontal acceleration force" }, // 水平加速力
	{ 0x1413e2890, u8"Horizontal deceleration force/normal" }, // 水平減速力/通常
	{ 0x1413e29b8, u8"Horizontal deceleration force/no input" }, // 水平減速力/無入力
	{ 0x1413e2b38, u8"Horizontal deceleration force/reverse input" }, // 水平減速力/逆入力
	{ 0x1413ec1e0, u8"Minimum maximum horizontal speed" }, // 水平方向の最高速度の最低値
	{ 0x1413e2d88, u8"Initial velocity limit in opposite direction of gravity" }, // 重力逆方向の初速制限
	{ 0x1413e32a8, u8"Air resistance scale [minimum]" }, // 空気抵抗スケール[最小]
	{ 0x1413e3458, u8"Air resistance scale [maximum]" }, // 空気抵抗スケール[最大]
	{ 0x141394038, u8"Speed during jump" }, // ジャンプ中速度
	{ 0x1413ebf28, u8"Vertical initial velocity" }, // 垂直方向の初速
	{ 0x1413ec0b0, u8"Vertical initial velocity (when bouncing)" }, // 垂直方向の初速(バウンド時)
	{ 0x141394118, u8"Double jump" }, // 2段ジャンプ
	{ 0x1413d61e0, u8"Vertical velocity considered as falling" }, // 落下とみなす垂直速度
	{ 0x1413d6280, u8"Time to allow transition to jump" }, // ジャンプへの遷移を許容する時間
	{ 0x1413d63a0, u8"For a failed fall, the time the countdown starts" }, // 落下失敗用、カウントダウンが開始される時間
	{ 0x1413d64f0, u8"Grace time from start of countdown for fall failure" }, // 落下失敗用、カウントダウン開始からの猶予時間
	{ 0x1413ec6d4, u8"Falling" }, // 落下
	{ 0x1413cabd8, u8"Preliminary invincibility time" }, // 予備無敵時間
	{ 0x1413ca028, u8"Default horizontal initial velocity" }, // デフォルト水平初速
	{ 0x1413ca128, u8"Default vertical initial velocity" }, // デフォルト垂直初速
	{ 0x1413ca228, u8"Deceleration/Aerial" }, // 減速度/空中
	{ 0x1413ccbb0, u8"Fall transition time" }, // 落下遷移時間
	{ 0x1413ca438, u8"Gravity scale" }, // 重力スケール
	{ 0x1413a94a0, u8"Action fixed time" }, // アクション固定時間
	{ 0x1413c9308, u8"Lean back" }, // 仰け反る
	{ 0x1413ca320, u8"Deceleration/Ground" }, // 減速度/地上
	{ 0x1413ca620, u8"Time when interrupt transition to next action is possible" }, // 次の行動へ割込遷移可能になる時間
	{ 0x1413c9400, u8"Blow away" }, // 吹き飛ぶ
	{ 0x1413caf08, u8"Vertical speed" }, // 垂直速度
	{ 0x1413cb058, u8"Horizontal speed" }, // 水平速度
	{ 0x1413c9550, u8"Guarded" }, // ガードされた
	{ 0x1413c96a0, u8"Guarded [Super Sonic]" }, // ガードされた[スパソニ]
	{ 0x1413a9358, u8"Action time" }, // アクション時間
	{ 0x1413cdd40, u8"Deceleration speed" }, // 減速速度
	{ 0x1413cdee8, u8"Deceleration time" }, // 減速時間
	{ 0x1413c97e0, u8"While running" }, // 走りながら
	{ 0x1413cd0b0, u8"Default action time" }, // デフォルトアクション時間
	{ 0x1413c9908, u8"Stagger" }, // よろける
	{ 0x14139e9d0, u8"Jump speed" }, // ジャンプ速度
	{ 0x1413cb928, u8"Action prohibited time" }, // アクション禁止時間
	{ 0x1413c9a00, u8"Lava" }, // 溶岩
	{ 0x1413cc1b8, u8"Gravity size when falling" }, // 落下時重力サイズ
	{ 0x1413cc3b0, u8"Camera vibration name during explosion" }, // 爆発時カメラ振動名
	{ 0x1413cc4d0, u8"Controller vibration name during explosion" }, // 爆発時コントローラ振動名
	{ 0x1413f6d7c, u8"Landmine" }, // 地雷
	{ 0x141394288, u8"Damage" }, // ダメージ
	{ 0x1413aa648, u8"Invincibility time" }, // 無敵時間
	{ 0x1413f3190, u8"Deceleration jerk" }, // 減速躍度
	{ 0x1413f3288, u8"Deceleration jerk during button input" }, // ボタン入力中の減速躍度
	{ 0x1413f3330, u8"Maximum deceleration" }, // 最大減速度
	{ 0x1413f3578, u8"Frontal turning rate" }, // 正面方向旋回率
	{ 0x1413f35f8, u8"Turning force [autorun]" }, // 旋回力[オートラン]
	{ 0x1413f36b8, u8"Slope considered as slope [deg]" }, // 斜面とみなす傾斜[deg]
	{ 0x1413f38b0, u8"Minimum duration" }, // 最小継続時間
	{ 0x1413f39a0, u8"Maximum time to continue sliding in autorun" }, // オートランでスライディングを継続する最大時間
	{ 0x1413f3a58, u8"Sliding end speed during autorun" }, // オートラン中のスライディング終了速度
	{ 0x1413f3b58, u8"Time to transition to loop kick" }, // ループキックに遷移する時間
	{ 0x1413f2560, u8"Sliding" }, // スライディング
	{ 0x1413933d8, u8"Fall/initial velocity" }, // 落下/初速
	{ 0x1413934d0, u8"Fall/initial acceleration" }, // 落下/初加速度
	{ 0x1413935f0, u8"Fall/maximum acceleration" }, // 落下/最高加速度
	{ 0x1413936a8, u8"Fall/momentum" }, // 落下/躍度
	{ 0x141393748, u8"Fall/maximum fall speed" }, // 落下/最高落下速度
	{ 0x141393820, u8"Fall/angle" }, // 落下/角度
	{ 0x141393928, u8"Landing/uninterruptible time" }, // 着地/割込不能時間
	{ 0x141393a40, u8"Bound Stomp 3rd Collision Size Scale" }, // バウンドストンプ3回目のコリジョンサイズスケール
	{ 0x1413db990, u8"Boost maximum speed" }, // ブースト最高速度
	{ 0x1413dbbc0, u8"Minimum guaranteed speed" }, // 最低保証速度
	{ 0x1413d76e0, u8"Angle to start braking" }, // ブレーキを始める角度
	{ 0x1413d77d0, u8"Angle to start strong braking" }, // 強いブレーキを始める角度
	{ 0x1413d7908, u8"Slope braking speed after a slope slide" }, // 坂滑り後の斜面ブレーキ速度
	{ 0x1413d7a40, u8"Horizontal braking speed after hill sliding" }, // 坂滑り後の水平面ブレーキ速度
	{ 0x1413d7c48, u8"Air gravity size" }, // 空中重力サイズ
	{ 0x1413d7d30, u8"Forward slip end speed threshold" }, // 前方向滑り終了速度閾値
	{ 0x1413d7ec8, u8"Backward slip end speed threshold" }, // 後方向滑り終了速度閾値
	{ 0x1413d80a0, u8"Transition time to fall due to upside down position" }, // 逆さま姿勢による落下への遷移時間
	{ 0x1413d8230, u8"Threshold time for transition from falling to sliding" }, // 落下から滑りに遷移する閾時間
	{ 0x1413d83b0, u8"Slip time at the start of hill sliding" }, // 坂滑り開始時のスリップ時間
	{ 0x1413d8570, u8"Minimum slip time" }, // 最小スリップ時間
	{ 0x141394418, u8"Hill sliding" }, // 坂滑り
	{ 0x1413d6f10, u8"Threshold speed" }, // 閾値速度
	{ 0x1413d7100, u8"Backflip angle" }, // バックフリップ角度
	{ 0x1413944f0, u8"Hill backflip" }, // 坂バックフリップ
	{ 0x1413984b8, u8"Do you do a falling action?" }, // 転倒アクションをするか
	{ 0x1413985a8, u8"Inclination of falling mid-air to horizontal rotation" }, // 転倒空中が横回転になる傾き
	{ 0x1413986a0, u8"Initial velocity in the falling air" }, // 転倒空中の初速
	{ 0x141398800, u8"Falling gravity size in the air" }, // 転倒空中の重力サイズ
	{ 0x141398950, u8"Gravity size in falling air [maximum]" }, // 転倒空中の重力サイズ[最大]
	{ 0x141398a50, u8"Falling mid-air deceleration" }, // 転倒空中の減速度
	{ 0x141398b10, u8"Minimum speed in the air when falling" }, // 転倒空中の最低速度
	{ 0x141398c68, u8"Turning start interpolation time for left and right falls" }, // 左右転倒の旋回開始補間時間
	{ 0x141398dd0, u8"Turning start interpolation time for forward and backward falls" }, // 前後転倒の旋回開始補間時間
	{ 0x141398f18, u8"Minimum turning speed for left/right overturning" }, // 左右転倒の最小旋回速度
	{ 0x1413990a0, u8"Maximum turning speed for left and right overturning" }, // 左右転倒の最大旋回速度
	{ 0x1413991d0, u8"Minimum turning speed for forward and backward overturning" }, // 前後転倒の最小旋回速度
	{ 0x141399298, u8"Maximum turning speed for forward and backward overturning" }, // 前後転倒の最大旋回速度
	{ 0x141399340, u8"Left/right angle for fall detection/stagger" }, // 転倒判定の左右角度/よろけ
	{ 0x141399438, u8"Left/right angle for fall detection/fall" }, // 転倒判定の左右角度/転倒
	{ 0x141399600, u8"Anteroposterior angle for fall determination/fall" }, // 転倒判定の前後角度/転倒
	{ 0x141399770, u8"The angle of the ground where the fall judgment is a big fall" }, // 転倒判定が大転倒になる地面の角度
	{ 0x141399910, u8"Fall determination start time 1" }, // 転倒判定の開始時間1
	{ 0x141399aa0, u8"Fall judgment start time 2" }, // 転倒判定の開始時間2
	{ 0x141399c58, u8"Forward speed at the time of fall" }, // 転倒時の前方向速度
	{ 0x141399dc0, u8"Speed attenuation rate during large falls" }, // 大転倒時の速度減衰率
	{ 0x141399ee8, u8"Drop dash transition hold time" }, // ドロップダッシュ移行ホールド時間
	{ 0x14139a060, u8"Vertical speed at which vertical air brake is applied" }, // 垂直エアブレーキがかかる垂直方向速度
	{ 0x14139a148, u8"Vertical air brake force" }, // 垂直エアブレーキ力
	{ 0x14139a290, u8"Air trick altitude" }, // エアトリック可能な高度
	{ 0x14139a3a8, u8"Time until air trick becomes possible" }, // エアトリック可能になるまでの時間
	{ 0x141394590, u8"Bump jump" }, // バンプジャンプ
	{ 0x1413fb760, u8"Jump ascending acceleration power" }, // ジャンプ上昇加速力
	{ 0x1413fb878, u8"Longest acceleration time" }, // 最長加速時間
	{ 0x1413fba38, u8"Horizontal deceleration force" }, // 水平減速力
	{ 0x1413fbb88, u8"Brake deceleration force" }, // ブレーキ減速力
	{ 0x1413fbd18, u8"Horizontal movement minimum speed" }, // 水平移動最低速度
	{ 0x1413fbe58, u8"Maximum horizontal movement speed" }, // 水平移動最高速度
	{ 0x141394678, u8"Spin attack" }, // スピンアタック
	{ 0x1413dc728, u8"Attack/Sonic" }, // 攻撃/ソニック
	{ 0x1413dc7d0, u8"Attack/Super Sonic" }, // 攻撃/スーパーソニック
	{ 0x1413dbf28, u8"Knockback vertical speed" }, // ノックバック垂直速度
	{ 0x1413dc0a0, u8"Knockback horizontal speed" }, // ノックバック水平速度
	{ 0x1413dd960, u8"Knockback acceleration" }, // ノックバック加速度
	{ 0x1413dda58, u8"Knockback deceleration" }, // ノックバック減速度
	{ 0x1413ddb98, u8"Knockback angle width" }, // ノックバック角度幅
	{ 0x1413ddc90, u8"Knockback behavior time" }, // ノックバック挙動時間
	{ 0x1413ddd30, u8"Unable to attack time" }, // 攻撃不能時間
	{ 0x1413dddc8, u8"Stomping disabled time" }, // ストンピング不能時間
	{ 0x1413dc8c0, u8"Bounce/Sonic [Strong]" }, // バウンス/ソニック[強]
	{ 0x1413dc998, u8"Bounce/Sonic [weak]" }, // バウンス/ソニック[弱]
	{ 0x1413dca90, u8"Bounce/Sonic [Arashichu]" }, // バウンス/ソニック[嵐中]
	{ 0x1413dcb88, u8"Bounce/Sonic [Storm sucking]" }, // バウンス/ソニック[嵐吸い込み中]
	{ 0x1413dcc38, u8"Bounce/supersonic" }, // バウンス/スーパーソニック
	{ 0x1413dcd28, u8"Homing camera start interpolation time" }, // ホーミングカメラ開始補間時間
	{ 0x1413dce48, u8"Homing camera end interpolation time" }, // ホーミングカメラ終了補間時間
	{ 0x1413f27f0, u8"Homing attack" }, // ホーミングアタック
	{ 0x1413dc1a8, u8"Unable to attack again after attack" }, // 攻撃後再攻撃不能時間
	{ 0x1413dc2a0, u8"Attackable time after attack" }, // 攻撃後攻撃可能時間
	{ 0x141394800, u8"Enemy contact" }, // 敵接触
	{ 0x14139e898, u8"Falling acceleration" }, // 落下加速度
	{ 0x14139ffe8, u8"Maximum horizontal speed" }, // 最高水平速度
	{ 0x1413a0100, u8"Horizontal movement acceleration" }, // 水平移動加速度
	{ 0x1413cf810, u8"Damage in progress" }, // ダメージ中
	{ 0x1413cf8f8, u8"During ring dash" }, // リングダッシュ中
	{ 0x1413fdf18, u8"Altitude required to start" }, // 開始に必要な高度
	{ 0x1413cfa58, u8"Fall speed required to start" }, // 開始に必要な落下速度
	{ 0x1413cfb48, u8"Falling deceleration" }, // 落下減速度
	{ 0x1413cfc18, u8"Horizontal deceleration" }, // 水平減速度
	{ 0x1413cfd68, u8"No-input deceleration" }, // 無入力減速度
	{ 0x1413fe8a0, u8"Damage time" }, // ダメージ時間
	{ 0x1413cffc8, u8"Ring dash time" }, // リングダッシュ時間
	{ 0x1413af650, u8"Diving" }, // ダイビング
	{ 0x1413d8eb0, u8"Vertical velocity decay rate" }, // 垂直方向の速度減衰率
	{ 0x1413d9028, u8"Horizontal velocity decay rate" }, // 水平方向の速度減衰率
	{ 0x1413d91c8, u8"Acceleration rate depending on distance from fan" }, // ファンからの距離に応じた加速率
	{ 0x1413d9360, u8"Movement acceleration FV by stick" }, // スティックによる移動加速度FV
	{ 0x1413d95a0, u8"Movement acceleration SV with stick" }, // スティックによる移動加速度SV
	{ 0x1413d9768, u8"Speed at which you can transition from a jump" }, // ジャンプから移行できる速度
	{ 0x1413aaa90, u8"Upward initial velocity" }, // 上方向初速
	{ 0x1413aabd8, u8"Reverse initial velocity" }, // 後方向初速
	{ 0x1413aad78, u8"Vertical damper" }, // 垂直方向ダンパー
	{ 0x1413aae38, u8"Horizontal damper" }, // 水平方向ダンパー
	{ 0x141394988, u8"Backward somersault" }, // 後方宙返り
	{ 0x1413f5c60, u8"Initial velocity at damage" }, // ダメージ時初速
	{ 0x1413ae1c8, u8"Damage deceleration" }, // ダメージ時減速度
	{ 0x1413f5e58, u8"The speed at which the rotational speed becomes" }, // 回転速度になる速度
	{ 0x141394a08, u8"Slow movement" }, // 低速移動
	{ 0x1413fae88, u8"Activation slope" }, // 発動斜度
	{ 0x1413af7e0, u8"End slope" }, // 終了斜度
	{ 0x1413fafa0, u8"Activation speed" }, // 発動速度
	{ 0x1413fb0a8, u8"Brake judgment stick angle" }, // ブレーキ判定スティック角度
	{ 0x1413fb280, u8"Deceleration during stick input" }, // スティック入力時の減速度
	{ 0x141394ae8, u8"Spin" }, // スピン
	{ 0x14139c638, u8"Movement speed basic value" }, // 移動速度 基本値
	{ 0x14139c740, u8"Movement speed maximum value" }, // 移動速度 最大値
	{ 0x14139c888, u8"Dash movement speed basic value" }, // ダッシュ移動速度 基本値
	{ 0x14139c9b0, u8"Dash movement speed maximum value" }, // ダッシュ移動速度 最大値
	{ 0x14139cad0, u8"Movement speed On deformed mesh Basic value" }, // 移動速度 変形メッシュ上 基本値
	{ 0x14139cbf8, u8"Movement speed Maximum value on deformed mesh" }, // 移動速度 変形メッシュ上 最大値
	{ 0x14139cce8, u8"Dash movement speed On deformed mesh Basic value" }, // ダッシュ移動速度 変形メッシュ上 基本値
	{ 0x14139ce58, u8"Dash movement speed Maximum value on deformed mesh" }, // ダッシュ移動速度 変形メッシュ上 最大値
	{ 0x14139cf50, u8"Wall movement transition minimum speed" }, // 壁移動移行最低速度
	{ 0x14139d060, u8"Paste speed with stick" }, // スティックで貼り付ける速度
	{ 0x1413ae998, u8"Dedicated gravitational acceleration" }, // 専用重力加速度
	{ 0x14139d300, u8"Deceleration brake" }, // 減速ブレーキ
	{ 0x14139d430, u8"Stop brake" }, // 停止ブレーキ
	{ 0x14139d560, u8"Fall transition speed" }, // 落下移行速度
	{ 0x1413aef40, u8"Rotation speed 1" }, // 回転速度1
	{ 0x1413af0e8, u8"Rotation speed 2" }, // 回転速度2
	{ 0x1413af350, u8"Speed at which rotation speed becomes 1" }, // 回転速度1になる速度
	{ 0x1413af408, u8"Speed at rotation speed 2" }, // 回転速度2になる速度
	{ 0x14139d938, u8"Running time required to start" }, // 開始に必要な走行時間
	{ 0x14139dac0, u8"Wall movement energy consumption speed base" }, // 壁移動エネルギー消費速度 ベース
	{ 0x14139db90, u8"Wall movement energy consumption speed based on deformed mesh" }, // 壁移動エネルギー消費速度 ベース 変形メッシュ上
	{ 0x14139dc38, u8"Wall movement energy consumption speed variable" }, // 壁移動エネルギー消費速度 可変
	{ 0x14139dd10, u8"Wall movement energy consumption speed variable On deformed mesh" }, // 壁移動エネルギー消費速度 可変 変形メッシュ上
	{ 0x14139de68, u8"Wall moving energy variable angle" }, // 壁移動エネルギー可変角度
	{ 0x14139dfa8, u8"Wall movement energy variable angle on deformed mesh" }, // 壁移動エネルギー可変角度 変形メッシュ上
	{ 0x14139e0c8, u8"Deceleration start energy" }, // 減速開始エネルギー
	{ 0x14139e1b8, u8"Deceleration start energy on deformed mesh" }, // 減速開始エネルギー 変形メッシュ上
	{ 0x1413b67f0, u8"Homing enemy search range Near" }, // ホーミング索敵範囲　近
	{ 0x1413b6958, u8"Homing enemy search range far" }, // ホーミング索敵範囲　遠
	{ 0x14139e4c0, u8"Height of wall unevenness: up" }, // 壁の凹凸の高さ 上り
	{ 0x14139e598, u8"Height of wall unevenness (downhill)" }, // 壁の凹凸の高さ 下り
	{ 0x1413b6b60, u8"Recovery target search time during blowout transition" }, // 吹っ飛び移行時のリカバリーターゲット検索時間
	{ 0x141394be8, u8"Wall movement" }, // 壁移動
	{ 0x14139b6d8, u8"Sticky/gravity size" }, // 張り付き/重力サイズ
	{ 0x14139b798, u8"Sticking/Minimum time" }, // 張り付き/最小時間
	{ 0x14139b8d0, u8"Sticking/maximum time" }, // 張り付き/最大時間
	{ 0x14139b9b8, u8"Sticking/non-slip time" }, // 張り付き/滑らない時間
	{ 0x14139bbc0, u8"Sticking/maximum sliding speed" }, // 張り付き/最高滑落速度
	{ 0x14139bcf0, u8"Height to the ground where it sticks/falls" }, // 張り付き/落下になる地面までの高さ
	{ 0x14139bdf0, u8"Jump/frontal speed" }, // ジャンプ/正面方向速度
	{ 0x14139be98, u8"Jump/upward speed" }, // ジャンプ/上方向速度
	{ 0x14139bf48, u8"Jump/ejection time" }, // ジャンプ/射出時間
	{ 0x1413b5120, u8"Forward movement speed" }, // 前方移動速度
	{ 0x1413b5258, u8"Forward movement speed (dash)" }, // 前方移動速度(ダッシュ時)
	{ 0x1413b5390, u8"Lateral movement speed" }, // 側方移動速度
	{ 0x1413b5450, u8"Lateral movement speed (dashing)" }, // 側方移動速度(ダッシュ時)
	{ 0x1413b5598, u8"Backward movement speed" }, // 後方移動速度
	{ 0x1413b56a8, u8"Magnification when dashing" }, // ダッシュ時の倍率
	{ 0x1413b5798, u8"Moving motion playback rate maximum value" }, // 移動モーション再生レート 最大値
	{ 0x1413b5910, u8"Climbing fatigue constant angle" }, // クライミング疲労一定角度
	{ 0x1413b5a88, u8"Climbing fatigue constant angle on deformed mesh" }, // クライミング疲労一定角度 変形メッシュ上
	{ 0x1413b5be0, u8"Basic amount of climbing fatigue Consumption amount per 1m at starting angle" }, // クライミング疲労基本量　開始角度時の1mあたりの消費量
	{ 0x1413b5d70, u8"Climbing fatigue basic amount on deformed mesh" }, // クライミング疲労基本量　 変形メッシュ上
	{ 0x1413b5f40, u8"Climbing fatigue increase rate proportional to angle" }, // クライミング疲労増加率　角度に比例
	{ 0x1413b6128, u8"Climbing fatigue increase rate on deformed mesh" }, // クライミング疲労増加率　 変形メッシュ上
	{ 0x1413b62c8, u8"Grip strength gauge consumption rate" }, // 握力ゲージ消費速度
	{ 0x1413b6450, u8"Grip strength gauge consumption rate on deformed mesh" }, // 握力ゲージ消費速度  変形メッシュ上
	{ 0x1413b6578, u8"Grip strength gauge consumption during turbo" }, // ターボ時握力ゲージ消費量
	{ 0x1413b66b8, u8"Grip force gauge consumption during turbo on deformed mesh" }, // ターボ時握力ゲージ消費量  変形メッシュ上
	{ 0x1413b6a60, u8"The angle at which the posture resets in the upward direction of gravity" }, // 姿勢の重力上方向リセットが起こる角度
	{ 0x141394cf8, u8"Climbing" }, // クライミング
	{ 0x1413f1ce0, u8"Sliding time" }, // 滑落時間
	{ 0x1413f1d88, u8"Sliding speed" }, // 滑落速度
	{ 0x1413f1e30, u8"Slide speed on mesh" }, // 滑落速度 メッシュ上
	{ 0x1413f2010, u8"Deceleration on mesh" }, // 減速度 メッシュ上
	{ 0x141394e34, u8"Slipping down" }, // 滑落
	{ 0x1413b0ae0, u8"Consumption rate/sonic" }, // 消費速度/ソニック
	{ 0x1413b0bc8, u8"Consumption Speed/Supersonic" }, // 消費速度/スーパーソニック
	{ 0x1413b0c80, u8"Recovery speed/Sonic" }, // 回復速度/ソニック
	{ 0x1413b0db0, u8"Recovery speed/Supersonic" }, // 回復速度/スーパーソニック
	{ 0x1413b0ee0, u8"Allowable reignition rate" }, // 再点火許容レート
	{ 0x1413b1058, u8"Amount recovered by ring" }, // リングによる回復量
	{ 0x1413b1178, u8"Amount of recovery from attack" }, // 攻撃による回復量
	{ 0x1413b12e0, u8"Blur strength during boost" }, // ブースト中のブラー強度
	{ 0x1413b1410, u8"Blur interpolation time at start of boost" }, // ブースト開始時のブラー補間時間
	{ 0x1413b14f8, u8"Blur interpolation time at end of boost" }, // ブースト終了時のブラー補間時間
	{ 0x1413b15b0, u8"Lower limit speed for boost release" }, // ブースト解除の下限速度
	{ 0x1413b16a0, u8"Cooldown time for power boost effect" }, // パワーブースト演出のクールタイム
	{ 0x1413b17c0, u8"Infinite boost time" }, // 無限ブースト時間
	{ 0x14139d260, u8"Horizontal initial velocity basic value" }, // 水平方向初速  基本値
	{ 0x14139d338, u8"Horizontal initial velocity maximum value" }, // 水平方向初速  最高値
	{ 0x14139d4a8, u8"Vertical initial velocity" }, // 垂直方向初速
	{ 0x14139d5b0, u8"Minimum horizontal speed Basic value" }, // 水平方向最低速度 基本値
	{ 0x14139d6b0, u8"Minimum horizontal speed Maximum value" }, // 水平方向最低速度 最高値
	{ 0x14139d758, u8"Time from initial speed to minimum speed" }, // 初速から最低速度になるまでの時間
	{ 0x14139d840, u8"Minimum initial velocity duration" }, // 最低初速持続時間
	{ 0x14139d9c0, u8"Maximum initial velocity duration" }, // 最高初速持続時間
	{ 0x14139db00, u8"Maximum duration" }, // 最高持続時間
	{ 0x14139dbf0, u8"Gravitational acceleration multiplier" }, // 重力加速度倍率
	{ 0x14139dd70, u8"Additional time considered as transition from AirBoost even if falling" }, // 落下状態になってもAirBoostからの遷移とみなす追加時間
	{ 0x14139deb8, u8"Judgment time when inputting during Autorun of Super Sonicc" }, // スパソニのオートラン中入力時の判定時間
	{ 0x1413c4010, u8"Air boost" }, // エアブースト
	{ 0x1413a8830, u8"Initial lateral velocity" }, // 横方向の初速
	{ 0x1413a8918, u8"Lateral acceleration" }, // 横方向の加速度
	{ 0x1413a8a38, u8"Lateral deceleration" }, // 横方向の減速度
	{ 0x1413a8bb8, u8"Maximum lateral speed" }, // 横方向の最高速度
	{ 0x1413f0a58, u8"Cartwheel speed" }, // 側転速度
	{ 0x1413f0ad8, u8"Cartwheel brake force" }, // 側転ブレーキ力
	{ 0x1413f0c28, u8"Motion speed ratio" }, // モーション速度比率
	{ 0x1413f0d50, u8"Step speed" }, // ステップ速度
	{ 0x1413f0e38, u8"Maximum step distance" }, // 最大ステップ距離
	{ 0x1413f0f70, u8"Minimum step distance" }, // 最小ステップ距離
	{ 0x1413f1088, u8"Speed at which step distance is maximum" }, // ステップ距離が最大になる速度
	{ 0x1413f1188, u8"Speed at which step distance is minimum" }, // ステップ距離が最小になる速度
	{ 0x1413af608, u8"Side step" }, // サイドステップ
	{ 0x141395170, u8"Side step (special situation)" }, // サイドステップ(特殊状況)
	{ 0x1413eb6d8, u8"Minimum speed required for activation" }, // 発動に必要な最低速度
	{ 0x1413eb8a0, u8"Forward acceleration" }, // 前方への加速度
	{ 0x1413eb9f0, u8"Pulling acceleration to the moving line" }, // 移動ラインへの引き込み加速度
	{ 0x1413ebab0, u8"Initial speed of side step" }, // サイドステップの初速
	{ 0x1413ebba0, u8"Acceleration when avoiding wall collision" }, // 壁衝突回避時の加速度
	{ 0x1413ebcb8, u8"Additional acceleration when just evading" }, // ジャスト回避時の追加加速度
	{ 0x1413ebdd8, u8"Maximum speed when just evading" }, // ジャスト回避時の最大速度
	{ 0x1413ebfb0, u8"Boost time when just evading" }, // ジャスト回避時のブースト時間
	{ 0x1413ec110, u8"Deceleration at the end of just evasion boost" }, // ジャスト回避ブーストの終了時減速度
	{ 0x1413e9b28, u8"Minimum reception time" }, // 最小受付時間
	{ 0x1413e9d18, u8"Maximum reception time" }, // 最大受付時間
	{ 0x1413e9e78, u8"Just Parry reception hours" }, // ジャストパリィ受付時間
	{ 0x1413e9fe0, u8"Recovery time on failure" }, // 失敗時硬直時間
	{ 0x1413ea0e8, u8"Stiffness time when failing just parry" }, // ジャストパリィ時の失敗時硬直時間
	{ 0x1413ea1c8, u8"Short effect start interpolation time" }, // 短エフェクト開始補間時間
	{ 0x1413ea300, u8"Short effect end interpolation time" }, // 短エフェクト終了補間時間
	{ 0x1413ea458, u8"Short effect time" }, // 短エフェクト時間
	{ 0x1413ea528, u8"Long effect start interpolation time" }, // 長エフェクト開始補間時間
	{ 0x1413ea650, u8"Long effect end interpolation time" }, // 長エフェクト終了補間時間
	{ 0x1413ea798, u8"Long effect time" }, // 長エフェクト時間
	{ 0x1413ea8b0, u8"Trial effect start interpolation time" }, // 試練用エフェクト開始補間時間
	{ 0x1413ea9b8, u8"Trial effect end interpolation time" }, // 試練用エフェクト終了補間時間
	{ 0x1413eab18, u8"Trial effect time" }, // 試練用エフェクト時間
	{ 0x1413b4490, u8"Camera name" }, // カメラ名
	{ 0x141395290, u8"Parry" }, // パリー
	{ 0x1413a95c8, u8"Continuous avoidance input ahead start time" }, // 連続回避の先行入力開始時間
	{ 0x1413a96b0, u8"Prohibited time for continuous avoidance" }, // 連続回避の禁止時間
	{ 0x1413a9748, u8"Forward avoidance angle" }, // 前方回避角度
	{ 0x1413a9890, u8"Rear avoidance angle" }, // 後方回避角度
	{ 0x1413a9998, u8"Additive falling speed for aerial evasion" }, // 空中回避の加算落下速度
	{ 0x1413aa3b8, u8"Attenuation" }, // 減衰
	{ 0x1413aa510, u8"Parry available time" }, // パリー可能時間
	{ 0x1413a9b58, u8"Detour reference distance" }, // 回り込み基準距離
	{ 0x1413a9c70, u8"Wrap around limit angle" }, // 回り込み制限角度
	{ 0x1413c38a4, u8"Avoidance" }, // 回避
	{ 0x1413ec2b0, u8"Attenuation start time of climbing force" }, // 上昇力の減衰開始時間
	{ 0x1413ec370, u8"Attenuation coefficient of rising force" }, // 上昇力の減衰係数
	{ 0x1413ec4c8, u8"Gravity size during descent" }, // 下降中の重力サイズ
	{ 0x1413ec570, u8"Minimum duration" }, // 最低持続時間
	{ 0x1413ec640, u8"Vertical speed to erase tarot for ascension" }, // 上昇用のタロットを消す垂直速度
	{ 0x1413ec788, u8"Long press to hover" }, // 長押しでホバリングへ
	{ 0x1413ec808, u8"Practice judgment time" }, // 練習判定時間
	{ 0x141396830, u8"Propeller tarot" }, // プロペラタロット
	{ 0x1413ece18, u8"Attack judgment scale upon landing" }, // 着地時の攻撃判定スケール
	{ 0x141396930, u8"Tarot stomping" }, // タロットストンピング
	{ 0x1413969c8, u8"High jump" }, // ハイジャンプ
	{ 0x1413a3370, u8"Judgment conditions" }, // 判定条件
	{ 0x1413a3490, u8"Number of judgments" }, // 判定回数
	{ 0x1413a35b8, u8"Interval time" }, // 間隔時間
	{ 0x1413ed100, u8"Hit judgment" }, // ヒット判定
	{ 0x1413ed150, u8"Rotation hit detection" }, // 回転ヒット判定
	{ 0x1413ed208, u8"Speed parameters [normal]" }, // 速度パラメータ[通常]
	{ 0x1413ed318, u8"Speed parameter [Ring MAX]" }, // 速度パラメータ[リングMAX]
	{ 0x1413ed3c0, u8"Speed interpolation time at start of rotation" }, // 回転開始時の速度補間時間
	{ 0x1413ed490, u8"Speed interpolation time at end of rotation" }, // 回転終了時の速度補間時間
	{ 0x1413ed520, u8"Minimum speed multiplier during interpolation" }, // 補間中の最低速度倍率
	{ 0x1413ed700, u8"Minimum rotation time" }, // 最短回転時間
	{ 0x1413ed7b0, u8"Longest rotation time" }, // 最長回転時間
	{ 0x141396a70, u8"Tarot attack" }, // タロット攻撃
	{ 0x141396b80, u8"Rhinoceros hammer" }, // サイハンマー
	{ 0x141396c38, u8"Aerial rhinohammer" }, // 空中サイハンマー
	{ 0x141396d20, u8"Psyhammer/gimmick appears" }, // サイハンマー/ギミック出現
	{ 0x1413ee8c8, u8"Premonition display time" }, // 予兆表示時間
	{ 0x1413f0c90, u8"[Bike construction in progress] Initial speed" }, // [バイク構築中]初速
	{ 0x1413f0dc8, u8"[While building a bike] Minimum speed" }, // [バイク構築中]最低速度
	{ 0x1413f0f48, u8"[While building a bike] Maximum speed" }, // [バイク構築中]最高速度
	{ 0x1413f1000, u8"[While building a bike/in the air] Turning speed" }, // [バイク構築中/空中]旋回速度
	{ 0x1413eeab0, u8"Boost speed" }, // ブースト速度
	{ 0x1413eecc0, u8"Air/Gravity start time" }, // 空中/重力開始時間
	{ 0x1413eed80, u8"Air/gravity maximum time" }, // 空中/重力最大化時間
	{ 0x1413eee30, u8"Air/minimum gravity size" }, // 空中/最小重力サイズ
	{ 0x1413eef10, u8"Air/maximum gravity size" }, // 空中/最大重力サイズ
	{ 0x1413ef260, u8"Angle that causes jumping behavior" }, // 飛び出し挙動になる角度
	{ 0x1413ef3b0, u8"Speed at which jumping behavior occurs" }, // 飛び出し挙動になる速度
	{ 0x1413ef480, u8"Will you jump out on the hill?" }, // 丘で飛び出すか
	{ 0x1413ef548, u8"Air acceleration motion" }, // 空中加速動作
	{ 0x1413ef678, u8"Speed threshold at which air acceleration is possible" }, // 空中加速が可能になる速度の閾値
	{ 0x1413ef7d0, u8"Quick turn time" }, // クイックターン時間
	{ 0x1413efc40, u8"Stomping altitude limit" }, // ストンピングの高度制限
	{ 0x1413efce0, u8"Time until stomping becomes possible after jumping into the air" }, // 空中へ飛び出し後、ストンピング可能になるまでの時間
	{ 0x1413f3b80, u8"Follow time" }, // 追従時間
	{ 0x1413efdc8, u8"Quick turn camera parameters" }, // クイックターンのカメラパラメータ
	{ 0x141396de0, u8"Tarot boost" }, // タロットブースト
	{ 0x1413f1b10, u8"Turning judgment input angle" }, // 旋回判定入力角度
	{ 0x1413f1c58, u8"Minimum speed basic value" }, // 最低速度 基本値
	{ 0x1413f1d40, u8"Minimum speed Maximum value" }, // 最低速度 最大値
	{ 0x1413f1d98, u8"Maximum speed basic value" }, // 最高速度 基本値
	{ 0x1413f1eb8, u8"Maximum speed Maximum value" }, // 最高速度 最大値
	{ 0x1413f2090, u8"Maximum drift angle" }, // 最大ドリフト角
	{ 0x1413f2168, u8"Rudder acceleration" }, // 舵加速度
	{ 0x1413f22e8, u8"Maximum rudder speed" }, // 最大舵速度
	{ 0x1413f2410, u8"Rudder acceleration with no input" }, // 無入力時舵加速度
	{ 0x1413f2578, u8"Maximum rudder speed without input" }, // 無入力時最大舵速度
	{ 0x1413f26b0, u8"Maximum rotation speed" }, // 最大回転速度
	{ 0x1413f2810, u8"Fixed left and right rudder operation" }, // 舵の左右固定操作
	{ 0x1413f28e8, u8"Drift dash time" }, // ドリフトダッシュ時間
	{ 0x1413f3458, u8"Gravity direction offset" }, // 重力方向オフセット
	{ 0x1413f3590, u8"Maximum screen horizontal offset [m]" }, // スクリーン左右方向の最大オフセット[m]
	{ 0x141396f10, u8"Tarot drift" }, // タロットドリフト
	{ 0x141397000, u8"Charm attack" }, // チャームアタック
	{ 0x1413fc4b8, u8"Forced running time" }, // 強制走り時間
	{ 0x1413fc5f8, u8"Normal speed" }, // 通常速度
	{ 0x1413fce20, u8"How long you can move through the air while spinning" }, // スピン状態のまま空中を移動できる時間
	{ 0x1413fced0, u8"The speed at which the spin boost ends and the rolling movement begins." }, // スピンブーストが終了してころころ移動になる速度
	{ 0x1413fd210, u8"Turning speed during charge" }, // チャージ中回頭速度
	{ 0x1413fd298, u8"The angle at which the turning speed becomes 0 during charging" }, // チャージ中回頭速度が0になる角度
	{ 0x1413fd380, u8"The angle at which the turning speed is maximum during charging" }, // チャージ中回頭速度が最大になる角度
	{ 0x1413fd4b0, u8"Camera shake name when charging" }, // チャージ時のカメラシェイク名
	{ 0x1413ea584, u8"Underwater" }, // 水中
	{ 0x1413f9344, u8"S.V." }, // SV
	{ 0x1413fa888, u8"Travel scale" }, // 移動量スケール
	{ 0x1413fa938, u8"Motion playback speed ratio" }, // モーション再生速度比率
	{ 0x1413f56e8, u8"Combo/Punch 1" }, // コンボ/パンチ1
	{ 0x1413f57b8, u8"Combo/Punch 2" }, // コンボ/パンチ2
	{ 0x1413f5870, u8"Combo/uppercut" }, // コンボ/アッパーカット
	{ 0x1413f58e0, u8"Parry defense down effect" }, // パリィ防御ダウン効果
	{ 0x14139bc08, u8"Motion playback speed ratio/accelerator" }, // モーション再生速度比率/アクセル
	{ 0x14139bf64, u8"Sonic" }, // sonic
	{ 0x14139c008, u8"SuperSonic1" }, // superSonic1
	{ 0x14139c0d0, u8"SuperSonic2" }, // superSonic2
	{ 0x1413f5d00, u8"Attack Act / Accele Combo" }, // 攻撃Act / Accele Combo
	{ 0x1413e6e98, u8"Loop radius" }, // ループ半径
	{ 0x1413e6f70, u8"Loop time" }, // ループ時間
	{ 0x1413e7068, u8"Loop movement curve ratio" }, // ループ移動カーブ比率
	{ 0x1413e71c0, u8"Quiet time after loop" }, // ループ後静止時間
	{ 0x1413e72a0, u8"Maximum movement speed after loop" }, // ループ後最大移動速度
	{ 0x1413e7380, u8"Kick speed" }, // キック速度
	{ 0x1413e7468, u8"Maximum kick time" }, // キック最大時間
	{ 0x1413c0700, u8"Warp distance during combo" }, // コンボ中ワープ距離
	{ 0x1413f5d68, u8"Attack Act / Loop Kick" }, // 攻撃Act / Loop Kick
	{ 0x1413bfc90, u8"Distance ratio for each zigzag point" }, // ジグザグポイント毎の距離比率
	{ 0x1413bfdb0, u8"Angle for each zigzag point" }, // ジグザグポイント毎の角度
	{ 0x1413bfe90, u8"Radius per zigzag point" }, // ジグザグポイント毎の半径
	{ 0x1413bff30, u8"Zigzag maximum distance" }, // ジグザグ最大距離
	{ 0x1413bfff8, u8"Zigzag movement time/initial" }, // ジグザク移動時間/最初
	{ 0x1413c00e0, u8"Zigzag movement time/last" }, // ジグザク移動時間/最後
	{ 0x1413c02c8, u8"Max lunge time" }, // 突進最大時間
	{ 0x1413c0470, u8"Camera offset elevation angle" }, // カメラオフセット仰角
	{ 0x1413c0590, u8"Camera offset azimuth" }, // カメラオフセット方位角
	{ 0x1413c0648, u8"Camera Roll" }, // カメラRoll
	{ 0x1413f5e08, u8"Attack Act / Crasher" }, // 攻撃Act / Crasher
	{ 0x1413b3650, u8"Hit judgment/last" }, // ヒット判定/ラスト
	{ 0x1413fe8b8, u8"Homing speed" }, // ホーミング速度
	{ 0x1413fe918, u8"Bounce time" }, // バウンス時間
	{ 0x1413fea08, u8"Rotating attack radius" }, // 回転攻撃半径
	{ 0x1413feaf0, u8"Rotation attack time" }, // 回転攻撃時間
	{ 0x1413b4380, u8"Timing to switch damage judgment to last hit" }, // ダメージ判定をラストヒットに切り替えるタイミング
	{ 0x1413c1a98, u8"Slow ratio" }, // スロー比率
	{ 0x1413fee38, u8"Slow ratio/during rotation" }, // スロー比率/回転中
	{ 0x1413928c8, u8"Camera name/after hit" }, // カメラ名/ヒット後
	{ 0x141392960, u8"Camera vibration while rotating" }, // 回転中カメラ振動
	{ 0x1413f5f20, u8"Attack Act / Spin Slash" }, // 攻撃Act / Spin Slash
	{ 0x14139cf38, u8"Invalid swing time" }, // 空振り無効時間
	{ 0x1413b3830, u8"Enemy throw ratio while rising" }, // 上昇中のエネミースロー比率
	{ 0x1413b3cb0, u8"Climb distance 2" }, // 上昇距離2
	{ 0x1413b3d58, u8"Climb distance 2/after last hit" }, // 上昇距離2/ラストヒット後
	{ 0x1413b3e40, u8"Launch speed after ascent" }, // 上昇後の打ち上げ速度
	{ 0x1413b3f48, u8"Spiral radius" }, // 螺旋半径
	{ 0x1413b40b0, u8"Time for the spiral to expand to its maximum" }, // 螺旋が最大にまで広がる時間
	{ 0x1413b41e8, u8"Time for the spiral to close to its minimum" }, // 螺旋が最小にまで閉じる時間
	{ 0x1413f6008, u8"Attack Act / Charge Attack" }, // 攻撃Act / Charge Attack
	{ 0x141393e88, u8"Hit detection/enhanced stomping" }, // ヒット判定/強化ストンピング
	{ 0x141393f70, u8"Hit detection/enhanced stomping last" }, // ヒット判定/強化ストンピングラスト
	{ 0x141394100, u8"Flip speed" }, // フリップ速度
	{ 0x141394218, u8"Attack motion time" }, // 攻撃モーション時間
	{ 0x141394360, u8"Push/Minimum push time" }, // 押し込み/最小押し込み時間
	{ 0x1413943d8, u8"Push-in/drop height resulting in minimum push-in time" }, // 押し込み/最小押し込み時間になる落下高度
	{ 0x141394528, u8"Push/maximum push time" }, // 押し込み/最大押し込み時間
	{ 0x141394610, u8"Push-in/drop height resulting in maximum push-in time" }, // 押し込み/最大押し込み時間になる落下高度
	{ 0x1413947d8, u8"Warp distance during combo/Asura only" }, // コンボ中ワープ距離/Asura専用
	{ 0x1413948d8, u8"Camera name/after start of continuous attack" }, // カメラ名/連撃開始後
	{ 0x1413f6150, u8"Attack Act / Stomping" }, // 攻撃Act / Stomping
	{ 0x1413f6208, u8"Attack Act / Combo Finish" }, // 攻撃Act / Combo Finish
	{ 0x1413f6f28, u8"Waiting time before first shot" }, // 初撃前の待ち時間
	{ 0x1413c1648, u8"Shooting interval" }, // 撃ち出し間隔
	{ 0x1413f7108, u8"Falling speed" }, // 落下速度
	{ 0x1413f71e8, u8"Duration of auto combo" }, // オートコンボ時の持続時間
	{ 0x1413c2300, u8"Maximum bullet movement speed" }, // 弾の最大移動速度
	{ 0x1413f6320, u8"Attack Act / Sonic Boom" }, // 攻撃Act / Sonic Boom
	{ 0x1413c17d0, u8"Movement angle" }, // 移動角度
	{ 0x1413c1918, u8"Rotation rest time" }, // 回転静止時間
	{ 0x1413c1ba8, u8"Cycle deviation of spin performance" }, // スピン演出の周期ずれ
	{ 0x1413c1d68, u8"Radius of spin effect" }, // スピン演出の半径
	{ 0x1413c1ee8, u8"Speed of spin performance" }, // スピン演出の速度
	{ 0x1413c2140, u8"Appearance rotation local offset" }, // 出現回転ローカルオフセット
	{ 0x1413c2618, u8"Camera vibration when firing" }, // 発射時カメラ振動
	{ 0x1413f6440, u8"Attack Act / Cross Slash" }, // 攻撃Act / Cross Slash
	{ 0x1413de1b0, u8"Bullet appearance time" }, // 弾出現演出時間
	{ 0x1413de280, u8"Bullet appearance phase time" }, // 弾出現演出位相時間
	{ 0x1413de360, u8"Bullet rotation radius" }, // 弾回転演出半径
	{ 0x1413de410, u8"Bullet rotation speed" }, // 弾回転演出速度
	{ 0x1413de510, u8"Bullet rotation speed/after start of firing" }, // 弾回転演出速度/発射開始後
	{ 0x1413de608, u8"Bullet generation delay time" }, // 弾生成遅延時間
	{ 0x1413de6f8, u8"Waiting time before launch" }, // 発射前待機時間
	{ 0x1413de980, u8"Randomize firing order" }, // 射出順のランダム化
	{ 0x1413deab8, u8"Waiting time after injection" }, // 射出後待機時間
	{ 0x1413decd0, u8"Inclination of injection direction" }, // 射出方向の傾き
	{ 0x1413dee68, u8"Strength at the beginning" }, // 始端の強度
	{ 0x1413defa8, u8"Termination strength" }, // 終端の強度
	{ 0x1413df100, u8"Waiting time for starting spiral rotation" }, // 螺旋回転開始待機時間
	{ 0x1413df2c8, u8"Helical rotation speed/initial speed" }, // 螺旋回転速度/初速度
	{ 0x1413df430, u8"Helical rotation speed/maximum speed" }, // 螺旋回転速度/最大速度
	{ 0x1413df5f0, u8"Helical rotation speed/acceleration" }, // 螺旋回転速度/加速度
	{ 0x1413df720, u8"Number of shots" }, // ショット数
	{ 0x1413dfb88, u8"Whiteout start time" }, // ホワイトアウト開始時間
	{ 0x1413dfd10, u8"Whiteout start fade time" }, // ホワイトアウト開始フェード時間
	{ 0x1413dfe68, u8"Whiteout fade retention time" }, // ホワイトアウトフェード維持時間
	{ 0x1413dff88, u8"Whiteout end fade time" }, // ホワイトアウト終了フェード時間
	{ 0x1413f6560, u8"Attack Act / Homing Shot" }, // 攻撃Act / Homing Shot
	{ 0x1413f63f8, u8"Hit judgment 1" }, // ヒット判定1
	{ 0x1413f6520, u8"Hit judgment 2" }, // ヒット判定2
	{ 0x1413f6668, u8"Super Sonic alter ego position offset" }, // スパソニ分身の位置オフセット
	{ 0x1413f66b8, u8"Attack Act / Smash" }, // 攻撃Act / Smash
	{ 0x1413ad330, u8"Travel time/Super Sonic" }, // 移動時間/スパソニ
	{ 0x1413ad438, u8"Wraparound size scale/Super Sonic" }, // 回り込みの大きさスケール/スパソニ
	{ 0x1413ad688, u8"Camera rotation ratio" }, // カメラ旋回比率
	{ 0x1413f67e8, u8"Defense Act / Behind" }, // 防御Act / Behind
	{ 0x141396ef0, u8"Number of rings required for transformation" }, // 変身に必要なリング数
	{ 0x141397050, u8"Time to consume one ring [sec]" }, // リング1枚消費する時間[sec]
	{ 0x141397160, u8"Ring absorption radius" }, // リングの吸収範囲半径
	{ 0x141397258, u8"Speed at which movement SE starts sounding" }, // 移動SEを鳴らし始める速度
	{ 0x141397300, u8"Super Sonic 2 aura color" }, // スパソニ2のオーラカラー
	{ 0x141397e48, u8"Roll rotation" }, // ロール回転
	{ 0x141397448, u8"Afterimage effect settings" }, // 残像エフェクト設定
	{ 0x1413f69b8, u8"Super sonic" }, // スーパーソニック
	{ 0x1413efb60, u8"Deceleration when blown away" }, // 吹き飛ばされ時の減速度
	{ 0x1413efc88, u8"Deceleration after being blown away and landing" }, // 吹き飛ばされ着地後の減速度
	{ 0x1413efd30, u8"Gravity scale when blown away" }, // 吹き飛ばされ時の重力スケール
	{ 0x1413efe20, u8"Time when blown away can be canceled" }, // 吹き飛ばされをキャンセル可能になる時間
	{ 0x1413eff00, u8"Blown down time" }, // 吹き飛ばされダウン時間
	{ 0x1413effc8, u8"Size of upward bounce when pylon collides" }, // パイロン衝突時の上方向の跳ね上げの大きさ
	{ 0x1413f0168, u8"Blow-off speed when pylon collides" }, // パイロン衝突時の吹き飛び速度
	{ 0x1413f0300, u8"Hitstop name for pylon collision" }, // パイロン衝突時のヒットストップ名
	{ 0x1413f4590, u8"Moving timescale" }, // 移動中のタイムスケール
	{ 0x1413f46d8, u8"Remaining time until hit to play hit effect" }, // ヒット演出を再生するヒットまでの残り時間
	{ 0x1413f47b8, u8"Hit camera offset" }, // ヒットカメラオフセット
	{ 0x1413f48b0, u8"Hit camera In interpolation time" }, // ヒットカメラIn補間時間
	{ 0x1413f4990, u8"Hit Camera Fovy Angle" }, // ヒットカメラFovy角度
	{ 0x1413f4ac0, u8"Hit timescale value" }, // ヒットタイムスケール値
	{ 0x1413f4bd0, u8"Hit time scale In interpolation time" }, // ヒットタイムスケールIn補間時間
	{ 0x1413f4cd0, u8"Camera release transition time" }, // カメラ解除遷移時間
	{ 0x1413f4ea8, u8"Timescale release transition time" }, // タイムスケール解除遷移時間
	{ 0x1413f4ff0, u8"Time to maintain hitstop" }, // ヒットストップを維持する時間
	{ 0x1413f5110, u8"Firing effect offset" }, // 発射エフェクトのオフセット
	{ 0x1413f5200, u8"Hit effect offset" }, // ヒットエフェクトのオフセット
	{ 0x1413f52d8, u8"Camera shake playback timing" }, // カメラシェイク再生タイミング
	{ 0x1413ed308, u8"Meandering angle" }, // 蛇行角度
	{ 0x1413ed4d8, u8"Time to run away" }, // 逃げ回る時間
	{ 0x1413f6bc0, u8"Bee running around" }, // 蜂逃げ回り
	{ 0x1413eedd0, u8"Maximum number of Kodama" }, // コダマ最大数
	{ 0x1413ef3a0, u8"Kodama number" }, // コダマ数
	{ 0x1413ef470, u8"Initial speed of movement" }, // 移動初速
	{ 0x1413ef590, u8"Minimum movement speed" }, // 移動最小速度
	{ 0x1413ef718, u8"Maximum movement speed" }, // 移動最大速度
	{ 0x1413eef98, u8"Parameters for each Kodama number" }, // コダマ数ごとのパラメータ
	{ 0x1413f6c68, u8"Carrying Kodama" }, // コダマ担ぎ歩き
	{ 0x1413e8898, u8"Low risk radius" }, // 危険度低の半径
	{ 0x1413e8988, u8"Medium danger radius" }, // 危険度中の半径
	{ 0x1413e8a88, u8"High risk radius" }, // 危険度高の半径
	{ 0x1413a9130, u8"Damage reduction rate" }, // ダメージ減少レート
	{ 0x1413f7a08, u8"Parry attack down effect" }, // パリィ攻撃ダウン効果
	{ 0x1413fb230, u8"Search range" }, // 検索範囲
	{ 0x1413fb338, u8"Number of search result notifications" }, // 検索結果の通知数
	{ 0x1413fb4a0, u8"Cyline effect time/Wall" }, // サイラインのエフェクト時間/Wall
	{ 0x1413fb638, u8"Effect activation range" }, // 効果発動範囲
	{ 0x1413fb680, u8"Stomping search distance" }, // ストンピング検索距離
	{ 0x1413fb6f8, u8"Stomping search range" }, // ストンピング検索範囲
	{ 0x1413fc220, u8"Diving size scale" }, // 飛び込み大きさスケール
	{ 0x1413fc3f0, u8"Attack range scale when stomping" }, // ストンピング時の攻撃範囲スケール
	{ 0x1413fc4e8, u8"Digging from stomping" }, // ストンピングから穴掘り
	{ 0x1413fd558, u8"Minimum glide speed" }, // 最低滑空速度
	{ 0x1413fd628, u8"Minimum glide speed/Ring MAX" }, // 最低滑空速度/リングMAX
	{ 0x1413fd6a8, u8"Performance time before movement starts" }, // 移動開始前演出時間
	{ 0x1413fd7b0, u8"Minimum action time" }, // 最小アクション時間
	{ 0x1413fd8f0, u8"Basic falling speed" }, // 基本落下速度
	{ 0x1413fda10, u8"Basic falling acceleration" }, // 基本落下加速度
	{ 0x1413fdb00, u8"Maximum rotation speed" }, // 最大回転になる速度
	{ 0x1413fdba0, u8"Roll speed" }, // ロール速度
	{ 0x1413fdc10, u8"Return roll speed" }, // 戻りロール速度
	{ 0x1413fdca8, u8"Yaw speed" }, // ヨー速度
	{ 0x1413fdd60, u8"Maximum roll angle" }, // 最大ロール角
	{ 0x1413fde70, u8"Behavior maintenance altitude" }, // 挙動維持高度
	{ 0x141397750, u8"Gliding" }, // 滑空
	{ 0x1413977b0, u8"Sin knuckle" }, // サイナックル
	{ 0x141397888, u8"Cyknuckle/digging" }, // サイナックル/穴掘り
	{ 0x141397990, u8"Cyknuckle/Warp Hole" }, // サイナックル/ワープ穴
	{ 0x141397a70, u8"Psyknuckle/Item appearance hole" }, // サイナックル/アイテム出現穴
	{ 0x141397b10, u8"Psy-knuckle/gimmick appearance" }, // サイナックル/ギミック出現
	{ 0x1413fe968, u8"Waiting time before attack start 1" }, // 攻撃開始前待機時間1
	{ 0x1413fea48, u8"Waiting time before attack start 2" }, // 攻撃開始前待機時間2
	{ 0x1413fec38, u8"Slow rate" }, // スローレート
	{ 0x1413fed90, u8"Hit judgment / Last" }, // ヒット判定 / Last
	{ 0x1413fee78, u8"Hit position correction" }, // ヒット位置補正
	{ 0x1413fef60, u8"Hit effect correction" }, // ヒットエフェクト補正
	{ 0x141392880, u8"Movement speed after attack failure" }, // 攻撃失敗後移動速度
	{ 0x141392940, u8"Movement time after a failed attack" }, // 攻撃失敗後移動時間
	{ 0x141392a08, u8"Camera setting name while moving" }, // 移動中のカメラ設定名
	{ 0x141392b48, u8"Camera setting name during attack" }, // 攻撃中のカメラ設定名
	{ 0x141392c00, u8"Start charging the camera while moving" }, // 移動中カメラをチャージ開始時から開始
	{ 0x141392c68, u8"Height from the ground at which the production camera becomes effective" }, // 演出カメラが有効になる地面からの高さ
	{ 0x141392df8, u8"Camera vibration name during attack" }, // 攻撃中のカメラ振動名
	{ 0x141392f20, u8"Controller vibration name during attack" }, // 攻撃中のコントローラ振動名
	{ 0x141392fd0, u8"End interpolation time of camera vibration and controller vibration" }, // カメラ振動とコントローラ振動の終了補間時間
	{ 0x141397be8, u8"Maximum Heat Knuckle" }, // マキシマムヒートナックル
	{ 0x141397cb0, u8"Drill boost" }, // ドリルブースト
	{ 0x1413957f8, u8"Horizontal deceleration" }, // 水平方向の減速度
	{ 0x1413958c0, u8"Acceleration rate according to distance from specified altitude" }, // 指定高度からの距離に応じた加速率
	{ 0x141395968, u8"Movement acceleration with stick" }, // スティックによる移動加速度
	{ 0x141395a68, u8"Maximum horizontal speed" }, // 水平方向の最高速度
	{ 0x141395b48, u8"Maximum vertical speed" }, // 垂直方向の最高速度
	{ 0x141395c28, u8"Vortex rotation speed" }, // 渦の回転速度
	{ 0x141395d30, u8"Rotational angular velocity of the vortex" }, // 渦の回転角速度
	{ 0x141395e10, u8"Vortex suction speed minimum value" }, // 渦の吸い込み速度最小値
	{ 0x141395f28, u8"Maximum vortex suction speed" }, // 渦の吸い込み速度最大値
	{ 0x141396060, u8"Distance at which the vortex suction velocity reaches the minimum value" }, // 渦の吸い込み速度最小値になる距離
	{ 0x141396148, u8"Distance at which the vortex suction speed reaches its maximum value" }, // 渦の吸い込み速度最大値になる距離
	{ 0x141396328, u8"Time not to apply the brakes when damaged" }, // ダメージ時ブレーキをかけない時間
	{ 0x141396460, u8"Power to change direction during homing attack reaction" }, // ホーミングアタックリアクション時の向き変更する力
	{ 0x1413965c0, u8"Force to change direction during homing attack reaction (while being caught)" }, // ホーミングアタックリアクション時の向き変更する力(巻き込まれ中)
	{ 0x141398240, u8"Storm floating behavior" }, // 嵐浮遊挙動
	{ 0x1413982b0, u8"Cloud jump" }, // 雲ジャンプ
	{ 0x14139e968, u8"Water resistance coefficient" }, // 水の抵抗係数
	{ 0x14139ea80, u8"Stomping speed" }, // ストンピング速度
	{ 0x14139eb00, u8"Bounce speed" }, // バウンド速度
	{ 0x141398370, u8"Behavior inside the aqua ball" }, // アクアボール内挙動
	{ 0x1413f2520, u8"Forward acceleration force" }, // 前方加速力
	{ 0x1413f2690, u8"Forward deceleration force" }, // 前方減速力
	{ 0x1413f27b0, u8"Lateral deceleration force" }, // 横方減速力
	{ 0x1413f29d0, u8"Automatic heading correction speed" }, // 自動進行方向修正速度
	{ 0x1413f2ac8, u8"Direction correction speed" }, // 進行方向修正速度
	{ 0x1413f2ba0, u8"Air direction correction speed" }, // 空中進行方向修正速度
	{ 0x141398450, u8"Slider" }, // スライダー
	{ 0x14139e6a8, u8"Value when acquiring skill piece" }, // スキルピース取得時の値
	{ 0x1413d1340, u8"Minimum speed basic value when boosting" }, // ブースト時　最低速度 基本値
	{ 0x1413d14e8, u8"When boosting Minimum speed Maximum value" }, // ブースト時　最低速度 最大値
	{ 0x1413d15f8, u8"Maximum speed when boosting Basic value" }, // ブースト時　最高速度 基本値
	{ 0x1413d16d0, u8"Maximum speed during boost" }, // ブースト時　最高速度 最大値
	{ 0x1413d1e48, u8"Driving return time" }, // 走行復帰時間
	{ 0x1413d1ef0, u8"Maximum charge time" }, // 最大チャージ時間
	{ 0x1413d1fc8, u8"Minimum dash speed" }, // 最低ダッシュ速度
	{ 0x1413d2080, u8"Max dash speed" }, // 最大ダッシュ速度
	{ 0x1413d2130, u8"Minimum dash jump speed" }, // 最低ダッシュジャンプ速度
	{ 0x1413d2240, u8"Max dash jump speed" }, // 最大ダッシュジャンプ速度
	{ 0x1413d2430, u8"Fall judgment time" }, // 落下判定時間
	{ 0x1413f23b0, u8"Drift" }, // ドリフト
	{ 0x1413985f0, u8"Aerial drift" }, // 空中ドリフト
	{ 0x1413d4468, u8"Uncontrollable speed" }, // 制御不能速度
	{ 0x1413d4568, u8"Travel return speed" }, // 走行復帰速度
	{ 0x1413986f0, u8"Drift dash" }, // ドリフトダッシュ
	{ 0x1413ae108, u8"Acceleration to minimum speed" }, // 最低速度への加速度
	{ 0x1413ae280, u8"Damage slowdown time" }, // ダメージ減速時間
	{ 0x1413ae2e8, u8"Damage motion time" }, // ダメージモーション時間
	{ 0x1413ae448, u8"Invincibility time after damage" }, // ダメージ後無敵時間
	{ 0x1413ae588, u8"Maximum speed damper coefficient" }, // 最高速ダンパー係数
	{ 0x1413ae698, u8"Aerial damper vertical" }, // 空中ダンパー垂直方向
	{ 0x1413ae820, u8"Aerial damper horizontal" }, // 空中ダンパー水平方向
	{ 0x1413aead8, u8"Air jump speed" }, // 空中ジャンプ速度
	{ 0x1413aebe0, u8"Ground jump speed" }, // 地上ジャンプ速度
	{ 0x1413aeca8, u8"Air acceleration" }, // 空中加速度
	{ 0x1413aed48, u8"Maximum air acceleration" }, // 最高空中加速度
	{ 0x1413aee10, u8"Downforce coefficient" }, // ダウンフォース係数
	{ 0x1413af208, u8"Rotation speed 3" }, // 回転速度3
	{ 0x1413af4b0, u8"Starting speed" }, // 開始速度
	{ 0x1413af598, u8"Starting slope" }, // 開始斜度
	{ 0x1413af680, u8"Starting slope when stationary" }, // 静止時開始斜度
	{ 0x1413af8d8, u8"Ending time" }, // 終了時間
	{ 0x1413afa20, u8"Great landing judgment time" }, // 大着地判定時間
	{ 0x141398820, u8"Board riding" }, // 板乗り
	{ 0x1413d4d08, u8"Minimum dash speed basic value" }, // 最低ダッシュ速度 基本値
	{ 0x1413d4de8, u8"Minimum dash speed Maximum value" }, // 最低ダッシュ速度 最大値
	{ 0x1413d4f10, u8"Maximum dash speed basic value" }, // 最大ダッシュ速度 基本値
	{ 0x1413d5048, u8"Maximum dash speed Maximum value" }, // 最大ダッシュ速度 最大値
	{ 0x1413d5140, u8"Dash speed from bump jump" }, // バンプジャンプからのダッシュ速度
	{ 0x1413c41e8, u8"Drop dash" }, // ドロップダッシュ
	{ 0x1413b2298, u8"Starting falling speed" }, // 開始落下速度
	{ 0x1413b23d8, u8"Jump multiplier 1st stage" }, // ジャンプ倍率　１段目
	{ 0x1413b24f0, u8"Jump magnification 2nd stage" }, // ジャンプ倍率　2段目
	{ 0x1413b2618, u8"Jump multiplier 3rd stage" }, // ジャンプ倍率　3段目
	{ 0x1413b2750, u8"Unable to jump time" }, // ジャンプ不能時間
	{ 0x1413989d8, u8"Bound jump" }, // バウンドジャンプ
	{ 0x1413e4410, u8"Dash speed basic value" }, // ダッシュ速度　基本値
	{ 0x1413e44d8, u8"Dash speed maximum value" }, // ダッシュ速度　最大値
	{ 0x1413e45e0, u8"Movement speed basic value" }, // 移動速度　基本値
	{ 0x1413e46f8, u8"Movement speed maximum value" }, // 移動速度　最大値
	{ 0x1413c4130, u8"Light dash" }, // ライトダッシュ
	{ 0x1413f28d0, u8"Spin dash" }, // スピンダッシュ
	{ 0x1413d9e28, u8"Normal maximum speed" }, // 通常最高速度
	{ 0x1413d9f40, u8"Dash max speed" }, // ダッシュ最高速度
	{ 0x1413da010, u8"Normal acceleration" }, // 通常加速度
	{ 0x1413da128, u8"Dash acceleration" }, // ダッシュ加速度
	{ 0x1413da310, u8"Rotation speed When speed is 0" }, // 回転速度　速度０の時
	{ 0x1413da418, u8"Rotation speed When dash is at maximum speed" }, // 回転速度　ダッシュ最高速の時
	{ 0x1413da510, u8"Blowback time when attacked" }, // 攻撃を受けた時の吹き飛び時間
	{ 0x1413da678, u8"Angle when attacked" }, // 攻撃を受けた時の角度
	{ 0x1413da788, u8"Deceleration when turning" }, // 減速度 ターン時
	{ 0x1413da880, u8"Rotation speed when turning" }, // ターン時の回転速度
	{ 0x1413daa40, u8"Angle considered as quick turn input" }, // クイックターン入力と見なす角度
	{ 0x1413dac38, u8"Speed of transition to quick turn" }, // クイックターンに移行する速度
	{ 0x1413dae00, u8"Deceleration during quick turns" }, // クイックターン時の減速度
	{ 0x1413daf30, u8"Rotation speed during quick turn" }, // クイックターン時の回転速度
	{ 0x1413db008, u8"Combo reception hours" }, // コンボ受付時間
	{ 0x141398c90, u8"Super Sonic flight" }, // スパソニ飛行
	{ 0x1413e5038, u8"Course return acceleration" }, // コース復帰加速度
	{ 0x1413e5230, u8"Course return speed" }, // コース復帰速度
	{ 0x1413e5460, u8"Course return acceleration High speed" }, // コース復帰加速度 高速
	{ 0x1413e5630, u8"Course return speed High speed" }, // コース復帰速度 高速
	{ 0x1413e5918, u8"Maximum speed 2D" }, // 最高速度2D
	{ 0x1413e5a30, u8"Acceleration 2d" }, // 加速度2D
	{ 0x1413e5b30, u8"Deceleration 2D" }, // 減速度2D
	{ 0x1413e5cc8, u8"Course return speed 2D" }, // コース復帰速度2D
	{ 0x1413e5db0, u8"Orientation change speed 2D" }, // 向き変更速度2D
	{ 0x141398d98, u8"Super Sonic flight when range limited" }, // スパソニ飛行 範囲制限時
	{ 0x1413c3580, u8"Low gravity mode/gravity scale" }, // 低重力モード/重力スケール
	{ 0x1413c36f0, u8"Timescale mode/timescale" }, // タイムスケールモード/タイムスケール
	{ 0x1413c3870, u8"Maximum speed challenge mode/maximum acceleration force" }, // 最高速チャレンジモード/最大加速力
	{ 0x1413c3a58, u8"Fastest challenge mode/jump" }, // 最高速チャレンジモード/躍度
	{ 0x1413c3bf0, u8"Maximum speed challenge mode/Speed to be treated as minimum speed" }, // 最高速チャレンジモード/最低速扱いとする速度
	{ 0x1413c3db0, u8"Maximum speed challenge mode/speed to be treated as maximum speed" }, // 最高速チャレンジモード/最高速扱いとする速度
	{ 0x1413c3f48, u8"Fastest challenge mode/gauge recovery speed" }, // 最高速チャレンジモード/ゲージ回復速度
	{ 0x1413c40e0, u8"Top speed challenge mode/top speed" }, // 最高速チャレンジモード/最高速度
	{ 0x1413c42c0, u8"Maximum speed challenge mode/maximum speed [Boost]" }, // 最高速チャレンジモード/最高速度[Boost]
	{ 0x1413c4420, u8"Fastest challenge mode/number of stages" }, // 最高速チャレンジモード/段階数
	{ 0x14139b488, u8"Speed/power boost" }, // 速度/パワーブースト
	{ 0x14139b528, u8"Gauge rising speed" }, // ゲージ上昇速度
	{ 0x1413c45b0, u8"Fastest challenge mode/stage parameters" }, // 最高速チャレンジモード/段階パラメータ
	{ 0x1413c46c8, u8"Animal transport mode/minimum movement speed" }, // アニマル運搬モード/最低移動速度
	{ 0x1413c4868, u8"Animal transport mode/maximum movement speed" }, // アニマル運搬モード/最高移動速度
	{ 0x1413c4a78, u8"Animal transport mode/initial speed" }, // アニマル運搬モード/初速度
	{ 0x1413c4c50, u8"Animal transport mode/minimum movement speed when turning" }, // アニマル運搬モード/旋回時最低移動速度
	{ 0x1413c4e18, u8"Animal carrying mode/jumping power" }, // アニマル運搬モード/ジャンプ力
	{ 0x1413c4f80, u8"Animal transport mode/gravity size" }, // アニマル運搬モード/重力サイズ
	{ 0x1413c5170, u8"Nitro mode/gauge consumption rate" }, // ニトロモード/ゲージ消費速度
	{ 0x1413c52f8, u8"Nitro mode/maximum air resistance" }, // ニトロモード/最大空気抵抗
	{ 0x1413c5438, u8"Nitro mode/minimum air resistance" }, // ニトロモード/最小空気抵抗
	{ 0x1413c5600, u8"Nitro mode/time pause" }, // ニトロモード/時間一時停止
	{ 0x1413c57b0, u8"Nitro mode/time pause/air boost" }, // ニトロモード/時間一時停止/エアブースト
	{ 0x1413c5930, u8"Nitro mode/camera vibration" }, // ニトロモード/カメラ振動
	{ 0x1413c5a28, u8"Nitro mode/camera vibration/air boost" }, // ニトロモード/カメラ振動/エアブースト
	{ 0x1413c5b58, u8"Nitro mode/controller vibration" }, // ニトロモード/コントローラ振動
	{ 0x1413c5c08, u8"Nitro mode/controller vibration/air boost" }, // ニトロモード/コントローラ振動/エアブースト
	{ 0x1413c5d60, u8"Nitro mode/start delay time of vibration system while running" }, // ニトロモード/走り中の振動系の開始遅延時間
	{ 0x1413c5e98, u8"Nitro mode/camera vibration/driving" }, // ニトロモード/カメラ振動/走行中
	{ 0x1413c5f90, u8"Nitro mode/controller vibration/running" }, // ニトロモード/コントローラ振動/走行中
	{ 0x1413c6098, u8"Nitro mode/default speed" }, // ニトロモード/デフォルト速度
	{ 0x1413c6120, u8"Nitro mode/Nitro activation speed" }, // ニトロモード/ニトロ発動速度
	{ 0x1413c61c0, u8"Nitro mode/default speed/power boost" }, // ニトロモード/デフォルト速度/パワーブースト
	{ 0x1413c6280, u8"Nitro mode/Nitro activation speed/Power boost" }, // ニトロモード/ニトロ発動速度/パワーブースト
	{ 0x141398eb8, u8"Cyber mode" }, // 電脳固有モード
	{ 0x1413a78c0, u8"Y deceleration" }, // Y減速度
	{ 0x1413a8dc0, u8"Maximum horizontal speed" }, // 水平最高速度
	{ 0x1413e085c, u8"Start" }, // 開始
	{ 0x1413a1f50, u8"Initial rise speed" }, // 上昇初速
	{ 0x1413a82c8, u8"Maximum climbing speed" }, // 上昇最高速
	{ 0x1413a84d8, u8"Number of possible climbs" }, // 上昇可能回数
	{ 0x1413a8650, u8"Climb altitude magnification" }, // 上昇高度倍率
	{ 0x1413a6528, u8"Rise" }, // 上昇
	{ 0x1413a7a30, u8"Altitude holding time" }, // 高度維持時間
	{ 0x1413a6564, u8"Cruise" }, // 巡航
	{ 0x1413a7d40, u8"Maximum descent speed" }, // 降下最高速
	{ 0x1413a7f10, u8"Descent acceleration" }, // 降下加速度
	{ 0x1413a6614, u8"Descent" }, // 降下
	{ 0x1413a66e0, u8"Maximum speed (climb/cruise)" }, // 最高速度(上昇・巡航)
	{ 0x1413a6890, u8"Deceleration when no input is applied" }, // 無入力時減速度
	{ 0x1413a6cc0, u8"Altitude upper limit" }, // 高度上限
	{ 0x141399f1c, u8"Flight" }, // 飛行
	{ 0x1413a9ea0, u8"[Mountain] Speed" }, // [山なり]速さ
	{ 0x1413a9f58, u8"[Mountain] Minimum speed when homing" }, // [山なり]ホーミング時最低速度
	{ 0x1413aa098, u8"[Mountain] Maximum speed when homing" }, // [山なり]ホーミング時最高速度
	{ 0x1413aa190, u8"[Mountain] Distance at minimum speed when homing" }, // [山なり]ホーミング時最低速度になる距離
	{ 0x1413aa2f8, u8"[Mountain] Distance at maximum speed when homing" }, // [山なり]ホーミング時最高速度になる距離
	{ 0x1413aa420, u8"[Mountain] Range" }, // [山なり]射程
	{ 0x1413aa540, u8"[Mountain] Height" }, // [山なり]高さ
	{ 0x1413aa698, u8"[Mountain] Minimum height when homing" }, // [山なり]ホーミング時高さ最小
	{ 0x1413aa710, u8"[Mountain] Maximum height when homing" }, // [山なり]ホーミング時高さ最大
	{ 0x1413aa830, u8"[Mountain] Distance at which the height is minimum during homing" }, // [山なり]ホーミング時高さ最小になる距離
	{ 0x1413aa938, u8"[Mountain] Distance at maximum height when homing" }, // [山なり]ホーミング時高さ最大になる距離
	{ 0x1413aaaf0, u8"[Mountain] Model scale" }, // [山なり]モデルスケール
	{ 0x1413aac40, u8"[Mountain] Collision radius" }, // [山なり]コリジョン半径
	{ 0x1413aad00, u8"[Mountain] Rotation speed (°/s)" }, // [山なり]自転速度(°/s)
	{ 0x1413aade8, u8"[Airborne] Initial velocity" }, // [滞空]初速
	{ 0x1413aaec8, u8"[Airborne] Minimum speed when homing" }, // [滞空]ホーミング時最低速度
	{ 0x1413aafc8, u8"[Airborne] Range" }, // [滞空]射程
	{ 0x1413ab040, u8"[Airborne] Y offset when in the air" }, // [滞空]空中時のYオフセット
	{ 0x1413ab0f0, u8"[Airborne] Maximum airborne time" }, // [滞空]最大滞空時間
	{ 0x1413ab1c0, u8"[Airborne] Consecutive hit interval" }, // [滞空]連続ヒット間隔
	{ 0x1413ab258, u8"[Airborne] Model scale" }, // [滞空]モデルスケール
	{ 0x1413ab2e8, u8"[Airborne] Collision radius" }, // [滞空]コリジョン半径
	{ 0x1413ab3c0, u8"[Airborne] [Center] Rotation speed" }, // [滞空][中央]自転速度
	{ 0x1413ab478, u8"[Airborne] [Revolution] Radius of revolution" }, // [滞空][公転]公転半径
	{ 0x1413ab540, u8"[Airborne] [Revolution] Revolution speed" }, // [滞空][公転]公転速度
	{ 0x1413ab610, u8"[Airborne] [Revolution] Rotation speed" }, // [滞空][公転]自転速度
	{ 0x1413a4618, u8"Movement speed addition magnification" }, // 移動速度加算倍率
	{ 0x1413ab7b8, u8"Model expansion time" }, // モデル拡大時間
	{ 0x1413a94d0, u8"Spanner bullet" }, // スパナの弾
	{ 0x1413a98f0, u8"Combo first input wait time" }, // コンボ初回入力待ち時間
	{ 0x1413a99f0, u8"Combo continuation input waiting time" }, // コンボ継続入力待ち時間
	{ 0x1413a9af8, u8"Falling speed during combo" }, // コンボ中落下速度
	{ 0x1413a95b0, u8"Spanner combo" }, // スパナのコンボ
	{ 0x14139a040, u8"Spanner attack" }, // スパナ攻撃
	{ 0x1413a3e94, u8"Range" }, // 射程
	{ 0x1413a42b8, u8"Charge time" }, // タメ時間
	{ 0x1413a43a8, u8"Tame rotation speed (°/s)" }, // タメ回転速度(°/s)
	{ 0x1413a4500, u8"Charge falling speed" }, // タメ落下速度
	{ 0x1413a4720, u8"Will it explode at the end of its life?" }, // 寿命で炸裂するか
	{ 0x1413a4880, u8"Cyloop effect radius" }, // サイループエフェクト半径
	{ 0x1413a4978, u8"Ciroop effect rotation period time" }, // サイループエフェクト回転周期時間
	{ 0x1413a4a98, u8"Cyloop Effect Lifetime" }, // サイループエフェクトライフタイム
	{ 0x1413a2a30, u8"Walking" }, // 歩行
	{ 0x1413f5924, u8"Air" }, // 空中
	{ 0x1413a2c40, u8"Lifting time" }, // 持ち上げにかかる時間
	{ 0x1413a2de8, u8"Lifting position (relative to feet)" }, // 持ち上げ位置(足元からの相対)
	{ 0x1413a2430, u8"Lifted state" }, // 持ち上げ状態
	{ 0x1413a2570, u8"Item drop" }, // アイテムドロップ
	{ 0x14139a0f8, u8"Cyblaster" }, // サイブラスター
	{ 0x1413acad8, u8"Landing attack judgment scale" }, // 着地攻撃判定スケール
	{ 0x14139a1b8, u8"Pile bunker (stomping)" }, // パイルバンカー(ストンピング)
	{ 0x1413a05d0, u8"Horizontal initial velocity" }, // 水平方向初速
	{ 0x1413a0a60, u8"Maximum yaw speed" }, // 最大ヨー速度
	{ 0x1413a55b8, u8"Yaw acceleration" }, // ヨー加速度
	{ 0x1413a5638, u8"Return yaw deceleration" }, // 戻りヨー減速度
	{ 0x1413a1188, u8"Attack judgment scale" }, // 攻撃判定スケール
	{ 0x14139a2b8, u8"Cyclone boost" }, // サイクロンブースト
	{ 0x1413a06f8, u8"Maximum vertical speed" }, // 垂直最高速
	{ 0x1413a07d8, u8"Vertical acceleration" }, // 垂直加速度
	{ 0x1413a08b8, u8"Vertical deceleration" }, // 垂直減速度
	{ 0x1413a0998, u8"Max altitude climb" }, // 上昇高度最大
	{ 0x1413a0e88, u8"Vertical speed at maximum pitch angle" }, // 最大ピッチ角になる垂直速度
	{ 0x1413a0f78, u8"Rolling time at start" }, // 開始時のローリング時間
	{ 0x1413a2010, u8"Minimum altitude climb" }, // 上昇高度最小
	{ 0x1413a13dc, u8"Take off" }, // 離陸
	{ 0x14139a3e0, u8"Cyclone air boost" }, // サイクロンエアブースト
	{ 0x1413ac788, u8"Step distance" }, // ステップ距離
	{ 0x1413ac880, u8"Rolling time" }, // ローリング時間
	{ 0x14139a4e0, u8"Quick Step (Cyclone Boost)" }, // クイックステップ(サイクロンブースト)
	{ 0x1413acdd0, u8"Change direction speed while charging" }, // チャージ中の方向転換速度
	{ 0x1413acec0, u8"Firing duration" }, // 発射持続時間
	{ 0x1413acf68, u8"Change direction speed during launch" }, // 発射中の方向転換速度
	{ 0x1413ad068, u8"Direction change limit (how many degrees on one side)" }, // 方向転換制限(片側何°)
	{ 0x1413ad158, u8"Collision length maximum" }, // コリジョン長さ最大
	{ 0x1413ad308, u8"How quickly collisions grow" }, // コリジョンが伸びる速さ
	{ 0x1413ad420, u8"Consecutive hit interval" }, // 連続ヒット間隔
	{ 0x1413ad578, u8"Hit effect interval" }, // ヒットエフェクト間隔
	{ 0x14139a5c8, u8"Wave Cannon" }, // 波動砲
	{ 0x1413e7fe0, u8"Forward initial velocity" }, // 前方初速
	{ 0x14139fab0, u8"Cyberspace / FV" }, // 電脳空間/FV
	{ 0x14139fb78, u8"Cyberspace / SV" }, // 電脳空間/SV
	{ 0x1413b7768, u8"[sy1030] Occurrence time interval [minutes]" }, // [sy1030]発生時間間隔[分]
	{ 0x1413b7c90, u8"Enable this preset" }, // このプリセットを有効にする
	{ 0x1413b7da8, u8"First target gaze reference position" }, // 第1目標の注視基準位置
	{ 0x1413b7e50, u8"Gaze reference position of second target" }, // 第2目標の注視基準位置
	{ 0x1413b7f08, u8"Position ratio of gaze point" }, // 注視点の位置比率
	{ 0x1413b8010, u8"Initial rotation angle in pitch direction" }, // ピッチ方向の初期回転角度
	{ 0x1413b8120, u8"Initial rotation angle in yaw direction" }, // ヨー方向の初期回転角度
	{ 0x1413b8270, u8"Distance from gaze target to camera" }, // 注視目標からカメラまでの距離
	{ 0x1413b84a8, u8"Point of interest offset" }, // 注視点のオフセットX
	{ 0x1413b8608, u8"Gaze point offset Y" }, // 注視点のオフセットY
	{ 0x1413b8760, u8"Offset Z of the point of interest" }, // 注視点のオフセットZ
	{ 0x1413b91e8, u8"Camera preset table" }, // カメラプリセットテーブル
	{ 0x1413b9ca0, u8"Sensor restart time" }, // センサーの再稼働時間
	{ 0x1413b9d68, u8"Sensor relocation time" }, // センサーの再配置時間
	{ 0x1413b9e78, u8"Survival radius of sound" }, // 音の生存半径
	{ 0x1413b9f80, u8"Sound survival time" }, // 音の生存時間
	{ 0x1413ba070, u8"Sound fade time" }, // 音のフェード時間
	{ 0x1413ba488, u8"Playback start speed" }, // 再生開始速度
	{ 0x1413bac20, u8"Maximum send level per point" }, // 1点あたりの最大センドレベル
	{ 0x1413baf18, u8"Sensor radius" }, // センサー半径
	{ 0x1413bb130, u8"AmbSoundInfo" }, // ambSoundInfo
	{ 0x1413bb1b8, u8"Bass noise" }, // ベースノイズ
	{ 0x1413bb288, u8"Relative wind" }, // 相対風
	{ 0x1413bb328, u8"Insect voice" }, // 虫の声
	{ 0x1413bb3d0, u8"The sound of grass swaying" }, // 草の揺れる音
	{ 0x1413bb460, u8"River [nearest point version]" }, // 川[最近点バージョン]
	{ 0x1413bb4f0, u8"River [Multiple sound source version]" }, // 川[複数音源バージョン]
	{ 0x1413bb9c0, u8"Aisac value" }, // Aisac値
	{ 0x1413cfbd4, u8"Sunny" }, // 晴れ
	{ 0x1413cfc78, u8"Cloudy" }, // 曇り
	{ 0x1413bbcf0, u8"Underwater start" }, // 水中開始
	{ 0x1413bbda0, u8"End underwater [Aisac value is fixed at 0]" }, // 水中終了[Aisac値は0固定]
	{ 0x1413bc810, u8"Cue name" }, // Cue名
	{ 0x1413bc918, u8"BlockID" }, // BlockID
	{ 0x1413bce38, u8"AISAC value fade time" }, // AISAC値フェード時間
	{ 0x1413bcfb0, u8"Adjust the volume according to the input" }, // 入力に応じた音量調整を行う
	{ 0x1413bd1a0, u8"Play multiple times" }, // 重複再生する
	{ 0x1413bd400, u8"Behavior when interrupted by other BGM" }, // 他BGMに割り込まれた時の動作
	{ 0x1413bdfa0, u8"BGM information" }, // BGM情報
	{ 0x1413bf358, u8"BGM number before transition [minimum]" }, // 遷移前のBGM番号[最小]
	{ 0x1413bf468, u8"BGM number before transition [maximum, -1 disables]" }, // 遷移前のBGM番号[最大、-1で無効]
	{ 0x1413bf518, u8"BGM number after transition" }, // 遷移後のBGM番号
	{ 0x1413bf620, u8"[Previous] Stop type" }, // [前]停止タイプ
	{ 0x1413bf6b8, u8"[Previous] Fade out time" }, // [前]フェードアウト時間
	{ 0x1413bf7c8, u8"[Next] Fade-in time" }, // [後]フェードイン時間
	{ 0x1413bf8a0, u8"[Next] Playback delay time" }, // [後]再生遅延時間
	{ 0x1413be1e8, u8"Special settings when switching BGM" }, // BGM切り替え時の特殊設定
	{ 0x1413be9a8, u8"BGM number to play" }, // 再生するBGM番号
	{ 0x1413beb20, u8"Disable playback of the same BGM number until the scene starts playing" }, // シーン再生開始まで同一のBGM番号の再生を無効にする
	{ 0x1413be390, u8"SceneBgmInfo" }, // sceneBgmInfo
	{ 0x1413c0260, u8"Sonic stop" }, // ソニック停止
	{ 0x1413c0368, u8"Sonic slow" }, // ソニック低速
	{ 0x1413c0420, u8"Sonic fast" }, // ソニック高速
	{ 0x1413c0530, u8"Fishing ups and downs start" }, // 釣り浮き沈み開始
	{ 0x1413c05d0, u8"Fishing ups and downs end" }, // 釣り浮き沈み終了
	{ 0x1413c0628, u8"Melody ruins start" }, // メロディー遺跡開始
	{ 0x1413c06c8, u8"Melody ruins end" }, // メロディー遺跡終了
	{ 0x1413c1140, u8"Fade out timing [seconds]" }, // フェードアウトのタイミング[秒]
	{ 0x1413c1260, u8"Unlockable" }, // アンロック対象
	{ 0x1413c1340, u8"Type of song" }, // 曲の種類
	{ 0x1413c17a8, u8"Song transition settings" }, // 楽曲の遷移設定
	{ 0x1413c1898, u8"Custom song information" }, // カスタム楽曲情報
	{ 0x1413c1a00, u8"Time until no operation display starts" }, // 無操作表示を開始するまでの時間
	{ 0x1413c2388, u8"Fade out time at start of pause" }, // ポーズ開始時のフェードアウト時間
	{ 0x1413c2478, u8"Fade-in time at end of pause" }, // ポーズ終了時のフェードイン時間
	{ 0x1413c24f8, u8"Fade-out time when starting to move to cyber space" }, // 電脳空間へ移動開始時のフェードアウト時間
	{ 0x1413c26a0, u8"Fade-out time at the end of Cyber Space Results" }, // 電脳空間リザルト終了時のフェードアウト時間
	{ 0x1413c27c8, u8"BGM volume change" }, // BGM音量変化
	{ 0x1413c28b8, u8"Aisac change" }, // Aisac変化
	{ 0x1413c3fa8, u8"Display type" }, // 表示タイプ
	{ 0x1413c4208, u8"Waiting time after voice ends" }, // ボイス終了後の待ち時間
	{ 0x1413c3634, u8"Conversation" }, // 会話
	{ 0x1413c3748, u8"Can it be displayed even when subtitles are turned off?" }, // 字幕OFFでも表示するか
	{ 0x1413c3268, u8"Conversation data" }, // 会話データ
	{ 0x1413c4848, u8"License notation line spacing" }, // ライセンス表記の行間
	{ 0x1413c4a60, u8"Line spacing in name notation" }, // 名前表記の行間
	{ 0x1413c4c38, u8"Line spacing for title display" }, // 役職表示の行間
	{ 0x1413c4e00, u8"Company display line spacing" }, // 会社表示の行間
	{ 0x1413c4f68, u8"Logo display line spacing" }, // ロゴ表示の行間
	{ 0x1413c5140, u8"Last director display stop time" }, // 最後のディクター表示停止時間
	{ 0x1413c59c0, u8"Camera position" }, // カメラの位置
	{ 0x1413c5f20, u8"Camera settings for each fish" }, // 各魚のカメラ設定
	{ 0x1413c62e8, u8"Wireframe transparency" }, // ワイヤーフレームの透明度
	{ 0x1413c63e8, u8"Fluctuation noise scale" }, // ゆらぎノイズのスケール
	{ 0x1413c6508, u8"Change speed of fluctuation noise" }, // ゆらぎノイズの変化速度
	{ 0x1413c66c8, u8"Vertex movement scale" }, // 頂点の移動スケール
	{ 0x1413c6848, u8"DoF parameters" }, // DoFパラメーター
	{ 0x1413c6ef8, u8"Challenge ID for each grid" }, // グリッドごとのチェレンジID
	{ 0x1413c74a0, u8"Challenge ID" }, // チェレンジID
	{ 0x1413c7b30, u8"Challenge ID for one horizontal column of grid" }, // 横１列分のグリッドのチェレンジID
	{ 0x1413c80c0, u8"Challenge ID two-dimensional array of grid for each island map" }, // 島マップごとのグリッドのチェレンジID二次元配列
	{ 0x1413c81c8, u8"ID display color on editor grid" }, // エディタのグリッド上のID表示色
	{ 0x1413c8688, u8"Stage code" }, // ステージコード
	{ 0x1413c87b8, u8"Play" }, // プレイ
	{ 0x1413be100, u8"Coordinate" }, // 座標
	{ 0x1413c90a8, u8"Play user" }, // プレイユーザー
	{ 0x1413c91b8, u8"Play start time" }, // プレイ開始時刻
	{ 0x1413ca3a8, u8"Best time" }, // 最高タイム
	{ 0x1413ca4c8, u8"High score" }, // ハイスコア
	{ 0x1413c92f8, u8"Summary" }, // サマリ
	{ 0x1413c9a68, u8"Number of enemies defeated" }, // エネミー撃破数
	{ 0x1413c9b20, u8"Number of damage" }, // ダメージ回数
	{ 0x1413c9bb8, u8"Number of mistakes" }, // ミス回数
	{ 0x1413c9c58, u8"Number of retries" }, // リトライ回数
	{ 0x1413c9d20, u8"Number of restarts" }, // リスタート回数
	{ 0x1413c9e08, u8"Play log" }, // プレイログ
	{ 0x1413c93f8, u8"Log" }, // ログ
	{ 0x1413ec9f0, u8"HightScore" }, // hightScore
	{ 0x1413ecaf4, u8"Rank" }, // rank
	{ 0x1413cea88, u8"Padding0" }, // padding0
	{ 0x1413cbdd0, u8"Reserved0" }, // reserved0
	{ 0x1413ca8a8, u8"HightState" }, // hightState
	{ 0x1413d9cb8, u8"Signature" }, // シグニチャ
	{ 0x1413d9e18, u8"Version" }, // バージョン
	{ 0x1413da0b0, u8"Play time" }, // プレイ時間
	{ 0x1413da1b8, u8"Save data icon" }, // セーブデータアイコン
	{ 0x1413da2e0, u8"App version" }, // アプリバージョン
	{ 0x1413da3e0, u8"Config 2" }, // コンフィグ2
	{ 0x1413cbf80, u8"Reserved1" }, // reserved1
	{ 0x1413da5f0, u8"Save date and time" }, // 保存日時
	{ 0x1413e2b98, u8"Index" }, // index
	{ 0x1413caeb0, u8"Reserved" }, // reserved
	{ 0x1413cac40, u8"Header" }, // ヘッダ
	{ 0x1413cacf0, u8"Arcade mode unlocked" }, // アーケードモード解放済み
	{ 0x1413d1480, u8"Bit flag" }, // ビットフラグ
	{ 0x1413cbcb0, u8"Best time" }, // ベストタイム
	{ 0x1413d1698, u8"Bit flag 2" }, // ビットフラグ2
	{ 0x1413d1020, u8"Cyber space stage information" }, // 電脳空間ステージ情報
	{ 0x1413cade0, u8"Cyber space stage container" }, // 電脳空間ステージコンテナ
	{ 0x1413cc0a0, u8"Reserved2" }, // reserved2
	{ 0x1413cc198, u8"Reserved3" }, // reserved3
	{ 0x1413cb878, u8"Phase score" }, // フェイズスコア
	{ 0x1413cb438, u8"Stage" }, // ステージ
	{ 0x1413ccb98, u8"Best rank" }, // ベストランク
	{ 0x1413ccc58, u8"Stage number in order of clearing" }, // クリア順のステージ番号
	{ 0x1413cd460, u8"Cyber challenge" }, // 電脳チャレンジ
	{ 0x1413cd5b8, u8"Battle rush" }, // バトルラッシュ
	{ 0x1413cd680, u8"Battle rush all islands" }, // バトルラッシュ全島
	{ 0x1413ce138, u8"Number of rings" }, // リング数
	{ 0x1413ce240, u8"Ring level" }, // リングレベル
	{ 0x1413ce378, u8"Speed level" }, // スピードレベル
	{ 0x1413ce4c0, u8"Obtain Chaos Emerald" }, // 取得カオスエメラルド
	{ 0x1413ce5e8, u8"Owned Chaos Emerald" }, // 所有カオスエメラルド
	{ 0x1413ce6b0, u8"Skill level" }, // スキルレベル
	{ 0x1413ce810, u8"[obsolete] Possession skill points" }, // [廃止]所持スキルポイント
	{ 0x1413ce958, u8"[obsolete] level" }, // [廃止]レベル
	{ 0x1413cecc0, u8"Power level" }, // チカラレベル
	{ 0x1413cee18, u8"Mamori level" }, // マモリレベル
	{ 0x1413cef38, u8"Boost gauge level" }, // ブーストゲージレベル
	{ 0x1413cf058, u8"Number of seeds of power" }, // チカラのタネ数
	{ 0x1413cf160, u8"Number of Mamori seeds" }, // マモリのタネ数
	{ 0x1413cf250, u8"Number of fishing coins" }, // 釣りコイン数
	{ 0x1413cf380, u8"Number of fishing tokens" }, // 釣りトークン数
	{ 0x1413cf4d0, u8"Fishing gold card number" }, // 釣りゴールドカード数
	{ 0x1413cf688, u8"Meteor shower slot star number" }, // 流星雨スロットスター数
	{ 0x1413cf788, u8"Quick sirloop gauge" }, // クイックサイループゲージ
	{ 0x1413cf868, u8"Maximum number of practice clears" }, // 練習最大クリア数
	{ 0x1413cf990, u8"Maximum number of practice combos" }, // 練習最大コンボ数
	{ 0x1413cfb28, u8"Possession skill points" }, // 所持スキルポイント
	{ 0x1413cfd20, u8"Experience points from DLCRSV (treasure chest 1)" }, // 経験値のうちDLCRSV(宝箱1)によるもの
	{ 0x1413cfe18, u8"Of the number of seeds of power, those from DLCRSV (treasure chest 1)" }, // チカラのタネ数のうちDLCRSV(宝箱1)によるもの
	{ 0x1413cff88, u8"Of the number of Mamori seeds, those from DLCRSV (treasure chest 1)" }, // マモリのタネ数のうちDLCRSV(宝箱1)によるもの
	{ 0x1413d1b60, u8"Friends character data" }, // フレンズキャラクターデータ
	{ 0x1413d2058, u8"Number of rings for Master King Coco Trial" }, // 達人王ココ試練用リング数
	{ 0x1413d20f8, u8"Master King Coco Trial Cyloop Gauge" }, // 達人王ココ試練用サイループゲージ
	{ 0x1413d2218, u8"Master King Coco Trial Flag" }, // 達人王ココ試練用フラグ
	{ 0x1413d2420, u8"Reserved4" }, // reserved4
	{ 0x1413d24c8, u8"Reserved5" }, // reserved5
	{ 0x1413d2628, u8"Reserved6" }, // reserved6
	{ 0x1413d27a8, u8"Reserved7" }, // reserved7
	{ 0x1413d2928, u8"Reserved8" }, // reserved8
	{ 0x1413d2ac8, u8"Reserved9" }, // reserved9
	{ 0x1413d2bf8, u8"Reserved10" }, // reserved10
	{ 0x1413d2d60, u8"Reserved11" }, // reserved11
	{ 0x1413d2e88, u8"Reserved12" }, // reserved12
	{ 0x1413d2f70, u8"Reserved13" }, // reserved13
	{ 0x1413d3038, u8"Reserved14" }, // reserved14
	{ 0x1413d3148, u8"Reserved15" }, // reserved15
	{ 0x1413d3250, u8"Reserved16" }, // reserved16
	{ 0x1413d3358, u8"Reserved17" }, // reserved17
	{ 0x1413d3428, u8"Reserved18" }, // reserved18
	{ 0x1413d3540, u8"Reserved19" }, // reserved19
	{ 0x1413d1cc8, u8"Extended character data" }, // 拡張キャラクターデータ
	{ 0x1413d4598, u8"Byte flag" }, // バイトフラグ
	{ 0x1413d46b0, u8"Uint32 data" }, // uint32データ
	{ 0x1413d3fe0, u8"Flag data World common" }, // フラグデータWorld共通
	{ 0x1413d40a0, u8"Flag data Extra specific" }, // フラグデータExtra固有
	{ 0x1413dbcf8, u8"Map destination" }, // マップ目的地
	{ 0x1413aad68, u8"X" }, // x
	{ 0x1413aad94, u8"Y" }, // y
	{ 0x1413db0b0, u8"Released coordinates" }, // 解放済座標
	{ 0x1413db148, u8"Number of opened grids" }, // 開放済みグリッド数
	{ 0x1413dbe40, u8"Map data" }, // マップデータ
	{ 0x1413d4b68, u8"Island data" }, // 島データ
	{ 0x1413d7a10, u8"Last stage played" }, // 最後にプレイしていたステージ
	{ 0x1413988f0, u8"Sequence" }, // sequence
	{ 0x1413d8310, u8"Seconds" }, // 秒
	{ 0x1413d84c8, u8"Last character played" }, // 最後にプレイしていたキャラクタ
	{ 0x1413d5488, u8"Gameplay" }, // ゲームプレイ
	{ 0x1413d5518, u8"Charactor" }, // キャラクタ
	{ 0x1413d55f8, u8"Island stage container" }, // 島ステージコンテナ
	{ 0x1413d5788, u8"Flag container" }, // フラグコンテナ
	{ 0x1413eef68, u8"Island flag data" }, // 島フラグデータ
	{ 0x1413ef048, u8"Development island flag data" }, // 開発用島フラグデータ
	{ 0x1413d5890, u8"Flag containers by world" }, // ワールド別フラグコンテナ
	{ 0x1413d59e8, u8"Extra flag container" }, // Extraフラグコンテナ
	{ 0x1413d5ac8, u8"Action chain" }, // アクションチェイン
	{ 0x1413d5be0, u8"Extra character container" }, // エキストラキャラクターコンテナ
	{ 0x1413d5d60, u8"Extra Cyber Stage Container" }, // Extra電脳ステージコンテナ
	{ 0x1413d5e78, u8"Extra island stage container" }, // Extra島ステージコンテナ
	{ 0x1413ed2b0, u8"Has it been assigned?" }, // アサイン済みか
	{ 0x1413ecf0c, u8"Statistics" }, // 統計
	{ 0x1413d6110, u8"Statistics container" }, // 統計コンテナ
	{ 0x1413ee180, u8"Current weather" }, // 今の天気
	{ 0x1413ee2a8, u8"Past weather" }, // 過去の天気
	{ 0x1413eecb8, u8"Progress" }, // 進行
	{ 0x1413ee368, u8"Weather transition timer" }, // 天気遷移タイマー
	{ 0x1413ed868, u8"Block end time" }, // ブロック終了時間
	{ 0x1413ed948, u8"End of block" }, // ブロック終了分
	{ 0x1413ed9f0, u8"Block end seconds" }, // ブロック終了秒
	{ 0x1413ee420, u8"Weather block" }, // 天気ブロック
	{ 0x1413ee4c0, u8"Weather record stage number" }, // 天候記録ステージ番号
	{ 0x1413eb258, u8"NumFootMarks" }, // numFootMarks
	{ 0x1413eb318, u8"FootMarksIndex" }, // footMarksIndex
	{ 0x1413eacdc, u8"Time" }, // 時刻
	{ 0x1413eade0, u8"X coordinate" }, // X座標
	{ 0x1413eae70, u8"Z coordinate" }, // Z座標
	{ 0x1413eb428, u8"Footmarks" }, // footmarks
	{ 0x1413eb4d8, u8"NumEvents" }, // numEvents
	{ 0x1413eb528, u8"EventsIndex" }, // eventsIndex
	{ 0x141396718, u8"Event" }, // イベント
	{ 0x1413ec3e0, u8"For event supplements" }, // イベント補足用
	{ 0x1413eb5a4, u8"Events" }, // events
	{ 0x1413dc760, u8"Master volume" }, // マスタボリューム
	{ 0x1413dc868, u8"SE volume" }, // SEボリューム
	{ 0x1413dc948, u8"Music volume" }, // 音楽ボリューム
	{ 0x1413dca70, u8"Voice volume" }, // ボイスボリューム
	{ 0x1413dcbb8, u8"Music selection function ON/OFF" }, // ミュージックセレクト機能のON・OFF
	{ 0x1413dcc78, u8"Playback type of music select function" }, // ミュージックセレクト機能の再生タイプ
	{ 0x1413dce40, u8"Padding" }, // padding
	{ 0x1413dd880, u8"Rotation speed when operating the pad" }, // パッド操作時の回転速度
	{ 0x1413dd9e0, u8"Vertical rotation speed when operating the mouse" }, // マウス操作時の垂直回転速度
	{ 0x1413ddab8, u8"Horizontal rotation speed when operating the mouse" }, // マウス操作時の水平回転速度
	{ 0x1413ddbd0, u8"Aiming speed when operating the pad" }, // パッド操作時の照準速度
	{ 0x1413ddcc0, u8"Vertical aiming speed when operating the mouse" }, // マウス操作時の垂直照準速度
	{ 0x1413ddd48, u8"Horizontal aiming speed when operating the mouse" }, // マウス操作時の水平照準速度
	{ 0x1413ddde8, u8"Rotation speed rate" }, // 回転速度レート
	{ 0x1413dde68, u8"Distance rate" }, // 距離レート
	{ 0x1413ddf28, u8"Angle of view rate" }, // 画角レート
	{ 0x1413e3df0, u8"Gamepad/mapping" }, // ゲームパッド/マッピング
	{ 0x1413e3f58, u8"Keyboard/mapping/main" }, // キーボード/マッピング/メイン
	{ 0x1413e40c0, u8"Keyboard/mapping/sub" }, // キーボード/マッピング/サブ
	{ 0x1413e5e80, u8"Initial velocity rate" }, // 初速レート
	{ 0x1413e5f58, u8"Initial speed rate/boost" }, // 初速レート/ブースト
	{ 0x1413e6088, u8"Acceleration rate" }, // 加速度レート
	{ 0x1413e6148, u8"Maximum speed rate" }, // 最高速度レート
	{ 0x1413e6298, u8"Turning speed rate" }, // 旋回速度レート
	{ 0x1413e6350, u8"Turn speed rate/boost" }, // 旋回速度レート/ブースト
	{ 0x1413e6470, u8"Turning force rate" }, // 旋回力レート
	{ 0x1413e6570, u8"Bump jump gravity rate" }, // バンプジャンプ重力レート
	{ 0x1413e66e8, u8"Spoken language" }, // 音声言語
	{ 0x1413e67d8, u8"Written language" }, // 文字言語
	{ 0x1413e68b8, u8"Vibration intensity" }, // 振動強度
	{ 0x1413e6b00, u8"Pad sensitivity" }, // パッド感度
	{ 0x1413e6be8, u8"Mouse sensitivity" }, // マウス感度
	{ 0x1413e6c58, u8"Neutral stop damping rate" }, // ニュートラル停止減衰率
	{ 0x1413e6d28, u8"[DLC] Sonic costume" }, // [DLC]ソニック衣装
	{ 0x1413e6e58, u8"[DLC] Coco's appearance" }, // [DLC]ココの見た目
	{ 0x1413e6f58, u8"[DLC] Ring acquisition sound" }, // [DLC]リング取得音
	{ 0x1413e7028, u8"[DLC] Config" }, // [DLC]コンフィグ
	{ 0x1413e7228, u8"Appearance of the island" }, // 島の見た目
	{ 0x1413e7310, u8"NPC appearance" }, // NPCの見た目
	{ 0x1413e73f8, u8"Basic air resistance scale rate" }, // 基本空気抵抗スケールレート
	{ 0x1413e4678, u8"Control" }, // コントロール
	{ 0x1413e9108, u8"Resolution" }, // 解像度
	{ 0x1413e9220, u8"Graphic quality" }, // グラフィック品質
	{ 0x1413e92a8, u8"FPS" }, // FPS
	{ 0x1413e93d8, u8"Brightness" }, // Brightness
	{ 0x1413e9510, u8"AA" }, // AA
	{ 0x1413e95ec, u8"AF" }, // AF
	{ 0x1413e8ba4, u8"Bloom" }, // Bloom
	{ 0x1413e9820, u8"HUD display" }, // HUDの表示
	{ 0x1413e9978, u8"Shadow quality" }, // 影の品質
	{ 0x1413e9be0, u8"Rendering scale" }, // レンダリングスケール
	{ 0x1413e4868, u8"Graphics" }, // グラフィックス
};
