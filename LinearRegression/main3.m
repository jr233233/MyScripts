clear; clc;
% 使用多元线性回归来预测房价

% 这个程序可能要运行几十分钟才能结束
% 如果要缩短时间，请改变迭代次数 maxIterationNum

filename = 'house_price.csv';   % 数据集来源：
Data = xlsread(filename);   % https://github.com/ageron/handson-ml2

m = length(Data);           % number of data
i = 1;
while i < m
    if Data(i, 9) >= 400000
        % Data(i, 9)为房价，房价超过40万的数据不太适合拟合，除非
        % 用一些比较复杂的数据处理手段，此处为简便起见，直接删除。
        Data(i, :) = [];
        m = m - 1;
        i = i - 1;
    end
    i = i + 1;
end

m = length(Data);           % 数据个数
n = length(Data(1, :)) - 1; % 每个数据自变量个数

for i = 1:n
    for j = 1:m
        if isnan(Data(j, i))
            % 把缺失值 nan 用同一列上一个数据代替
            Data(j, i) = Data(j-1, i);
        end
    end
end

train_size = round(0.9*m);  % 训练集数量
x_train = Data(1:train_size, 1:8);  % 训练集自变量
y_train = Data(1:train_size, 9);    % 训练集因变量
x_test = Data(train_size+1:m, 1:8); % 测试集自变量 
y_test = Data(train_size+1:m, 9);   % 测试集因变量

eta = 0.05;  % 学习率
tol = 1e-9;  % 两次迭代代价函数之差 < tol，则迭代终止
% -------------------------------------------------------------
maxIterationNum = 1e8;  % 最大迭代次数，要改变运行时长请修改此参数
% -------------------------------------------------------------
rng(1);
w0 = rand(1, n);   % 初始迭代点
b0 = rand(1, 1);

% 新建一个模型
model = LinearRegression(eta, tol, maxIterationNum);
fprintf("开始迭代\n");
% 用该模型来训练，并存储训练的结果
t1 = tic;
model = model.Fit(x_train, y_train, w0, b0);
t2 = toc(t1);
fprintf("迭代次数 = %d, 用时 = %.2f min.\n", maxIterationNum, t2/60);
% ------------------------------------------------------------------------------
% 如果要改变打印迭代次数的频率，请前往LinearRegression.m文件的 GradientDescent 函数
% ------------------------------------------------------------------------------

% 对测试集进行预测
y_predict = model.Predict(x_test);

% 数据可视化
% 横坐标：房价真实值
% 纵坐标：房价预测值
scatter(y_test, y_predict, 'filled');
hold on;

% 生成直线 y = x
X = linspace(min(y_test), max(y_test), 101);
plot(X, X, '--');

xlabel('price');
ylabel('price\_predicted');
grid on;


