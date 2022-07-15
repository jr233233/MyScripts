"""
用户给定目标文件夹及筛选条件，这个脚本将查找出目标文件夹内所有满足条件的文件
"""

import os
import sys
import traceback
from getArgs import getArgs

def searchFiles(Path, searchAll, judge=None):
    """
    搜索目录 Path 下所有的文件。
    searchAll 表示是否搜索当前文件夹下子文件夹中的文件。
    如果为  True，搜索范围包含子文件夹。
    如果为 False，仅搜索当前文件夹，不搜索子文件夹。
    judge 为一个判断函数，接受一个文件路径作为参数，并返回 True/False，用以筛选符合特定要求的文件
    """
    allFiles = []
    files = []
    dirs = []
    failedDirs = []

    try:
        for filesOrDirs in os.listdir(Path):
            if filesOrDirs == '$RECYCLE.BIN' or filesOrDirs == 'System Volume Information':
                continue
            if os.path.isdir(os.path.join(Path, filesOrDirs)):
                dirs.append(filesOrDirs)
            else:
                files.append(filesOrDirs)
    except PermissionError:
        traceback.print_exc()
        failedDirs.append(Path)
        return allFiles, failedDirs

    for file in files:
        if judge is not None:
            if judge(file):
                filePath = os.path.join(Path, file)
                allFiles.append(filePath)
        else:
            filePath = os.path.join(Path, file)
            allFiles.append(filePath)

    if searchAll:
        for dir in dirs:
            subPath = os.path.join(Path, dir)
            subResult = searchFiles(subPath, searchAll, judge)
            allFiles = allFiles + subResult[0]
            failedDirs = failedDirs + subResult[1]
    return allFiles, failedDirs


if __name__ == '__main__':
    dirname, searchAll = getArgs()
    files, failedDirs = searchFiles(dirname, searchAll)
    print('*' * 20)
    print('找到以下文件：')
    for file in files:
        print(file)
    print('*' * 20)
    print('以下文件夹搜索失败：')
    for failedDir in failedDirs:
        print(failedDir)
    print('*' * 20)
    print(f'搜索到文件数量：{len(files)}')
    print(f'失败文件夹数量：{len(failedDirs)}')
    print('*' * 20)
