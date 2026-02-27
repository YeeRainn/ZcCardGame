# ZcCardGame - 展聪的纸牌游戏

本项目是一个使用 Cocos2d-x 3.17 开发的纸牌游戏，实现了点击匹配、翻牌替换、备用牌堆抽牌以及回退等功能。

## 编译环境要求

- **操作系统**：Windows 7/10/11
- **开发工具**：Visual Studio 2022（Windows）
- **游戏引擎**：Cocos2d-x 3.17
- **语言**：C++

## 编译步骤  --请严格按照以下步骤操作，以确保项目能成功编译。

### 1. 下载 Cocos2d-x 3.17 引擎
- 从 [Cocos2d-x 官方官网](https://www.cocos.com/cocos2dx-download) 下载 `3.17版本`。
- 解压到一个**英文路径**，例如 `D:\cocos2d-x-3.17`。

### 2. 用 Visual Studio 2022 打开并编译
- 进入 `ZcCardGame\proj.win32` 文件夹，双击 `ZcCardGame.sln`运行。
- 使用 VS2022时，首次打开会提示重定目标，点击“确定”即可。

### 3. 本项目使用Git LFS管理大文件
- 请先在Git Bash 窗口中执行 git lfs install 且成功后再进行克隆，否则"展聪-游戏演示视频.mp4"文件会克隆失败。
- 如果感觉用Git Bash 下载"展聪-游戏演示视频.mp4"过于缓慢，可能需要三个小时下载时间，也可以直接在GitHub网页上点击Download直接下载视频

## 注意事项
- 本项目使用的中文字体文件位于 `Resources/fonts/arial.ttf`，如果缺失可能导致按钮文字不显示，请确保该文件存在。