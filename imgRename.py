#! python3
"""
这个脚本可以把目标文件夹内的所有图片重命名，命名格式为文件夹名+编号，所有图片会被转变成.png格式。
可以递归修改目标文件夹内的子文件夹，也可只修改当前文件夹的图片，详见参数设置部分。
"""

import os
import cv2
from getArgs import getArgs


def JpgOrPng(file):
    magic_number = open(file, "rb").read(2)
    if magic_number == b"\xff\xd8":
        return 'jpg'
    elif magic_number == b"\x89\x50":
        return 'png'
    else:
        return 'other'


def rename(Path, RenameSubFolder):
    failedFile = []
    folderName = Path.split('\\')[-1]
    files = []
    dirs = []
    for filesOrDirs in os.listdir(Path):
        if os.path.isdir(os.path.join(Path, filesOrDirs)):
            dirs.append(filesOrDirs)
        else:
            files.append(filesOrDirs)

    suffix = 'abc'
    numFiles = len(files)
    fileIdx = 1
    for file in files:
        originFilePath = os.path.join(Path, file)

        if JpgOrPng(originFilePath) == 'jpg':
            img = cv2.imread(originFilePath)
            if img is None:
                print('发现异常图片：', originFilePath)
                failedFile.append(originFilePath)
                numFiles -= 1
            else:
                newName = folderName + str(fileIdx) + suffix + '.png'
                fileIdx += 1
                newFilePath = os.path.join(Path, newName)
                cv2.imwrite(newFilePath, img)
                os.remove(originFilePath)
        elif JpgOrPng(originFilePath) == 'png':
            newName = folderName + str(fileIdx) + suffix + '.png'
            fileIdx += 1
            newFilePath = os.path.join(Path, newName)
            os.rename(originFilePath, newFilePath)
    print(f'在文件夹 {Path} 中发现了 {numFiles} 张有效图片。')

    for idx in range(numFiles):
        originFilePath = os.path.join(
            Path, folderName + str(idx + 1) + suffix + '.png')
        newFilePath = os.path.join(Path, folderName + str(idx + 1) + '.png')
        os.rename(originFilePath, newFilePath)

    if RenameSubFolder:
        for dir in dirs:
            subPath = os.path.join(Path, dir)
            subPath = os.path.abspath(subPath)
            failedFile = failedFile + rename(subPath, RenameSubFolder)

    return failedFile


if __name__ == '__main__':
    Path, RenameSubFolder = getArgs()
    order = input(f'警告：这个程序将把 {Path} 中所有图片重命名，请确保这个修改是你想要的，按y继续，按n结束。')
    if order == 'y':
        if RenameSubFolder:
            print('本次修改将包括目标文件夹及其子文件夹')
            failedFiles = rename(Path, RenameSubFolder)
            print('任务完成，本次修改包括了目标文件夹及其子文件夹')
        else:
            print('本次修改仅涉及目标文件夹，不包括其子文件夹。')
            failedFiles = rename(Path, RenameSubFolder)
            print('任务完成，本次修改仅涉及目标文件夹，不包括其子文件夹。')
        if failedFiles:
            print('下列文件修改失败：')
            print('*' * 10)
            for failedFile in failedFiles:
                print(failedFile)
            print('*' * 10)
        else:
            print('所有文件修改成功。')
