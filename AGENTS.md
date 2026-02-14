# RP2040-Thermohygrometer 開発

## 概要

RP2040-Thermohygrometerは、RP2040マイコンを使用した温湿度・気圧計のプロジェクトです。
このドキュメントには AI Coding Agent を使用して効率的にアプリケーションの保守を行うために必要な情報を記載します。

## 保守対象の仕様

### 機能仕様

- AHT20 から温度・湿度を取得し、0.1単位の `int16_t` で扱う
- BMP280 から気圧を取得し、0.1hPa単位の `int16_t` で扱う
- OLED (SSD1306, 128x64, I2C) に温湿度・気圧を表示する
- 表示モードは以下の5種類
  - 全チャート
  - 温度チャート
  - 湿度チャート
  - 気圧チャート
  - 全テキスト
- ボタン操作
  - 短押し: 表示モード切り替え
  - 長押し: 画面上下反転
- 起動時にNeoPixelを3回点滅して状態を通知

### ハードウェア設定

- RGB LED ピン: `16`
- ボタン: `29`
- 表示I2Cアドレス: `0x3C`
- 表示解像度: `128x64`
- 測定間隔: `3000ms`
- グラフ横ステップ: `HORIZONTAL_STEP=1`

### センサー値の扱い

- 単位は 0.1（例: `253` は `25.3`）
- 無効値は `INT16_MIN`（`INVALID_SENSOR_VALUE`）
- `Model` は無効値をそのまま保持し、`View` 側で `--.-` などの代替表示を行う

## ソフトウェア構造

### ファイル構成と責務

- `RP2040-Thermohygrometer.ino`
  - 依存オブジェクト生成
  - `setup()/loop()` 制御
  - ボタンイベント/測定完了イベントに応じた再描画
- `SensorManager.h/.cpp`
  - AHT20/BMP280 の周期読み取り
  - 状態機械: `IDLE` / `READING`
- `SensorDataHistory.h/.cpp`
  - 固定長履歴バッファ（先頭追加）
  - 最小/最大値計算
- `Model.h/.cpp`
  - 温度/湿度/気圧と履歴へのアクセス抽象
- `View.h/.cpp`
  - 5表示モードの描画
  - 画面反転・モード切替

### レイヤ構造

- Application: `RP2040-Thermohygrometer.ino`
- Domain/UI: `Model`, `View`, `SensorDataHistory`
- Device service: `SensorManager`
- Device driver/library: Adafruit AHTX0/BMP280/SSD1306/NeoPixel, DigitalButton

依存方向は上位から下位のみを維持し、下位層から上位層を参照しないこと。

## 実行フロー

### setup

1. NeoPixel初期化・起動点滅
2. `Serial.begin(115200)` と起動ログ出力
3. ボタン初期化
4. `sensorManager.begin()` でセンサー初期化
5. `model.begin()` / `view.begin(0x3C)`

### loop

1. `button.update()`
2. `sensorManager.update()`
3. 長押しなら `view.flip()`
4. クリックなら `view.switchToNextViewMode()`
5. `sensorManager.isReady()` が true なら `getSensorData()` で反映
6. 変更があれば `view.render()`
7. `delay(10)`

## 各モジュールの保守ポイント

### SensorManager

- `begin()` で AHT20/BMP280 の `begin()` を実行
- 読み取り失敗時はログ出力し、直前値または無効値を保持
- `isReady()` は結果取得フラグを参照のみ（非破壊）
- `consumeReady()` は結果取得フラグを消費する（1回だけ true）

### SensorDataHistory

- `prepend()` は先頭挿入のため、既存値を後方にシフト
- `getMinMaxValue()` は有効値のみ評価

### View

- 表示モード初期値は `VIEW_MODE_ALL_CHARTS`
- 温度単位 `C` は小円 + `C` で描画
- 履歴グラフは最小/最大値で正規化し、レンジが0近傍でも描画可能なよう `range >= 1` を保証
- グラフ描画は `horizontalStep` を直接使用する

### センサードライバ

- AHT20/BMP280 は Adafruit ライブラリを利用する
- `SensorManager` がライブラリ呼び出しを一元管理し、`Model`/`View` に生データ型を漏らさない

### デバッグ出力

- デバッグ出力は `Serial` クラスを直接使用する
- 通常運用時は不要な連続ログを避ける

### プラットフォーム制約

- RP2040ターゲットでは `Serial.begin()` が有効で、起動ログ出力に使用している
- UF2デプロイは BOOTSEL 接続と配置先ドライブ状態に依存する

## ビルド・書き込み・検証

### 主要コマンド

- 依存導入: `make install`
- ビルド: `make build/rp2040-zero`
- 書き込み: `make deploy/rp2040-zero`

### Makefile上の重要設定

- `PROJECT=RP2040-Thermohygrometer`
- `BOARDS=rp2040-zero`
- `CORES=rp2040:rp2040`
- 依存ライブラリ: Adafruit AHTX0 / BMP280 / SSD1306 / NeoPixel / DigitalButton
- `deploy/rp2040-zero` は UF2 配置（`robocopy`）を使用

### arduino-cli 設定

- `arduino-cli.yaml` は RP2040 ボードマネージャURLを使用
- `default_fqbn` は `rp2040:rp2040:rpipico`

### 検証手順

1. `make build/rp2040-zero` を実行してコンパイル確認
2. 必要に応じて `make deploy/rp2040-zero` でUF2配置
3. 実機確認
   - 起動時にLED点滅
   - ボタン短押しで表示切替
   - ボタン長押しで反転
   - 温湿度/気圧が更新される

## 変更ルール（AI Coding Agent向け）

- 既存公開API・ファイル構成・定数名を維持する
- 新規機能追加より、既存機能の安定保守を優先する
- 変更は最小差分で行う
- ピン配置・表示サイズ・測定間隔を変更する場合は理由を明記する
- 変更後は必ず `make build/rp2040-zero` を実行して確認する
- `Makefile` など共通化対象を変更する場合は、他プロジェクト/テンプレートへの展開要否を事前確認する
