#! python3
"""
把同一文件夹下所有的 ppt/pptx 文件转化为同名、同路径的 pdf 文件

参数： searchAll，来设定待转换 ppt/pptx 文件的范围。
如果 searchAll 为  True，将把当前文件夹下，以及当前文件夹包含的子文件夹下所有的 ppt/pptx 文件全部转换为 pdf。
如果 searchAll 为 False，仅把当前文件夹下的 ppt/pptx 文件全部转换为 pdf。

注意事项：搜索到的 ppt 在进行转换前，会先检查同文件夹下有没有同名的 pdf 文件，
如果有，则默认该 ppt 已被转化为 pdf，不再重复转化
"""

from topdf import *
from fileTransform import *

input_suffixes = ['.xlsx', '.xls']
output_suffix = '.pdf'

if __name__ == '__main__':
    transform(input_suffixes, output_suffix, converter)
