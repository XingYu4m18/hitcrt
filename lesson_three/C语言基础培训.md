# 视觉组 C 语言基础培训
---

- 编译的简单原理和过程
- 运行一个C文件或者C工程的环境要求，hello.c为例一个最小C文件的基本内容
- C的基本知识，数据类型（常量、变量） 常用结构（if else） 输入输出(cin printf)  基本运算符
- 多文件编译实践，cmakelist是什么，多文件编译原理
---
- clang配置
- 基本的调试方法（官方方法，无脑打印输出）

---

## 一、比赛中的定位

视觉程序通常先得到许多疑似装甲板，再从中筛选：

1. 这个目标是不是敌方机器人？颜色识别，装甲板数字
2. 置信度够不够高？神经网络识别
3. 距离是否适合当前打击？根据图像距离姿态估计
4. 在多个合格目标中，优先选择哪一个？目标选择，攻击策略
5. 最终把目标位置交给电控或云台 串口通讯 延迟估计

---

## 二、编译的简单原理和过程

### 2.1 四个主要阶段

```text
hello.c ？ hello.cpp
源码
  │  预处理：展开 #include、宏和条件编译
  ▼
hello.i
预处理后的源码
  │  编译：检查语法，把 C 代码翻译成汇编
  ▼
hello.s
汇编代码
  │  汇编：把汇编代码翻译成机器指令目标文件
  ▼
hello.o
目标文件
  │  链接：合并多个 .o，并连接 printf 等库函数
  ▼
hello
可执行文件
```

四个阶段可以概括为：

```text
预处理 → 编译 → 汇编 → 链接
```

平时只需要使用 `gcc hello.c -o hello`，因为 GCC （？G++）  会自动完成这四个阶段。本节不需要记住底层细节，只需知道下面三个结论：

1. **语法错误主要发生在编译阶段**，例如少写分号、把 `int` 拼错。
2. **“声明过但找不到实现”通常发生在链接阶段**，例如头文件里声明了函数，项目却漏写实现它的 `.c` 文件。忘记包含头文件通常在编译阶段就会收到声明缺失的提示。
3. **`.h` 文件通常不单独编译或链接**。它保存类型和函数声明，被 `#include` 展开到包含它的 `.c` 文件中。

## 三、运行一个C文件或者C工程的环境要求，hello.c为例一个最小C文件的基本内容

### 3.1 检查工具

在 Ubuntu 终端运行：

```bash
gcc --version
cmake --version
```

如果提示“未找到命令”，安装基础工具：

```bash
sudo apt update
sudo apt install build-essential cmake
```

创建并进入练习目录：

```bash
mkdir test01
cd ./test01/
```

### 3.2 编写 `hello.c`

```c
#include <stdio.h>

int main(void) {
    printf("Hello Robomaster!\n");
    return 0;
}
```

逐行理解：

- `#include <stdio.h>`：引入标准输入输出函数的声明。这里为了使用 `printf`。
- `int main(void)`：C 程序入口。操作系统启动程序后从这里开始执行。
- `printf`：把文本输出到终端。
- `\n`：换行。
- `return 0;`：返回 0 通常表示程序正常结束。
- 每条普通语句结尾都有分号 `;`。

### 3.3 编译并运行

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic hello.c -o hello
gcc  hello.c -o hello（名字可自定义，表示规范常统一）
./hello
```

输出示例：

```text
Hello RoboMaster！
```

命令参数的含义：

| 参数 | 作用 |
|---|---|
| `-std=c11` | 使用 C11 标准，减少不同环境下的语法差异 |
| `-Wall -Wextra` | 打开常用警告，尽早发现可疑代码 |
| `-Wpedantic` | 提示不符合标准 C 的写法 |
| `-o hello` | 把生成的可执行文件命名为 `hello` |
| `./hello` | 运行当前目录中的 `hello` |

常见小问题：

- 输入 `hello` 后提示找不到命令：Linux 默认不会在当前目录查找程序，要写 `./hello`。
- 提示 `Permission denied`：确认可执行文件由当前用户生成，并检查运行命令。
- 提示找不到 `stdio.h`：编译器或开发环境没有安装完整，不要先怀疑代码。
- 编译没报错就一定有正确输出：检查警告，并实际运行程序；能编译不等于逻辑正确。

---

## 四、C的基本知识

本培训只讲一些常用的基本内容，推荐自行学习一些深入的内容，参见课程[黑马程序员](https://www.bilibili.com/video/BV1Xa4y1k7LU/?spm_id_from=333.788.videopod.episodes&vd_source=6561b949ebef0191d17ef5ccb3df9308&p=9)。

### 4.1 变量和常见类型

```c
int id = 101;
float distance_m = 4.2f;
char command = '1';
```

- `int` 保存整数，如目标编号、置信度。
- `float` 保存小数，如距离。数字后的 `f` 表示这是单精度浮点数。
- `char` 保存一个字符。字符串通常写为 `"hello"`，由多个字符和结尾的 `'\0'` 组成。

### 4.2 条件判断

```c
if (confidence >= 70) {
    printf("target accepted\n");
} else {
    printf("target rejected\n");
}
```

常见比较运算符：

```text
==  等于        !=  不等于
>   大于        <   小于
>=  大于等于    <=  小于等于
```

注意：判断相等使用 `==`，赋值使用 `=`。

### 4.3 循环

明确知道要访问多少项时，使用 `for`：

```c
for (int i = 0; i < count; ++i) {
    printf("%d\n", i);
}
```

需要持续运行、直到某个条件出现时，使用 `while`：

```c
while (running) {
    /* 持续接收新命令或新一帧数据 */
}
```

`break` 立即结束当前循环，`continue` 跳过本次循环剩下的语句，进入下一次迭代。

### 4.4 多分支选择

少量范围判断通常用 `if / else if / else`：

```c
if (distance < 3.0f) {
    /* 近 */
} else if (distance < 6.0f) {
    /* 中 */
} else {
    /* 远 */
}
```

针对一个整数或枚举值的多个固定选项，通常使用 `switch`：

```c
switch (command) {
    case 1:
        /* 执行选项 1 */
        break;
    case 2:
        /* 执行选项 2 */
        break;
    default:
        /* 其他值 */
        break;
}
```

忘记 `break` 会让程序继续执行下一个 `case`，初学者应主动避免。

### 4.5 结构体

一个目标有编号、类型、阵营、距离、置信度等多个属性。如果分别使用五个变量，很快会变乱。结构体允许把这些属性放在一起：

```c
typedef struct {
    int id;
    float distance_m;
    int confidence;
} ExampleTarget;

ExampleTarget target = {101, 4.2f, 92};
printf("id=%d\n", target.id);
```
---

## 五、多文件编译实践，cmakelist是什么，多文件编译原理
### 5.1 编译器、构建工具和 CMake 不是一回事

| 工具 | 主要职责 |
|---|---|
| 编译器，如 `gcc` | 把一个源码文件变成汇编或目标文件，并完成链接 |
| 构建工具，如 `make`、Ninja | 按规则调用编译器，只重建发生变化的文件 |
| CMake | 描述“这个项目要构建什么”，再生成 Makefile 或 Ninja 文件 |

因此，CMake 本身不是 C 编译器。它生成构建文件，构建工具再调用 GCC。

---
### 5.2 需求

假设我方是红方。程序接收一组模拟检测结果，并完成以下规则：

1. 忽略同队目标。
2. 置信度低于 70 的目标不攻击。
3. 距离超过 8 米的敌方目标暂时不攻击。
4. 根据机器人类型、距离和置信度计算优先级。
5. 从合格目标中选出优先级最高的一个。
6. 通过菜单反复查看结果，输入 `0` 退出。

菜单行为：

```text
1：查看原始检测结果
2：查看筛选后的敌方目标
3：查看攻击建议
0：退出
```

### 5.3 工程结构

```text
rm_target_demo/
├── CMakeLists.txt
├── include/
│   └── target.h
└── src/
    ├── main.c
    └── target.c
```

职责划分：

| 文件 | 放什么 | 为什么 |
|---|---|---|
| `target.h` | 类型定义、常量、函数声明 | 让其他 `.c` 文件知道有哪些类型和函数可用 |
| `target.c` | 筛选、评分、打印等函数实现 | 把可复用逻辑和入口分开 |
| `main.c` | 演示数据、菜单和程序入口 | 只负责组织流程 |
| `CMakeLists.txt` | 构建规则 | 告诉 CMake 需要编译哪些源码 |

建立目录：

```bash
mkdir -p rm_target_demo/include rm_target_demo/src
cd rm_target_demo
```

### 5.4 `include/target.h`

```c
#ifndef RM_TARGET_H
#define RM_TARGET_H

#define MAX_TARGETS 8

typedef enum {
    ROBOT_UNKNOWN = 0,
    ROBOT_HERO,
    ROBOT_INFANTRY,
    ROBOT_ENGINEER
} RobotType;

typedef enum {
    TEAM_UNKNOWN = 0,
    TEAM_RED,
    TEAM_BLUE
} TeamType;

typedef struct {
    int id;
    RobotType type;
    TeamType team;
    float distance_m;
    int confidence;
} Target;

typedef struct {
    Target items[MAX_TARGETS];
    int count;
} TargetList;

const char *robot_type_name(RobotType type);
const char *team_name(TeamType team);
void print_target(Target target);
void print_targets(const char *title, TargetList list);
TargetList filter_enemies(TargetList all, TeamType our_team);
int target_priority(Target target);
void print_best_target(TargetList list);

#endif
```

关键点：

- `typedef struct` 定义 `Target` 类型。`Target` 是结构体类型名，某个具体目标则是 `Target` 类型变量。
- `TargetList` 把固定大小数组与有效元素数量 `count` 放在一起，避免到处猜测“数组里真正有几项”。
- `enum` 给机器人类型和阵营起可读的名字，避免到处写难以理解的数字。
- 头文件中的函数没有函数体，只有声明；函数体在 `target.c` 中。
- `#ifndef / #define / #endif` 是头文件保护，避免同一翻译单元重复包含声明。

### 5.5 `src/target.c`

```c
#include "target.h"

#include <stdio.h>

const char *robot_type_name(RobotType type) {
    switch (type) {
        case ROBOT_HERO:
            return "HERO";
        case ROBOT_INFANTRY:
            return "INFANTRY";
        case ROBOT_ENGINEER:
            return "ENGINEER";
        case ROBOT_UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

const char *team_name(TeamType team) {
    switch (team) {
        case TEAM_RED:
            return "RED";
        case TEAM_BLUE:
            return "BLUE";
        case TEAM_UNKNOWN:
        default:
            return "UNKNOWN";
    }
}

void print_target(Target target) {
    printf(
        "#%3d | %-8s | %-4s | %4.1f m | %3d %%\n",
        target.id,
        robot_type_name(target.type),
        team_name(target.team),
        target.distance_m,
        target.confidence
    );
}

void print_targets(const char *title, TargetList list) {
    printf("\n%s (count = %d)\n", title, list.count);
    for (int i = 0; i < list.count; ++i) {
        print_target(list.items[i]);
    }
}

TargetList filter_enemies(TargetList all, TeamType our_team) {
    TargetList result = {0};
    const int min_confidence = 70;
    const float max_distance_m = 8.0f;

    for (int i = 0; i < all.count; ++i) {
        Target target = all.items[i];

        if (target.team == our_team) {
            continue;
        }
        if (target.confidence < min_confidence) {
            continue;
        }
        if (target.distance_m > max_distance_m) {
            continue;
        }
        if (result.count >= MAX_TARGETS) {
            break;
        }

        result.items[result.count] = target;
        result.count++;
    }

    return result;
}

int target_priority(Target target) {
    int priority = 0;

    switch (target.type) {
        case ROBOT_HERO:
            priority = 30;
            break;
        case ROBOT_INFANTRY:
            priority = 20;
            break;
        case ROBOT_ENGINEER:
            priority = 10;
            break;
        case ROBOT_UNKNOWN:
        default:
            priority = 0;
            break;
    }

    if (target.distance_m <= 3.0f) {
        priority += 5;
    } else if (target.distance_m >= 6.0f) {
        priority -= 5;
    } else {
        priority += 0;
    }

    if (target.confidence >= 90) {
        priority += 3;
    }

    return priority;
}

void print_best_target(TargetList list) {
    if (list.count == 0) {
        printf("\nNo attackable target.\n");
        return;
    }

    int best_index = 0;
    int best_score = target_priority(list.items[0]);

    for (int i = 1; i < list.count; ++i) {
        int current_score = target_priority(list.items[i]);
        if (current_score > best_score) {
            best_index = i;
            best_score = current_score;
        }
    }

    printf("\nBest target (priority = %d):\n", best_score);
    print_target(list.items[best_index]);
}
```

这段实现如何串联知识点：

| 知识点 | 出现位置 | 作用 |
|---|---|---|
| `struct` | `Target`、`TargetList` | 把同一目标的数据组织在一起 |
| `enum` | `RobotType`、`TeamType` | 用名字表示有限的状态 |
| `switch` | 类型名称、阵营名称、基础优先级 | 根据固定选项走不同分支 |
| `if / else if / else` | 距离评分 | 按范围决定加分、扣分或不加分 |
| `for` | 遍历目标、寻找最高分 | 对每一项执行相同操作 |
| `continue` | 筛选循环 | 条件不满足时跳过当前目标 |
| `break` | 筛选循环、`switch` | 结束循环，或结束当前分支 |
| 函数 | 名称转换、筛选、评分、打印 | 把大问题拆成小步骤 |

`TargetList` 按值传给函数，是新手阶段最容易理解的写法：函数拿到一份副本，原始数据不会被无意修改。代价是会复制整个结构体；后续 C++ 课会用引用和 `const` 改进。

### 5.6 `src/main.c`

```c
#include "target.h"

#include <stdio.h>

TargetList create_demo_targets(void) {
    TargetList detections = {
        .count = 6,
        .items = {
            {
                .id = 101,
                .type = ROBOT_INFANTRY,
                .team = TEAM_BLUE,
                .distance_m = 4.2f,
                .confidence = 92
            },
            {
                .id = 102,
                .type = ROBOT_HERO,
                .team = TEAM_BLUE,
                .distance_m = 7.3f,
                .confidence = 85
            },
            {
                .id = 103,
                .type = ROBOT_INFANTRY,
                .team = TEAM_RED,
                .distance_m = 2.0f,
                .confidence = 95
            },
            {
                .id = 104,
                .type = ROBOT_ENGINEER,
                .team = TEAM_BLUE,
                .distance_m = 1.5f,
                .confidence = 65
            },
            {
                .id = 105,
                .type = ROBOT_INFANTRY,
                .team = TEAM_BLUE,
                .distance_m = 9.5f,
                .confidence = 78
            },
            {
                .id = 106,
                .type = ROBOT_UNKNOWN,
                .team = TEAM_BLUE,
                .distance_m = 3.0f,
                .confidence = 88
            }
        }
    };

    return detections;
}

void print_menu(void) {
    printf("\n===== RM Target Filter =====\n");
    printf("1. Show all detections\n");
    printf("2. Show filtered enemies\n");
    printf("3. Show best target\n");
    printf("0. Exit\n");
    printf("Choose: ");
}

int main(void) {
    const TeamType our_team = TEAM_RED;
    TargetList all_targets = create_demo_targets();

    printf("Input contains %d simulated detections.\n", all_targets.count);

    while (1) {
        int command = -1;
        print_menu();

        if (scanf("%d", &command) != 1) {
            printf("Invalid input: please enter a number.\n");
            break;
        }

        if (command == 0) {
            break;
        }

        switch (command) {
            case 1:
                print_targets("All detections", all_targets);
                break;
            case 2: {
                TargetList enemies = filter_enemies(all_targets, our_team);
                print_targets("Filtered enemies", enemies);
                break;
            }
            case 3: {
                TargetList enemies = filter_enemies(all_targets, our_team);
                print_best_target(enemies);
                break;
            }
            default:
                printf("Unknown command. Please choose 0, 1, 2, or 3.\n");
                break;
        }
    }

    printf("Program finished.\n");
    return 0;
}
```

`main.c` 的流程：

```text
创建模拟数据
    ↓
while：持续显示菜单
    ↓
if：输入 0 时退出
    ↓
switch：执行 1、2、3 对应操作
    ↓
结束
```

演示数据故意覆盖多种情况：

- `101`、`102`、`106`：敌方、置信度足够、距离合格。
- `103`：同队目标，应被排除。
- `104`：敌方，但置信度不足。
- `105`：敌方，但距离太远。

运行后选择 `2`，应看到 `101`、`102`、`106`。选择 `3`，应选择 `102`，优先级为 25。

### 5.7 `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.16)

project(rm_target_demo LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(rm_target_demo
    src/main.c
    src/target.c
)

target_include_directories(
    rm_target_demo
    PRIVATE
    include
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        rm_target_demo
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
```

逐段理解：

- `project(... LANGUAGES C)`：声明这是一个 C 项目。
- `CMAKE_C_STANDARD 11`：要求使用 C11。
- `add_executable(...)`：可执行文件名为 `rm_target_demo`，由两个 `.c` 文件共同构建。
- `target_include_directories(...)`：让源码中的 `#include "target.h"` 能找到 `include/` 下的头文件。
- `target_compile_options(...)`：启用与命令行第一个例子相同的警告。

注意这里列出了 `.c` 文件，没有列出 `.h` 文件。`.h` 会由源码中的 `#include` 使用，并不是独立编译单元。

### 5.8 用 CMake 构建和运行

在 `rm_target_demo/` 目录运行：

```bash
mkdir build && cd build
cmake .. &&  make -j8
./build/rm_target_demo
```

三条命令的作用：

| 命令 | 作用 |
|---|---|
| `cmake ..` | 以当前目录 .. 为源码目录，在 `build/` 中生成构建文件 |
| `make -j8 ` | 使用生成的构建文件调用编译器 |
| `./build/rm_target_demo` | 运行编译产物 |



也可以不用 CMake，手动编译这两个源文件：

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic \
    -Iinclude \
    src/main.c src/target.c \
    -o rm_target_demo
./rm_target_demo
```

`-Iinclude` 告诉编译器额外到 `include/` 目录寻找头文件。CMake 只是把这种构建命令组织起来，并没有改变 C 的编译原理。

> **讲师提示：** 建议现场故意从 `add_executable` 中删掉 `src/target.c`，观察“某个函数未定义”的链接错误，再恢复。借此区分“找不到头文件”和“链接找不到函数实现”。

---

## 六、一次完整演示

启动：

```text
Input contains 6 simulated detections.

===== RM Target Filter =====
1. Show all detections
2. Show filtered enemies
3. Show best target
0. Exit
Choose:
```

选择 `2`：

```text
Filtered enemies (count = 3)
#101 | INFANTRY | BLUE |  4.2 m |  92 %
#102 | HERO     | BLUE |  7.3 m |  85 %
#106 | UNKNOWN  | BLUE |  3.0 m |  88 %
```

选择 `3`：

```text
Best target (priority = 25):
#102 | HERO     | BLUE |  7.3 m |  85 %
```

计算过程：

| 目标 | 类型分 | 距离调整 | 置信度调整 | 最终优先级 |
|---|---:|---:|---:|---:|
| 101 INFANTRY | 20 | 0 | +3 | 23 |
| 102 HERO | 30 | -5 | 0 | 25 |
| 106 UNKNOWN | 0 | +5 | 0 | 5 |

> **比赛提醒：** 这里的分值和阈值只是教学示例，不能直接用于比赛。真实策略需要结合赛场规则、云台响应、目标稳定性和弹道约束，由算法与电控共同确定。

---


1. 从 `.c` 到可执行文件经过哪些主要阶段？
2. `.h` 文件和 `.c` 文件分别放什么？
3. 编译错误和链接错误分别是什么表现？
4. CMake、构建工具、编译器分别负责什么？
5. `if / else if / else`、`for`、`while`、`switch` 在本项目中分别解决什么问题？
6. `struct` 为什么适合表示一个检测目标？
7. `switch` 中忘记 `break` 会发生什么？
8. 为什么筛选函数要先判断同队、置信度和距离，再保存目标？

---

## 七、作业

### 扩展目标筛选器：加入哨兵机器人

在“装甲板候选目标筛选器”的基础上完成一次扩展：

1. 新增机器人类型 `ROBOT_SENTRY`，并让名称输出显示为 `SENTRY`。
2. 在模拟数据中新增两个蓝色哨兵目标：
   - 一个满足当前筛选条件。
   - 一个不满足当前筛选条件。
3. 为哨兵设置一个合理的类型优先级，并说明理由。
4. 在菜单中新增选项 `4`：输出筛选统计，包括：
   - 原始目标总数
   - 筛选后目标数量
   - 被排除的同队目标数量
   - 被排除的低置信度目标数量
   - 被排除的超距离目标数量
5. 重新使用 CMake 构建，确保：
   - 编译成功。
   - `0`、`1`、`2`、`3`、`4` 和未知编号都能得到预期反馈。

提交内容：

- 修改后的 `target.h`、`target.c`、`main.c`。
- `CMakeLists.txt`。
- 一张运行选择 `4` 后的终端截图。
- 用 3～5 句话说明新增哨兵优先级的原因。

提示：统计时可在 `filter_enemies` 中增加记录排除原因的变量，也可以通过单独遍历数据完成。不要为了完成统计而复制大段筛选规则；先思考怎样让统计结果和实际筛选使用同一套条件。

---
