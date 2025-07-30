#include "../exercise.h"
#include <memory>

// READ: `std::shared_ptr` <https://zh.cppreference.com/w/cpp/memory/shared_ptr>
// READ: `std::weak_ptr` <https://zh.cppreference.com/w/cpp/memory/weak_ptr>

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    auto shared = std::make_shared<int>(10);
    std::shared_ptr<int> ptrs[]{shared, shared, shared};

    std::weak_ptr<int> observer = shared;
    ASSERT(observer.use_count() == 4, ""); // shared + ptrs[0] + ptrs[1] + ptrs[2]

    ptrs[0].reset();
    ASSERT(observer.use_count() == 3, ""); // shared + ptrs[1] + ptrs[2]

    ptrs[1] = nullptr;
    ASSERT(observer.use_count() == 2, ""); // shared + ptrs[2]

    ptrs[2] = std::make_shared<int>(*shared); // 指向一个新的数据
    ASSERT(observer.use_count() == 1, "");  // shared

    ptrs[0] = shared;
    ptrs[1] = shared;
    ptrs[2] = std::move(shared);
    // std::cout << "shared use_count: " << shared.use_count() << "\n"; //0
    // std::cout << "ptrs[0] use_count: " << (ptrs[0] ? ptrs[0].use_count() : 0) << "\n"; //3
    // std::cout << "ptrs[1] use_count: " << (ptrs[1] ? ptrs[1].use_count() : 0) << "\n"; //3
    // std::cout << "ptrs[2] use_count: " << (ptrs[2] ? ptrs[2].use_count() : 0) << "\n"; //3
    // std::cout << "observer use_count: " << observer.use_count() << "\n"; // 3
    ASSERT(observer.use_count() == 3, ""); // ptrs[0] + ptrs[1] + ptrs[2]

    std::ignore = std::move(ptrs[0]);
    ptrs[1] = std::move(ptrs[1]);
    ptrs[1] = std::move(ptrs[2]);
    ptrs[2] = nullptr;  // 新加, 不影响 count, 说明 ptrs[2] 不影响
    // ptrs[0] = nullptr; // 若增加, 会影响 count, 说明 ptrs[0] 还是指向 shared
    // std::cout << "shared use_count: " << shared.use_count() << "\n"; //0
    // std::cout << "ptrs[0] use_count: " << (ptrs[0] ? ptrs[0].use_count() : 0) << "\n"; //2
    // std::cout << "ptrs[1] use_count: " << (ptrs[1] ? ptrs[1].use_count() : 0) << "\n"; //2
    // std::cout << "ptrs[2] use_count: " << (ptrs[2] ? ptrs[2].use_count() : 0) << "\n"; //0
    // std::cout << "observer use_count: " << observer.use_count() << "\n"; // 2
    ASSERT(observer.use_count() == 2, ""); // ptrs[0] + ptrs[1]

    shared = observer.lock();
    ASSERT(observer.use_count() == 3, ""); // ptrs[0] + ptrs[1] + shared

    shared = nullptr;
    for (auto &ptr : ptrs) ptr = nullptr;
    ASSERT(observer.use_count() == 0, "");

    shared = observer.lock();
    ASSERT(observer.use_count() == 0, "");

    return 0;
}
