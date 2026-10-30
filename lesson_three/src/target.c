#include "target.h"
#include <stdio.h>

/* 枚举在内部适合保存和比较，显示给用户时则需要转换成字符串。 */
const char *robot_type_name(RobotType type) {
    switch (type) {
        case ROBOT_HERO:
            return "HERO";
        case ROBOT_INFANTRY:
            return "INFANTRY";
        case ROBOT_ENGINEER:
            return "ENGINEER";
        case ROBOT_SENTRY:
            return "SENTRY";
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

/*
 * printf 的格式串依次对应：编号、类型、阵营、距离、置信度。
 * %-8s 表示字符串左对齐并至少占 8 列，%4.1f 表示保留一位小数。
 */
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

/* 只遍历有效区间 [0, list.count)，避免读取 items 中尚未赋值的元素。 */
void print_targets(const char *title, TargetList list) {
    printf("\n%s (count = %d)\n", title, list.count);
    for (int i = 0; i < list.count; ++i) {
        print_target(list.items[i]);
    }
}

/*
 * 返回一个全新的敌方目标列表。以下条件必须同时满足：
 * 1. 不是队友；2. 置信度不低于 70；3. 距离不超过 8 米。
 */
TargetList filter_enemies(TargetList all, TeamType our_team,int *ours,int *low_confidence,int *large_distance) {
    /* {0} 将结果数组清零，并把 count 初始化为 0。 */
    TargetList result = {0};
    const int min_confidence = 70;
    const float max_distance_m = 8.0f;
    if(ours != NULL) {
        *ours =0;
    }
    if(low_confidence != NULL) {
        *low_confidence = 0;
    }
    if(large_distance != NULL) {
        *large_distance = 0;
    }

    for (int i = 0; i < all.count; ++i) {
        Target target = all.items[i];

        /* 当前目标只要不满足任一过滤条件，就跳过并检查下一个目标。 */
        if (target.team == our_team) {
        if(ours != NULL) {
            (*ours)++;
        }
            continue;
        }
        if (target.confidence < min_confidence) {
        if(low_confidence != NULL) {
            (*low_confidence)++;
        }
            continue;
        }
        if (target.distance_m > max_distance_m) {
        if(large_distance != NULL) {
            (*large_distance)++;
        }
            continue;
        }
        if (result.count >= MAX_TARGETS) {
            break;
        }

        /* 通过全部条件后，把目标追加到 result 的有效区间末尾。 */
        result.items[result.count] = target;
        result.count++;
    }

    return result;
}

/*
 * 优先级分数由两部分组成：
 * - 基础分取决于类型：英雄 30、步兵 20、工程 10；
 * - 近处更容易攻击，远处更难攻击，高置信度也会增加少量分数。
 */
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
        case ROBOT_SENTRY:
            priority = 25;
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

    /* 先假设第 0 个目标最好，再逐个比较并记录当前最高分及其下标。 */
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
