# MR Summary - EditorExtensionDemo (UE5.7)

## 背景
本 MR 用于将 `EditorExtensionDemo` 从“代码骨架可编译”推进到“可启动、可验收、可回归验证”的交付状态。

## 核心改动
1. 构建与兼容性修复
- Target 升级为 `BuildSettingsVersion.V6` 与 `EngineIncludeOrderVersion.Unreal5_7`
- 修复 Editor/Runtime 头文件与模块依赖问题
- 修复 `AMyCustomActor` 反射属性重名问题

2. 启动与配置修复
- 修复 `ProjectID`、`DefaultGraphicsRHI`、`ActiveClassRedirects` 配置
- 创建缺失 `Content/` 目录
- `.uproject` 中禁用 `ResonanceAudio`（本机模块缺失导致启动失败）

3. 功能可用性补齐
- 新增 `UMyCustomAssetFactory`，打通 `Custom Asset` 创建链路
- 资产分类改为显式注册高级分类，并传入 `TypeActions`
- 增强可发现性：`Tools` 菜单新增 `Activate My Editor Mode`
- 调整编辑器模式优先级，提升在模式列表中的可见性

4. 模式崩溃修复
- 修复 `MyEditorMode` 激活断言崩溃（`FModeToolkit::Init` 传入空 `ToolkitHost`）
- 改为在 `Enter()` 中基于有效 host 初始化 toolkit，并在 `Exit()` 中正确关闭释放

5. 自动化测试
- 新增 `EditorExtensionDemo` 编辑器自动化测试集，覆盖：
  - 菜单项注册
  - 工具栏命令注册
  - Details 自定义注册
  - 编辑器模式注册
  - Custom Asset 工厂创建
  - AssetTypeActions 注册

## 关键文件
- `Source/EditorExtensionDemo.Target.cs`
- `Source/EditorExtensionDemoEditor.Target.cs`
- `Config/DefaultEngine.ini`
- `Config/DefaultGame.ini`
- `EditorExtensionDemo.uproject`
- `Source/EditorExtensionDemoEditor/EditorExtensionDemoEditor.cpp`
- `Source/EditorExtensionDemoEditor/AssetEditorExtension/MyCustomAssetFactory.h`
- `Source/EditorExtensionDemoEditor/AssetEditorExtension/MyCustomAssetFactory.cpp`
- `Source/EditorExtensionDemoEditor/AssetEditorExtension/MyCustomAssetTypeActions.h`
- `Source/EditorExtensionDemoEditor/AssetEditorExtension/MyCustomAssetTypeActions.cpp`
- `Source/EditorExtensionDemoEditor/EditorModeExtension/MyEditorMode.cpp`
- `Source/EditorExtensionDemoEditor/EditorExtensionDemoEditorTests.cpp`

## 验证
1. 编译
```cmd
"G:\workspace\repo\github\UnrealEngine\Engine\Build\BatchFiles\Build.bat" EditorExtensionDemoEditor Win64 Development "G:\workspace\ai\UE5Plugin\EditorExtensionDemo\EditorExtensionDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

2. 自动化测试（6/6 成功）
```cmd
"G:\workspace\repo\github\UnrealEngine\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "G:\workspace\ai\UE5Plugin\EditorExtensionDemo\EditorExtensionDemo.uproject" -unattended -nop4 -nosplash -nullrhi -NoSound -log -ExecCmds="Automation RunTests EditorExtensionDemo" -TestExit="Automation Test Queue Empty"
```

## 风险与备注
- `Tools -> Activate My Editor Mode` 为增强入口，不影响既有模式系统。
- `ResonanceAudio` 当前被禁用是环境约束；如后续补齐该插件运行模块，可再启用并复测启动。
