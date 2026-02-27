//
// Created by rambo.liu on 2026/2/11.
//

#include "sdk.h"
#include "sdk_class.h"
#include "sdk_internal.h"
#include "sdk_template.h"

/* ===============================
 * 真实对象
 * =============================== */
struct sdk_object_t {
    SdkCore core;
};

extern "C" {

    /* ===============================
 * 异步任务实现
 * =============================== */
    sdk_result_t sdk_start_async_task(
        sdk_object_t* obj,
        int input,
        sdk_async_callback cb,
        void* user_data
    ) {
        if (!obj) return SDK_INVALID;

        // 异步执行，不阻塞 C 调用方
        std::thread([input, cb, user_data]() {
            int result = heavy_work(input);

            // 回调 C
            if (cb) {
                cb(result, user_data);
            }
        }).detach();

        return SDK_OK;
    }

    /* 生命周期 */
    sdk_object_t* sdk_create(void) {
        return new sdk_object_t{};
    }

    void sdk_destroy(sdk_object_t* obj) {
        delete obj;
    }

    /* 成员方法 */
    sdk_result_t sdk_set_value(sdk_object_t* obj, int v) {
        if (!obj) return SDK_INVALID;
        obj->core.setValue(v);
        return SDK_OK;
    }

    sdk_result_t sdk_get_value(sdk_object_t* obj, int* out) {
        if (!obj || !out) return SDK_INVALID;
        *out = obj->core.getValue();
        return SDK_OK;
    }

    sdk_result_t sdk_add(
        sdk_object_t* obj,
        int x,
        int* out
    ) {
        if (!obj || !out) return SDK_INVALID;
        *out = obj->core.add(x);
        return SDK_OK;
    }

    /* 类静态方法 */
    int sdk_get_version(void) {
        return SdkCore::version();
    }

    /* 模板实例 */
    sdk_result_t sdk_sum_int(
        const int* d, int l, int* out
    ) {
        if (!d || !out || l <= 0) return SDK_INVALID;
        *out = sum<int>(d, l);
        return SDK_OK;
    }

    sdk_result_t sdk_sum_double(
        const double* d, int l, double* out
    ) {
        if (!d || !out || l <= 0) return SDK_INVALID;
        *out = sum<double>(d, l);
        return SDK_OK;
    }

    /* 模板元编程结果 */
    int sdk_type_id_int(void) {
        return type_id<int>();
    }

    int sdk_type_id_double(void) {
        return type_id<double>();
    }

    int sdk_is_type_supported_int(void) {
        return TypeInfo<int>::is_supported;
    }

    int sdk_is_type_supported_double(void) {
        return TypeInfo<double>::is_supported;
    }

}
