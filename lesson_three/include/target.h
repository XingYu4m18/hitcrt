#ifndef RM_TARGET_H
#define RM_TARGET_H

/* TargetList 使用固定长度的数组，当前示例最多保存 8 个目标。 */
#define MAX_TARGETS 8

/* 机器人类型。UNKNOWN 用于表示识别程序暂时无法判断的类型。 */
typedef enum {
    ROBOT_UNKNOWN = 0,
    ROBOT_HERO,
    ROBOT_INFANTRY,
    ROBOT_ENGINEER,
    ROBOT_SENTRY
} RobotType;

/* 红蓝阵营。与 our_team 比较即可判断一个目标是队友还是敌人。 */
typedef enum {
    TEAM_UNKNOWN = 0,
    TEAM_RED,
    TEAM_BLUE
} TeamType;

/* 一条目标检测结果：识别到的是谁、属于哪方、有多远以及判断有多可信。 */
typedef struct {
    int id;
    RobotType type;
    TeamType team;
    float distance_m;
    int confidence;
} Target;

/* 目标集合。items 中只有前 count 个元素是有效数据。 */
typedef struct {
    Target items[MAX_TARGETS];
    int count;
} TargetList;

/* 将枚举值转换成便于显示的文字。 */
const char *robot_type_name(RobotType type);

const char *team_name(TeamType team);

/* 打印一条目标记录或一组目标。 */
void print_target(Target target);
void print_targets(const char *title, TargetList list);

/* 返回敌方目标的新列表，不修改传入的原始列表。 */
TargetList filter_enemies(TargetList all, TeamType our_team,int *ours,int *low_confidence,int *large_distance);

/* 根据机器人类型、距离和置信度计算威胁优先级。 */
int target_priority(Target target);

/* 从已过滤的敌方列表中选出优先级最高的目标。 */
void print_best_target(TargetList list);

#endif
