# EditorExtensionDemo (UE 5.7)

一个基于 Unreal Engine 5.7 的编辑器扩展示例工程，覆盖常见的 Editor 开发场景：

- 菜单与工具栏扩展
- Details 面板自定义
- 自定义资产（`UMyCustomAsset`）与资产编辑器（`FAssetEditorToolkit` + Slate）
- 自定义编辑器模式（`FEdMode` + Toolkit）
- 自动化验收测试（Automation Tests）

本项目已经完成可编译、可启动、可验收的闭环，适合用作 UE 编辑器扩展开发模板。

---

## 功能清单

### 1) 菜单与工具栏扩展

- `Tools` 主菜单新增分组 `Editor Extension Demo`
- 新增菜单项：
  - `Editor Extension Demo`（触发通知）
  - `Activate My Editor Mode`（激活自定义编辑器模式）
- Level Editor 工具栏新增按钮 `Ext Demo`

核心实现：
- `Source/EditorExtensionDemoEditor/EditorExtensionDemoEditor.cpp`
- `Source/EditorExtensionDemoEditor/MenuToolbarExtension/EditorCommands.*`

### 2) Details 面板自定义

对 `AMyCustomActor` 注册自定义 Details 布局，演示：
- 自定义 Row 与分组
- 自定义按钮/复选框/数值控件
- 属性与交互逻辑联动

核心实现：
- `Source/EditorExtensionDemo/CustomAsset/MyCustomActor.*`
- `Source/EditorExtensionDemoEditor/DetailsPanelExtension/MyCustomActorDetails.*`

### 3) 自定义资产 + 资产编辑器

新增资产类型 `Custom Asset`（`UMyCustomAsset`）并打通完整链路：
- 通过 `Factory` 出现在 Content Browser 的创建菜单中
- 注册 `AssetTypeActions` 到高级分类 `Editor Extension Demo`
- 双击资产打开自定义编辑器（多 Tab + Details + Slate 交互）

自定义编辑器包含：
- `Properties`：属性面板
- `Viewport`：`SMyAssetEditorWidget` 预览与操作区
- `Settings`：重置按钮（恢复默认值）

核心实现：
- `Source/EditorExtensionDemo/CustomAsset/MyCustomAsset.*`
- `Source/EditorExtensionDemoEditor/AssetEditorExtension/MyCustomAssetFactory.*`
- `Source/EditorExtensionDemoEditor/AssetEditorExtension/MyCustomAssetTypeActions.*`
- `Source/EditorExtensionDemoEditor/AssetEditorExtension/MyCustomAssetEditor.*`
- `Source/EditorExtensionDemoEditor/AssetEditorExtension/SMyAssetEditorWidget.*`

### 4) 自定义编辑器模式（Editor Mode）

注册 `My Editor Mode`，支持：
- 菜单激活模式
- 模式 Toolkit 面板
- 视口点击交互与 HUD 信息显示
- ESC 重置状态

已修复历史崩溃问题：`FModeToolkit::Init` 空 `ToolkitHost` 断言。

核心实现：
- `Source/EditorExtensionDemoEditor/EditorModeExtension/MyEditorMode.*`
- `Source/EditorExtensionDemoEditor/EditorModeExtension/MyEditorModeToolkit.*`
- `Source/EditorExtensionDemoEditor/EditorModeExtension/MyEditorModeCommands.*`

---

## 环境要求

- Windows 10/11
- Unreal Engine 5.7（源码版或可用二进制版）
- Visual Studio 2022（包含 C++ 桌面开发组件）

> 说明：项目当前在本地环境中将 `ResonanceAudio` 设为禁用，避免缺少运行模块导致启动失败。

---

## 快速开始

### 1) 编译

```cmd
"G:\workspace\repo\github\UnrealEngine\Engine\Build\BatchFiles\Build.bat" EditorExtensionDemoEditor Win64 Development "G:\workspace\ai\UE5Plugin\EditorExtensionDemo\EditorExtensionDemo.uproject" -WaitMutex -NoHotReloadFromIDE
```

### 2) 启动编辑器

```cmd
"G:\workspace\repo\github\UnrealEngine\Engine\Binaries\Win64\UnrealEditor.exe" "G:\workspace\ai\UE5Plugin\EditorExtensionDemo\EditorExtensionDemo.uproject" -log
```

### 3) 自动化验收

```cmd
"G:\workspace\repo\github\UnrealEngine\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "G:\workspace\ai\UE5Plugin\EditorExtensionDemo\EditorExtensionDemo.uproject" -unattended -nop4 -nosplash -nullrhi -NoSound -log -ExecCmds="Automation RunTests EditorExtensionDemo" -TestExit="Automation Test Queue Empty"
```

---

## 验收项（手工）

启动后按下面清单验收：

1. `Tools` 菜单下存在 `Editor Extension Demo` 分组与命令。
2. 工具栏存在 `Ext Demo` 按钮。
3. `My Editor Mode` 可通过 `Tools -> Activate My Editor Mode` 激活且不崩溃。
4. Content Browser `Add` 菜单中可创建 `Custom Asset`。
5. 双击 `Custom Asset` 可打开自定义资产编辑器并交互。
6. 选中 `AMyCustomActor` 时可见自定义 Details UI。

---

## 自动化测试覆盖

测试文件：
- `Source/EditorExtensionDemoEditor/EditorExtensionDemoEditorTests.cpp`

当前覆盖项：
- `EditorExtensionDemo.Menu.ToolsEntryExists`
- `EditorExtensionDemo.Toolbar.CommandRegistered`
- `EditorExtensionDemo.Mode.RegistryContainsMode`
- `EditorExtensionDemo.Asset.FactoryCanCreateAsset`
- `EditorExtensionDemo.Details.CustomLayoutRegistered`
- `EditorExtensionDemo.Asset.TypeActionsRegistered`

---

## 目录结构（核心）

```text
EditorExtensionDemo/
├─ Config/
├─ Content/
├─ Source/
│  ├─ EditorExtensionDemo/                      # Runtime 模块
│  │  └─ CustomAsset/                           # AMyCustomActor, UMyCustomAsset
│  └─ EditorExtensionDemoEditor/                # Editor 模块
│     ├─ MenuToolbarExtension/                  # 菜单/工具栏扩展
│     ├─ DetailsPanelExtension/                 # Details 自定义
│     ├─ AssetEditorExtension/                  # 资产工厂/TypeActions/资产编辑器
│     ├─ EditorModeExtension/                   # 自定义编辑器模式
│     └─ EditorExtensionDemoEditorTests.cpp     # 自动化测试
├─ EditorExtensionDemo.uproject
└─ README.md
```

---

## 已知限制与后续建议

- `ResonanceAudio` 当前按环境约束禁用；若后续补齐模块可重新启用并复测。
- 示例以演示 Editor 扩展链路为主，生产化建议补充：
  - 更严格的错误处理和日志分级
  - 更多资产编辑器命令与 Undo/Redo 支持
  - 自动化测试拆分到 Smoke/Regression 分层

