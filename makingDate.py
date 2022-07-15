"""
打印日程表，单次最好不要超过52个星期（1年）
"""
import sys
print(f'version = {sys.version}')
import datetime
import os
import traceback

termNums = {
    1: '春学期',
    2: '夏学期',
    3: '暑假',
    4: '秋学期',
    5: '冬学期',
    6: '寒假'
}
fallStart = None    # 秋学期开学日期
SpringStart = None  # 春学期开学日期

writeFilePath = 'E:'

def getInfo():
    global SpringStart, fallStart
    today = datetime.datetime.now()
    fallYear = today.year if today.month < 9 else today.year + 1    # 下一个秋学期的年份
    springYear = today.year if today.month < 3 else today.year + 1  # 下一个春学期的年份

    fallStart = f'{fallYear}.' + input(f'请输入{fallYear}年秋学期开学的日期（输入格式：mm.dd，可留空）：')
    try:
        fallStart = datetime.datetime.strptime(fallStart, '%Y.%m.%d')
        if fallStart.weekday() != 0:    # 返回 0~6，代表星期一到星期天
            print('这一天不是星期一')
            raise Exception()
    except Exception:
        fallStart = datetime.datetime.strptime(str(fallYear)+'.09.01', '%Y.%m.%d')
        # 下面的循环搜索9月第2个星期一
        MondayCnt = 0
        while MondayCnt != 2:
            fallStart = fallStart + datetime.timedelta(days=1)
            if fallStart.weekday() == 0:
                MondayCnt += 1
        print(f'未获得合法的秋学期开学日期，默认9月第2个星期一（{fallStart.year}.{fallStart.month:>02d}.{fallStart.day:>02d}）')
    
    SpringStart = f'{springYear}.' + input(f'请输入{springYear}年春学期开学的日期（输入格式：mm.dd，可留空）：')
    try:
        SpringStart = datetime.datetime.strptime(SpringStart, '%Y.%m.%d')
        if SpringStart.weekday() != 0:
            print('这一天不是星期一')
            raise Exception()
    except Exception:
        SpringStart = None
        print('未获得合法的春学期开学日期，默认寒假长为5周，之后切换为春学期')

    print('学期序号表：')
    for status, term in termNums.items():
        print(f'{status:2d} {term}')
    while True:
        try:
            status = int(input('请输入今天的学期序号：'))
            weekNum = int(input('请输入今天的周数：'))
            dayNum = int(input('请输入今天的周内天数：'))
            totalWeeks = int(input('请输入总共要打印的周数：'))
            return totalWeeks, status, weekNum, dayNum
        except Exception:
            traceback.print_exc()
            print('输入有误！请重新输入。')


def updateStatus(status, weekNum, this_day):
    if status == 1:
        if weekNum == 9:
            status = 2
            weekNum = 1
    elif status == 2:
        if weekNum == 10:
            status = 3
            weekNum = 1
    elif status == 3:
        if this_day >= fallStart:
            status = 4
            weekNum = 1
    elif status == 4:
        if weekNum == 9:
            status = 5
            weekNum = 1
    elif status == 5:
        if weekNum == 10:
            status = 6
            weekNum = 1
    elif status == 6:
        if SpringStart is not None:
            if this_day >= SpringStart:
                status = 1
                weekNum = 1
        else:
            if weekNum == 6:
                status = 1
                weekNum = 1

    return status, weekNum


def print_schedule(totalWeeks, status, weekNum, dayNum):
    first_day = datetime.datetime.now()
    delta_days = 0
    with open(writeFilePath, 'w') as f:
        for _ in range(totalWeeks):
            if dayNum == 1:
                if weekNum == 9 and status in [2, 5]:   # 夏、冬学期的第9周是考试周
                    msg = f'{termNums[status]}考试周'
                else:
                    msg = f'{termNums[status]}第{weekNum}周'
                print(msg)
                f.write(msg + '\n')
            for _ in range(8-dayNum):
                delta_day = datetime.timedelta(days=delta_days)
                this_day = first_day + delta_day
                msg = f'{this_day.year}.{this_day.month:>02d}.{this_day.day:>02d}'
                print(msg)
                f.write(msg + '\n')
                delta_days += 1
            dayNum = 1

            weekNum += 1
            delta_day = datetime.timedelta(days=delta_days)
            this_day = first_day + delta_day    # 如有可能，更新现在的学期
            status, weekNum = updateStatus(status, weekNum, this_day)


if __name__ == '__main__':
    if not os.path.exists(writeFilePath):
        print(f'默认写入文件的目录不存在：{writeFilePath}')
        print(f'如果继续，将在当前文件目录创建 date.txt 文件，请问是否继续？(y/n)')
        if input() == 'y':
            writeFilePath = os.path.join(os.getcwd(), 'date.txt')
        else:
            exit(0)
    else:
        writeFilePath = os.path.join(writeFilePath, 'date.txt')
    
    totalWeeks, status, weekNum, dayNum = getInfo()
    print_schedule(totalWeeks, status, weekNum, dayNum)

    print(f'日期成功写入文件{writeFilePath}。')

