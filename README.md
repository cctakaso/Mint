# Mint Project readme
Copyright (c) 2025 Takashi Ogisawa
last update  2025/12/09

## 概要
Mintは、Microsoft Foundation Class (MFC)を使用して開発されたWindowsアプリケーションプロジェクトです。このリポジトリには、アプリケーションのソースコード、リソース、および関連する設定ファイルが含まれています。

## プログラム配布
動作プログラムは、下記URLのVectorサイトで配布しています。
* https://www.vector.co.jp/soft/win95/writing/se482621.html


## 特徴・機能
Mintの特徴は、速い・簡単！結局エディタの善し悪しは、基本性能じゃないかなと私は考えてます。
基本機能はしっかりしながら、高度な事も簡単に操作できるタブファイルセレクター付きで、しかも縦書きもできるシンプルエディタです。

1. 軽快な入力とシンプルな操作性
2. 巨大ファイルもロケットスタート表示
3. 堅実な禁則処理と英文ワードラップ機能
4. ワード単位のアンデゥ・リデゥ機能
5. ダイナミック　ズームイン／ズームアウト機能
6. ＩＭＥインカーソル変換 ※インライン変換よりスムーズに編集できるよ。
7. 縦書きサポート
8. フリーカーソルサポート
9. ＳＤＩ形式で複数ファイルサポート
10. タブツールバーでの、文書ウィンドウ一発切替機能
11. 箇条書き自動インデント機能
12. 複数設定及びその瞬時切替え機能
13. グループ・キーワード登録編集機能
14. 書式設定可能な短文登録と入力変換機能
15. 縦横書き印刷・印刷プレビュー機能

## スクリーンショット
![横書きショット](https://bit.ly/3MnBqKs)

![縦書きショット](https://bit.ly/4oL3rJn)

## 特徴
- **プラットフォーム**: Windows (Win32)
- **開発環境**: Visual Studio
- **使用技術**: MFC (Microsoft Foundation Class)
- **ビルド構成**:
  - Debug
  - Release
  - Free Debug
  - Free Release
  - Green Debug
  - Green Release

## 必要条件
- **開発ツール**: Visual Studio 2026最新（2019以降OK）
- **プラットフォームツールセット**: v145
- **依存プロジェクト**:
  - [MDFC](../MDFC/Mdfc.vcxproj)
  - [UnKanj](../Unkanj/UnKanj.vcxproj)

## ビルド手順
1. Visual Studioで`Mint.sln`を開きます。
2. 使用するビルド構成（例: Debug, Release）を選択します。
3. ソリューションをビルドします。

## ディレクトリ構成
```
Mint/
 ├── .vs/ # Visual Studio関連ファイル（無視されます）
 ├── Debug/ # デバッグビルド出力（無視されます）
 ├── Release/ # リリースビルド出力（無視されます）
 ├── res/ # リソースファイル
 ├── Mint.vcxproj # プロジェクトファイル
 ├── action.cpp
 ├── action.h
 ├── ...
 └── README.md
```

## 無視ファイル
このプロジェクトでは、`.gitignore`を使用して以下のファイルやディレクトリをGit管理から除外しています。

### Visual Studio / MSVC / MFC関連
- `.vs/`
- `*.suo`
- `*.user`
- `*.userosscache`
- `*.sln.docstates`

### IntelliSense / ブラウズ情報
- `*.VC.db`
- `*.VC.opendb`
- `*.sdf`
- `*.ipch/`
- `*.opensdf`

### ビルド成果物
- `**/Debug/`
- `**/Release/`
- `**/Free_Debug/`
- `**/Free_Release/`
- `**/Green_Debug/`
- `**/Green_Release/`
- `**/build/`
- `**/bin/`
- `**/obj/`
- `**/ipch/`

### コンパイラ / リンカ生成物
- `*.obj`
- `*.o`
- `*.lib`

## ライセンス
## License
This project is licensed under the zlib License - see the [LICENSE](LICENSE) file for details.
