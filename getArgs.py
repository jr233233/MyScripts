import os
import sys


def getArgs():
    """
    从键盘获取参数，返回2个元素构成的元组: (path, searchAll)
    path 为 str 类型，表示目标文件夹

    searchAll 为 bool 类型：
        如果 searchAll 为 True, 将搜索范围包括目标文件夹和目标文件夹内部的子文件夹
        如果 searchAll 为 False，搜索范围仅包括目标文件夹
    """
    path = os.getcwd()
    searchAll = True
    error = False

    def getPath(input):
        if input[:2] in ['C:', 'c:', 'D:', 'd:', 'e:', 'E:']:
            # 绝对路径
            res = input
        else:
            # 相对路径转绝对路径
            res =  os.path.join(os.getcwd(), input)
        
        return os.path.abspath(res)

    if len(sys.argv) == 1:
        return (path, searchAll)
    elif len(sys.argv) == 2:
        if sys.argv[1] in ['0', '1', '-h']:
            if sys.argv[1] == '-h':
                usage = '''
                参数使用方法：
                不加参数，则默认搜索范围包括子目录，搜索目标文件夹为当前文件夹；
                若加 1 个参数，可以填 1 或 0, 1 代表搜索范围包括子目录, 0 则不包括；目标文件夹默认为当前文件夹；
                        也可以填目标文件夹的路径，则默认搜索范围包括子目录；
                若加 2 个参数，则其中一个参数填 0 或 1, 另一个参数填目标文件夹路径，顺序随意
                '''
                print(usage)
                exit(0)
            elif sys.argv[1] == '1':
                searchAll = True
            elif sys.argv[1] == '0':
                searchAll = False
        else:
            path = getPath(sys.argv[1])
            if not os.path.isdir(path):
                error = True
                print(f'Folder not found: {path}')
    elif len(sys.argv) == 3:
        if sys.argv[1] in ['0', '1']:
            path = getPath(sys.argv[2])
            if not os.path.isdir(path):
                error = True
                print(f'Folder not found: {path}')
            if sys.argv[1] == '1':
                searchAll = True
            elif sys.argv[1] == '0':
                searchAll = False
        elif sys.argv[2] in ['0', '1']:
            path = getPath(sys.argv[1])
            if not os.path.isdir(path):
                error = True
                print(f'Folder not found: {path}')
            if sys.argv[2] == '1':
                searchAll = True
            elif sys.argv[2] == '0':
                searchAll = False
        else:
            error = True

    if error:
        usage = '''
        参数格式错误或文件不存在，不执行任何操作！
        参数使用方法：
        不加参数，则默认搜索范围包括子目录，搜索目标文件夹为当前文件夹；
        若加 1 个参数，可以填 1 或 0, 1 代表搜索范围包括子目录, 0 则不包括；目标文件夹默认为当前文件夹；
                   也可以填目标文件夹的路径，则默认搜索范围包括子目录；
        若加 2 个参数，则其中一个参数填 0 或 1, 另一个参数填目标文件夹路径，顺序随意
        '''
        print(usage)
        exit(0)

    return (path, searchAll)


if __name__ == '__main__':
    path, searchAll = getArgs()
    print(f'path = {path}\nsearchAll = {searchAll}')
