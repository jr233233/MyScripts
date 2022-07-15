#! python3
"""
把同一文件夹下所有的 office 文件转化为同名、同路径的 pdf 文件

参数： searchAll，来设定待转换 office 文件的范围。
如果 searchAll 为  True，将把当前文件夹下，以及当前文件夹包含的子文件夹下所有的 office 文件全部转换为 pdf。
如果 searchAll 为 False，仅把当前文件夹下的 office 文件全部转换为 pdf。

注意事项：搜索到的 office 在进行转换前，会先检查同文件夹下有没有同名的 pdf 文件，
如果有，则默认该 office 已被转化为 pdf，不再重复转化
"""

from win32com.client import Dispatch
import traceback
from fileTransform import *


output_number = {
    ('.pptx', '.pdf'): 32,
    ('.ppt', '.pdf'): 32,
    ('.docx', '.pdf'): 17,
    ('.doc', '.pdf'): 17,
    ('.ppt', '.pptx'): 11,
    ('.doc', '.docx'): 12,
}

def converter(input, output):
    try:
        input_suffix = '.' + input.split('.')[-1]
        output_suffix = '.' + output.split('.')[-1]
        if input.endswith('.pptx') or input.endswith('.ppt'):
            office_app = Dispatch('PowerPoint.Application')
            office = office_app.Presentations.Open(input)
            office.SaveAs(output, output_number[(input_suffix, output_suffix)])
        elif input.endswith('.docx') or input.endswith('.doc'):
            office_app = Dispatch('Word.Application')
            office = office_app.Documents.Open(input)
            office.SaveAs(output, output_number[(input_suffix, output_suffix)])
        elif input.endswith('.xlsx') or input.endswith('.xls'):
            office_app = Dispatch('Excel.Application')
            office = office_app.Workbooks.Open(input)
            # office.SaveAs(output, 17)
            office.ExportAsFixedFormat(0, output)
        office.Close()
        office_app.Quit()
        return True
    except Exception:
        traceback.print_exc()
        return False


if __name__ == '__main__':
    input_suffixes = ['.pptx', '.ppt', '.docx', '.doc', '.xlsx', '.xls']
    output_suffix = '.pdf'
    transform(input_suffixes, output_suffix, converter)
