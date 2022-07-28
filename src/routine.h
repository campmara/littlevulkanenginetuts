#ifndef ROUTINE_H
#define ROUTINE_H

namespace XIV {
    struct Routine {
        float waitFor = 0;   // Current "waiting time" before we run the next block
        float repeatFor = 0; // Used during `rt_for`, which repeats the given block for X time
        int at = 0;          // Current block of the routine
    };

    int constexpr rt_hash(const char *name) {
        unsigned int hash = 5381;
        for (int i = 0; name[i] != '\0'; i++) {
            hash = ((hash << 5) + hash) + name[i];
        }
        return hash;
    }
} // namespace XIV

// #define DELTA_TIME XIV::Time::DELTA

// Begins the Coroutine, location 0
// `routine` is a reference to a Routine struct which holds the state
#define rt_begin(routine)                                                                          \
    if (routine.wait_for > 0)                                                                      \
        routine.wait_for -= DELTA_TIME;                                                            \
    else {                                                                                         \
        auto &__rt = routine; /* routine ref */                                                    \
        auto __mn = true;     /* move-next */                                                      \
        switch (__rt.at) {                                                                         \
        case 0: {
// Waits until the next frame to begind the following block
#define rt_step()                                                                                  \
    }                                                                                              \
    if (__mn)                                                                                      \
        __rt.at = __LINE__;                                                                        \
    break;                                                                                         \
    case __LINE__: {
// Same as `rt_step` but can be jumped
// to by using rt_goto(name)
#define rt_label(name)                                                                             \
    }                                                                                              \
    if (__mn)                                                                                      \
        __rt.at = rt_hash(name);                                                                   \
    break;                                                                                         \
    case rt_hash(name): {
// Repeats the following block for the given amount of time
#define rt_for(time)                                                                               \
    rt_step();                                                                                     \
    if (__rt.repeat_for < time) {                                                                  \
        __rt.repeat_for += DELTA_TIME;                                                             \
        __mn = __rt.repeat_for >= time;                                                            \
        if (__mn)                                                                                  \
            __rt.repeat_for = 0;                                                                   \
    }

// Repeats the following block while the condition is met
#define rt_while(condition)                                                                        \
    }                                                                                              \
    if (__mn)                                                                                      \
        __rt.at = __LINE__;                                                                        \
    break;                                                                                         \
    case __LINE__:                                                                                 \
        if (condition) {                                                                           \
        __mn = false

// Waits until the condition is met
#define rt_until(condition)                                                                        \
    }                                                                                              \
    if (__mn)                                                                                      \
        __rt.at = ((condition) ? __LINE__ : -__LINE__);                                            \
    break;                                                                                         \
    case -__LINE__:                                                                                \
        if (condition)                                                                             \
            __rt.at = __LINE__;                                                                    \
        break;                                                                                     \
    case __LINE__: {
// Waits a given amount of time before beginning the following block
#define rt_wait(time)                                                                              \
    }                                                                                              \
    if (__mn) {                                                                                    \
        __rt.wait_for = time;                                                                      \
        __rt.at = __LINE__;                                                                        \
    }                                                                                              \
    break;                                                                                         \
    case __LINE__: {
// Ends the Coroutine
#define rt_end()                                                                                   \
    }                                                                                              \
    if (__mn)                                                                                      \
        __rt.at = -1;                                                                              \
    break;                                                                                         \
    }                                                                                              \
    }                                                                                              \
    goto rt_end_of_routine;                                                                        \
    rt_end_of_routine:

// Flow Statements:
// These can be used anywhere between rt_begin and rt_end,
// including if statements, while loops, etc.

// Repeats the block that this is contained within
// Skips the remainder of the block
#define rt_repeat() goto rt_end_of_routine

// Goes to a given block labeled with `rt_label`
#define rt_goto(name)                                                                              \
    do {                                                                                           \
        __rt.at = rt_hash(name);                                                                   \
        goto rt_end_of_routine;                                                                    \
    } while (false)

// Restarts the entire Coroutine;
// Jumps back to `rt_begin` on the next frame
#define rt_restart()                                                                               \
    do {                                                                                           \
        __rt.at = 0;                                                                               \
        __rt.wait_for = 0;                                                                         \
        __rt.repeat_for = 0;                                                                       \
        goto rt_end_of_routine;                                                                    \
    } while (false)

// Example:
//
// // Assuming you have a `routine` variable stored somewhere
// rt_begin(routine);
// {
//     // stuff that happens frame 1
// }
// rt_wait(1.0f);
// {
//     // after 1.0s, this block runs
// }
// rt_for(0.25f);
// {
//     // this block repeats for 0.25s
// }
// rt_step();
// {
//     // the following frame, this block is run
// }
// rt_label("ABC");
// {
//     if (something)
//         rt_repeat();
//
//     // not run if rt_repeat() was called
//     something_else();
// }
// rt_step();
// {
//     if (another) rt_goto("ABC"); // jumps to "ABC"
//     if (another2) rt_restart();  // jumps to rt_begin
//     // otherwise the next block will be run next frame
// }
// rt_while(condition_is_true);
// {
//     // this is repeated until condition_is_true is false
// }
// rt_end();
//

#endif