//
// Created by Rambo.Liu on 2025/3/27.
//

#include <iostream>
#include <list>
#include <vector>
#include <unistd.h>

// 定时器结构体
struct timer_node {
    int session_id;  // 定时器ID
    time_t expires;  // 超时时间
    void (*callback)(void *);  // 回调函数指针
    void *data;  // 回调函数参数
};

// 时间轮槽结构体
struct timer_slot {
    std::list<struct timer_node *> timer_list; // 槽内定时器列表
    int slot_idx;  // 槽位编号
};

// 时间轮定时器结构体
struct timer_wheel {
    int interval;  // 每个时间槽的时间间隔，单位秒
    int ticks;  // 时间轮信息，当前已经走多少个时间槽
    int slot_num;  // 时间轮槽数量
    std::vector<struct timer_slot *> wheel; // 时间轮数组
};

// 创建时间轮定时器
struct timer_wheel *timer_wheel_create(int interval, int slot_num) {
    struct timer_wheel *tw = new struct timer_wheel;

    tw->interval = interval;
    tw->ticks = 0;
    tw->slot_num = slot_num;

    // 创建时间轮数组
    for (int i = 0; i < slot_num; i++) {
        struct timer_slot *ts = new struct timer_slot;
        ts->slot_idx = i;
        tw->wheel.push_back(ts);
    }

    return tw;
}

// 删除时间轮定时器
void timer_wheel_delete(struct timer_wheel *tw) {
    if (tw == nullptr) {
        return;
    }

    for (int i = 0; i < tw->slot_num; i++) {
        struct timer_slot *ts = tw->wheel[i];
        for (auto iter = ts->timer_list.begin(); iter != ts->timer_list.end(); ++iter) {
            delete (*iter);
        }
        ts->timer_list.clear();
        delete ts;
    }
    tw->wheel.clear();

    delete tw;
}


// 添加定时器
int timer_add(struct timer_wheel *tw, int session_id, int timeout,
              void (*callback)(void *), void *data) {
    if (timeout <= 0) {
        return -1;
    }

    int ticks = timeout / tw->interval;
    if (ticks == 0) {
        ticks = 1;
    }

    int slot_idx = (tw->ticks + ticks) % tw->slot_num;
    struct timer_slot *ts = tw->wheel[slot_idx];

    // 创建定时器
    struct timer_node *node = new struct timer_node;
    node->session_id = session_id;
    node->expires = time(NULL) + timeout;
    node->callback = callback;
    node->data = data;

    ts->timer_list.push_back(node);

    return 0;
}

// 删除定时器
int timer_del(struct timer_wheel *tw, int session_id) {
    for (int i = 0; i < tw->slot_num; i++) {
        struct timer_slot *ts = tw->wheel[i];
        for (auto iter = ts->timer_list.begin(); iter != ts->timer_list.end(); ++iter) {
            if ((*iter)->session_id == session_id) {
                delete (*iter);
                ts->timer_list.erase(iter);
                return 0;
            }
        }
    }

    return -1;
}

// 处理定时器超时事件
int timer_tick(struct timer_wheel *tw) {
    int ticks = tw->ticks % tw->slot_num;
    struct timer_slot *ts = tw->wheel[ticks];

    int count = 0;
    for (auto iter = ts->timer_list.begin(); iter != ts->timer_list.end();) {
        struct timer_node *node = *iter;
        if (node->expires <= time(NULL)) {
            // 调用回调函数，如果回调函数返回-1，则删除对应定时器
            if (node->callback) {
                node->callback(node->data);
                delete node;
                iter = ts->timer_list.erase(iter);
            } else {
                iter++;
            }
            count++;
        } else {
            iter++;
        }
    }

    tw->ticks++;

    return count;
}


int main(int argc, char *argv[]) {
    struct timer_wheel *tw = timer_wheel_create(1, 10);

    // 添加定时器
    timer_add(tw, 1, 5, [](void *data) -> void  {
        printf("timeout - session_id:%d\n", *(int *) data);
        //return nullptr;
    }, new int(123));

    // 添加定时器
    timer_add(tw, 2, 3, [](void *data) -> void  {
        printf("timeout - session_id:%d\n", *(int *) data);
        //return nullptr;
    }, new int(456));

    // 循环处理定时器事件
    while (true) {
        int ret = timer_tick(tw);
        printf("%d\n", ret);
        if (ret == 0) {
            sleep(1);
        }
    }

    // 销毁定时器
    timer_wheel_delete(tw);

    return 0;
}

