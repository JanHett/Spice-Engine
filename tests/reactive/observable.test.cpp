#include <gtest/gtest.h>

#include "../../src/reactive/observable.h"

// -------------- //
//   Observable   //
// -------------- //

TEST(observable, constructor) {
    observable<int> obs;

    const auto internal_ptr = obs.get();
    EXPECT_EQ(nullptr, internal_ptr);
}

TEST(observable, get) {
    observable<int> obs;
    const auto internal_ptr = obs.get();
    static_assert(std::is_const<decltype(internal_ptr)>::value, "observable value might be modifiable via get()");

    EXPECT_EQ(nullptr, internal_ptr);
    obs.set(42);
    ASSERT_NE(nullptr, obs.get());
    EXPECT_EQ(42, *obs.get());
}

TEST(observable, set) {
    observable<int> obs;
    
    EXPECT_EQ(nullptr, obs.get());
    obs.set(42);
    auto ptr_before = obs.get();
    ASSERT_NE(nullptr, obs.get());
    EXPECT_EQ(42, *obs.get());
    obs.set(47);
    EXPECT_EQ(ptr_before, obs.get());   // expect memery to be reused
}

TEST(observable, reset) {
    observable<int> obs;
    
    EXPECT_EQ(nullptr, obs.get());
    obs.set(42);
    ASSERT_NE(nullptr, obs.get());
    EXPECT_EQ(42, *obs.get());
    obs.reset();
    ASSERT_EQ(nullptr, obs.get());
}

TEST(observable, subscribe) {
    observable<int> obs;
    
    int * local_data = nullptr;
    std::function<void(int * const)> cb = [&](int * const new_value) { local_data = new_value; };

    EXPECT_EQ(nullptr, obs.get());
    obs.set(42);

    auto subscribed = obs.subscribe(cb);
    EXPECT_EQ(true, subscribed);
    auto subscribed_again = obs.subscribe(cb);
    EXPECT_EQ(false, subscribed_again);
    ASSERT_NE(nullptr, local_data);
    EXPECT_EQ(42, *local_data);
    auto local_data_before = local_data;

    obs.set(47);
    ASSERT_NE(nullptr, local_data);
    EXPECT_EQ(local_data_before, local_data);   // observe reuse of allocated space
    EXPECT_EQ(47, *local_data);

    obs.reset();
    EXPECT_EQ(nullptr, local_data);
}

TEST(observable, unsubscribe) {
    observable<int> obs;
    
    int * local_data = nullptr;
    std::function<void(int * const)> cb = [&](int * const new_value) { local_data = new_value; };

    EXPECT_EQ(nullptr, obs.get());
    obs.set(42);

    auto subscribed = obs.subscribe(cb);
    EXPECT_EQ(true, subscribed);
    auto subscribed_again = obs.subscribe(cb);
    EXPECT_EQ(false, subscribed_again);
    ASSERT_NE(nullptr, local_data);
    EXPECT_EQ(42, *local_data);
    auto local_data_before = local_data;

    obs.set(47);
    ASSERT_NE(nullptr, local_data);
    EXPECT_EQ(local_data_before, local_data);   // observe reuse of allocated space
    EXPECT_EQ(47, *local_data);

    obs.reset();
    EXPECT_EQ(nullptr, local_data);

    auto unsubscribed = obs.unsubscribe(cb);
    EXPECT_EQ(true, unsubscribed);
    auto unsubscribed_again = obs.unsubscribe(cb);
    EXPECT_EQ(false, unsubscribed_again);

    // TODO: find a way to see if we really are unsubscribed
    // - can't check if obs.set() doesn't propagate because memory address
    //   will still be same
    // - can't dereference pointer after reset() and set() because
    //   memory will have been deleted
}

TEST(observable, operatorTRef) {
    observable<int> obs;

    obs.set(42);

    int & val = obs;
    EXPECT_EQ(42, obs.operator int&());
    EXPECT_TRUE(42 == obs);
    EXPECT_EQ(42, val);
    obs.set(47);
    EXPECT_EQ(47, obs.operator int&());
    EXPECT_TRUE(47 == obs);
    EXPECT_EQ(47, val);
}

TEST(observable, operatorTPtr) {
    observable<int> obs;

    EXPECT_EQ(nullptr, obs.operator int*());
    EXPECT_TRUE(nullptr == obs);

    obs.set(42);

    int * val = obs;
    EXPECT_EQ(42, *obs.operator int*());
    EXPECT_EQ(val, obs.operator int*());
    EXPECT_TRUE(42 == *obs);
    EXPECT_TRUE(val == obs);
    EXPECT_EQ(42, *val);
    obs.set(47);
    EXPECT_EQ(47, *obs.operator int*());
    EXPECT_EQ(val, obs.operator int*());
    EXPECT_TRUE(47 == *obs);
    EXPECT_TRUE(val == obs);
    EXPECT_EQ(47, *val);

    obs.reset();

    EXPECT_EQ(nullptr, obs.operator int*());
    EXPECT_TRUE(nullptr == obs);
}
