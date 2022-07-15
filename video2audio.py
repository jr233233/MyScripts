#! python3
"""
把同一文件夹下所有的 mp4 文件转化为同名、同路径的 mp3 文件

参数： searchAll，来设定待转换 mp4 文件的范围。
如果 searchAll 为  True，将把当前文件夹下，以及当前文件夹包含的子文件夹下所有的 mp4 文件全部转换为 mp3。
如果 searchAll 为 False，仅把当前文件夹下的 mp4 文件全部转换为 mp3。

注意事项：搜索到的 ppt 在进行转换前，会先检查同文件夹下有没有同名的 mp3 文件，
如果有，则默认该 ppt 已被转化为 mp3，不再重复转化
"""

import moviepy.editor as mpy
import traceback
from fileTransform import *

input_suffixes = ['.mp4', '.webm']
output_suffix = '.mp3'


def converter(input, output):
    try:
        audio_background = mpy.AudioFileClip(input)
        audio_background.write_audiofile(output)
        return True
    except Exception:
        traceback.print_exc()
        return False


if __name__ == '__main__':
    transform(input_suffixes, output_suffix, converter)
