//
// Created by rambo.liu on 2026/4/28.
//

#include <iostream>
#include "litepb.h"
#include "user.pb.h"


#include <iostream>
#include "pb_utils.h"

int test_base_ser() {
    // --- 1. 序列化 (打包) ---
    Serializer writer;

    int id = 1001;
    long long timestamp = 1698765432LL;
    double score = 99.5;
    std::string name = "Protobuf-Lite";

    // 链式写入，顺序很重要
    writer << id << timestamp << score << name;

    // 获取二进制数据（例如用于网络发送）
    auto& rawData = writer.getBuffer();
    std::cout << "序列化完成，总字节数: " << rawData.size() << std::endl;

    // --- 2. 反序列化 (拆包) ---
    Deserializer reader(rawData.data(), rawData.size());

    int r_id;
    long long r_timestamp;
    double r_score;
    std::string r_name;

    // 必须按照写入的顺序读出
    reader >> r_id >> r_timestamp >> r_score >> r_name;

    // --- 3. 验证结果 ---
    std::cout << "ID: " << r_id << std::endl;
    std::cout << "Time: " << r_timestamp << std::endl;
    std::cout << "Score: " << r_score << std::endl;
    std::cout << "Name: " << r_name << std::endl;

    return 0;
}


int main() {
    User user;
    user.id = 1001;
    user.name = "ZhangSan";
    user.age = 25;
    user.address.city = "Beijing";
    user.address.zipcode = 100000;
    user.scores = {90, 85, 95, 100};

    uint8_t buffer[1024];
    uint32_t len = user.SerializeToArray(buffer, sizeof(buffer));
    std::cout << "Serialize success, bytes: " << len << std::endl;

    User user2;
    auto err = user2.ParseFromArray(buffer, len);

    if (err == litepb::kSuccess) {
        std::cout << "\nParse success:\n";
        std::cout << "ID:      " << user2.id << "\n";
        std::cout << "Name:    " << user2.name << "\n";
        std::cout << "Age:     " << user2.age << "\n";
        std::cout << "City:    " << user2.address.city << "\n";
        std::cout << "Zipcode: " << user2.address.zipcode << "\n";
        // 打印 scores
        std::cout << "Scores:  ";
        for (auto s : user2.scores) {
            std::cout << s << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "Error: " << err << "\n";
    }

    std::cout << "测试基本数据类型的序列化: " << "\n";
    test_base_ser();

    return 0;
}