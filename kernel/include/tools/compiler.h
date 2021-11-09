#ifndef COMPILER_H
#define COMPILER_H

#define __unused __attribute__((unused))

#define __used __attribute__((used))

#define __packed __attribute__((packed))

#define __section(s) __attribute__((section(s)))

#endif // COMPILER_H