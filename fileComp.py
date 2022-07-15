#! python3
"""
比较 2 个或多个文件的内容是否完全一样，可以用来检查某个文件是否被更新。
参数：
    - 不加参数，则比较当前文件夹下所有文件是否相同
    - 至少 2 个文件路径作为参数，将比较这些文件是否相同
"""

import os
import sys

argv = sys.argv

if len(argv) == 2:
    print('\n参数格式错误！')
    print(f'使用方法： {sys.argv[0]} file1 file2(或者更多文件，要求至少两个文件)')
    sys.exit(0)
elif len(argv) == 1:
    files = os.listdir(os.getcwd())
    for file in files:
        if os.path.isdir(file):
            files.remove(file)
else:
    files = argv[1:]
    for file in files:
        try:
            with open(file, 'rb') as f:
                pass
        except FileNotFoundError:
            print(f'\n未找到文件：{file}，程序终止')
            sys.exit(0)

print('\n即将比较以下文件：')
for file in files:
    print(file)


def cmp_file(f1, f2):
    st1 = os.stat(f1)
    st2 = os.stat(f2)

    # 比较文件大小
    if st1.st_size != st2.st_size:
        return False

    bufsize = 8 * 1024
    with open(f1, 'rb') as fp1, open(f2, 'rb') as fp2:
        while True:
            b1 = fp1.read(bufsize)  # 读取指定大小的数据进行比较
            b2 = fp2.read(bufsize)
            if b1 != b2:
                return False
            if not b1:
                return True


if __name__ == '__main__':
    IsSame = True
    print('*' * 10)
    for i in range(0, len(files) - 1):
        print(f'正在比较 {files[i]} 和 {files[i+1]}...')
        IsSame = cmp_file(files[i], files[i + 1])
        if not IsSame:
            print('这 2 个文件不同，程序终止。')
            break
    print('*' * 10)
    if IsSame:
        print('所有的文件均相同。')
