#! python3
"""
从键盘获取文件夹路径，以及是否包括子目录（详情参考./getArgs.py），
把目标文件夹下所有某格式文件转化为同名、同路径的目标格式文件，输入
格式和目标格式由外给定，用户需要编辑converter函数和judge判断条件
"""

import traceback
import os
from getArgs import getArgs
from searchFiles import searchFiles


def transform(input_suffixes, output_suffix, converter, judge=None):
    """
    把指定文件夹中后缀名为 input_suffixes 的文件利用 converter 转换格式，并输出为同路径同名后缀名为 output_suffix 的文件
    参数：
        input_suffixes: 待转换未见的后缀名，可能有多个, list 类型
        output_suffix: 输出文件的后缀名，只能有一个，str 类型
        converter: 转换函数，把单个输入文件转化为目标格式，要求参数类型为 converter(input: str, output: str)
        judge: 筛选函数，搜索待转换文件时判断是否需要转换它，类型为 judge(file: str) -> bool，如果不给定，则默认使用后缀名筛选
    """

    dirname, searchAll = getArgs()
    print(f'即将开始搜索待转换文件，搜索目录：{dirname}，是否搜索子目录：{searchAll}')

    if judge is None:
        print(f'未给定搜索筛选条件，默认使用后缀名筛选')

        def judge(file):
            """
            判断给定文件的后缀名是否在 input_suffixes 中
            """
            for input_suffix in input_suffixes:
                if file.endswith(input_suffix):
                    return True
            return False

    print('搜索中...')
    validFiles, failedDirs = searchFiles(dirname, searchAll, judge)
    num_files = len(validFiles)
    print(f'搜索完成，总共找到 {num_files} 个待转换文件。')

    if num_files == 0:
        if failedDirs:
            print('下列目录搜索失败：')
            for failedDir in failedDirs:
                print(failedDir)
        print('本次程序结束')
        exit(0)

    num_exist = 0  # 已转换过的文件数量
    newFiles = []  # 转换后的文件名
    for file in validFiles.copy():
        newFile = None
        for suffix in input_suffixes:
            if file.endswith(suffix):
                newFile = file[:-len(suffix)] + output_suffix  # 把后缀名替换为目标后缀名
                break
        if os.path.exists(newFile):
            validFiles.remove(file)
            num_exist += 1
        else:
            newFiles.append(newFile)
    if num_exist != 0:
        print(f'其中 {num_exist} 个文件已转化')
    elif num_exist == num_files:
        print('所有文件已被转化, 本次程序结束')
        exit(0)

    print('待转换文件：')
    for idx, (validFile, newFile) in enumerate(zip(validFiles, newFiles)):
        print(f'{idx+1}: {validFile} ----> {newFile}')
    print('*' * 20)

    print('开始转换\n')
    FailedList = []
    num_all, num_success, num_failed = 0, 0, 0
    for input, output in zip(validFiles, newFiles):
        num_all += 1
        print(f'输入文件：{input}')
        success = converter(input, output)
        if success:
            print(f'输出文件：{output}\n')
            num_success += 1
        else:
            print('转化失败')
            FailedList.append(input)
            num_failed += 1

    print('转换完成：')
    print(f'所有文件数量：{num_all}')
    print(f'成功转换数量：{num_success}')
    print(f'失败转换数量：{num_failed}')

    if FailedList:
        print('转换失败文件列表:')
        for FailedFile in FailedList:
            print(FailedFile)
    if failedDirs:
        print('搜索失败目录列表')
        for failedDir in failedDirs:
            print(failedDir)


if __name__ == '__main__':
    input_suffixes = ['.pptx', '.ppt']
    output_suffix = ['.pdf']

    def converter(input, output):
        import win32com.client
        try:
            ppt_app = win32com.client.Dispatch('PowerPoint.Application')
            ppt = ppt_app.Presentations.Open(input)
            ppt.SaveAs(output, 32)
            ppt_app.Quit()
            return True
        except Exception:
            traceback.print_exc()
            return False

    transform(input_suffixes, output_suffix, converter)
