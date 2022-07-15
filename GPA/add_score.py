from cal_GPA import *

if __name__ == '__main__':
    file_read = os.path.join(sys.path[0], 'scores.xlsx')
    file_write = os.path.join(sys.path[0], '成绩信息.txt')
    
    score = Score(terms)
    score.read_data(file_read)
    score.add_score()

    score.read_data(file_read)
    score.describe()
    score.write_to_txt(file_write)

    for term, (table, Data, credit_sum, GPA) in score.scores.items():
        print(f'{term:>18s}: credits = {credit_sum:5.1f}, GPA = {GPA:.4f}')

