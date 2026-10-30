#include "target.h"

#include <stdio.h>

/*
 * 创建一组固定的模拟检测结果，相当于暂时替代真实视觉识别模块的输出。
 * 这里故意包含队友、低置信度目标和过远目标，方便观察后续过滤效果。
 */
TargetList create_demo_targets(void) {
    TargetList detections = {
        .count = 8,
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
            },
            {
                .id = 107,
                .type = ROBOT_SENTRY,
                .team = TEAM_RED,
                .distance_m = 4.0f,
                .confidence = 60
            },
            {
                .id = 108,
                .type = ROBOT_SENTRY,
                .team = TEAM_BLUE,
                .distance_m = 2.0f,
                .confidence = 90
            }
        }
    };

    return detections;
}

/* 每次循环都显示选项，因此用户完成一次查询后可以继续选择其他操作。 */
void print_menu(void) {
    printf("\n===== RM Target Filter =====\n");
    printf("1. Show all detections\n");
    printf("2. Show filtered enemies\n");
    printf("3. Show best target\n");
    printf("4. Show more details\n");
    printf("0. Exit\n");
    printf("Choose: ");
}

int main(void) {
    /* 假设本机属于红方；修改此值即可从蓝方视角观察过滤结果。 */
    const TeamType our_team = TEAM_RED;
    TargetList all_targets = create_demo_targets();

    printf("Input contains %d simulated detections.\n", all_targets.count);

    while (1) {
        int command = -1;
        print_menu();

        /* scanf 返回成功读取并转换的参数个数；它不是 1 时说明输入不是整数。 */
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
                TargetList enemies = filter_enemies(all_targets, our_team, NULL, NULL, NULL);
                print_targets("Filtered enemies", enemies);
                break;
            }
            case 3: {
                /* 先使用同样的规则得到可攻击目标，再从结果中选出最优目标。 */
                TargetList enemies = filter_enemies(all_targets, our_team, NULL, NULL, NULL);
                print_best_target(enemies);
                break;
            }
            case 4:{
                int ours = 0;
                int low_confidence = 0;
                int large_distance = 0;
                TargetList enemies = filter_enemies(all_targets, our_team, &ours, &low_confidence, &large_distance);
                printf("The number of initial detections: %d\n", all_targets.count);
                printf("The number of filtered detections: %d\n", enemies.count);
                printf("The number of filtered out team's detections: %d\n", ours);
                printf("The number of filtered low_confidence detections: %d\n", low_confidence);
                printf("The number of filtered large_distance detections: %d\n", large_distance);
                break;
            }
            default:
                printf("Unknown command. Please choose 0, 1, 2, 3, or 4.\n");
                break;
        }
    }

    printf("Program finished.\n");
    return 0;
}
