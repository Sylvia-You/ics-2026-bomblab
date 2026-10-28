#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    // TODO: 根据 phase_01 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t high = (uint32_t)(challenge >> 32);
    uint32_t mid  = (uint32_t)(challenge >> 16);
    uint32_t low  = (uint32_t)challenge;

    uint32_t res1 = (3 * high + mid) ^ 0x84c6;
    uint32_t res2 = (high ^ 0x1be7) + (low * 2);
    uint32_t result = 3 * (res2 - res1) + 0x4fc7;
    values[0] = result;
    count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    // TODO: 根据 phase_02 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    values[0] = ((challenge >> 7) & 0xFF) + 0x1F;

    int cl = 28; // 初始位移量
    for (int i = 1; i < 6; i++) {
        uint32_t term1 = ((challenge >> cl) & 0x1F) + i;
        uint32_t term2 = values[i - 1] * 3;
        values[i] = term1 ^ term2;
        cl += 5; 
    }
    count = 6;
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    // TODO: 根据 phase_03 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t val1 = (uint32_t)(challenge & 0x7);
    values[0] = val1;
    uint32_t arg2 = (uint32_t)(challenge >> 32) ^ (uint32_t)(challenge >> 3);
    uint32_t val2 = 0;
    switch (val1) {
        case 0:
            // selector 0 指向 0x402268 -> p3_case_0
            val2 = 5 * arg2 + 0xdad1;
            break;
        case 1:
            // selector 1 指向 0x402270 -> p3_case_1
            val2 = (arg2 ^ 0x2f5f) + (arg2 >> 1);
            break;
        case 2:
            // selector 2 指向 0x402278 -> p3_case_3 
            val2 = (4 * arg2) ^ 0xe2da;
            break;
        case 3:
            // selector 3 指向 0x402280 -> p3_case_7
            val2 = 5 * arg2 + 0x109dc;
            break;
        case 4:
            // selector 4 指向 0x402288 -> p3_case_6
            val2 = (arg2 ^ 0xc0ac) + (arg2 >> 1);
            break;
        case 5:
            // selector 5 指向 0x402290 -> p3_case_5
            val2 = (2 * arg2) ^ 0x33f4;
            break;
        case 6:
            // selector 6 指向 0x402258 -> p3_case_2
            val2 = 5 * arg2 + 0x2f68e;
            break;
        case 7:
            // selector 7 指向 0x402260 -> p3_case_4
            val2 = 5 * arg2 + 0x2943;
            break;
        default:
            val2 = 0;
            break;
    }
    values[1] = val2;
    count = 2;
    return write_answer(out, cap, values, count);
}

uint32_t p4_walk(uint32_t target, uint32_t low, uint32_t high, uint32_t key) {
    uint32_t mid = low + (high - low) / 2;
    
    if (mid == target) {
        return (mid + 0x39) ^ key;
    }
    
    uint32_t temp = mid + key;
    
    if (target > mid) {
        uint32_t ret = p4_walk(target, mid + 1, high, key);
        return ((ret * 5) ^ temp) ^ 0x1f;
    } else {
        uint32_t ret = p4_walk(target, low, mid - 1, key);
        return ((ret * 5) ^ temp) ^ 0xe;
    }
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    // TODO: 根据 phase_04 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t target = (uint32_t)challenge & 0x7F;
    uint32_t low = 0;
    uint32_t high = 0x7F;
    uint32_t key = (uint32_t)(challenge >> 7) ^ (uint32_t)(challenge >> 32);
    values[0] = p4_walk(target, low, high, key);
    count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    // TODO: 根据 phase_05 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t v1[] = {5126, 11198, 30717, 18454, 30373, 19463, 64621, 65508, 55439, 56070, 36190, 59793};
    uint32_t v2[] = {1410, 2910, 2637, 2440, 1661, 1825, 1030, 583, 311, 2067, 9, 827};

    int idx = challenge & 0x7; // 对应 challenge & 0x7
    int current_node = idx;

    uint32_t eax = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32);
    eax ^= v1[current_node];
    values[1] = v2[current_node]; // 第二个答案先取头结点的 val2
    eax ^= 0xc65c;
    eax += v2[current_node];

    int loop_count = (challenge >> 3) & 0x3;

    if (loop_count > 0) {
        for (int i = 1; i <= loop_count; i++) {
            current_node++; // 相当于 node = node->next
            eax ^= v1[current_node];
            eax += v2[current_node];
            values[1] = v2[current_node]; // 走到新结点后更新第二个答案
        }
    }
    values[0] = eax; 
    count = 2;
    return write_answer(out, cap, values, count);
}


uint32_t p6_op_0(uint32_t a) {
    return ((a >> 7) ^ a) + 0x629c;
}

uint32_t p6_op_1(uint32_t a) {
    // ror $0x7 (循环右移 7 位)
    return (a >> 7) | (a << 25);
}

uint32_t p6_op_2(uint32_t a) {
    return (a ^ 0xfbbc) + 2 * a;
}

uint32_t p6_op_3(uint32_t a) {
    return (8 * a + 0xc23f) ^ (5 * a);
}

uint32_t p6_op_4(uint32_t a) {
    return (a + 0xb336) ^ (a >> 2);
}

uint32_t p6_op_5(uint32_t a) {
    // rol $0x2 (循环左移 2 位)
    return ((a << 2) | (a >> 30)) ^ 0xabe0;
}

uint32_t my_p6_dispatch_table(int idx, uint32_t arg) {
    switch (idx) {
        case 0:
        case 2:
        case 3: return p6_op_5(arg);
        case 1: return p6_op_4(arg);
        case 4: return p6_op_0(arg);
        case 5: return p6_op_3(arg);
        case 6: return p6_op_2(arg);
        case 7: return p6_op_1(arg);
        default: return 0;
    }
}

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    // TODO: 根据 phase_06 的汇编，用 challenge 算出口令中的各个整数，依次存入 values，并把 count 改为整数个数。
    uint32_t idx1 = (uint32_t)(challenge & 0x7); // 取 bit 0-2
    uint32_t idx2 = (uint32_t)((challenge >> 3) & 0x7); // 取 bit 3-5
    uint32_t arg1 = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32);
    uint32_t res1 = my_p6_dispatch_table(idx1, arg1);
    uint32_t arg2 = res1 ^ 0xd985;
    uint32_t res2 = my_p6_dispatch_table(idx2, arg2);
    values[0] = res2 + 0xd2be;
    count = 1;
    return write_answer(out, cap, values, count);
}

uint32_t secret_mix(uint32_t a) {
    a = (a << 1) | (a >> 31);
    a += 0xf84;
    a ^= 0xb432;
    return a;
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    uint32_t v0 = (uint32_t)(context & 0x3);

    uint32_t r8 = (uint32_t)(context >> 32) ^ (uint32_t)(context & 0xffffffff);
    uint32_t r9 = (v0 * 0xf84) ^ (uint32_t)((context >> 16) & 0xffffffff);
    uint32_t esi = (uint32_t)(challenge >> 32) ^ (uint32_t)(challenge & 0xffffffff);
    uint32_t edi = esi ^ r8;

    uint32_t mix_out = secret_mix(edi);
    uint32_t v1 = mix_out ^ r9;

    uint32_t edx = 0xb432 + r8 + v0 + (esi >> 1);
    uint32_t v2 = v1 ^ edx;

    int len = snprintf(out, cap, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOPEN %u %u %u", v0, v1, v2);

    if (len < 0 || (size_t)len >= cap) {
        return 1;
    }
    return 0;
}
