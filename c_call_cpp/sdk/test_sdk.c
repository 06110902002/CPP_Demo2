//
// Created by rambo.liu on 2026/2/11.
//

#include <assert.h>
#include <stdio.h>
#include "sdk.h"

#include <unistd.h>   // sleep
#include <stdio.h>

static void async_cb(int result, void* user_data) {
    int* out = (int*)user_data;
    *out = result;
    printf("async callback result = %d\n", result);
}

void test_async_callback(void) {
    printf("==== async callback test ====\n");

    sdk_object_t* obj = sdk_create();

    int result = 0;
    sdk_start_async_task(
        obj,
        21,
        async_cb,
        &result
    );

    /* 等待异步完成 */
    sleep(1);

    /* 21 * 2 = 42 */
    assert(result == 42);

    sdk_destroy(obj);
    printf("[PASS]\n\n");
}




int main(void) {
    sdk_object_t* obj = sdk_create();
    assert(obj);

    assert(sdk_set_value(obj, 10) == SDK_OK);

    int v = 0;
    assert(sdk_get_value(obj, &v) == SDK_OK);
    printf("value: %d\n", v);
    assert(v == 10);

    int r = 0;
    assert(sdk_add(obj, 5, &r) == SDK_OK);
    assert(r == 15);

    int arr[] = {1,2,3};
    int sum = 0;
    assert(sdk_sum_int(arr, 3, &sum) == SDK_OK);
    assert(sum == 6);

    double darr[] = {1.5, 2.5};
    double dsum = 0;
    assert(sdk_sum_double(darr, 2, &dsum) == SDK_OK);
    assert(dsum > 3.9 && dsum < 4.1);

    assert(sdk_get_version() > 0);
    assert(sdk_type_id_int() == 1);
    assert(sdk_is_type_supported_int() == 1);


    test_async_callback();

    sdk_destroy(obj);
    printf("ALL C TESTS PASSED\n");
    return 0;
}
