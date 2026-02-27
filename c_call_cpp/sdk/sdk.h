//
// Created by rambo.liu on 2026/2/11.
//

#ifndef SDK_H
#define SDK_H

#ifdef __cplusplus
extern "C" {
#endif

    /* ===============================
     * 不透明句柄
     * =============================== */
    typedef struct sdk_object_t sdk_object_t;

    /* ===============================
     * 返回码
     * =============================== */
    typedef enum {
        SDK_OK = 0,
        SDK_ERR = -1,
        SDK_INVALID = -2,
        SDK_UNSUPPORTED = -3
    } sdk_result_t;

    /* ===============================
     * 生命周期
     * =============================== */
    sdk_object_t* sdk_create(void);
    void sdk_destroy(sdk_object_t* obj);

    /* ===============================
     * 类成员变量 / 方法
     * =============================== */
    sdk_result_t sdk_set_value(sdk_object_t*, int);
    sdk_result_t sdk_get_value(sdk_object_t*, int* out);

    sdk_result_t sdk_add(
        sdk_object_t*,
        int x,
        int* out_result
    );

    /* ===============================
     * 类静态方法
     * =============================== */
    int sdk_get_version(void);

    /* ===============================
     * 模板实例 API
     * =============================== */
    sdk_result_t sdk_sum_int(
        const int* data, int len, int* out);

    sdk_result_t sdk_sum_double(
        const double* data, int len, double* out);

    /* ===============================
     * 模板元编程结果暴露
     * =============================== */
    int sdk_type_id_int(void);
    int sdk_type_id_double(void);

    int sdk_is_type_supported_int(void);
    int sdk_is_type_supported_double(void);


    /* ===============================
 * 回调定义（C++ → C）
 * =============================== */

    /**
     * @param result   异步任务结果
     * @param user_data 用户自定义上下文
     */
    typedef void (*sdk_async_callback)(
        int result,
        void* user_data
    );

    /* ===============================
     * 异步接口
     * =============================== */

    /**
     * 启动一个异步耗时任务
     * @param obj SDK 对象
     * @param input 输入参数
     * @param cb 回调函数（可为 NULL）
     * @param user_data 用户上下文
     */
    sdk_result_t sdk_start_async_task(
        sdk_object_t* obj,
        int input,
        sdk_async_callback cb,
        void* user_data
    );


#ifdef __cplusplus
}
#endif

#endif
