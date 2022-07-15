import pandas as pd
import os
import sys

# 选课学期，这里的学期名称需要与 excel 里的 sheet 名称一致
# total 为所有课程
# major courses 为主修课程， 教务网的“成绩查询”右边有一个叫“成绩主干数据查询”，就是里面的表格
# freshman year, sophomore year, junior year 依次为大一、大二、大三
# freshman year 1 代表大一上学期(秋冬学期)，2 代表下学期(春夏学期)
# 以上各个学期的数据均可在教务网直接查询并复制
terms = ['total', 'major courses', 'freshman year', 'sophomore year', 'junior year', 'senior year', 'freshman year 1',
        'freshman year 2', 'sophomore year 1', 'sophomore year 2', 'junior year 1', 'junior year 2', 'senior year1']

class Score:
    def __init__(self, terms):
        self.filePath = None
        self.terms = terms  # 学期
        self.scores = {term:None for term in terms} # 各学期的分数

    def read_data(self, table_name):
        self.filePath = table_name
        for term in self.scores:
            table0 = pd.read_excel(table_name, sheet_name=term)
            table = table0.copy()
            table.index += 1
            table['成绩'] = table['成绩'].astype(str)   # 可能有非百分制成绩，无法用数字表示
            table = table[['课程名称', '成绩', '学分', '绩点']]
            table = table[table['成绩'] != '弃修']  # 算 GPA 时不包括弃修和水测
            table = table[table['课程名称'] != '英语水平测试']

            credits = table['学分']
            scores5 = table['绩点']

            self.table = table0
            self.Data = table
            self.credit_sum = sum(credits)
            self.GPA = sum(credits*scores5) / self.credit_sum
            if '英语水平测试' in self.table['课程名称'].values:
                self.credit_sum += 1    # 加上水测 1 学分
            self.scores[term] = [self.table, self.Data, self.credit_sum, self.GPA]

    def describe(self):
        pd.set_option('display.unicode.ambiguous_as_wide', True)
        pd.set_option('display.unicode.east_asian_width', True)
        pd.set_option('display.width', 3600)  # 设置打印宽度
        
        for term, (table, Data, credit_sum, GPA) in self.scores.items():
            print(f'In {term}:')
            print(Data)
            print(f'credits = {credit_sum:5.1f}')
            print(f'GPA = {GPA:.4f}\n\n')

    def write_to_txt(self, file_write):
        f = open(file_write, 'w')
        with open(file_write, 'w') as f:
            for term, (table, Data, credit_sum, GPA) in self.scores.items():
                f.write(f'{term:>18s}: credits = {credit_sum:5.1f}, GPA = {GPA:.4f}\n\n')
    
    def add_score(self):
        """
        开发于 2022.06，使用次数还不多，出现 bug 概率较高，建议有能力的同学自己在主函数中编写代码调用它
        功能：每次都从教务网复制整个表太麻烦了，这个函数从键盘读入新出分的课程
        """

        print('请按照提示在成绩单中添加一门课程成绩。')
        term_dict = {i:term for i, term in enumerate(self.terms)}
        print(term_dict)
        term_key = int(input('请输入课程所在学期对应的数字(参考上表)：'))
        while term_key not in term_dict:
            print('输入错误，请重新输入')
            term_key = int(input('请输入课程所在学期对应的数字(参考上表)：'))

        isMain = bool(int(input('是否为主修课？是请输入1，不是请输入0：')))

        number = input('请输入选课课号：')
        course = input('请输入课程名称：')

        score100 = input('请输入成绩(百分制)：')
        try:
            score100 = int(score100)
        except Exception:
            pass

        credit = 0
        while True:
            try:
                credit = float(input('请输入学分：'))
                break
            except Exception:
                print('输入的不是数字，请重新输入。')
        
        score5 = 0
        while True:
            try:
                score5 = float(input('请输入绩点：'))
                break
            except Exception:
                print('输入的不是数字，请重新输入。')
        
        record = {'选课课号':number, '课程名称':course, '成绩':score100, '学分':credit, '绩点':score5}
        self.scores[self.terms[0]][0] = self.scores[self.terms[0]][0].append([record], ignore_index=True)
        self.scores[self.terms[term_key]][0] = self.scores[self.terms[term_key]][0].append([record], ignore_index=True)
        if self.terms[term_key] in ['freshman year 1', 'freshman year 2']:
            self.scores['freshman year'][0] = self.scores['freshman year'][0].append([record], ignore_index=True)
        elif self.terms[term_key] in ['sophomore year 1', 'sophomore year 2']:
            self.scores['sophomore year'][0] = self.scores['sophomore year'][0].append([record], ignore_index=True)
        elif self.terms[term_key] in ['junior year 1', 'junior year 2']:
            self.scores['junior year'][0] = self.scores['junior year'][0].append([record], ignore_index=True)

        if isMain:
            record['学年'] = record['选课课号'][1:10]
            self.scores['major courses'][0] = self.scores['major courses'][0].append([record], ignore_index=True)

        writer = pd.ExcelWriter(self.filePath)
        for term, (table, Data, credit_sum, GPA) in self.scores.items():
            table.to_excel(writer, sheet_name=term, index=False)
        writer.save()
        writer.close()


if __name__ == '__main__':
    file_read = os.path.join(sys.path[0], 'scores.xlsx')
    file_write = os.path.join(sys.path[0], '成绩信息.txt')
    print(f'Read scores data from {file_read}')
    
    score = Score(terms)
    score.read_data(file_read)
    score.describe()
    score.write_to_txt(file_write)

    for term, (table, Data, credit_sum, GPA) in score.scores.items():
        print(f'{term:>18s}: credits = {credit_sum:5.1f}, GPA = {GPA:.4f}')


